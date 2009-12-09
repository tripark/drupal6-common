<?php
// $Id: default.profile,v 1.22 2007/12/17 12:43:34 goba Exp $

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function tpc_profile_modules() {
  return array('comment', 'help', 'menu', 'taxonomy', 'dblog', 'admin_menu', 'content', 'devel', 'filefield', 'imageapi', 'imageapi_gd', 'imagecache', 'imagecache_ui', 'imagefield', 'path', 'pathauto', 'pathologic', 'php', 'search', 'text', 'textile', 'token', 'upload', 'vertical_tabs', 'views', 'views_ui', 'wysiwyg');
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
    'name' => 'Triangle Park Creative - Basic',
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
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status', 'revision'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Don't display date and author information for page nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  variable_set('theme_settings', $theme_settings);

  // Only index 20 nodes per cron run
  variable_set('search_cron_limit', 20);

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
      'bold' => 1,
      'italic' => 1,
      'underline' => 1,
      'bullist' => 1,
      'numlist' => 1,
      'link' => 1,
      'unlink' => 1,
      'anchor' => 1,
      'image' => 1,
      'blockquote' => 1,
    ),
    'font' => array('formatselect' => 1),
    'fullscreen' => array('fullscreen' => 1),
    'paste' => array('pastetext' => 1, 'pasteword' => 1),
    'table' => array('tablecontrols' => 1),
    'safari' => array('safari' => 1),
    'drupal' => array('break' => 1),
  ),
  'toolbar_loc' => 'top',
  'toolbar_align' => 'left',
  'path_loc' => 'bottom',
  'resizing' => 1,
  'verify_html' => 1,
  'preformatted' => 0,
  'convert_fonts_to_spans' => 1,
  'remove_linebreaks' => 0,
  'apply_source_formatting' => 0,
  'paste_auto_cleanup_on_paste' => 0,
  'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
  'css_setting' => 'theme',
  'css_path' => '',
  'css_classes' => '',
  );
  db_query("INSERT INTO {wysiwyg} (format, editor, settings) VALUES (1, '', NULL), (2, '', NULL), (3, '', NULL), (4, 'tinymce', '%s')", serialize($wysiwyg_config));

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

  // Disable configurable timezones
  variable_set('configurable_timezones', 0);

  // Configure file upload settings
  variable_set('upload_list_default', 0); // DO NOT list files by default
  variable_set('upload_uploadsize_default', 8);
  variable_set('upload_usersize_default', 100);

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
