<?php
// $Id: date_api_filter_handler.inc,v 1.1.2.2 2008/10/02 20:22:50 karens Exp $

/**
 * A flexible, configurable date filter.
 *
 * This filter allows you to select a granularity of date parts to filter on,
 * such as year, month, day, etc.
 *
 * Each part can be set to blank to show all values; 'now' to filter for
 * the current value of that part, or a specific value.
 *
 * An adjustment field is provided that will adjust the selected filter
 * value by something like '+90 days' or '-1 month';
 */
class date_api_filter_handler extends views_handler_filter_numeric {
  var $date_handler = NULL;

  // Add a date handler to the filter.
  function construct() {
    parent::construct();
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_sql.inc');
    $this->date_handler = new date_sql_handler();
    $this->date_handler->construct();
    $this->date_handler->granularity = $this->options['granularity'];
  }

  function init(&$view, $options) {
    parent::init($view, $options);
    $handler = $this->date_handler;
    $handler->granularity = isset($options['granularity']) ? $options['granularity'] : 'day';
    $handler->adjustment_field = isset($options['adjustment_field']) ? $options['adjustment_field'] : 0;
  }

  // Set default values for the date filter.
  function options(&$options) {
    parent::options($options);
    $options['date_fields'] = array();
    $options['granularity'] = 'day';
    $options['form_type'] = 'date_select';
    $options['default_date'] = '';
    $options['default_to_date'] = '';
  }
  
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Set the granularity of the date parts to use in the filter.
    */
  function has_extra_options() { return TRUE; }

  /**
   * Date selection options.
   * 
   * TODO Only select widget is working right now.
   */
  function widget_options() {
    $options = array(
      'date_select' => t('Select'), 
      //'date_text' => t('Text'), 
      //'date_popup' => t('Popup'),
      );
    //if (!module_exists('date_popup')) {
    //  unset($options['date_popup']);
    //}  
    return $options;
  }
   
  function extra_options_form(&$form, &$form_state) {
    $form['form_type'] = array(
      '#type' => 'radios',
      '#title' => t('Date form type'),
      '#default_value' => $this->options['form_type'],
      '#options' => $this->widget_options(),
      '#description' => t('Choose the form element to use for date selection (more options will be available later).'),
      );
   
    $form['granularity'] = $this->date_handler->granularity_form($this->options['granularity']);
    $form['granularity']['#description'] = '<p>'. t("Select a granularity for the date filter. For instance, selecting 'day' will create a filter where users can select the year, month, and day.") .'</p>';
    
    $fields = date_api_fields();
    $options = array();
    foreach ($fields['name'] as $name => $field) {
      $options[$name] = $field['label'];
    }
    $form['date_fields'] = array(
      '#title' => t('Date field(s)'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['date_fields'],
      '#multiple' => FALSE,
      '#description' => t('Select date field(s) to filter with this argument.'),
    );
  }

  function extra_options_validate($form, &$form_state) {
    $check_fields = array_filter($form_state['values']['options']['date_fields']);
    if (empty($check_fields)) {
      form_error($form['date_fields'], t('You must select at least one date field for this filter.'));
    }
  }
  
  function extra_options_submit($form, &$form_state) {
    $form_state['values']['options']['date_fields'] = array_filter($form_state['values']['options']['date_fields']);
  }
    
  /**
   * Add the selectors to the value form using the date handler.
   */
  function value_form(&$form, &$form_state) {
    // We use different values than the parent form, so we must
    // construct our own form element.
    $form['value'] = array();
    $form['value']['#tree'] = TRUE;
    
    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    $source = '';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }

    if (!empty($form_state['exposed'])) {
      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . form_clean_id($this->options['expose']['operator']);
      }
    }

    $handler = $this->date_handler;
       
    if ($which == 'all' || $which == 'value') {
      $form['value'] += $this->date_parts_form($form_state, 'value', $source, $which, $this->operator_values(1));
    }

    if ($which == 'all' || $which == 'minmax') {
      $form['value'] += $this->date_parts_form($form_state, 'min', $source, $which, $this->operator_values(2));
      $form['value'] += $this->date_parts_form($form_state, 'max', $source, $which, $this->operator_values(2));
    }
    
    $form['value']['value']['#prefix'] = t('<div class="form-item"><label>Absolute value</label></div>');
    $form['value']['default_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Date default'),
      '#default_value' => $this->options['default_date'],
      '#prefix' => '<div class="form-item">' . t("<label>Relative value</label><p>Relative values will be used if no date is set above. Use 'now' to default to the current date at runtime or add modifiers like 'now +1 day'. The To date default value is used when the operator is set to 'between' or 'not between'. </p><p>If the filter is exposed, these values will be used to set the inital value of the exposed filter. Leave both date and default values blank to start with no value in the exposed filter. </p>") .'</div>',
      );
    $form['value']['default_to_date'] = array(
      '#type' => 'textfield',
      '#title' => t('To date default'),
      '#default_value' => $this->options['default_to_date'],
      );

    // Test if this value is in the UI or exposed, only show these elements in the UI.
    // We'll save it as an option rather than a value to store it for use
    // in the exposed filter.
    if (!empty($form_state['exposed'])) {
      $form['value']['default_date']['#type'] = 'hidden';
      $form['value']['default_date']['#value'] = $form['value']['default_date']['#default_value'];
      $form['value']['default_to_date']['#type'] = 'hidden';
      $form['value']['default_to_date']['#value'] = $form['value']['default_to_date']['#default_value'];
      unset($form['value']['default_date']['#prefix']);
    }
    
    $form['value']['#theme'] = 'date_views_filter_form';  
  }

  /**
   * A form element to select date part values.
   *
   * @param string $prefix
   *   A prefix for the date values, 'value', 'min', or 'max'.
   * @param string $source
   *   The operator for this element.
   * @param string $which
   *   Which element to provide, 'all', 'value', or 'minmax'.
   * @param array $operator_values
   *   An array of the allowed operators for this element.
   * @param array $limit
   *   An array of date parts to limit this element to.
   *
   * @return
   *   The form date part element for this instance.
   */
  function date_parts_form($form_state, $prefix, $source, $which, $operator_values) {
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_elements.inc');
    switch($prefix) {
      case 'min':
        $name = t('From date');
        break;
      case 'max':
        $name = t('To date');
        break;
      default:
        $name = '';
        break;
    }
    $type = $this->options['form_type'];
    $format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $granularity = array_keys($this->date_handler->date_parts($this->options['granularity']));
    
    // Don't set a default date in the UI, only in the exposed form.
    $default_date = '';
    if (!empty($form_state['exposed'])) {
      $default_date = $this->default_value($prefix);
    }
    $form[$prefix] = array(
      '#type' => $type,
      '#title' => $name,
      '#size' => 20,
      '#default_value' => !empty($this->value[$prefix]) ? $this->value[$prefix] : $default_date,
      '#date_format' => date_limit_format($format, $granularity),
      '#date_label_position' => 'within',
    );
    if ($which == 'all') {
      $dependency = array(
        '#process' => array($type .'_process', 'views_process_dependency'),
        '#dependency' => array($source => $operator_values),
      );
      $form[$prefix] += $dependency;
    }
    return $form;
  }

   function default_value($prefix, $options = NULL) {
    $default_date = '';
    if (empty($options)) {
      $options = $this->options;
    }
    $default_option = $prefix == 'max' ? $options['default_to_date'] : $options['default_date'];
    if (!empty($default_option)) {
      $date = date_now();
      if ($add = trim(str_replace('now', '', $default_option))) {
        date_modify($date, $add);
      }
      $default_date = date_format($date, DATE_FORMAT_DATETIME);
    }
    return $default_date;
  }
  
  /**
   * Value validation.
   * 
   * TODO add in more validation.
   * 
   * We are setting an extra option using a value form
   * because it makes more sense to set it there. 
   * That's not the normal method, so we have to manually
   * transfer the selected value back to the option.
   */
  function value_validate(&$form, &$form_state) {
    if (($form_state['values']['options']['operator'] == 'between' || $form_state['values']['options']['operator'] == 'not between') && 
    !empty($form_state['values']['options']['value']['default_date']) && empty($form_state['values']['options']['value']['default_to_date'])) {
      form_error($form['value']['default_to_date'], t('Please set a default value for the To date as well as the From date when using default values with the Between or Not between operators.'));
    }
    if (isset($form_state['values']['options']['value']['default_date'])) {
      $this->options['default_date'] = $form_state['values']['options']['value']['default_date'];
      $this->options['default_to_date'] = $form_state['values']['options']['value']['default_to_date'];
    }
    parent::value_validate($form, $form_state);
  }

  /**
   * Be sure that default values get set properly when there is no input.
   */
  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator']) && isset($input[$this->options['expose']['operator']])) {
      $this->operator = $input[$this->options['expose']['operator']];
    }
    if (!empty($this->options['expose']['identifier'])) {
      $value = $input[$this->options['expose']['identifier']];
      if (!isset($_GET[$this->options['expose']['identifier']])) {
        $value['min'] = $this->default_value('min');
        $value['max'] = $this->default_value('max');
        $value['value'] = $this->default_value('value');
      }
      
      // Various ways to check for the absence of optional input.
      if (!empty($this->options['expose']['optional'])) {
        if ($value == 'All' || $value === array()) {
          return FALSE;
        }
        if (!empty($this->no_single) && $value === '') {
          return FALSE;
        }
      }

      if (isset($value)) {
        $this->value = $value;
      }
      else {
        return FALSE;
      }
    }
    return TRUE;
  }
  
  // Update the summary values to provide
  // meaningful information for each option.
  function admin_summary() {
    if (empty($this->options['date_fields'])) {
      return t('Missing date fields!');
    }
    $handler = $this->date_handler;
    $field = implode(', ', $this->options['date_fields']);
    $output = "$field ". check_plain($this->operator) . ' ';
    $parts = $handler->date_parts();
    
    $widget_options = $this->widget_options();
    // If the filter is exposed, display the granularity.
    if ($this->options['exposed']) {
      return t('(@field) <strong>Exposed</strong> @widget @format', array('@field' => $field, '@format' => $parts[$handler->granularity], '@widget' => $widget_options[$this->options['form_type']]));
    }
    // If not exposed, display the value.
    if (in_array($this->operator, $this->operator_values(2))) {
      $min = check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['mindate']);
      $max = check_plain(!empty($this->options['default_to_date']) ? $this->options['default_to_date'] : $this->options['value']['maxdate']);
      $output .= t('@min and @max', array('@min' => $min, '@max' => $max));
    }
    else {
      $output .= check_plain(!empty($this->options['default_date']) ? $this->options['default_date'] : $this->options['value']['valuedate']);
    }
    return $output;
  }
 
  function op_between($field) {
    $this->date_filter('min', $field, '>=');
    $this->date_filter('max', $field, '<=');
    return;
  }

  function op_simple($field) {
    $this->date_filter('value', $field, $this->operator);
    return;
  }

  function date_filter($prefix, $field, $operator) {
    // If no value is supplied from an exposed filter use the default value.
    if (empty($this->options['exposed']) && empty($this->value[$prefix])) {
      $default = $this->default_value($prefix);
      if (empty($default)) {
        return;
      }
      else {
        $this->value[$prefix] = $default;
      }
    }
    // If there is no value, do no filtering.
    if (empty($this->value[$prefix])) {
      return;
    }
    $date_handler = $this->date_handler;
    $granularity = $this->options['granularity'];
    //$selected = $date_handler->arg_parts($this->value[$prefix]);
    //$complete_date = $date_handler->complete_date($selected[0]['date']);
    $complete_date = $this->value[$prefix];
    $date = date_make_date($complete_date);
    $this->format = $date_handler->views_formats($granularity, 'sql');
    $value = date_format($date, $this->format);
    $range = $this->date_handler->arg_range($value);
    $min_date = $range[0];
    $max_date = $range[1];
    
    $this->min_date = $min_date;
    $this->max_date = $max_date;
    
    $this->get_query_fields();
    if (!empty($this->query_fields)) {
      // Use set_where_group() with the selected date_method 
      // of 'AND' or 'OR' to create the where clause.
      //$this->query->set_where_group($this->options['date_method'], 'date');
      foreach ((array) $this->query_fields as $query_field) {
        $field = $query_field['field'];
        $date_handler = $query_field['date_handler'];
        $parts = $date_handler->date_parts();
    
        // Explicitly add this table using add_table so Views does not
        // remove it if it is a duplicate, since that will break the query.
        $this->query->add_table($field['table_name'], NULL, NULL, $field['table_name']);
        // Make sure the real field is added to the query.
        $this->query->add_field($field['table_name'], $field['field_name']);
        foreach ($field['related_fields'] as $related) {
          $bits = explode('.', $related);
          if ($bits[1] != $field['field_name']) {
            $this->query->add_field($field['table_name'], $bits[1]);
          }
        }
        $value = $prefix == 'max' ? date_format($max_date, $this->format) : date_format($min_date, $this->format);
        $sql = $date_handler->sql_where_format($this->format, $field['fullname'], $operator, $value);
        $this->query->add_where($this->options['group'], $sql);
        continue;
      }
    }
  }
  
  function get_query_fields() {
    $fields = date_api_fields();
    $fields = $fields['name'];
    $min_date = isset($this->min_date) ? $this->min_date : NULL;
    $min_utc = isset($this->min_utc) ? $this->min_utc : NULL;
    $max_date = isset($this->max_date) ? $this->max_date : NULL;
    $max_utc = isset($this->max_utc) ? $this->max_utc : NULL;
    $this->query_fields = array();
    foreach ((array) $this->options['date_fields'] as $delta => $name) {
      if (array_key_exists($name, $fields) && $field = $fields[$name]) {
        $date_handler = new date_sql_handler();
        $date_handler->construct($field['sql_type'], date_default_timezone_name());
        $date_handler->granularity = $this->options['granularity'];
        date_views_set_timezone($date_handler, $this->view, $field);
        $this->query_fields[] = array('field' => $field, 'date_handler' => $date_handler);
      }
    }
  }
}