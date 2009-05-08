<?php
// $Id: captcha.admin.inc,v 1.19 2009/04/15 20:58:01 soxofaan Exp $

/**
 * Return an array with the available CAPTCHA types, for use as options array
 * for a select form elements.
 * The array is an associative array mapping "$module/$type" to
 * "$type ($module)" with $module the module name implementing the CAPTCHA
 * and $type the name of the CAPTCHA type.
 * (It also includes a 'none' => '<none>' option)
 */
function _captcha_available_challenge_types() {
  $captcha_types['none'] = '<'. t('none') .'>';
  foreach (module_implements('captcha') as $module) {
    $result = call_user_func_array($module .'_captcha', 'list');
    if (is_array($result)) {
      foreach ($result as $type) {
        $captcha_types["$module/$type"] = "$type ($module)";
      }
    }
  }
  return $captcha_types;
}

/**
 * Form builder function for the general CAPTCHA configuration
 */
function captcha_admin_settings() {
  module_load_include('inc', 'captcha');

  // Configuration of which forms to protect, with what challenge.
  $form['captcha_form_protection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form protection'),
    '#description' => t('Select the challenge type you want for each of the listed forms (identified by their so called <em>form_id</em>\'s). You can easily add arbitrary forms with textfield at the bottom of the table or with the help of the \'%CAPTCHA_admin_links\' option below.',
      array('%CAPTCHA_admin_links' => t('Add CAPTCHA administration links to forms'))
      ),
  );
  // List known form_ids.
  $form['captcha_form_protection']['captcha_form_id_overview'] = array(
    '#theme' => 'captcha_admin_settings_captcha_points',
    '#tree' => TRUE,
  );
  $form['captcha_form_protection']['captcha_form_id_overview']['captcha_captcha_points'] = array();
  $captcha_types = _captcha_available_challenge_types();
  $result = db_query("SELECT * FROM {captcha_points} ORDER BY form_id");
  while ($captcha_point = db_fetch_object($result)) {
    $form['captcha_form_protection']['captcha_form_id_overview']['captcha_captcha_points'][$captcha_point->form_id] = array();
    $form['captcha_form_protection']['captcha_form_id_overview']['captcha_captcha_points'][$captcha_point->form_id]['form_id'] = array(
      '#value' => $captcha_point->form_id,
    );
    // Select widget for CAPTCHA type.
    $form['captcha_form_protection']['captcha_form_id_overview']['captcha_captcha_points'][$captcha_point->form_id]['captcha_type'] = array(
      '#type' => 'select',
      '#default_value' => isset($captcha_point->module) ? "{$captcha_point->module}/{$captcha_point->type}": "none",
      '#options' => $captcha_types,
    );
    // Additional operations.
    $form['captcha_form_protection']['captcha_form_id_overview']['captcha_captcha_points'][$captcha_point->form_id]['operations'] = array(
      '#value' => implode(", ", array(
        l(t('delete'), "admin/user/captcha/captcha/captcha_point/{$captcha_point->form_id}/delete"),
      ))
    );
  }

  // Form items for new form_id.
  $form['captcha_form_protection']['captcha_form_id_overview']['captcha_new_captcha_point'] = array();
  // Textfield for form_id.
  $form['captcha_form_protection']['captcha_form_id_overview']['captcha_new_captcha_point']['form_id'] = array(
    '#type' => 'textfield',
    '#size' => 16,
  );
  // Select widget for CAPTCHA type.
  $form['captcha_form_protection']['captcha_form_id_overview']['captcha_new_captcha_point']['captcha_type'] = array(
    '#type' => 'select',
    '#default_value' => "{$captcha_point->module}/{$captcha_point->type}",
    '#options' => $captcha_types,
  );

  // Field for the CAPTCHA administration mode
  $form['captcha_form_protection']['captcha_administration_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add CAPTCHA administration links to forms'),
    '#default_value' => variable_get('captcha_administration_mode', FALSE),
    '#description' => t('This option is very helpful to enable/disable challenges on forms. When enabled, users with the "%admincaptcha" permission will see CAPTCHA administration links on all forms (except on administrative pages, which shouldn\'t be accessible to untrusted users in the first place). These links make it possible to enable a challenge of the desired type or disable it.', array('%admincaptcha' => t('administer CAPTCHA settings'))),
  );

  // field(s) for setting the additional CAPTCHA description
  if (module_exists('locale')) {
    $langs = locale_language_list();
    $form['captcha_descriptions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Challenge description'),
      '#description' => t('With this description you can explain the purpose of the challenge to the user.'),
    );
    foreach ($langs as $lang_code => $lang_name) {
      $form['captcha_descriptions']["captcha_description_$lang_code"] = array(
        '#type' => 'textfield',
        '#title' => t('For language %lang_name (code %lang_code)', array('%lang_name' => $lang_name, '%lang_code' => $lang_code)),
        '#default_value' => _captcha_get_description($lang_code),
        '#maxlength' => 256,
      );
    }
  }
  else {
    $form['captcha_description'] = array(
      '#type' => 'textfield',
      '#title' => t('Challenge description'),
      '#description' => t('With this description you can explain the purpose of the challenge to the user.'),
      '#default_value' => _captcha_get_description(),
      '#maxlength' => 256,
    );
  }

  // Field for CAPTCHA persistence.
  $form['captcha_persistence'] = array(
    '#type' => 'radios',
    '#title' => t('Persistence'),
    '#default_value' => variable_get('captcha_persistence', CAPTCHA_PERSISTENCE_SHOW_ALWAYS),
    '#options' => array(
      CAPTCHA_PERSISTENCE_SHOW_ALWAYS => t('Always add a challenge.'),
      CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL_PER_FORM => t('Omit challenges for a form once the user has successfully responded to a challenge for that form.'),
      CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL => t('Omit challenges for all forms once the user has successfully responded to a challenge.'),
    ),
    '#description' => t('Define if challenges should be omitted during the rest of a session once the user successfully responses to a challenge.'),
  );

  // Option for logging wrong responses.
  $form['captcha_log_wrong_responses'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log wrong responses'),
    '#description' => t('Report information about wrong responses to the !log.', array('!log' => l(t('log'), 'admin/reports/dblog'))),
    '#default_value' => variable_get('captcha_log_wrong_responses', FALSE),
  );

  // Submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Custom theme function for a table of (form_id -> CAPTCHA type) settings
 */
function theme_captcha_admin_settings_captcha_points($form) {
  $header = array('form_id', t('Challenge type (module)'), t('Operations'));
  $rows = array();
  // Existing CAPTCHA points.
  foreach (element_children($form['captcha_captcha_points']) as $key) {
    $row = array();
    $row[] = drupal_render($form['captcha_captcha_points'][$key]['form_id']);
    $row[] = drupal_render($form['captcha_captcha_points'][$key]['captcha_type']);
    $row[] = drupal_render($form['captcha_captcha_points'][$key]['operations']);
    $rows[] = $row;
  }
  // For new CAPTCHA point.
  $row = array();
  $row[] = drupal_render($form['captcha_new_captcha_point']['form_id']);
  $row[] = drupal_render($form['captcha_new_captcha_point']['captcha_type']);
  $row[] = '';
  $rows[] = $row;

  $output = theme('table', $header, $rows);
  return $output;
}

/**
 * Validation handler for captcha_admin_settings form.
 */
function captcha_admin_settings_validate($form, $form_state) {
  $form_id = $form_state['values']['captcha_form_id_overview']['captcha_new_captcha_point']['form_id'];
  if (!preg_match('/^[a-z0-9_]*$/', $form_id)) {
    form_set_error('captcha_form_id_overview][captcha_new_captcha_point][form_id', t('Illegal form_id'));
  }
}

/**
 * Submission function for captcha_admin_settings form.
 */
function captcha_admin_settings_submit($form, &$form_state) {

  variable_set('captcha_administration_mode', $form_state['values']['captcha_administration_mode']);

  // Process CAPTCHA points
  foreach ($form_state['values']['captcha_form_id_overview']['captcha_captcha_points'] as $captcha_point_form_id => $data) {
    captcha_set_form_id_setting($captcha_point_form_id, $data['captcha_type']);
  }

  // Add new CAPTCHA point?
  $captcha_point_form_id = $form_state['values']['captcha_form_id_overview']['captcha_new_captcha_point']['form_id'];
  if (!empty($captcha_point_form_id)) {
    $captcha_type = $form_state['values']['captcha_form_id_overview']['captcha_new_captcha_point']['captcha_type'];
    captcha_set_form_id_setting($captcha_point_form_id, $captcha_type);
    drupal_set_message(t('Added CAPTCHA point.'), 'status');
  }

  // CAPTCHA description stuff.
  if (module_exists('locale')) {
    $langs = locale_language_list();
    foreach ($langs as $lang_code => $lang_name) {
      variable_set("captcha_description_$lang_code", $form_state['values']["captcha_description_$lang_code"]);
    }
  }
  else {
    variable_set('captcha_description', $form_state['values']['captcha_description']);
  }

  variable_set('captcha_persistence', $form_state['values']['captcha_persistence']);

  variable_set('captcha_log_wrong_responses', $form_state['values']['captcha_log_wrong_responses']);

  drupal_set_message(t('The CAPTCHA settings were saved.'), 'status');
}


/**
 * Central handler for CAPTCHA point administration (adding, disabling, deleting)
 */
function captcha_point_admin($captcha_point_form_id=NULL, $op=NULL) {
  // if $captcha_point_form_id and action $op given: do the action
  if ($captcha_point_form_id) {
    switch ($op) {
      case 'disable':
        return drupal_get_form('captcha_point_disable_confirm', $captcha_point_form_id, FALSE);
      case 'delete':
        return drupal_get_form('captcha_point_disable_confirm', $captcha_point_form_id, TRUE);
    }
    // return edit form for CAPTCHA point
    return drupal_get_form('captcha_point_admin_form', $captcha_point_form_id);
  }
  // return add form for CAPTCHA point
  return drupal_get_form('captcha_point_admin_form');
}

function captcha_point_admin_form($form_state, $captcha_point_form_id=NULL) {
  $form = array();
  $default_captcha_type = 'none';
  if (isset($captcha_point_form_id)) {
    // use given CAPTCHA point form_id
    $form['captcha_point_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form ID'),
      '#description' => t('The Drupal form_id of the form to add the CAPTCHA to.'),
      '#value' => check_plain($captcha_point_form_id),
      '#disabled' => TRUE,
    );
    $captcha_point = captcha_get_form_id_setting($captcha_point_form_id);
    if ($captcha_point) {
      $default_captcha_type = "{$captcha_point->module}/{$captcha_point->type}";
    }
  }
  else {
    // textfield for CAPTCHA point form_id
    $form['captcha_point_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form ID'),
      '#description' => t('The Drupal form_id of the form to add the CAPTCHA to.'),
    );
  }
  // select widget for CAPTCHA type
  $form['captcha_type'] = array(
    '#type' => 'select',
    '#title' => t('Challenge type'),
    '#description' => t('The CAPTCHA type to use for this form'),
    '#default_value' => $default_captcha_type,
    '#options' => _captcha_available_challenge_types(),
  );
  // redirect to general CAPTCHA settings page after submission
  $form['#redirect'] = 'admin/user/captcha';
  // submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}


/**
 * validation function for captcha_point_admin_form
 */
function captcha_point_admin_form_validate($form, $form_state) {
  if (!preg_match('/^[a-z0-9_]+$/', $form_state['values']['captcha_point_form_id'])) {
    form_set_error('captcha_point_form_id', t('Illegal form_id'));
  }
}


/**
 * Submit function for captcha_point_admin_form.
 */
function captcha_point_admin_form_submit($form, $form_state) {
  $captcha_point_form_id = $form_state['values']['captcha_point_form_id'];
  $captcha_type = $form_state['values']['captcha_type'];
  captcha_set_form_id_setting($captcha_point_form_id, $captcha_type);
  drupal_set_message(t('Saved CAPTCHA point settings.'), 'status');
}

/**
 * Confirm dialog for disabling/deleting a CAPTCHA point
 */
function captcha_point_disable_confirm(&$form_state, $captcha_point_form_id, $delete) {
  $form = array();
  $form['captcha_point_form_id'] = array(
    '#type' => 'value',
    '#value' => $captcha_point_form_id,
  );
  $form['captcha_point_delete'] = array(
    '#type' => 'value',
    '#value' => $delete,
  );
  if ($delete) {
    $message = t('Are you sure you want to delete the CAPTCHA for form_id %form_id?', array('%form_id' => $captcha_point_form_id));
    $yes = t('Delete');
  }
  else {
    $message = t('Are you sure you want to disable the CAPTCHA for form_id %form_id?', array('%form_id' => $captcha_point_form_id));
    $yes = t('Disable');
  }
  return confirm_form($form, $message, 'admin/user/captcha/captcha', '', $yes);
}

/**
 * submission handler of CAPTCHA point disabling/deleting confirm_form
 */
function captcha_point_disable_confirm_submit($form, &$form_state) {
  $captcha_point_form_id = $form_state['values']['captcha_point_form_id'];
  $delete = $form_state['values']['captcha_point_delete'];
  if ($delete) {
    db_query("DELETE FROM {captcha_points} WHERE form_id = '%s'", $captcha_point_form_id);
    drupal_set_message(t('Deleted CAPTCHA for form %form_id.', array('%form_id' => $captcha_point_form_id)));
  }
  else {
    db_query("UPDATE {captcha_points} SET module = NULL, type = NULL WHERE form_id = '%s'", $captcha_point_form_id);
    drupal_set_message(t('Disabled CAPTCHA for form %form_id.', array('%form_id' => $captcha_point_form_id)));
  }
  $form_state['redirect'] = 'admin/user/captcha/captcha';
}

/**
 * Helper function for generating an example challenge.
 * TODO: port this to hook_elements() usage.
 */
function _captcha_generate_example_challenge($module, $type) {
  $captcha_sid = _captcha_generate_captcha_session(CAPTCHA_STATUS_EXAMPLE);
  $captcha = call_user_func_array($module .'_captcha', array('generate', $type, $captcha_sid));
  _captcha_update_captcha_session($captcha_sid, $captcha['solution']);
  $captcha['form']['captcha_response']['#default_value'] = $captcha['solution'];
  return $captcha['form'];
}

/**
 * Funtion for generating a page with CAPTCHA examples.
 *
 * If the arguments $module and $challenge are not set, generate a list with
 * examples of the available CAPTCHA types.
 * If $module and $challenge are set, generate 10 examples of the concerning
 * CAPTCHA.
 */
function captcha_examples($form_state, $module, $challenge) {
  module_load_include('inc', 'captcha');

  $form = array();
  if ($module && $challenge) {
    // Generate 10 example challenges.
    for ($i=0; $i<10; $i++) {
      $form["challenge_{$i}"] = _captcha_generate_example_challenge($module, $challenge);
    }
  }
  else {
    // generate a list with examples of the available CAPTCHA types
    $form['info'] = array(
      '#value' => t('This page gives an overview of all available challenge types, generated with their current settings.'),
    );
    foreach (module_implements('captcha') as $mkey => $module) {
      $challenges = call_user_func_array($module .'_captcha', 'list');
      if ($challenges) {
        foreach ($challenges as $ckey => $challenge) {
          $form["captcha_{$mkey}_{$ckey}"] = array(
            '#type' => 'fieldset',
            '#title' => t('Challenge "%challenge" by module "%module"', array('%challenge' => $challenge, '%module' => $module)),
            'challenge' => _captcha_generate_example_challenge($module, $challenge),
            'more_examples' => array(
              '#value' => l(t('10 more examples of this challenge.'), "admin/user/captcha/captcha/examples/$module/$challenge"),
            ),
          );
        }
      }
    }
  }
  return $form;
}
