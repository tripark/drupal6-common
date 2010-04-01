<?php

/**
 * Implementation of hook_views_default_views().
 */
function tpc_twitter_feed_views_default_views() {
  module_load_include('inc', 'tpc_twitter_feed', 'tpc_twitter_feed.features.views');
  $args = func_get_args();
  return call_user_func_array('_tpc_twitter_feed_views_default_views', $args);
}
