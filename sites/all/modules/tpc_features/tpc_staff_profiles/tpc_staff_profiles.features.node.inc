<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _tpc_staff_profiles_node_info() {
  $items = array(
    'staff_profile' => array(
      'name' => t('Staff Profile'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Bio'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
