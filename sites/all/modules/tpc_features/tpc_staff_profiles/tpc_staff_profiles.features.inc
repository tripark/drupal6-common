<?php

/**
 * Implementation of hook_content_default_fields().
 */
function tpc_staff_profiles_content_default_fields() {
  module_load_include('inc', 'tpc_staff_profiles', 'tpc_staff_profiles.defaults');
  $args = func_get_args();
  return call_user_func_array('_tpc_staff_profiles_content_default_fields', $args);
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function tpc_staff_profiles_imagecache_default_presets() {
  module_load_include('inc', 'tpc_staff_profiles', 'tpc_staff_profiles.defaults');
  $args = func_get_args();
  return call_user_func_array('_tpc_staff_profiles_imagecache_default_presets', $args);
}

/**
 * Implementation of hook_node_info().
 */
function tpc_staff_profiles_node_info() {
  module_load_include('inc', 'tpc_staff_profiles', 'tpc_staff_profiles.features.node');
  $args = func_get_args();
  return call_user_func_array('_tpc_staff_profiles_node_info', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function tpc_staff_profiles_views_default_views() {
  module_load_include('inc', 'tpc_staff_profiles', 'tpc_staff_profiles.features.views');
  $args = func_get_args();
  return call_user_func_array('_tpc_staff_profiles_views_default_views', $args);
}
