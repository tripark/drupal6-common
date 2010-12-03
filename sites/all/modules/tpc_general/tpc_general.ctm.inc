<?php
/**
 * Code adapted from Menu Settings per Content Type module
 * http://drupal.org/project/ctm
 */

/**
 * Implementation of hook_form_alter().
 */
function tpc_general_ctm_form_alter(&$form, &$form_state, $form_id) {
  // Add menu settings to the content type form.
  if ($form_id == 'node_type_form') {
		$form['tpc_general_ctm']['#type'] = 'fieldset';
		$form['tpc_general_ctm']['#title'] = t('Menu Settings');
		$form['tpc_general_ctm']['#collapsible'] = TRUE;
		$form['tpc_general_ctm']['tpc_general_ctm'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Menus enabled for this content type'),
			'#default_value' => variable_get($form['#node_type']->type.'_enabled_menus', array()),
			'#options' => menu_get_menus(),
		);
		$form['#submit'][] = 'tpc_general_ctm_node_type_form_submit';
  }
	
  // Filter the menu parent options on the node form.
  else if (isset($form['#node']) && $form['#node']->type . '_node_form' == $form_id) {
    $enabled_menus = variable_get($form['#node']->type . '_enabled_menus', FALSE);
		if (is_array($enabled_menus)) {
      if (empty($enabled_menus)) {
        $form['menu']['#access'] = FALSE;
      }
      else {
        // Check for an existing menu selection, and add the to the options if
        // it isn't present.
        if (!empty($form['menu']['parent']['#default_value'])) {
          list($current_menu) = explode(':', $form['menu']['parent']['#default_value']);
          if (!in_array($current_menu, $enabled_menus)) {
            $enabled_menus[$current_menu] = $current_menu;
          }
        }
        $menus = array_intersect_key(menu_get_menus(), $enabled_menus);
        $options = menu_parent_options($menus, $form['menu']['#item']);
        $form['menu']['parent']['#options'] = $options;
      }
		}
  }
}

/**
 *  Save enabled menus settings on the content type edit form.
 */
function tpc_general_ctm_node_type_form_submit(&$form, $form_state) {
  $enabled_menus = array_filter($form_state['values']['tpc_general_ctm']);
  variable_set($form_state['values']['type'].'_enabled_menus', $enabled_menus);
}

/**
 * Implements hook_node_type().
 */
function tpc_general_ctm_node_type($op, $info) {
  switch ($op) {
    case 'delete':
      variable_del($info->type . '_enabled_menus');
      break;
    case 'update':
      if (!empty($info->old_type) && $info->old_type != $info->type) {
        $enabled_menus = variable_get($info->old_type . '_enabled_menus', array());
        variable_del($info->old_type . '_enabled_menus');
        if (!empty($enabled_menus)) {
          variable_set($info->type . '_enabled_menus', $enabled_menus);
        }
      }
      break;
  }
}
