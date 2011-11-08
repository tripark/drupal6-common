<?php
function tpc_general_admin_settings_form() {
  drupal_set_title('TPC General Settings');
  $form = array();
  $form['tpc_general_node_promote_disable'] = array(
    '#type' => 'checkbox',
    '#title' => 'Remove "Promoted to front page" checkbox on node forms',
    '#description' => 'This is often confusing to users since we almost never do anything with this.',
    '#default_value' => variable_get('tpc_general_node_promote_disable', 0),
  );
  $form['tpc_general_body_label_disable'] = array(
    '#type' => 'checkbox',
    '#title' => 'Remove "Body field label" option on node type forms',
    '#description' => 'Our typical pattern is to use a CCK field for the body instead of the core body field option.',
    '#default_value' => variable_get('tpc_general_body_label_disable', 0),
  );
  $form['tpc_general_core_blocks_disable'] = array(
    '#type' => 'checkbox',
    '#title' => 'Don\'t show blocks we almost never use on block admin interface',
    '#description' => 'This includes core blocks like "Powered by Drupal", "Execute PHP code", etc.',
    '#default_value' => variable_get('tpc_general_core_blocks_disable', 0),
  );
  $form['tpc_general_core_themes_disable'] = array(
    '#type' => 'checkbox',
    '#title' => 'Don\'t show themes we almost never use on themes admin interface',
    '#description' => 'No one uses the core themes.',
    '#default_value' => variable_get('tpc_general_core_themes_disable', 0),
  );
  $form['tpc_general_heartbeat_site_nid'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => 'Node ID of this site\'s entry on TPC Wiki',
    '#description' => 'Leave blank to stop sending automatic status updates to TPC.',
    '#default_value' => variable_get('tpc_general_heartbeat_site_nid', NULL),
  ); 
  $form['tpc_general_heartbeat_url'] = array(
    '#type' => 'textfield',
    '#title' => 'TPC Wiki XML-RPC processor',
    '#description' => 'The full URL of the xmlrpc.php file for the TPC Wiki.',
    '#default_value' => variable_get('tpc_general_heartbeat_url', 'https://www.tripark.org/wiki/xmlrpc.php'),
  ); 
  $form['tpc_general_wiki_ip'] = array(
    '#type' => 'textfield',
    '#title' => 'TPC Wiki IP address',
    '#description' => 'If set, allows TPC to ask for information about this website.',
    '#default_value' => variable_get('tpc_general_wiki_ip', '209.240.78.13,209.240.68.25'),
  ); 
  return system_settings_form($form);
}

/**
 * Print a JSON-encoded string of site information.
 */
function tpc_general_heartbeat_report() {
  $GLOBALS['conf']['cache'] = FALSE;
  drupal_set_header('Content-Type: text/plain; charset=UTF-8');
  $data = tpc_general_heartbeat_data();
  print json_encode($data);
}

/**
 * Returns an array of site information.
 */
function tpc_general_heartbeat_data() {
  // Determine enabled modules.
  $modules = array();
  $files = module_rebuild_cache();
  foreach ($files as $filename => $file) {
    if ($file->status && $file->info['package'] != 'Core - required' && !isset($file->info['hidden'])) {
      $modules[$filename] = array(
        'module' => $filename,
        'name' => $file->info['name'],
        'version' => $file->info['version'],
        'core' => $file->info['core'],
        'path' => array_shift(explode("/$filename", $file->filename)),
      );
    }
  }

  // Determine last cron run.
  $cron_last = variable_get('cron_last', '');
  
  // Count PHP errors recorded since last cron.
  $php_errors = db_result(db_query("SELECT COUNT(wid) FROM {watchdog} w WHERE w.type = 'php' AND timestamp > %d GROUP BY type", $cron_last));
  
  // Check for error-level status reports.
  module_load_include('inc', 'system', 'system.admin');
  $status_errors = system_status(TRUE);
  
  $data = array(
    'modules' => $modules,
    'cron' => $cron_last,
    'status_errors' => $status_errors,
    'php_errors' => $php_errors,
  );
  return $data;
}