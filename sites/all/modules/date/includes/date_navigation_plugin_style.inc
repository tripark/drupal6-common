<?php
// $Id: date_navigation_plugin_style.inc,v 1.1.2.3 2008/09/19 21:54:17 karens Exp $
/**
 * Style plugin to create date back/next navigation.
 * 
 * The style plugin passes some argument values to the theme, and
 * ensures that the date argument is present and that the default
 * value is set to the current date.
 */
class date_navigation_plugin_style extends views_plugin_style {
  
  /**
   * Style validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $arguments = $this->display->handler->get_option('arguments');
    $count = 0;
    $found = FALSE;
    foreach ($arguments as $id => $argument) {
      if ($argument['field'] == 'date_argument') {
        if ($count > 0) {
          $errors[] = t('The @style cannot use more than one Date: Date argument.', array('@style' => $this->definition['title']));          
        }
        elseif ($argument['default_argument_type'] != 'date') {
          $errors[] = t('The @style requires the Date: Date argument be set to default to the current date.', array('@style' => $this->definition['title']));      
        }
        $count++;
        $found = TRUE;
      }
    }
    if (!$found) {
      $errors[] = t('The @style requires the Date: Date argument.', array('@style' => $this->definition['title']));      
    }
    return $errors;
  }
  
  function query() {
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_sql.inc');
        
    // Bring the argument information into the view so our theme can access it.
    $i = 0;    
    foreach ($this->view->argument as $id => $argument) {
      if ($id == 'date_argument') {
        $this->view->granularity = $argument->granularity;
        $this->view->date_arg = $argument->argument;
        $this->view->date_arg_pos = $i;
        $this->view->year = isset($argument->year) ? $argument->year : NULL;
        $this->view->month = isset($argument->month) ? $argument->month: NULL;
        $this->view->day = isset($argument->day) ? $argument->day : NULL;
        $this->view->week = isset($argument->week) ? $argument->week : NULL;
        $this->view->min_date = $argument->min_date;
        $this->view->max_date = $argument->max_date;
        $this->view->url = $this->view->get_url();
      }
      $i++;
    }
     
    // bring the node type into the query so we can use it in the theme
    $this->view->query->add_field('node', 'type');
     
  }
}
