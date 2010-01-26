<?php

/**
 * Implementation of hook_content_default_fields().
 */
function tpc_calendar_content_default_fields() {
  module_load_include('inc', 'tpc_calendar', 'tpc_calendar.defaults');
  return _tpc_calendar_content_default_fields();
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function tpc_calendar_imagecache_default_presets() {
  module_load_include('inc', 'tpc_calendar', 'tpc_calendar.defaults');
  return _tpc_calendar_imagecache_default_presets();
}

/**
 * Implementation of hook_node_info().
 */
function tpc_calendar_node_info() {
  module_load_include('inc', 'tpc_calendar', 'tpc_calendar.defaults');
  return _tpc_calendar_node_info();
}

/**
 * Implementation of hook_views_default_views().
 */
function tpc_calendar_views_default_views() {
  module_load_include('inc', 'tpc_calendar', 'tpc_calendar.defaults');
  return _tpc_calendar_views_default_views();
}
