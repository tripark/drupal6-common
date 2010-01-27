<?php

/**
 * Helper to implemetation of hook_content_default_fields().
 */
function _tpc_calendar_content_default_fields() {
  $fields = array();

  // Exported field: field_event_as_appear
  $fields[] = array(
    'field_name' => 'field_event_as_appear',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_event_as_appear][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Date',
      'weight' => '-3',
      'description' => 'Date as you would like it to appear.',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_event_date
  $fields[] = array(
    'field_name' => 'field_event_date',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      '4' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'date',
    'required' => '1',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'site',
    'todate' => 'optional',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'medium',
    'widget' => array(
      'default_value' => 'blank',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'input_format' => 'F j, Y - g:ia',
      'input_format_custom' => '',
      'increment' => '1',
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Date',
      'weight' => '-4',
      'description' => '',
      'type' => 'date_popup',
      'module' => 'date',
    ),
  );

  // Exported field: field_event_image
  $fields[] = array(
    'field_name' => 'field_event_image',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => 0,
      'parent' => '',
      '4' => array(
        'format' => 'event_fullsize_default',
        'exclude' => 0,
      ),
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'event_thumbnail_linked',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'event_fullsize_default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => 'png jpg jpeg gif',
      'file_path' => 'event_images',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => 0,
      'min_resolution' => 0,
      'alt' => '',
      'custom_alt' => 1,
      'title' => '',
      'custom_title' => 1,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'label' => 'Image',
      'weight' => 0,
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_event_location
  $fields[] = array(
    'field_name' => 'field_event_location',
    'type_name' => 'event',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '100',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_event_location][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Location',
      'weight' => '-1',
      'description' => 'The location where this event will be held.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Translatables
  array(
    t('Date'),
    t('Image'),
    t('Location'),
  );

  return $fields;
}

/**
 * Helper to implemetation of hook_context_default_contexts().
 */
function _tpc_calendar_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'tpc_calendar',
    'attribute' => 'calendar',
    'value' => 'tpc_calendar',
    'description' => 'Calendar of upcoming events',
    'node' => array(
      '0' => 'event',
    ),
    'views' => array(
      '0' => 'Event_Calendar',
      '1' => 'Event_Lists',
      '2' => 'Event_Lists:page_1',
      '3' => 'Event_Lists:page_2',
    ),
    'block' => array(
      'views_Event_Calendar-calendar_block_1' => array(
        'module' => 'views',
        'delta' => 'Event_Calendar-calendar_block_1',
        'weight' => 20,
        'region' => 'right',
        'status' => '0',
        'label' => 'Calendar',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implemetation of hook_imagecache_default_presets().
 */
function _tpc_calendar_imagecache_default_presets() {
  $items = array(
    'event_fullsize' => array(
      'presetname' => 'event_fullsize',
      'actions' => array(
        '0' => array(
          'actionid' => '6',
          'presetid' => '6',
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '480',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'event_thumbnail' => array(
      'presetname' => 'event_thumbnail',
      'actions' => array(
        '0' => array(
          'actionid' => '5',
          'presetid' => '5',
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '150',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implemetation of hook_node_info().
 */
function _tpc_calendar_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'module' => 'node',
      'description' => t('Add an event to the calendar.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
    ),
  );
  return $items;
}

/**
 * Helper to implemetation of hook_views_default_views().
 */
function _tpc_calendar_views_default_views() {
  $views = array();

  // Exported view: Event_Calendar
  $view = new view;
  $view->name = 'Event_Calendar';
  $view->description = 'A multi-dimensional calendar view with back/next navigation.';
  $view->tag = 'Event Calendar';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'field' => 'title',
      'table' => 'node',
      'relationship' => 'none',
    ),
    'changed' => array(
      'label' => '',
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'changed',
      'field' => 'changed',
      'table' => 'node',
      'relationship' => 'none',
      'date_format' => 'small',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_event_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'date_argument' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'date',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'year_range' => '-3:+3',
      'date_method' => 'OR',
      'granularity' => 'month',
      'id' => 'date_argument',
      'table' => 'node',
      'field' => 'date_argument',
      'relationship' => 'none',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'article' => 0,
        'book' => 0,
        'event' => 0,
        'feed' => 0,
        'legislative_update' => 0,
        'newsletter' => 0,
        'page' => 0,
        'policy_blog_post' => 0,
        'profile' => 0,
        'video' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '5' => 0,
        '6' => 0,
        '3' => 0,
        '2' => 0,
        '1' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
      'default_options_div_prefix' => '',
      'user_argument_type' => '',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'tid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => 'tid_op',
        'label' => 'Taxonomy: Term',
        'use_operator' => FALSE,
        'identifier' => 'tid',
        'remember' => FALSE,
        'single' => TRUE,
        'optional' => TRUE,
        'reduce' => FALSE,
      ),
      'type' => 'select',
      'limit' => TRUE,
      'vid' => '10003',
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'hierarchy' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Calendar');
  $handler->override_option('header_empty', 1);
  $handler->override_option('items_per_page', 0);
  $handler->override_option('use_more', 0);
  $handler->override_option('style_plugin', 'calendar_nav');
  $handler = $view->new_display('calendar_block', 'Calendar block', 'calendar_block_1');
  $handler->override_option('arguments', array(
    'date_argument' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'date',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'year_range' => '-3:+3',
      'date_method' => 'OR',
      'granularity' => 'month',
      'id' => 'date_argument',
      'table' => 'node',
      'field' => 'date_argument',
      'relationship' => 'none',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'webform' => 0,
        'contact' => 0,
        'event' => 0,
        'member' => 0,
        'member_location' => 0,
        'news' => 0,
        'page' => 0,
        'success_story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
        '3' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Use default',
      ),
      'default_options_div_prefix' => '',
      'user_argument_type' => '',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
    ),
  ));
  $handler->override_option('title', '');
  $handler->override_option('footer', '<div class="calendar-filter">
<?php
$vid = variable_get(\'tpc_calendar_event_vocab_vid\', FALSE);
if ($vid) {
  $view = views_get_current_view();
  $terms = taxonomy_get_tree($vid);
  $links = array();

  $href_base = $view->display[\'page_1\']->handler->options[\'path\'] . \'/\' . $view->args[0];

  foreach ($terms as $term) {
    $links[$term->tid] = array(
      \'data\' => \'<span></span>\' . l($term->name, $href_base . \'/\' . $term->name),
      \'class\' => \'calendar-filter-link calendar-filter-\' . $term->tid,
    );
  }

  $links[\'all\'] = array(
    \'data\' => l(t(\'View all\'), \'calendar\'),
    \'class\' => \'calendar-filter-link calendar-filter-all\',
  );

  print \'<strong>\' . t(\'Filter events by type\') . \'</strong>\';
  print theme(\'item_list\', $links, NULL, \'ul\', array(\'class\' => \'calendar-filter-links\'));
}
?>
</div>');
  $handler->override_option('footer_format', '3');
  $handler->override_option('footer_empty', 1);
  $handler->override_option('block_description', 'Calendar');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('calendar_period', 'Block view', 'calendar_period_5');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'field' => 'title',
      'table' => 'node',
      'relationship' => 'none',
    ),
    'field_event_date_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'field_event_date_default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'both',
      ),
      'exclude' => 0,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_event_date_value2' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_event_date_value2',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value2',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_event_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'date_argument' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'date',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'ignore',
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'year_range' => '-3:+3',
      'date_method' => 'OR',
      'granularity' => 'month',
      'id' => 'date_argument',
      'table' => 'node',
      'field' => 'date_argument',
      'relationship' => 'none',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'article' => 0,
        'book' => 0,
        'event' => 0,
        'feed' => 0,
        'legislative_update' => 0,
        'newsletter' => 0,
        'page' => 0,
        'policy_blog_post' => 0,
        'profile' => 0,
        'video' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '5' => 0,
        '6' => 0,
        '3' => 0,
        '2' => 0,
        '1' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Use default',
      ),
      'default_options_div_prefix' => '',
      'user_argument_type' => '',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
    ),
  ));
  $handler->override_option('style_plugin', 'calendar_style');
  $handler->override_option('style_options', array(
    'display_type' => 'month',
    'name_size' => '1',
  ));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', TRUE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
    'calendar_1' => 0,
    'default' => 0,
    'calendar_block_1' => 'calendar_block_1',
  ));
  $handler->override_option('calendar_type', 'month');
  $handler = $view->new_display('block', 'Page - Upcoming', 'block_1');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'field' => 'title',
      'table' => 'node',
      'relationship' => 'none',
      'format' => 'default',
    ),
    'changed' => array(
      'label' => '',
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'changed',
      'field' => 'changed',
      'table' => 'node',
      'relationship' => 'none',
      'date_format' => 'small',
      'format' => 'default',
    ),
  ));
  $handler->override_option('arguments', array());
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'value' => NULL,
        'min' => NULL,
        'max' => NULL,
        'default_date' => 'now',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node.changed' => 'node.changed',
      ),
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now',
      'default_to_date' => '',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Upcoming');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('block_description', 'Calendar - Upcoming');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('arguments', array(
    'date_argument' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Calendar: %1',
      'breadcrumb' => '',
      'default_argument_type' => 'date',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'year_range' => '-3:+3',
      'date_method' => 'OR',
      'granularity' => 'month',
      'id' => 'date_argument',
      'table' => 'node',
      'field' => 'date_argument',
      'relationship' => 'none',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'article' => 0,
        'book' => 0,
        'event' => 0,
        'feed' => 0,
        'legislative_update' => 0,
        'newsletter' => 0,
        'page' => 0,
        'policy_blog_post' => 0,
        'profile' => 0,
        'video' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '5' => 0,
        '6' => 0,
        '3' => 0,
        '2' => 0,
        '1' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Use default',
      ),
      'default_options_div_prefix' => '',
      'user_argument_type' => '',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
    ),
    'tid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Calendar: %1 - %2',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'add_table' => 0,
      'require_value' => 0,
      'reduce_duplicates' => 0,
      'set_breadcrumb' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'event' => 0,
        'gallery' => 0,
        'article' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '10' => 0,
        '10003' => 0,
      ),
      'validate_argument_type' => 'convert',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Calendar of Events');
  $handler->override_option('footer', '<?php

print l(t(\'Past Events Archive\'), \'calendar/archive\');

?>');
  $handler->override_option('footer_format', '3');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('empty', 'There are no events to display for this time period.');
  $handler->override_option('empty_format', '4');
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
    'grouping' => '',
  ));
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $handler->override_option('path', 'calendar/view');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => 'Events',
    'description' => '',
    'weight' => '0',
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  // Exported view: Event_Lists
  $view = new view;
  $view->name = 'Event_Lists';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_as_appear_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_as_appear_value',
      'table' => 'node_data_field_event_as_appear',
      'field' => 'field_event_as_appear_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_location_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_location_value',
      'table' => 'node_data_field_event_location',
      'field' => 'field_event_location_value',
      'relationship' => 'none',
    ),
    'view_node' => array(
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
    ),
    'teaser' => array(
      'id' => 'teaser',
      'table' => 'node_revisions',
      'field' => 'teaser',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_event_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'tid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => 'Upcoming: %1',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'add_table' => 0,
      'require_value' => 0,
      'reduce_duplicates' => 0,
      'set_breadcrumb' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'article' => 0,
        'book' => 0,
        'event' => 0,
        'feed' => 0,
        'legislative_update' => 0,
        'newsletter' => 0,
        'page' => 0,
        'policy_blog_post' => 0,
        'profile' => 0,
        'video' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '6' => 6,
        '5' => 0,
        '3' => 0,
        '2' => 0,
        '1' => 0,
        '4' => 0,
      ),
      'validate_argument_type' => 'convert',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '=',
      'value' => array(
        'min' => '',
        'max' => '',
        'value' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => '',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'users',
      'field' => 'date_filter',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('header_format', '4');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 2);
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'teaser' => 1,
    'links' => 1,
    'comments' => 0,
  ));
  $handler = $view->new_display('block', 'Upcoming Events - Promoted - Block', 'block_1');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_as_appear_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_as_appear_value',
      'table' => 'node_data_field_event_as_appear',
      'field' => 'field_event_as_appear_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_location_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_location_value',
      'table' => 'node_data_field_event_location',
      'field' => 'field_event_location_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'promote' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'promote',
      'table' => 'node',
      'field' => 'promote',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('footer', '"Visit the events calendar":calendar');
  $handler->override_option('footer_format', '4');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('row_plugin', 'fields');
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
  ));
  $handler->override_option('block_description', 'Upcoming events - promoted');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'Upcoming Events - Block', 'block_2');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_as_appear_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_as_appear_value',
      'table' => 'node_data_field_event_as_appear',
      'field' => 'field_event_as_appear_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_event_location_value' => array(
      'label' => '',
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_event_location_value',
      'table' => 'node_data_field_event_location',
      'field' => 'field_event_location_value',
      'relationship' => 'none',
    ),
    'teaser' => array(
      'id' => 'teaser',
      'table' => 'node_revisions',
      'field' => 'teaser',
    ),
    'view_node' => array(
      'label' => '',
      'text' => 'MORE >>',
      'exclude' => 0,
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'sticky' => array(
      'order' => 'DESC',
      'id' => 'sticky',
      'table' => 'node',
      'field' => 'sticky',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_event_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'value' => NULL,
        'min' => NULL,
        'max' => NULL,
        'default_date' => 'now -6 hours',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
        'node_data_field_event_date.field_event_date_value2' => 'node_data_field_event_date.field_event_date_value2',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now -6 hours',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('footer', '<?php

print l(t(\'Visit the calendar of events\'), \'calendar\');');
  $handler->override_option('footer_format', '3');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('block_description', 'Upcoming Events');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'Event Archive - Page', 'page_1');
  $handler->override_option('sorts', array(
    'field_event_date_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_event_date_value',
      'table' => 'node_data_field_event_date',
      'field' => 'field_event_date_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '<',
      'value' => array(
        'min' => NULL,
        'max' => NULL,
        'value' => NULL,
        'default_date' => 'now',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Past Events Archive');
  $handler->override_option('items_per_page', 10);
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_options', array(
    'teaser' => 1,
    'links' => 0,
    'comments' => 0,
  ));
  $handler->override_option('path', 'calendar/archive');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  $handler = $view->new_display('page', 'Upcoming Events - Page', 'page_2');
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'event' => 'event',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'value' => NULL,
        'min' => NULL,
        'max' => NULL,
        'default_date' => 'now -6 hours',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_event_date.field_event_date_value' => 'node_data_field_event_date.field_event_date_value',
        'node_data_field_event_date.field_event_date_value2' => 'node_data_field_event_date.field_event_date_value2',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now -6 hours',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Calendar: Upcoming Events');
  $handler->override_option('items_per_page', 10);
  $handler->override_option('use_pager', '1');
  $handler->override_option('path', 'calendar');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Calendar',
    'description' => '',
    'weight' => '0',
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  return $views;
}
