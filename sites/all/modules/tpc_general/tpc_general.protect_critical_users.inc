<?php
/**
 * Code taken from protect_critical_users.module
 * http://drupal.org/project/protect_critical_users
 *
 * Prevents accidental deletion of superuser and anonymous user accounts.
 */

/**
 * Implementation of hook_init().
 */
function tpc_general_protect_critical_users_init() {
  global $user;

  // Catch user/*/delete requests.
  if (preg_match('#^user/([0-9]+)/delete#', $_GET['q'], $matches)) {
    $uid = (int)$matches[1];
    if ($uid == 0 || $uid == 1) {
      drupal_set_message(t('User @uid is a protected account and may not be deleted.', array('@uid' => $uid)), 'error');
      drupal_goto($uid == 0 ? '<front>' : 'user/'. $uid .'/edit');
    }
    else if ($uid == $user->uid) {
      drupal_set_message(t('You cannot delete your own account.'), 'error');
      drupal_goto('user/'. $uid .'/edit');
    }
  }

  // Catch delete requests from user administration.
  if (strpos($_GET['q'], 'admin/user/user') === 0) {
    if (!empty($_POST['accounts']) && isset($_POST['operation']) && ($_POST['operation'] == 'delete')) {
      if (isset($_POST['accounts'][1])) {
        drupal_set_message(t('User 1 is a protected account and may not be deleted.'), 'error');
        drupal_goto('admin/user/user');
      }
      else if (isset($_POST['accounts'][$user->uid])) {
        drupal_set_message(t('You cannot delete your own account.'), 'error');
        drupal_goto('admin/user/user');
      }
    }
  }
}

/**
 * Implementation of hook_form_alter().
 */
function tpc_general_form_user_profile_form_alter(&$form, &$form_state) {
  global $user;

  // Remove delete button from user 1 edit form.
  if (isset($form['delete']) && isset($form['_account']['#value']->uid)) {
    if ($form['_account']['#value']->uid == 1 || $form['_account']['#value']->uid == $user->uid) {
      unset($form['delete']);
    }
  }
}
