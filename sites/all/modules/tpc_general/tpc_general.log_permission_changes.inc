<?php
/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 * Pass original permissions to $form_state so that the values can be compared
 * for the watchdog log.
 */
function tpc_general_form_user_admin_perm_alter(&$form, &$form_state) {
  foreach ($form['checkboxes'] AS $rid => $perms) {
    $form_state['original'][$rid] = $perms['#default_value'];    
  }
  // Register the additional submit callback.
  $form['#submit'][] = 'tpc_general_user_admin_perm_submit';
}

/**
 * Submit callback for the admin permissions form. Compares and logs old and new
 * permissions settings.
 */
function tpc_general_user_admin_perm_submit($form, &$form_state) {
  // Determine the changes between the original and updated permissions.
  foreach ($form_state['original'] AS $rid => $perms_original) {
    $perms_updated = $form_state['values'][$rid];
    $removed[$rid] = array_diff($perms_original, $perms_updated);
    $added[$rid] = array_diff($perms_updated, $perms_original);
  }
  
  // Format the message for the log.
  foreach ($removed AS $rid => $perms) {
    if (empty($perms)) {
      continue;
    }
    $role_name = $form['role_names'][$rid]['#value'];
    $perms = implode(', ', $perms);
    $message .= '<p>';
    $message .= t('Removed from %role: %perms', array('%role' => $role_name, '%perms' => $perms));
    $message .= '</p>';
  }
  foreach ($added AS $rid => $perms) {
    if (empty($perms)) {
      continue;
    }
    $role_name = $form['role_names'][$rid]['#value'];
    $perms = implode(', ', $perms);
    $message .= '<p>';
    $message .= t('Added to %role: %perms', array('%role' => $role_name, '%perms' => $perms));
    $message .= '</p>';
  }
  global $user;
  
  // if Rules is installed, trigger the "changed permissions" event.
  if (function_exists('rules_invoke_event')) {
    rules_invoke_event('tpc_general_permissions_change', $user, $message);    
  }
  
  watchdog('permissions', $message);
}
