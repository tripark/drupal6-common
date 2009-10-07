<?php
// $Id: features.export.inc,v 1.1.2.38 2009/09/14 14:47:31 yhahn Exp $

/**
 * Form callback for features export form. Acts as a router based on the form_state.
 */
function features_export_form($form_state, $feature = NULL) {
  features_include();

  $form = array(
    '#attributes' => array('class' => 'features-export-form'),
    '#feature' => isset($feature) ? $feature : NULL,
  );
  $form['info'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
  );
  $form['info']['name'] = array(
    '#title' => t('Name'),
    '#description' => t('Example: Image gallery'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($feature->info['name']) ? $feature->info['name'] : '',
    '#attributes' => array('class' => 'feature-name'),
  );
  $form['info']['project'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine-readable name'),
    '#description' => t('Example: image_gallery'). '<br/>' .t('May only contain lowercase letters, numbers and underscores. <strong>Try to avoid conflicts with the names of existing Drupal projects.</strong>'),
    '#required' => TRUE,
    '#default_value' => !empty($feature->name) ? $feature->name : '',
    '#attributes' => array('class' => 'feature-project'),
  );
  $form['info']['description'] = array(
    '#title' => t('Description'),
    '#description' => t('Provide a short description of what users should expect when they enable your feature.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($feature->info['description']) ? $feature->info['description'] : '',
  );
  $form['info']['version'] = array(
    '#title' => t('Version'),
    '#description' => t('Examples: 6.x-1.0, 6.x-1.0-beta1'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => !empty($feature->info['version']) ? $feature->info['version'] : '',
    '#size' => 30,
  );
  $form['info']['project_status_url'] = array(
    '#title' => t('URL of update XML'),
    '#description' => t('Example: http://mywebsite.com/fserver'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => !empty($feature->info['project status url']) ? $feature->info['project status url'] : '',
    '#size' => 30,
  );

  // User-selected feature source components.
  $components = array_keys(features_get_components());
  $components[] = 'dependencies';
  $form['export'] = array(
    '#type' => 'fieldset', 
    '#tree' => FALSE,
    '#theme' => 'features_form_export',
  );
  $form['export']['components'] = array(
    '#title' => t('Add components'),
    '#type' => 'select',
    '#options' => array('------'),
    '#attributes' => array('class' => 'features-select-components'),
  );
  $form['export']['sources'] = array(
    '#tree' => TRUE,
    '#theme' => 'features_form_components',
  );
  foreach ($components as $component) {
    // Tack on dependency options as they are not a feature component proper.
    if ($component == 'dependencies') {
      $required = drupal_required_modules();
      $options = array();
      foreach (features_get_modules() as $module_name => $info) {
        if (!in_array($module_name, $required) && $info->status && !empty($info->info)) {
          $options[$module_name] = $info->info['name'];
        }
      }
      $default_value = !empty($feature->info['dependencies']) ? $feature->info['dependencies'] : array();
    }
    else {
      $options = features_invoke($component, 'features_export_options');
      $default_value = !empty($feature->info['features'][$component]) ? $feature->info['features'][$component] : array();
    }
    if ($options) {
      $form['export']['components']['#options'][$component] = $component;
      $form['export']['sources'][$component] = array(
        '#type' => 'checkboxes',
        '#options' => $options,
        '#title' => $component,
        '#default_value' => $default_value,
        '#ahah' => array(
          'path' => 'admin/build/features/export/populate',
          'wrapper' => 'features-export-populated',
        ),
      );
    }
  }
  $form['export']['features'] = array(
    '#tree' => TRUE,
    '#type' => 'markup',
    '#prefix' => "<div id='features-export-populated'>",
    '#suffix' => "</div>",
    '#value' => !empty($feature->info) ? theme('features_components', $feature->info, $feature->info['features'], array(), TRUE) : "<div class='placeholder'></div>",
  );

  $form['buttons'] = array('#theme' => 'features_form_buttons', '#tree' => FALSE);
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download feature'),
    '#weight' => 10,
    '#submit' => array('features_export_build_form_submit'),
  );
  return $form;  
}

/**
 * Submit handler for features_export_form_build().
 */
function features_export_build_form_submit($form, &$form_state) {
  // Assemble the combined component list
  $stub = array();
  $components = array_keys(features_get_components());
  foreach ($components as $component) {
    // User-selected components take precedence.
    if (!empty($form_state['values']['sources'][$component])) {
      $stub[$component] = array_filter($form_state['values']['sources'][$component]);
    }
    // Only fallback to an existing feature's values if there are no export options for the component.
    else if (!empty($form['#feature']->info['features'][$component])) {
      $stub[$component] = $form['#feature']->info['features'][$component];
    }
  }

  // Generate populated feature
  $export = features_populate($stub, $form_state['values']['project']);

  // Directly copy the following attributes
  $attr = array('name', 'description', 'project', 'version');
  foreach ($attr as $key) {
    $export[$key] = isset($form_state['values'][$key]) ? $form_state['values'][$key] : NULL;
  }
  if (!empty($form_state['values']['project_status_url'])) {
    $export['project status url'] = $form_state['values']['project_status_url'];
  }

  // Minimize detected dependencies and then merge in existing dependencies
  if (!empty($form_state['values']['sources']['dependencies'])) {
    $dependencies = array_filter($form_state['values']['sources']['dependencies']);
    $export['dependencies'] = _features_export_preserve_dependencies($export['dependencies'], NULL, $dependencies);
  }

  // Generate download
  if ($files = features_export_render($export, $export['project'], TRUE)) {
    $module_name = $export['project'];
    $filename = !empty($export['version']) ? "{$module_name}-{$export['version']}" : $module_name;

    $tar = array();
    foreach ($files as $extension => $file_contents) {
      if (!in_array($extension, array('module', 'info'))) {
        $extension .= '.inc';
      }
      $tar["{$module_name}/{$module_name}.$extension"] = $file_contents;
    }
    $tar = features_tar_create($tar);

    $header = function_exists('gzencode') ? 'Content-type: application/x-gzip' : 'Content-type: application/x-tar';
    $filename = !empty($filename) ? $filename : $module_name;
    $filename = function_exists('gzencode') ? "{$filename}.tgz" : "{$filename}.tar";

    drupal_set_header($header);
    drupal_set_header('Content-Disposition: attachment; filename="'. $filename .'"');
    print $tar;
    exit;
  }
}

/**
 * AHAH handler for features_export_form_build().
 */
function features_export_build_form_populate() {
  $form_state = array();
  $submitted = $_POST;

  // Only uncomment this for debugging AJAX action
  $submitted = !empty($_POST) ? $_POST : $_GET;

  if ($form = form_get_cache($submitted['form_build_id'], $form_state)) {
    $stub = array();

    // Assemble the combined component list
    $components = array_keys(features_get_components());
    foreach ($components as $component) {
      // User-selected components take precedence.
      if (!empty($submitted['sources'][$component])) {
        $stub[$component] = $submitted['sources'][$component];
      }
      // Only fallback to an existing feature's values if there are no export options for the component.
      else if (!isset($form['export']['sources'][$component]) && !empty($form['#feature']->info['features'][$component])) {
        $stub[$component] = $form['#feature']->info['features'][$component];
      }
    }

    // Ensure source dependencies are populated
    $stub['dependencies'] = isset($submitted['sources']['dependencies']) ? $submitted['sources']['dependencies'] : array();

    // Generate populated feature
    $export = features_populate($stub, $submitted['project']);
    $export['dependencies'] = _features_export_preserve_dependencies($export['dependencies'], NULL, $stub['dependencies']);

    drupal_json(array('status' => TRUE, 'data' => theme('features_components', $export, $stub, array(), TRUE)));
    exit;
  }
  drupal_json(array('status' => FALSE, 'data' => ''));
  exit;
}

/**
 * @param $items
 * @param $module_name
 * @return
 */
function features_populate($items, $module_name) {
  // Sanitize items.
  $items = array_filter($items);

  // Populate stub
  $stub = array('features' => array(), 'dependencies' => array(), 'conflicts' => array());
  $export = _features_populate($items, $stub, $module_name);
  $export['dependencies'] = _features_export_minimize_dependencies($export['dependencies'], $module_name);

  // Clean up and standardize order
  foreach (array_keys($export['features']) as $k) {
    ksort($export['features'][$k]);
  }
  ksort($export['features']);
  ksort($export['dependencies']);

  return $export;
}

/**
 * Iterate and descend into a feature definition to extract module
 * dependencies and feature definition. Calls hook_features_export for modules
 * that implement it.
 *
 * @param $pipe
 *  Associative of array of module => info-for-module
 * @param $export
 *  Associative array of items, and module dependencies which define a feature.
 *  Passed by reference.
 *
 * @return fully populated $export array.
 */
function _features_populate($pipe, &$export, $module_name = '') {
  features_include();
  foreach ($pipe as $component => $data) {
    if (features_hook($component, 'features_export')) {
      // Pass module-specific data and export array.
      // We don't use features_invoke() here since we need to pass $export by reference.
      $function = "{$component}_features_export";
      $more = $function($data, $export, $module_name);
      // Allow for export functions to request additional exports.
      if (!empty($more)) {
        _features_populate($more, $export, $module_name);
      }
    }
  }
  return $export;
}

/**
 * Iterates over a list of dependencies and kills modules that are
 * captured by other modules 'higher up'.
 */
function _features_export_minimize_dependencies($dependencies, $module_name = '') {
  // Ensure that the module doesn't depend upon itself
  if (!empty($module_name) && !empty($dependencies[$module_name])) {
    unset($dependencies[$module_name]);
  }

  // Do some cleanup:
  // - Remove modules required by Drupal core.
  // - Protect against direct circular dependencies.
  // - Remove "intermediate" dependencies.
  $required = drupal_required_modules();
  foreach ($dependencies as $k => $v) {
    if (empty($v) || in_array($v, $required)) {
      unset($dependencies[$k]);
    }
    else {
      $module = features_get_modules($v);
      if ($module && !empty($module->info['dependencies'])) {
        // If this dependency depends on the module itself, we have a circular dependency.
        // Don't let it happen. Only you can prevent forest fires.
        if (in_array($module_name, $module->info['dependencies'])) {
          unset($dependencies[$k]);
        }
        // Iterate through the dependency's dependencies and remove any dependencies
        // that are captured by it.
        else {
          foreach ($module->info['dependencies'] as $j => $dependency) {
            if (array_search($dependency, $dependencies) !== FALSE) {
              $position = array_search($dependency, $dependencies);
              unset($dependencies[$position]);
            }
          }
        }
      }
    }
  }
  return drupal_map_assoc(array_unique($dependencies));
}

/**
 * Iterates over a list of dependencies and maximize the list of modules.
 */
function _features_export_maximize_dependencies($dependencies, $module_name = '', $first = TRUE) {
  $maximized = $dependencies;
  foreach ($dependencies as $k => $v) {
    $module = features_get_modules($v);
    if ($module && !empty($module->info['dependencies'])) {
      $maximized = array_merge($maximized, _features_export_maximize_dependencies($module->info['dependencies'], $module_name, FALSE));
    }
  }
  return array_unique($maximized);
}

/**
 * Preserve existing dependencies.
 */
function _features_export_preserve_dependencies($dependencies, $module_name = NULL, $existing = array()) {
  if (!empty($module_name)) {
    $feature = feature_load($module_name);
    $existing = !empty($feature->info['dependencies']) ? $feature->info['dependencies'] : array();
  }
  $merged = array_merge($existing, $dependencies);
  $merged = array_unique($merged);
  ksort($merged);
  return $merged;
}

/**
 * Prepare a feature export array into a finalized info array.
 */
function features_export_prepare($export, $module_name, $reset = FALSE) {
  $existing = features_get_modules($module_name, $reset);

  // Prepare info string -- if module exists, merge into its existing info file
  $defaults = $existing ? $existing->info : array('core' => '6.x', 'package' => 'Features', 'project' => $module_name);
  $export = array_merge($defaults, $export);

  // Cleanup info array
  foreach ($export['features'] as $component => $data) {
    $export['features'][$component] = array_keys($data);
  }
  if (isset($export['dependencies'])) {
    $export['dependencies'] = array_values($export['dependencies']);
  }
  if (isset($export['conflicts'])) {
    unset($export['conflicts']);
  }
  ksort($export);
  return $export;
}

/**
 * Generate an array of hooks and their raw code.
 */
function features_export_render_hooks($export, $module_name, $reset = FALSE) {
  features_include();
  $code = array();

  // Sort components to keep exported code consistent
  ksort($export['features']);

  foreach ($export['features'] as $component => $data) {
    if (!empty($data)) {
      // Sort the items so that we don't generate different exports based on order
      asort($data);
      if (features_hook($component, 'features_export_render')) {
        $hooks = features_invoke($component, 'features_export_render', $module_name, $data, $export);
        $code[$component] = $hooks;
      }
    }
  }
  return $code;
}

/**
 * Render feature export into an array representing its files.
 *
 * @param $export
 *  An exported feature definition.
 * @param $module_name
 *  The name of the module to be exported.
 * @param $reset
 *  Boolean flag for resetting the module cache. Only set to true when
 *  doing a final export for delivery.
 *
 * @return array of info file and module file contents.
 */
function features_export_render($export, $module_name, $reset = FALSE) {
  $code = array();

  // Generate hook code
  $component_hooks = features_export_render_hooks($export, $module_name, $reset);
  $components = features_get_components();

  // Group component code into their respective files
  foreach ($component_hooks as $component => $hooks) {
    $file = array('name' => 'defaults', 'stub' => true);
    if (isset($components[$component]['default_file'])) {
      switch ($components[$component]['default_file']) {
        case FEATURES_DEFAULTS_INCLUDED:
          $file['name'] = "features.$component";
          break;
        case FEATURES_DEFAULTS_CUSTOM:
          $file['name'] = $components[$component]['default_filename'];
          $file['stub'] = false;
          break;
      }
    }

    if (!isset($code[$file['name']])) {
      $code[$file['name']] = array();
    }

    foreach ($hooks as $hook_name => $hook_code) {
      if ($file['stub']) {
        if (!isset($code['features'])) {
          $code['features'] = array();
        }
        $code['features'][$hook_name] = features_export_render_features($module_name, $hook_name, $file['name']);
      }
      $code[$file['name']][$hook_name] = features_export_render_defaults($module_name, $hook_name, $hook_code);
    }
  }

  // Finalize strings to be written to files
  foreach ($code as $filename => $contents) {
    $code[$filename] = "<?php\n\n". implode("\n\n", $contents) ."\n";
  }

  // Generate info file output
  $export = features_export_prepare($export, $module_name, $reset);
  $code['info'] = features_export_info($export);

  // Prepare the module
  // If module exists, let it be and include it in the files
  $existing = features_get_modules($module_name, TRUE);
  if ($existing) {
    $code['module'] = file_get_contents($existing->filename);
  }
  // Add a stub module to include the defaults
  else if (!empty($code['features'])) {
    $code['module'] = "<?php\n\ninclude_once('{$module_name}.features.inc');\n";
  }
  return $code;
}

/**
 * Detect differences between DB and code components of a feature.
 */
function features_detect_overrides($module) {
  static $cache;
  if (!isset($cache)) {
    $cache = array();
  }
  if (!isset($cache[$module->name])) {
    // Make necessary inclusions
    if (module_exists('views')) {
      views_include('view');
    }

    // Retrieve default hooks
    $default_hooks = features_get_default_hooks();

    // Rebuild feature from .info file description and prepare an export from current DB state.
    $export = features_populate($module->info['features'], $module->name);
    $export['dependencies'] = _features_export_preserve_dependencies($export['dependencies'], $module->name);
    $export = features_export_prepare($export, $module->name);
    $code = features_export_render_hooks($export, "_features_comparison_{$module->name}");

    $overridden = array();

    // Compare feature info
    ksort($module->info);
    ksort($export);
    $compare = array(
      'current' => features_export_info($export),
      'default' => features_export_info($module->info)
    );
    if ($compare['current'] !== $compare['default']) {
      $overridden['info'] = $compare;
    }

    // Merge items from both for comparison
    $merged = array_merge($export['features'], $module->info['features']);

    foreach (array_keys($merged) as $component) {
      if (isset($default_hooks[$component]) && ($default_hook = $default_hooks[$component])) {
        $compare = array('current' => array(), 'default' => array());
        // Eval the database version of the export
        if (isset($code[$component][$default_hook])) {
          $compare['current'] = eval($code[$component][$default_hook]);
          $compare['current'] = is_array($compare['current']) ? $compare['current'] : array();
        }
        // Call the existing in-code function and collect results
        if (module_hook($module->name, $default_hook)) {
          $compare['default'] = module_invoke($module->name, $default_hook);
          $compare['default'] = is_array($compare['default']) ? $compare['default'] : array();
        }
        // Export both versions
        foreach ($compare as $storage => $items) {
          foreach ($items as $k => $v) {
            // Special case for views which provides its own export method
            if (is_object($v) && get_class($v) == 'view') {
              $compare[$storage][$k] = $v->export();
            }
            else {
              $compare[$storage][$k] = features_var_export($v);
            }
          }
        }
        // Collect differences between the two arrays
        foreach ($compare['current'] as $k => $v) {
          if (isset($compare['current'][$k], $compare['default'][$k])) {
            if (_features_linetrim($compare['current'][$k]) !== _features_linetrim($compare['default'][$k])) {
              $overridden[$component][$k] = array('current' => $compare['current'][$k], 'default' => $compare['default'][$k]);
            }
          }
          else {
            $overridden[$component][$k] = array('current' => $compare['current'][$k], 'default' => '');
          }
        }
        foreach ($compare['default'] as $k => $v) {
          if (!isset($compare['current'][$k])) {
            $overridden[$component][$k] = array('current' => '', 'default' => $compare['default'][$k]);
          }
        }
      }
    }
    $cache[$module->name] = $overridden;
  }
  return $cache[$module->name];
}

/**
 * Gets the available default hooks keyed by components.
 */
function features_get_default_hooks() {
  static $hooks;
  if (!isset($hooks)) {
    $hooks = array();
    features_include();
    foreach (module_implements('features_api') as $module) {
      $info = module_invoke($module, 'features_api');
      foreach ($info as $k => $v) {
        if (isset($v['default_hook'])) {
          $hooks[$k] = $v['default_hook'];
        }
      }
    }
  }
  return $hooks;
}

/**
 * Return a code string representing an implementation of a module hook.
 * Includes the module's defaults .inc and calls the private helper function.
 *
 * @param $module
 *   The name of the module being generated
 * @param $hook
 *   The name of the hook, without the "hook_" prefix. "hook_node_info" should
 *   be "node_info"
 * @param $filename
 *   Name of the include file. For a module 'foo', and filename'defaults'
 *   becomes 'foo.defaults.inc'.
 */
function features_export_render_features($module, $hook, $filename = 'defaults') {
  $output = array();
  $output[] = "/**";
  $output[] = " * Implementation of hook_{$hook}().";
  $output[] = " */";
  $output[] = "function {$module}_{$hook}() {";
  $output[] = "  module_load_include('inc', '{$module}', '{$module}.{$filename}');";
  $output[] = '  $args = func_get_args();';
  $output[] = "  return call_user_func_array('_{$module}_{$hook}', ".'$args'.");";
  $output[] = "}";
  return implode("\n", $output);
}

/**
 * Return a code string representing an implementation of a defaults module hook.
 */
function features_export_render_defaults($module, $hook, $code) {
  $output = array();
  $output[] = "/**";
  $output[] = " * Helper to implementation of hook_{$hook}().";
  $output[] = " */";
  $output[] = "function _{$module}_{$hook}() {";
  $output[] = $code;
  $output[] = "}";
  return implode("\n", $output);
}

/**
 * Generate code friendly to the Drupal .info format from a structured array.
 *
 * @param $info
 *   An array or single value to put in a module's .info file.
 * @param $parents
 *   Array of parent keys (internal use only).
 *
 * @return
 *   A code string ready to be written to a module's .info file.
 */
function features_export_info($info, $parents = array()) {
  $output = '';
  if (is_array($info)) {
    foreach ($info as $k => $v) {
      $child = $parents;
      $child[] = $k;
      $output .= features_export_info($v, $child);
    }
  }
  else if (!empty($info) && count($parents)) {
    $line = array_shift($parents);
    foreach ($parents as $key) {
      $line .= is_numeric($key) ? "[]" : "[{$key}]";
    }
    $line .=  " = \"{$info}\"\n";
    return $line;
  }
  return $output;
}

/**
 * Tar creation function. Written by dmitrig01.
 *
 * @param $files
 *   A keyed array where the key is the filepath and the value is the
 *   string contents of the file.
 *
 * @return
 *   A string of the tar file contents.
 */
function features_tar_create($files) {
  $tar = '';
  foreach ($files as $name => $contents) {
    $binary_data_first = pack("a100a8a8a8a12A12",
      $name,
      '100644 ', // File permissions
      '   765 ', // UID,
      '   765 ', // GID,
      sprintf("%11s ", decoct(strlen($contents))), // Filesize,
      sprintf("%11s", decoct(time())) // Creation time
    );
    $binary_data_last = pack("a1a100a6a2a32a32a8a8a155a12", '', '', '', '', '', '', '', '', '', '');

    $checksum = 0;
    for ($i = 0; $i < 148; $i++) {
      $checksum += ord(substr($binary_data_first, $i, 1));
    }
    for ($i = 148; $i < 156; $i++) {
      $checksum += ord(' ');
    }
    for ($i = 156, $j = 0; $i < 512; $i++, $j++) {
      $checksum += ord(substr($binary_data_last, $j, 1));
    }

    $tar .= $binary_data_first;
    $tar .= pack("a8", sprintf("%6s ", decoct($checksum)));
    $tar .= $binary_data_last;

    $buffer = str_split($contents, 512);
    foreach ($buffer as $item) {
      $tar .= pack("a512", $item);
    }
  }
  if (function_exists('gzencode')) {
    $tar = gzencode($tar);
  }
  return $tar;
}

/**
 * Export var function -- from Views.
 */
function features_var_export($var, $prefix = '', $init = TRUE) {
  if (is_array($var)) {
    if (empty($var)) {
      $output = 'array()';
    }
    else {
      $output = "array(\n";
      foreach ($var as $key => $value) {
        $output .= "  '$key' => " . features_var_export($value, '  ', FALSE) . ",\n";
      }
      $output .= ')';
    }
  }
  else if (is_bool($var)) {
    $output = $var ? 'TRUE' : 'FALSE';
  }
  else if (is_string($var) && strpos($var, "\n") !== FALSE) {
    // Replace line breaks in strings with a token for replacement
    // at the very end. This protects whitespace in strings from
    // unintentional indentation.
    $var = str_replace("\n", "***BREAK***", $var);
    $output = var_export($var, TRUE);
  }
  else {
    $output = var_export($var, TRUE);
  }

  if ($prefix) {
    $output = str_replace("\n", "\n$prefix", $output);
  }

  if ($init) {
    $output = str_replace("***BREAK***", "\n", $output);
  }

  return $output;
}

/**
 * Helper function to return an array of t()'d translatables strings.
 * Useful for providing a separate array of translatables with your
 * export so that string extractors like potx can detect them.
 */
function features_translatables_export($translatables, $prefix = '') {
  sort($translatables);
  $translatables = array_unique($translatables);

  $output = $prefix . "// Translatables\n";
  $output .= $prefix . "array(\n";
  foreach ($translatables as $string) {
    $output .= $prefix . "  t('" . strtr($string, array("'" => "\'")) . "'),\n";
  }
  $output .= $prefix . ");\n";
  return $output;
}

/**
 * Helper function to eliminate whitespace differences in code.
 */
function _features_linetrim($code) {
  $code = explode("\n", $code);
  foreach ($code as $k => $line) {
    $code[$k] = trim($line);
  }
  return implode("\n", $code);
}
