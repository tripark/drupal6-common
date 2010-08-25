<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function tpc_staff_profiles_imagecache_default_presets() {
  $items = array(
    'portrait' => array(
      'presetname' => 'portrait',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '100',
            'height' => '150',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'staff_profile_full' => array(
      'presetname' => 'staff_profile_full',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '250',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'staff_profile_thumb' => array(
      'presetname' => 'staff_profile_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '100',
            'height' => '150',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function tpc_staff_profiles_node_info() {
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

/**
 * Implementation of hook_views_api().
 */
function tpc_staff_profiles_views_api() {
  return array(
    'api' => '2',
  );
}
