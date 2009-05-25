<?php
// $Id: captcha.inc,v 1.4 2009/05/03 21:20:46 soxofaan Exp $

/**
 * Helper function for adding/updating a CAPTCHA point.
 *
 * @param $form_id the form ID to configure.
 * @param captcha_type the setting for the given form_id,
 *   can be 'none' to disable CAPTCHA, 'default' to use the default challenge type
 *   or something of form 'image_captcha/Image'. If capcha_type is NULL, the entry for the
 *   CAPTCHA type will be removed.
 * @return nothing
 *
 * @todo captcha_get_form_id_setting and captcha_set_form_id_setting use a different way to
 * communicate the CAPTCHA type (object with fields and string respectively). Make this more symmetric?
 */
function captcha_set_form_id_setting($form_id, $captcha_type) {
  if ($captcha_type == 'none') {
    db_query("DELETE FROM {captcha_points} WHERE form_id = '%s'", $form_id);
    db_query("INSERT INTO {captcha_points} (form_id, module, type) VALUES ('%s', NULL, NULL)", $form_id);
  }
  elseif ($captcha_type == 'default') {
    db_query("DELETE FROM {captcha_points} WHERE form_id = '%s'", $form_id);
    db_query("INSERT INTO {captcha_points} (form_id, module, type) VALUES ('%s', NULL, '%s')", $form_id, 'default');
  }
  elseif ($captcha_type == NULL) {
    db_query("DELETE FROM {captcha_points} WHERE form_id = '%s'", $form_id);
  }
  elseif (substr_count($captcha_type, '/') == 1) {
    list($module, $type) = explode('/', $captcha_type);
    db_query("DELETE FROM {captcha_points} WHERE form_id = '%s'", $form_id);
    db_query("INSERT INTO {captcha_points} (form_id, module, type) VALUES ('%s', '%s', '%s')", $form_id, $module, $type);
  }
  else {
    drupal_set_message(t('Failed to set a CAPTCHA type for form %form_id: could not interpret value "@captcha_type"',
      array('%form_id' => $form_id, '@captcha_type' => $captcha_type)), 'warning');
  }
}

/**
 * Get the CAPTCHA setting for a given form_id.
 *
 * @param $form_id the form_id to query for
 * @param $symbolic flag to return as (symbolic) strings instead of object.
 *
 * @return NULL if no setting is known
 *   or a captcha_point object with fields 'module' and 'type'.
 *   If argument $symbolic is true, returns (symbolic) as 'none', 'default'
 *   or in the form 'captcha/Math'.
 */
function captcha_get_form_id_setting($form_id, $symbolic=FALSE) {
  $result = db_query("SELECT module, type FROM {captcha_points} WHERE form_id = '%s'", $form_id);
  if (!$result) {
    return NULL;
  }
  $captcha_point = db_fetch_object($result);
  if (!$captcha_point) {
    $captcha_point = NULL;
  }
  elseif ($captcha_point->type == 'default') {
    if (!$symbolic) {
      list($module, $type) = explode('/', variable_get('captcha_default_challenge', 'captcha/Math'));
      $captcha_point->module = $module;
      $captcha_point->type = $type;
    }
    else {
      $captcha_point = 'default';
    }
  }
  elseif ($captcha_point->module == NULL && $captcha_point->type == NULL && $symbolic) {
    $captcha_point = 'none';
  }
  elseif ($symbolic) {
    $captcha_point = $captcha_point->module .'/'. $captcha_point->type;
  }
  return $captcha_point;
}


/**
 * Helper function for generating a new CAPTCHA session.
 *
 * @param $form_id the form_id of the form to add a CAPTCHA to.
 * @param $status the initial status of the CAPTHCA session.
 * @return the session ID of the new CAPTCHA session.
 */
function _captcha_generate_captcha_session($form_id=NULL, $status=CAPTCHA_STATUS_UNSOLVED) {
  global $user;
  db_query("INSERT into {captcha_sessions} (uid, sid, ip_address, timestamp, form_id, solution, status, attempts) VALUES (%d, '%s', '%s', %d, '%s', '%s', %d, %d)", $user->uid, session_id(), ip_address(), time(), $form_id, 'undefined', $status, 0);
  $captcha_sid = db_last_insert_id('captcha_sessions', 'csid');
  return $captcha_sid;
}

/**
 * Helper function for updating the solution in the CAPTCHA session table.
 *
 * @param $captcha_sid the CAPTCHA session ID to update.
 * @param $solution the new solution to associate with the given CAPTCHA session.
 */
function _captcha_update_captcha_session($captcha_sid, $solution) {
  db_query("UPDATE {captcha_sessions} SET timestamp=%d, solution='%s' WHERE csid=%d", time(), $solution, $captcha_sid);
}

/**
 * Helper function for checking if CAPTCHA is required for user,
 * based on CAPTCHA session ID and user session info.
 */
function _captcha_required_for_user($captcha_sid, $form_id) {
  $captcha_session_status = db_result(db_query("SELECT status FROM {captcha_sessions} WHERE csid = %d", $captcha_sid));

  $captcha_success_form_ids = (array)($_SESSION['captcha_success_form_ids']);
  switch (variable_get('captcha_persistence', CAPTCHA_PERSISTENCE_SHOW_ALWAYS)) {
    case CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL:
      $captcha_persistence_status = (count($captcha_success_form_ids) > 0);
      break;
    case CAPTCHA_PERSISTENCE_SKIP_ONCE_SUCCESSFUL_PER_FORM:
      $captcha_persistence_status = isset($captcha_success_form_ids[$form_id]);
      break;
    default:
      $captcha_persistence_status = FALSE;
  }

  return ($captcha_session_status == CAPTCHA_STATUS_UNSOLVED) && !$captcha_persistence_status;
}

/**
 * Get the description which appears above the CAPTCHA in forms.
 * If the locale module is enabled, an optional language code can be given
 */
function _captcha_get_description($lang_code=NULL) {
  $default = t('This question is for testing whether you are a human visitor and to prevent automated spam submissions.');
  if (module_exists('locale')) {
    if ($lang_code == NULL) {
      global $language;
      $lang_code = $language->language;
    }
    $description = variable_get("captcha_description_$lang_code", $default);
  }
  else {
    $description = variable_get('captcha_description', $default);
  }
  return $description;
}

/**
 * Parse or interpret the given captcha_type.
 * @param $captcha_type string representation of the CAPTCHA type,
 *      e.g. 'default', 'none', 'captcha/Math', 'image_captcha/Image'
 * @return list($captcha_module, $captcha_type)
 */
function _captcha_parse_captcha_type($captcha_type) {
  if ($captcha_type == 'none') {
    return array(NULL, NULL);
  }
  if ($captcha_type == 'default') {
    $captcha_type = variable_get('captcha_default_challenge', 'captcha/Math');
  }
  return explode('/', $captcha_type);
}

/**
 * Helper function to get placement information for a given form_id.
 * @param $form_id the form_id to get the placement information for.
 * @param $form if a form corresponding to the given form_id, if there
 *   is no placement info for the given form_id, this form is examined to
 *   guess the placement.
 * @return placement info array (@see _captcha_insert_captcha_element() for more
 *   info about the fields 'path', 'key' and 'weight'.
 */
function _captcha_get_captcha_placement($form_id, $form) {
  // Get CAPTCHA placement map from cache. Two levels of cache:
  // static variable in this function and storage in the variables table.
  static $placement_map = NULL;
  // Try first level cache.
  if ($placement_map === NULL) {
    // If first level cache missed: try second level cache.
    $placement_map = variable_get('captcha_placement_map_cache', NULL);
    // TODO: add UI in admin UI for flushing this cache.

    if ($placement_map === NULL) {
      // If second level cache missed: start from a fresh placement map.
      $placement_map = array();
      // TODO: prefill with some hard coded default entries as follows?
      // $placement_map['comment_form'] = array('path' => array(), 'key' => NULL, 'weight' => 18.9);
      // $placement_map['user_login'] = array('path' => array(), 'key' => NULL, 'weight' => 1.9);
      // TODO: also make the placement 'overridable' from the admin UI?
    }
  }

  // Query the placement map.
  if (array_key_exists($form_id, $placement_map)) {
    $placement = $placement_map[$form_id];
  }
  // If no placement info is available in placement map:
  // search the form for buttons and guess placement from it.
  else {
    $buttons = _captcha_search_buttons($form);
    // TODO: make this more sofisticated? Use cases needed.
    // Use case: the CAPTCHA examples page (has no buttons).
    $placement = $buttons[0];
    // Store calculated placement in caches.
    $placement_map[$form_id] = $placement;
    variable_set('captcha_placement_map_cache', $placement_map);
  }

  return $placement;
}

/**
 * Helper function for searching the buttons in a form.
 *
 * @param $form the form to search button elements in
 * @return an array of paths to the buttons.
 *   A path is an array of keys leading to the button, the last
 *   item in the path is the weight of the button element
 *   (or NULL if undefined).
 */
function _captcha_search_buttons($form) {
  $buttons = array();
  foreach (element_children($form) as $key) {
    // Look for submit or button type elements.
    if (isset($form[$key]['#type']) && ($form[$key]['#type'] == 'submit' || $form[$key]['#type'] == 'button')) {
      $weight = isset($form[$key]['#weight']) ? $form[$key]['#weight'] : NULL;
      $buttons[] = array(
        'path' => array(),
        'key' => $key,
        'weight' => $weight,
      );
    }
    // Process children recurively.
    $children_buttons = _captcha_search_buttons($form[$key]);
    foreach ($children_buttons as $b) {
      $b['path'] = array_merge(array($key), $b['path']);
      $buttons[] = $b;
    }
  }
  return $buttons;
}

/**
 * Helper function to insert a CAPTCHA element in a form before a given form element.
 * @param $form the form to add the CAPTCHA element to.
 * @param $placement information where the CAPTCHA element should be inserted.
 *   $target should be an associative array with fields:
 *     - 'path': path of the container in the form where the CAPTCHA element should be inserted.
 *     - 'key': the key of the element before which the CAPTCHA element
 *       should be inserted. If the field 'key' is undefined or NULL, the CAPTCHA will
 *       just be appended to the container.
 *     - 'weight': if 'key' is not NULL: should be the weight of the element defined by 'key'.
 *       If 'key' is NULL and weight is not NULL: set the weight property of the CAPTCHA element
 *       to this value.
 * @param $captcha_element the CAPTCHA element to insert.
 */
function _captcha_insert_captcha_element(&$form, $placement, $captcha_element) {
  // Get common path, target and target weight.
  $target_key = $placement['key'];
  $target_weight = $placement['weight'];
  $path = $placement['path'];

  // Walk through the form along the path.
  $form_stepper = &$form;
  foreach ($path as $step) {
    if (isset($form_stepper[$step])) {
      $form_stepper = & $form_stepper[$step];
    }
    else {
      // Given path is invalid: stop stepping and
      // continue in best effort (append instead of insert).
      $target_key = NULL;
      break;
    }
  }

  // If no target is available: just append the CAPTCHA element to the container.
  if ($target_key == NULL || !array_key_exists($target_key, $form_stepper)) {
    // Optionally, set weight of CAPTCHA element.
    if ($target_weight != NULL) {
      $captcha_element['#weight'] = $target_weight;
    }
    $form_stepper['captcha'] =  $captcha_element;
  }
  // If there is a target available: make sure the CAPTCHA element comes right before it.
  else {
    // If target has a weight: set weight of CAPTCHA element a bit smaller
    // and just append the CAPTCHA: sorting will fix the ordering anyway.
    if ($target_weight != NULL) {
      $captcha_element['#weight'] = $target_weight - .1;
      $form_stepper['captcha'] =  $captcha_element;
    }
    else {
      // If we can't play with weights: insert the CAPTCHA element at the right position.
      // Because PHP lacks a function for this (array_splice() comes close,
      // but it does not preserve the key of the inserted element), we do it by hand:
      // chop of the end, append the CAPTCHA element and put the end back.
      $offset = array_search($target_key, array_keys($form_stepper));
      $end = array_splice($form_stepper, $offset);
      $form_stepper['captcha'] =  $captcha_element;
      foreach($end as $k => $v) {
        $form_stepper[$k] = $v;
      }
    }
  }
}

