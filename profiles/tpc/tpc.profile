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
    'menu',
    'taxonomy',
    'dblog',
    'admin_menu',
    'better_formats',
    'content',
    'devel',
    'filefield',
    'imageapi',
    'imageapi_gd',
    'imagecache',
    'imagecache_ui',
    'imagefield',
    'menu_block',
    'menutrails',
    'path',
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
    'views_ui',
    'wysiwyg'
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
    'description' => 'Custom installer, includes Admin Menu, CCK, Imagecache, Imagefield, Pathauto, Pathologic, Token, Views, WYSIWYG and basic configurations.'
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

  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $types = array(
    array(
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
    ),
    array(
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
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }
  
  // Create a CCK field for body content to replace Drupal's default body.
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');

  $settings = array('display_settings' => array());
  $settings['field_name'] = 'field_node_body';
  $settings['type_name'] = 'page';
  $settings['label'] = 'Body';
  $settings['weight'] = -3;
  $settings['rows'] = 15;
  $settings['type'] = 'text';
  $settings['widget_type'] = 'text_textarea';
  $settings['text_processing'] = 1; // Filtered text
  $settings['display_settings']['label'] = array('format' => 'hidden');
  $settings['display_settings']['teaser'] = array('format' => 'default', 'exclude' => 1);
  $settings['display_settings']['full'] = array('format' => 'default', 'exclude' => 0);

  content_field_instance_create($settings);

  // Create a CCK field for teaser content.
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');

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

  content_field_instance_create($settings);
  
  // Add body and teaser field instances to other content types.
  content_field_instance_create(array('field_name' => 'field_node_body', 'type_name' => 'news'));
  content_field_instance_create(array('field_name' => 'field_node_teaser', 'type_name' => 'news'));
  
  // Set default publishing options for content types.
  variable_set('node_options_page', array('status', 'revision'));
  variable_set('node_options_news', array('status', 'revision'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);
  variable_set('comment_news', COMMENT_NODE_DISABLED);
  variable_set('upload_news', 0);
  
  // Don't display date and author information for nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['toggle_node_info_news'] = FALSE;
  variable_set('theme_settings', $theme_settings);

  // Only index 20 nodes per cron run
  variable_set('search_cron_limit', 20);

  // Enable vertical tab fieldset selection on content type edit forms.
  variable_set('vertical_tabs_node_type_settings', 1);

  // Pathauto default settings.
  // Remove content/ from the default node setting.
  variable_set('pathauto_node_pattern', '');
  // Set default automated alias settings.
  variable_set('pathauto_node_page_pattern', '[menupath-raw]');
  variable_set('pathauto_node_news_pattern', 'news/[yyyy][mm]/[nid]');
  // Default update action to do nothing.
  variable_set('pathauto_update_action', 0);
  
  // Setup a textile input format
  //db_query('INSERT INTO {filters} (format, module, delta, weight) VALUES (4, "filter", 0, 1), (4, "textile", 0, 10), (4, "filter", 2, 0)');
  //db_query('INSERT INTO {filter_formats} (format, name, roles, cache) VALUES (4, "Textile", ",,", 1)');
  //variable_set('filter_default_format', 4);
  
  // Setup a WYSIWYG input format and default configurations.
  db_query('INSERT INTO {filters} (format, module, delta, weight) VALUES (4, "filter", 3, 10), (4, "pathologic", 0, 10)');
  db_query('INSERT INTO {filter_formats} (format, name, roles, cache) VALUES (4, "WYSIWYG Editor", ",,", 1)');
  variable_set('filter_default_format', 4);

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

  // Create a custom block named "Footer" and place it in the footer region.
  db_query("INSERT INTO {boxes} (body, info, format) VALUES ('%s', '%s', %d)", 'Footer content goes here.', 'Footer', FILTER_FORMAT_DEFAULT);
  $delta = db_last_insert_id('boxes', 'bid');
  // Can not use list_themes() here because we are in MAINTINANCE_MODE when
  // when installing so the list returned is inaccurate.
  $result = db_query("SELECT * FROM {system} WHERE type = '%s'", 'theme');
  while ($theme = db_fetch_object($result)) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'block', $theme->name, 1, 0, 'footer', $delta, BLOCK_NO_CACHE);
    }
  }

  // Create an inside navigation block using menu_block module.
  module_load_include('inc', 'menu_block', 'menu_block.admin');
  $edit = array(
    'values' => array(
      'title' => '',
      'parent_menu' => 'primary-links',
      'parent' => 'primary-links:0',
      'level' => '2',
    ),
  );
  drupal_execute('menu_block_add_block_form', $edit);
  // Assumes that the newly created block is :module => menu_block, :delta => 1.
  $result = db_query("SELECT * FROM {system} WHERE type = '%s'", 'theme');
  while ($theme = db_fetch_object($result)) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'menu_block', $theme->name, 1, -1, 'left', '1', BLOCK_NO_CACHE);
    }
  }

  // Disable the "Powered by Drupal" block.
  db_query('UPDATE {blocks} SET status = 0 WHERE module = "system" AND delta = "0"');

  // Create a home page.
  $node = array(
    'title' => 'Home',
    'type' => 'page',
    'status' => 1,
    'uid' => 1,
    'pathauto_perform_alias' => 0,
    'path' => 'home',
  );
  $node = (object) $node;
  node_save($node);

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
  
  // Set the site front page to the home page node.
  variable_set('site_frontpage', 'node/1');

  // Configure date formatting options.
  variable_set('configurable_timezones', 0);
  variable_set('date_first_day', 0);
  variable_set('date_format_long', 'l, F j, Y - g:ia');
  variable_set('date_format_medium', 'D, m/d/Y - g:ia');
  variable_set('date_format_short', 'm/d/Y - g:ia');

  // Configure file upload settings
  variable_set('upload_list_default', 0); // DO NOT list files by default
  variable_set('upload_uploadsize_default', 16);
  variable_set('upload_usersize_default', 1000);
  variable_set('upload_max_resolution', '800x600');

  // User registration settings.
  // Turn off requirement for users to verify new accounts via e-mail.
  variable_set('user_email_verification', 0);
  // Only site adminisrators can create new accounts.
  variable_set('user_register', 0);

  // Create administrator and site editor roles.
   db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", 3, 'administrator');
   db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", 4, 'site editor');

  // Set default permissions.
  db_query("UPDATE {permission} SET perm = '%s' WHERE rid = %d", 'show format tips, access content, search content, view uploaded files', 1);
  db_query("UPDATE {permission} SET perm = '%s' WHERE rid = %d", 'show format tips, access content, search content, view uploaded files', 2);
  db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', 0)", 3, 'access administration menu, collapse format fieldset by default, collapsible format selection, show format selection for blocks, show format selection for nodes, administer blocks, administer menu, administer content types, administer nodes, administer url aliases, create url aliases, assign roles, administer search, access administration pages, access site reports, administer site configuration, administer taxonomy, upload files, access user profiles, administer users, access all views, administer views');
  db_query("INSERT INTO {permission} (rid, perm, tid) VALUES (%d, '%s', 0)", 4, 'access administration menu, collapse format fieldset by default, collapsible format selection, show format selection for blocks, show format selection for nodes, administer blocks, administer menu, administer nodes, create url aliases, access administration pages, administer site configuration, administer taxonomy, upload files, access user profiles');

  // Set assignable roles.
  variable_set('roleassign_roles', array(3 => 3, 4 => 4));
  
  // Inflate the auto_increment value on the {vocabulary} table so that we can
  // reserve VID's for use by Features.
  // Please remember to log which VID's you are using in your Features in the
  // documentation.
  // @see https://www.tripark.org/wiki/node/165
  db_query('ALTER TABLE {vocabulary} auto_increment = 1000');

  // Create a 'footer navigation' menu block.
  $edit = array(
    'values' => array(
      'title' => '<none>',
      'admin_title' => 'Footer navigation',
      'parent_menu' => 'primary-links',
      'parent' => 'primary-links:0',
      'level' => '1',
      'expanded' => 1,
    ),
  );
  drupal_execute('menu_block_add_block_form', $edit);
  // Assumes that the newly created block is :module => menu_block, :delta => 2.
  $result = db_query("SELECT * FROM {system} WHERE type = '%s'", 'theme');
  while ($theme = db_fetch_object($result)) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, region, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, '%s', '%s', %d)", 0, '', 0, '', 'menu_block', $theme->name, 1, -1, 'footer', '2', BLOCK_NO_CACHE);
    }
  }

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
