<?php
// $Id: default.profile,v 1.22 2007/12/17 12:43:34 goba Exp $

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function tpc_profile_modules() {
  return array(
    'comment',
    'menu',
    'taxonomy',
    'dblog',
    'admin_menu',
    'better_formats',
    'content',
    'devel',
    'draggableviews',
    'filefield',
    'imageapi',
    'imageapi_gd',
    'imagecache',
    'imagecache_ui',
    'imagefield',
    'link',
    'menu_block',
    'menutrails',
    'optionwidgets',
    'path',
    'path_redirect',
    'pathauto',
    'pathologic',
    'php',
    'roleassign',
    'search',
    'text',
    'textile',
    'token',
    'tpc_general',
    'upload',
    'vertical_tabs',
    'views',
    'views_bulk_operations',
    'views_ui',
    'wysiwyg',
  );
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function tpc_profile_details() {
  return array(
    'name' => 'Triangle Park Creative - Wireframe site',
    'description' => 'Starter install for a TPC Wireframe site.'
  );
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function tpc_profile_task_list() {
}

/**
 * Perform any final installation tasks for this profile.
 *
 * The installer goes through the profile-select -> locale-select
 * -> requirements -> database -> profile-install-batch
 * -> locale-initial-batch -> configure -> locale-remaining-batch
 * -> finished -> done tasks, in this order, if you don't implement
 * this function in your profile.
 *
 * If this function is implemented, you can have any number of
 * custom tasks to perform after 'configure', implementing a state
 * machine here to walk the user through those tasks. First time,
 * this function gets called with $task set to 'profile', and you
 * can advance to further tasks by setting $task to your tasks'
 * identifiers, used as array keys in the hook_profile_task_list()
 * above. You must avoid the reserved tasks listed in
 * install_reserved_tasks(). If you implement your custom tasks,
 * this function will get called in every HTTP request (for form
 * processing, printing your information screens and so on) until
 * you advance to the 'profile-finished' task, with which you
 * hand control back to the installer. Each custom page you
 * return needs to provide a way to continue, such as a form
 * submission or a link. You should also set custom page titles.
 *
 * You should define the list of custom tasks you implement by
 * returning an array of them in hook_profile_task_list(), as these
 * show up in the list of tasks on the installer user interface.
 *
 * Remember that the user will be able to reload the pages multiple
 * times, so you might want to use variable_set() and variable_get()
 * to remember your data and control further processing, if $task
 * is insufficient. Should a profile want to display a form here,
 * it can; the form should set '#redirect' to FALSE, and rely on
 * an action in the submit handler, such as variable_set(), to
 * detect submission and proceed to further tasks. See the configuration
 * form handling code in install_tasks() for an example.
 *
 * Important: Any temporary variables should be removed using
 * variable_del() before advancing to the 'profile-finished' phase.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function tpc_profile_tasks(&$task, $url) {
  // Configure date formatting options.
  variable_set('configurable_timezones', 0);
  variable_set('date_first_day', 0);
  variable_set('date_format_long', 'l, F j, Y - g:ia');
  variable_set('date_format_medium', 'D, m/d/Y - g:ia');
  variable_set('date_format_short', 'm/d/Y - g:ia');

  // Pathauto default settings.
  variable_set('pathauto_ignore_words', 'and, our, the');
  variable_set('pathauto_node_pattern', '');
  variable_set('pathauto_update_action', 3); // Redirect to new alias via path_redirect.module.
  
  // Search module settings.
  variable_set('search_cron_limit', 20);

  // TPC General module settings for core behavior changes.
  variable_set('tpc_general_node_promote_disable', 1);
  variable_set('tpc_general_body_label_disable', 1);
  variable_set('tpc_general_core_blocks_disable', 1);
  variable_set('tpc_general_core_themes_disable', 1);

  // Upload module settings.
  variable_set('upload_extensions_default', 'jpg jpeg gif png tif tiff bmp psd txt doc rtf xls pdf eps ppt pps odt ods odp mp3 mp4 ogg docx xlsx pptx');
  variable_set('upload_list_default', 0);
  variable_set('upload_uploadsize_default', 16);
  variable_set('upload_usersize_default', 1000);
  variable_set('upload_max_resolution', '800x600');

  // User registration settings.
  variable_set('user_email_verification', 0);
  variable_set('user_register', 0);

  // Enable vertical tab fieldset selection on content type edit forms.
  variable_set('vertical_tabs_node_type_settings', 1);

  // Configure content types.
  tpc_configure_content_type_page();
  tpc_configure_content_type_news();
  
  // Don't display date and author information for nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['toggle_node_info_news'] = FALSE;
  variable_set('theme_settings', $theme_settings);

  // Configure blocks.
  tpc_configure_blocks();
  
  // Setup a WYSIWYG input format and default configurations.
  tpc_configure_wysiwyg();
  
  // Configure input format defaults.
  variable_set('filter_default_format', 1);
  variable_set('filter_html_nofollow_1', 1);
  variable_set('filter_pathologic_abs_paths_4', "/");
  variable_set('filter_pathologic_href_4', 1);
  variable_set('filter_pathologic_src_4', 1);

  // Create manager and editor roles.
  db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", 3, 'editor');
  db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", 4, 'manager');
  db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", 5, 'administrator');

  // Set default permissions.
  db_query("UPDATE {permission} SET perm = '%s' WHERE rid = %d", 'access comments, post comments, access content, search content, view uploaded files', 1);
  db_query("UPDATE {permission} SET perm = '%s' WHERE rid = %d", 'access comments, post comments, access content, search content, view uploaded files', 2);
  db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', 0)", 3, 'access administration menu, administer comments, post comments without approval, Allow Reordering, administer nodes, create url aliases, access administration pages, administer taxonomy, upload files, access user profiles');
  db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', 0)", 4, 'access administration menu, administer blocks, administer comments, post comments without approval, Allow Reordering, administer menu, administer nodes, administer url aliases, create url aliases, administer redirects, assign roles, administer search, access administration pages, access site reports, administer taxonomy, upload files, access user profiles, administer users, access all views');
  db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', 0)", 5, 'access administration menu, collapse format fieldset by default, collapsible format selection, show format selection for blocks, show format selection for comments, show format selection for nodes, administer blocks, administer comments, post comments without approval, Allow Reordering, administer imagecache, flush imagecache, administer menu, administer content types, administer nodes, administer url aliases, create url aliases, administer redirects, administer pathauto, administer search, access administration pages, access site reports, administer site configuration, administer taxonomy, upload files, access user profiles, administer permissions, administer users, access all views, administer views');

  // Set assignable roles.
  variable_set('roleassign_roles', array(3 => 3, 4 => 4));
  
  // Give user 1 the "administrator" role.
  db_query("INSERT INTO {users_roles} (uid, rid) VALUES (%d, %d)", 1, 5);

  // Create role entries for better formats and set defaults.
  $sql = "INSERT INTO {better_formats_defaults} (rid, type, format, type_weight, weight) VALUES (%d, '%s', %d, %d, %d)";
  for ($rid = 3; $rid <= 5; $rid++) {
    db_query($sql, $rid, 'node', 4, 1, 0 - $rid);
    db_query($sql, $rid, 'comment', 0, 1, 0 - $rid);
    db_query($sql, $rid, 'block', 4, 1, 0 - $rid);
  }
  
  // Inflate the auto_increment value on the {vocabulary} table so that we can
  // reserve VID's for use by Features.
  // Please remember to log which VID's you are using in your Features in the
  // documentation.
  // @see https://www.tripark.org/wiki/node/165
  //db_query('ALTER TABLE {vocabulary} auto_increment = 1000');

  // Update the menu router information.
  menu_rebuild();
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function tpc_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    // site info
    $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
    $form['site_information']['site_mail']['#default_value'] = 'webmaster@triangleparkcreative.com';
    // admin account
    $form['admin_account']['account']['name']['#default_value'] = 'admin';
    $form['admin_account']['account']['mail']['#default_value'] = 'webmaster@triangleparkcreative.com';
    // date/time
    $form['server_settings']['date_default_timezone']['#default_value'] = '-21600'; // Central Time Zone / UTC -6
    $form['server_settings']['clean_url']['#default_value'] = 1;
  }
}

function tpc_configure_wysiwyg() {
  db_query('INSERT INTO {filters} (format, module, delta, weight) VALUES (4, "filter", 3, 10), (4, "pathologic", 0, 10)');
  db_query('INSERT INTO {filter_formats} (format, name, roles, cache) VALUES (4, "WYSIWYG Editor", ",5,4,3,", 1)');

  $wysiwyg_config  = array(
  'default' => 1,
  'user_choose' => 0,
  'show_toggle' => 1,
  'theme' => 'advanced',
  'language' => 'en',
  'buttons' => array(
    'default' => array(
      'Bold' => 1,
      'Italic' => 1,
      'JustifyLeft' => 1,
      'JustifyCenter' => 1,
      'BulletedList' => 1,
      'NumberedList' => 1,
      'Link' => 1,
      'Unlink' => 1,
      'Image' => 1,
      'Blockquote' => 1,
      'PasteText' => 1,
      'PasteFromWord' => 1,
      'ShowBlocks' => 1,
      'RemoveFormat' => 1,
      'Format' => 1,
      'Table' => 1,
    ),
  ),
  'toolbar_loc' => 'top',
  'toolbar_align' => 'left',
  'path_loc' => 'bottom',
  'resizing' => 1,
  'verify_html' => 0,
  'preformatted' => 0,
  'convert_fonts_to_spans' => 1,
  'remove_linebreaks' => 0,
  'apply_source_formatting' => 1,
  'paste_auto_cleanup_on_paste' => 1,
  'block_formats' => 'p,h2,h3,h4,h5',
  'css_setting' => 'theme',
  'css_path' => '',
  'css_classes' => '',
  );
  db_query("INSERT INTO {wysiwyg} (format, editor, settings) VALUES (1, '', NULL), (2, '', NULL), (3, '', NULL), (4, 'ckeditor', '%s')", serialize($wysiwyg_config));
}

function tpc_configure_content_type_page() {
  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $type = array(
    'type' => 'page',
    'name' => st('Page'),
    'module' => 'node',
    'description' => st("A <em>page</em> is a simple content type for creating and displaying information that rarely changes, such as an \"About us\" section of a website."),
    'custom' => TRUE,
    'modified' => TRUE,
    'has_body' => FALSE,
    'locked' => FALSE,
    'help' => '',
    'min_word_count' => '',
  );
  $type = (object) _node_type_set_defaults($type);
  node_type_save($type);

  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');

  // Create a CCK field for body content to replace Drupal's default body.
  $settings = array('display_settings' => array());
  $settings['field_name'] = 'field_node_body';
  $settings['type_name'] = 'page';
  $settings['label'] = 'Body';
  $settings['weight'] = -1;
  $settings['rows'] = 15;
  $settings['type'] = 'text';
  $settings['widget_type'] = 'text_textarea';
  $settings['text_processing'] = 1; // Filtered text
  $settings['display_settings']['label'] = array('format' => 'hidden');
  $settings['display_settings']['teaser'] = array('format' => 'default', 'exclude' => 1);
  $settings['display_settings']['full'] = array('format' => 'default', 'exclude' => 0);
  content_field_instance_create($settings);

  // Create a CCK field for an image.
  $settings = array('display_settings' => array());
  $settings['field_name'] = 'field_node_image';
  $settings['type_name'] = 'page';
  $settings['label'] = 'Image';
  $settings['weight'] = -2;
  $settings['max_resolution'] = '800x600';
  $settings['file_path'] = 'images';
  $settings['custom_alt'] = 1;
  $settings['type'] = 'filefield';
  $settings['widget_type'] = 'imagefield_widget';
  $settings['text_processing'] = 1; // Filtered text
  $settings['display_settings']['label'] = array('format' => 'hidden');
  $settings['display_settings']['teaser'] = array('format' => 'image_plain', 'exclude' => 1);
  $settings['display_settings']['full'] = array('format' => 'image_plain', 'exclude' => 0);
  $settings['display_settings'][4] = array('format' => 'image_plain', 'exclude' => 1);
  content_field_instance_create($settings);

  // Create a CCK field for teaser content.
  $settings = array('display_settings' => array());
  $settings['field_name'] = 'field_node_teaser';
  $settings['type_name'] = 'page';
  $settings['label'] = 'Teaser/Summary';
  $settings['description'] = 'A single-paragraph version or description of this content. This text will be used to describe this page when it is indexed by search engines or shared on social networking sites. Line breaks will be ignored.';
  $settings['weight'] = -4;
  $settings['rows'] = 4;
  $settings['type'] = 'text';
  $settings['widget_type'] = 'text_textarea';
  $settings['text_processing'] = 0; // Plain text
  $settings['display_settings']['label'] = array('format' => 'hidden');
  $settings['display_settings']['teaser'] = array('format' => 'default', 'exclude' => 0);
  $settings['display_settings']['full'] = array('format' => 'default', 'exclude' => 1);
  $settings['display_settings'][4] = array('format' => 'default', 'exclude' => 1);
  content_field_instance_create($settings);

  // Set default publishing options for content types.
  variable_set('node_options_page', array('status', 'revision'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Set default pathauto pattern.
  variable_set('pathauto_node_page_pattern', '[menupath-raw]');
  
  // Create default page content.
  $node = array(
    'type' => 'page',
    'title' => 'Home',
    'status' => 1,
    'uid' => 1,
    'pathauto_perform_alias' => 0,
    'path' => 'home',
  );
  $node = (object) $node;
  node_save($node);

  // Set the site front page to the home page node.
  variable_set('site_frontpage', 'node/1');

}

function tpc_configure_content_type_news() {
  $type = array(
    'type' => 'news',
    'name' => st('News item'),
    'module' => 'node',
    'description' => st("<em>News items</em> are dated pieces of content that are often organized into chronological lists. As more news items are created, previous news items are pushed farther down the list."),
    'custom' => TRUE,
    'modified' => TRUE,
    'has_body' => FALSE,
    'locked' => FALSE,
    'help' => '',
    'min_word_count' => '',
  );
  $type = (object) _node_type_set_defaults($type);
  node_type_save($type);

  // Add body and teaser field instances defined in the Page node type.
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');

  content_field_instance_create(array('field_name' => 'field_node_body', 'type_name' => 'news'));
  content_field_instance_create(array('field_name' => 'field_node_teaser', 'type_name' => 'news'));
  
  // Set default publishing options.
  variable_set('node_options_news', array('status', 'revision'));
  variable_set('comment_news', COMMENT_NODE_DISABLED);
  variable_set('upload_news', 0);

  // Set default Pathauto pattern.
  variable_set('pathauto_node_news_pattern', 'news/[yyyy][mm]/[nid]');

  // Create placeholder news items.
  for ($i = 1; $i <= 5; $i++) {
    $node = array(
      'title' => 'News Headline ' . $i,
      'type' => 'news',
      'status' => 1,
      'uid' => 1,
      'created' => mktime() - 3600 + ($i * 60),
      'pathauto_perform_alias' => 1,
      'field_node_teaser' => array(0 => array('value' => 'This text is from the "Teaser/Summary" field of this News item. It is not seen when viewing the full version of this node.')),
      'field_node_body' => array(0 => array('value' => 'This text is from the "Body" field of this News item.')),
    );
    $node = (object) $node;
    node_save($node);
  }

  // Create default news view.
  tpc_view_default_news();
}

function tpc_configure_blocks() {
  // Block configurations.
  db_query('UPDATE {blocks} SET status = 0 WHERE module = "system" AND delta = "0"');

  // Create an inside navigation block using menu_block module.
  module_load_include('inc', 'menu_block', 'menu_block.admin');
  $values = array(
    'title' => '',
    'parent_menu' => 'primary-links',
    'parent' => 'primary-links:0',
    'level' => '2',
  );
  drupal_execute('menu_block_add_block_form', $edit = array('values' => $values)); // Should be delta = 1.
  
  // Create a 'footer navigation' menu block.
  $values = array(
    'title' => '<none>',
    'admin_title' => 'Footer navigation',
    'parent_menu' => 'primary-links',
    'parent' => 'primary-links:0',
    'level' => '1',
    'expanded' => 1,
  );
  drupal_execute('menu_block_add_block_form', $edit = array('values' => $values)); // Should be delta = 2.

  // Create a custom block named "Footer".
  db_query("INSERT INTO {boxes} (body, info, format) VALUES ('%s', '%s', %d)", 'Footer content goes here.', 'Footer', FILTER_FORMAT_DEFAULT);
  $delta = db_last_insert_id('boxes', 'bid');

  // Insert block table entries per theme for each newly created block.
  $result = db_query("SELECT * FROM {system} WHERE type = '%s'", 'theme');
  while ($theme = db_fetch_object($result)) {
    if ($theme->status) {
      // menu-block-1 (Primary links level 2)
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'menu_block', $theme->name, 1, -1, 'left', '1', BLOCK_NO_CACHE);
      // menu-block-2 (Footer navigation)
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'menu_block', $theme->name, 1, -1, 'footer', '2', BLOCK_NO_CACHE);
      // Footer block
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'block', $theme->name, 1, 0, 'footer', $delta, BLOCK_NO_CACHE);
      // tpc_view_default_news() blocks
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 1, '<front>', 0, '', 'views', $theme->name, 1, -9, 'right', 'news-block_1', BLOCK_NO_CACHE);
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 1, "news\nnews/*", 0, '', 'views', $theme->name, 1, -8, 'left', 'news-block_2', BLOCK_NO_CACHE);  
    }
  }  
}

/**
 * Default views definitions.
 */
function tpc_view_default_news() {
  $view = new view;
  $view->name = 'news';
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
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '<h4>',
        'suffix' => '</h4>',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
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
    'field_node_teaser_value' => array(
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
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
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
      'id' => 'field_node_teaser_value',
      'table' => 'node_data_field_node_teaser',
      'field' => 'field_node_teaser_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'view_node' => array(
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
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'text' => 'Read more...',
      'exclude' => 0,
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
      'override' => array(
        'button' => 'Override',
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
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'created' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'created_year_month' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'id' => 'created_year_month',
      'table' => 'node',
      'field' => 'created_year_month',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '5' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'news' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'news' => 'news',
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
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'News');
  $handler->override_option('use_pager', '1');
  $handler->override_option('use_more', 1);
  $handler->override_option('use_more_always', 1);
  $handler->override_option('use_more_text', 'more news >');
  $handler->override_option('row_options', array(
    'inline' => array(
      'field_node_teaser_value' => 'field_node_teaser_value',
      'view_node' => 'view_node',
    ),
    'separator' => ' ',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 0,
    'comments' => 0,
  ));
  $handler->override_option('path', 'news');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'News',
    'description' => '',
    'weight' => '10',
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('block', 'Block: front page', 'block_1');
  $handler->override_option('arguments', array());
  $handler->override_option('items_per_page', 3);
  $handler->override_option('use_pager', FALSE);
  $handler->override_option('block_description', 'Latest news');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('row_options', array());
  $handler->override_option('path', 'news/feed');
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
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'block_1' => 'block_1',
    'default' => 0,
  ));
  $handler->override_option('sitename_title', FALSE);
  $handler = $view->new_display('block', 'Block: posts by month', 'block_2');
  $handler->override_option('arguments', array(
    'created_year_month' => array(
      'default_action' => 'summary desc',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => 1,
        'override' => 1,
        'items_per_page' => '12',
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'id' => 'created_year_month',
      'table' => 'node',
      'field' => 'created_year_month',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '5' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'news' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('title', 'News items by month');
  $handler->override_option('use_more', 0);
  $handler->override_option('use_more_always', 0);
  $handler->override_option('block_description', 'News items by month');
  $handler->override_option('block_caching', -1);
  $view->save();
}
