<?php
// $Id: theme.inc,v 1.1.2.6 2009/09/13 23:23:23 yhahn Exp $

/**
 * Display feature component info
 */
function template_preprocess_features_admin_components(&$vars) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');

  $rows = array();
  foreach ($vars['dependencies'] as $dependency => $status) {
    $row = array(
      array('data' => $dependency, 'class' => 'component'),
      theme('features_module_status', $status),
    );
    $rows[] = $row;
  }
  $vars['dependencies'] = theme('table', array(t('Dependency'), t('Status')), $rows);
  $vars['components'] = theme('features_components', $vars['info'], $vars['info']['features'], $vars['conflicts']);
}

/**
 * Themes a module status display.
 */
function theme_features_module_status($status, $module = NULL) {
  switch ($status) {
    case FEATURES_MODULE_ENABLED:
      $text = !empty($module) ? $module : t('Enabled');
      return "<span class='admin-enabled'>{$text}</span>";
    case FEATURES_MODULE_DISABLED:
      $text = !empty($module) ? $module : t('Disabled');
      return "<span class='admin-disabled'>{$text}</span>";
    case FEATURES_MODULE_MISSING:
      $text = !empty($module) ? $module : t('Missing');
      return "<span class='admin-missing'>{$text}</span>";
  }
}

/**
 * Themes a module status display.
 */
function theme_features_storage($storage) {
  switch ($storage) {
    case FEATURES_OVERRIDDEN:
      return "<span class='admin-overridden'>". t('Overridden') ."</span>";
    case FEATURES_DEFAULT:
      return "<span class='admin-default'>". t('Default') ."</span>";
  }
}

/**
 * Theme function for displaying form buttons
 */
function theme_features_form_buttons($element) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');

  $output = drupal_render($element);
  return !empty($output) ? "<div class='buttons clear-block'>{$output}</div>" : '';
}

/**
 * Theme for features management form.
 */
function theme_features_form($form) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  drupal_add_js(drupal_get_path('module', 'features') .'/features.js');

  $output = '';

  $header = array('', t('Feature'), t('Signature'), t('State'), t('Actions'));

  $rows = array();
  foreach (element_children($form['status']) as $element) {
    // Yank title & description fields off the form element for
    // rendering in their own cells.
    $name = "<div class='feature'>";
    $name .= "<strong>{$form['status'][$element]['#title']}</strong>";
    $name .= "<div class='description'>{$form['status'][$element]['#description']}</div>";
    $name .= "</div>";
    unset($form['status'][$element]['#title']);
    unset($form['status'][$element]['#description']);

    $row = array(
      'status' => drupal_render($form['status'][$element]),
      'name' => $name,
      'sign' => drupal_render($form['sign'][$element]),
      'state' => drupal_render($form['state'][$element]),
      'actions' => drupal_render($form['actions'][$element]),
    );

    // Determine row classes
    $class = $form['status'][$element]['#default_value'] ? 'enabled' : 'disabled';

    // Collect data + classes & add to master array.
    foreach ($row as $key => $data) {
      $row[$key] = array('data' => $data, 'class' => $key);
    }
    $rows[] = array('data' => $row, 'class' => $class);
  }

  if (empty($rows)) {
    $rows[] = array('', array('data' => t('No features available.'), 'colspan' => 4));
  }

  $output .= theme('table', $header, $rows, array('id' => 'features-form-table', 'class' => 'features'));

  // Prevent section from being rendered by drupal_render().
  unset($form['features']);

  $output .= drupal_render($form['buttons']);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme functions ====================================================
 */

/**
 * Export selection / display for features export form.
 */
function theme_features_form_export($form) {
  drupal_add_css(drupal_get_path('module', 'features') .'/features.css');
  drupal_add_js(drupal_get_path('module', 'features') .'/features.js');

  $output = '';
  $output .= "<div class='clear-block features-components'>";
  $output .= "<div class='column'>". drupal_render($form['components']) . drupal_render($form['sources']) ."</div>";
  $output .= "<div class='column'>". drupal_render($form['features']) ."</div>";
  $output .= "</div>";
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme a set of features export components.
 */
function theme_features_form_components($form) {
  $options = '';
  foreach (element_children($form) as $key) {
    unset($form[$key]['#title']);
    $output .= "<div class='features-select features-select-{$key}'>". drupal_render($form[$key]) ."</div>";
  }
  return $output;
}

/**
 * Theme a set of features export components.
 */
function theme_features_components($info, $sources = array(), $conflicts = array(), $show_dependencies = FALSE) {
  $output = '';
  $rows = array();
  if (!empty($info['features']) || !empty($info['dependencies']) || !empty($sources)) {
    foreach ($info['features'] as $component => $items) {
      if (!empty($items) || !empty($sources[$component])) {
        $rows[] = array(
          array('data' => $component, 'header' => TRUE),
          array('data' => $conflicts[$component], 'header' => TRUE),
        );
        $source_items = isset($sources[$component]) ? $sources[$component] : array();
        $rows[] = array(array('data' => theme('features_component_list', $items, $source_items), 'colspan' => 2, 'class' => 'component'));
      }
    }
    if ($show_dependencies && !empty($info['dependencies'])) {
      $rows[] = array(array('data' => t('dependencies'), 'header' => TRUE, 'colspan' => 2));
      $source_items = isset($sources['dependencies']) ? $sources['dependencies'] : $info['dependencies'];
      $rows[] = array(array('data' => theme('features_component_list', $info['dependencies'], $source_items), 'colspan' => 2, 'class' => 'component'));
    }
    $output .= theme('table', array(), $rows);
    $output .= $show_dependencies ? theme('features_component_key') : '';
  }
  return $output;
}

/**
 * Theme individual components in a component list.
 */
function theme_features_component_list($items, $source = array()) {
  $list = array();
  foreach ($items as $component) {
    // If component is not in source list, it was autodetected
    if (!in_array($component, $source)) {
      $list[] = "<span class='features-detected'>{$component}</span>";
    }
    else {
      $list[] = "<span class='features-source'>{$component}</span>";
    }
  }
  foreach ($source as $component) {
    // If a source component is no longer in the items, it was removed because
    // it is provided by a dependency.
    if (!in_array($component, $items)) {
      $list[] = "<span class='features-dependency'>{$component}</span>";
    }
  }
  return "<span class='features-component-list'>". implode(' ', $list) ."</span>";
}

/**
 * Provide a themed key for a component list.
 */
function theme_features_component_key() {
  $list = array();
  $list[] = "<span class='features-source'>" . t('Normal') . "</span>";
  $list[] = "<span class='features-detected'>" . t('Auto-detected') . "</span>";
  $list[] = "<span class='features-dependency'>" . t('Provided by dependency') . "</span>";
  return "<span class='features-component-list features-component-key'>". implode(' ', $list) ."</span>";
}
