<?php
/**
 * Implementation of hook_views_default_views().
 */
function devel_notes_views_default_views() {
  $view = new view;
  $view->name = 'devel_notes';
  $view->description = 'A table of developer notes, filterable by path, status or note content.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'devel_notes';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'note_id' => array(
      'id' => 'note_id',
      'table' => 'devel_notes',
      'field' => 'note_id',
    ),
    'pg' => array(
      'label' => 'Path',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 1,
        'path' => '[pg]',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'pg',
      'table' => 'devel_notes',
      'field' => 'pg',
      'relationship' => 'none',
    ),
    'note' => array(
      'label' => 'Note',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'note',
      'table' => 'devel_notes',
      'field' => 'note',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'status' => array(
      'id' => 'status',
      'table' => 'devel_notes',
      'field' => 'status',
    ),
    'edit_note' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_note',
      'table' => 'devel_notes',
      'field' => 'edit_note',
      'relationship' => 'none',
    ),
    'delete_note' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'text' => '',
      'exclude' => 0,
      'id' => 'delete_note',
      'table' => 'devel_notes',
      'field' => 'delete_note',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'note' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'note_op',
        'identifier' => 'note',
        'label' => 'Search in Note',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 0,
      'id' => 'note',
      'table' => 'devel_notes',
      'field' => 'note',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'pg' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'pg_op',
        'identifier' => 'pg',
        'label' => 'Search in path:',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 0,
      'id' => 'pg',
      'table' => 'devel_notes',
      'field' => 'pg',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'status' => array(
      'operator' => 'in',
      'value' => array(
        '0' => '0',
        '2' => '2',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'status_op',
        'identifier' => 'status',
        'label' => 'Note Status',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'status',
      'table' => 'devel_notes',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access devel notes',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Developer Notes');
  $handler->override_option('empty', 'There are no developer notes matching the filter criteria.');
  $handler->override_option('empty_format', '4');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'note_id' => 'note_id',
      'pg' => 'pg',
      'note' => 'note',
      'status' => 'status',
      'edit_note' => 'edit_note',
      'delete_note' => 'delete_note',
    ),
    'info' => array(
      'note_id' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'pg' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'note' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'status' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'edit_note' => array(
        'separator' => '',
      ),
      'delete_note' => array(
        'separator' => '',
      ),
    ),
    'default' => 'pg',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/devel_notes');
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
  // end of export
  
  $views[$view->name] = $view;

  return $views;
}