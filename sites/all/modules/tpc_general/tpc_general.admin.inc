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
  return system_settings_form($form);
}
