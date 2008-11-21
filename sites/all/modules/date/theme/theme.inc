<?php
// $Id: theme.inc,v 1.1.4.18 2008/10/15 14:47:28 karens Exp $
/**
 *  Preprocessor to construct back and next navigation from the date argument.
 */
function template_preprocess_date_navigation(&$vars) {
  $view = $vars['view'];
  $next_args = $view->args;
  $prev_args = $view->args;
  $pos = $view->date_arg_pos;
  if (!isset($view->hide_nav)) {
    $min_date = is_object($view->min_date) ? $view->min_date : date_now();
    $max_date = is_object($view->max_date) ? $view->max_date : date_now();
    $prev_date = drupal_clone($min_date);
    date_modify($prev_date, '-1 '. $view->granularity);
    $next_date = drupal_clone($min_date);
    date_modify($next_date, '+1 '. $view->granularity);
    $format = array('year' => 'Y', 'month' => 'Y-m', 'day' => 'Y-m-d');
    switch ($view->granularity) {
      case 'week':
        $next_week = date_week(date_format($next_date, 'Y-m-d'));
        $prev_week = date_week(date_format($prev_date, 'Y-m-d'));
        $next_arg = date_format($next_date, 'Y-\W') . $next_week;
        $prev_arg = date_format($prev_date, 'Y-\W') . $prev_week;
        break;
      default:
        $next_arg = date_format($next_date, $format[$view->granularity]);
        $prev_arg = date_format($prev_date, $format[$view->granularity]);
    }
    $next_path = str_replace($view->date_arg, $next_arg, $view->url);
    $prev_path = str_replace($view->date_arg, $prev_arg, $view->url);
    $next_args[$pos] = $next_arg;
    $prev_args[$pos] = $prev_arg;
  }
  else {
    $next_path = '';
    $prev_path = '';
  }
  
  $vars['next_url'] = $next_path;
  $vars['prev_url'] = $prev_path;
    
  if (!empty($view->block)) {
    $current = $_GET;
    $current_query = '';
    $block_identifier = isset($view->block_identifier) ? $view->block_identifier : 'mini';
    unset($current['q']);
    unset($current[$block_identifier]);
    foreach($current as $key => $val) {
      $current_query .= $key .'='. $val .'&';
    }
    $vars['next_url'] = $vars['prev_url'] = $_GET['q'];
    $vars['next_options'] = array('query' => $current_query . $block_identifier .'='. $next_path);
    $vars['prev_options'] = array('query' => $current_query . $block_identifier .'='. $prev_path);
  }
  else {
    $vars['next_url'] = $next_path;
    $vars['prev_url'] = $prev_path;
    $vars['next_options'] = $vars['prev_options'] = array();
  }

  $link = FALSE;
  // Month navigation titles are used as links in the block view.
	if (!empty($view->block) && $view->granularity == 'month') {
	  $link = TRUE;
  }
  $nav_title = theme('date_nav_title', $view->granularity, $view, $link);
  $vars['nav_title'] = $nav_title;
  $vars['block'] = !empty($view->block);
}
/**
 * Theme the calendar title
 */
function theme_date_nav_title($granularity, $view, $link = FALSE, $format = NULL) {
  switch ($granularity) {
    case 'year':
      $title = $view->year;
      $url = $view->url .'/'. $view->year;
      break;
    case 'month':
      $title = date_format_date($view->min_date, 'custom', !empty($format) ? $format : 'F');
      $url = $view->url .'/'. $view->year .'-'. date_pad($view->month);
      break;
    case 'day':
      $title = date_format_date($view->min_date, 'custom', !empty($format) ? $format : 'l, F j Y');
      $url = $view->url .'/'. $view->year .'-'. date_pad($view->month) .'-'. date_pad($view->day);
      break;
    case 'week':
    	$title = t('Week of @date', array('@date' => date_format_date($view->min_date, 'custom', !empty($format) ? $format : 'F j')));
    	$url = $view->url .'/'. $view->year .'-W'. date_pad($view->week);
    	break;
  }
  // TODO Update this.
  //if (!empty($view->mini) || $link) {
  //	// Month navigation titles are used as links in the mini view.
  //	return l($title, $url, array(), calendar_url_append($view));
  //}
  //else {
    return $title;
  //}  
}

/**
 *  Preprocessor to construct an ical vcalendar
 * 
 * @param $events
 *   An array of events where each event is an array keyed on the uid:
 *    'start'
 *      Start date object,
 *    'end'
 *      End date object, optional, omit for all day event.
 *    'summary'
 *      Title of event (Text)
 *    'description'
 *      Description of event (Text)
 *    'location'
 *      Location of event (Text or vvenue id)
 *    'uid'
 *      ID of the event for use by calendaring program, usually the url of the node
 *    'url'
 *      URL of event information
 * 
 *    'alarm'
 *      sub-array of alarm information for the event, including:
 *      - 'action' - the action to take, either 'DISPLAY' or 'EMAIL'
 *      - 'trigger' - the time period for the trigger, like -P2D.
 *      - 'repeat' - the number of times to repeat the alarm.
 *      - 'duration' - the time period between repeated alarms, like P1D.
 *      - 'description' - the description of the alarm.
 *      An email alarm should have two additional parts:
 *      - 'email' - a comma-separated list of email recipients.
 *      - 'summary' - the subject of the alarm email.
 *
 * @param $calname
 *   Name of the calendar.  Use site name if none is specified.
 * 
 */
function template_preprocess_date_vcalendar(&$vars) {
    
  $vars['current_date'] = date_format(date_now(), DATE_FORMAT_ICAL);
  $vars['site_timezone'] = date_default_timezone_name();
  $vars['calname'] = date_ical_escape_text(!empty($vars['calname']) ? $vars['calname'] : variable_get('site_name', ''));
  
  // Format the event results as iCal expects.
  $events_in = $vars['events'];
  $events = array();
  foreach ($events_in as $uid => $event) {
    // Omit any items with empty dates.
    if (!empty($event['start'])) {
      $events[$uid] = $event;
      $timezone = timezone_name_get(date_timezone_get($event['start']));
      if (!empty($timezone)) {
        $events[$uid]['timezone'] = "TZID=$timezone;";
      }
      else {
        $events[$uid]['timezone'] = '';
      }
      $events[$uid]['start'] = date_format($event['start'], DATE_FORMAT_ICAL);
      if ($event['start'] && $event['end']) {
        $events[$uid]['end'] = date_format($event['end'], DATE_FORMAT_ICAL);
      }
      else {
        $events[$uid]['end'] = $events[$uid]['start'];
      }
      foreach ($event as $key => $value) {
        if (is_string($value)) {
          $event[trim($key)] = trim($value);
        }
      }
      
      // Escape text values.
      foreach ($event as $key => $value) {
        if ($key == 'alarm') {
          foreach ($value as $alarm_key => $alarm_value) {
            if (in_array($alarm_key, array('summary', 'description'))) {
              $events[$uid]['alarm'][$alarm_key] = date_ical_escape_text($alarm_value);
            }
          }
        }
        elseif (in_array($key, array('summary', 'description', 'location'))) {
          $events[$uid][$key] = date_ical_escape_text($value);
        }
      }
    }
  }
  
  $vars['events'] = $events;  
}

/**
 * Preprocessor for Date Views filter form.
 */
function template_preprocess_date_views_filter_form(&$vars) {
  $form = $vars['form'];
  $vars['date'] = drupal_render($form['valuedate']);
  $vars['mindate'] = drupal_render($form['mindate']);
  $vars['maxdate'] = drupal_render($form['maxdate']);
  $vars['adjustment'] = drupal_render($form['valueadjustment']);
  $vars['minadjustment'] = drupal_render($form['minadjustment']);
  $vars['maxadjustment'] = drupal_render($form['maxadjustment']);
  $vars['description'] = drupal_render($form['description']) . drupal_render($form);
}

/**
 * Format a date timezone element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_timezone($element) {
  return '<div class="date-clear">'. theme('form_element', $element, $element['#children']) .'</div>';
}

/**
 * Format a date selection element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_select($element) {
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="'. $class .'">'. theme('form_element', $element, $output) .'</div>';
}

/**
 * Format a date text element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_text($element) {
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="'. $class .'">'. theme('form_element', $element, $output) .'</div>';
}

/**
 *  Themes for date input form elements
 */
function theme_date_select_element($element) {
  $part = array_pop($element['#parents']);
  return '<div class="date-'. $part .'">'. theme('select', $element) .'</div>';
}

function theme_date_textfield_element($element) {
  $part = array_pop($element['#parents']);
  return '<div class="date-'. $part .'">'. theme('textfield', $element) .'</div>';
}

/**
 * Functions to separate date parts in form.
 *
 * Separators float up to the title level for elements with titles,
 * so won't work if this element has titles above the element date parts.
 */
function theme_date_part_hour_prefix($element) {
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">&nbsp;-&nbsp;</span>';
  }
}

function theme_date_part_minsec_prefix($element) {
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">:</span>';
  }
}

/**
 * Format labels for each date part in a date_select.
 *
 * @param $part_type
 *   the type of field used for this part, 'textfield' or 'select'
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 */
function theme_date_part_label_year($part_type, $element) {
  return t('Year');
}
function theme_date_part_label_month($part_type, $element) {
  return t('Month');
}
function theme_date_part_label_day($part_type, $element) {
  return t('Day');
}
function theme_date_part_label_hour($part_type, $element) {
  return t('Hour');
}
function theme_date_part_label_minute($part_type, $element) {
  return t('Minute');
}
function theme_date_part_label_second($part_type, $element) {
  return t('Second');
}
function theme_date_part_label_ampm($part_type, $element) {
  return t('am/pm');
}
function theme_date_part_label_timezone($part_type, $element) {
  return t('Timezone');
}