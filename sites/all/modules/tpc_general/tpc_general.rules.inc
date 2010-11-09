<?php
/**
 * Implementation of hook_rules_event_info().
 *
 * Register a Rules event for saving permissions.
 */
function tpc_general_rules_event_info() {
  $events = array(
    'tpc_general_permissions_change' => array(
      'label' => t('User has changed permission settings'),
      'module' => 'tpc_general',
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User who changed the permisssions.')),
        'summary' => array('type' => 'string', 'label' => t('Summary of permissions changes.')),
      ),
    ),
  );
  return $events;
}
