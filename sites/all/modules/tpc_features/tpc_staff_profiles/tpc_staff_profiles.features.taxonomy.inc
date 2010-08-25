<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function tpc_staff_profiles_taxonomy_default_vocabularies() {
  return array(
    'staff_type' => array(
      'name' => 'Staff Profile type',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '1',
      'required' => '1',
      'tags' => '0',
      'module' => 'features_staff_type',
      'weight' => '0',
      'type' => NULL,
      'nodes' => array(),
    ),
  );
}
