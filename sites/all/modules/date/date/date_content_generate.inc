<?php
/**
 * Implementation of Devel module's hook_content_generate().
 *
 * Included only when needed.
 */
function _date_content_generate($node, $field) {
  $node_field = array();
  if (isset($field['widget']['year_range'])) {
    $split = explode(':', $field['widget']['year_range']);
    $back = str_replace('-', '', $split[0]);
    $forward = str_replace('+', '', $split[1]);
  }
  else {
    $back = 2;
    $forward = 2;
  }
  // Pick a random year within the time range,
  // and a random second within that year.
	$year = date_format(date_now(), 'Y') - $back + rand(0, ($forward + $back));
	$start = date_make_date($year .'-01-01 00:00:00', 'UTC');
	$leap = date_format($start, 'L');
	$max_days = $leap ? 366 : 365;
	$seconds = rand(0, ($max_days * 86400));
	date_modify($start, "+$seconds seconds");

	// Modify To date by 1 hour to 1 day.
	$start2 = drupal_clone($start);
  date_modify($start2, '+'. rand(60, 1440) .' minutes');  
    
  if ($field['tz_handling'] == 'date') {
    // Choose a random timezone.
    $timezones = array_keys(date_timezone_names(TRUE, TRUE));
    shuffle($timezones);
    $timezone = $timezones[0];
  }
    
  switch ($field['type']) {
    case 'date':
      $format = DATE_FORMAT_ISO;
      break;
    case 'datestamp':
      $format = DATE_FORMAT_UNIX;
      break;
    case 'datetime':
      $format = DATE_FORMAT_DATETIME;
      break;
  }
  $count = $field['multiple'] > 0 && $field['multiple'] < 99  ? rand(1, 6) : 1;
  for($i = 0; $i < $count; $i++) {
    $node_field[$i]['value'] = date_format($start, $format);
    if ($field['todate']) {
      $node_field[$i]['value2'] = date_format($start2, $format);
    }
    if ($field['tz_handling'] == 'date') {
      date_timezone_set($start, timezone_open($timezone));
      $node_field[$i]['timezone'] = $timezone;
      $node_field[$i]['offset'] = date_offset_get($start);
      if ($field['todate']) {
        date_timezone_set($start2, timezone_open($timezone));
        $node_field[$i]['offset2'] = date_offset_get($start2);     	 	
      }
    }
  }
  if ($field['multiple'] == 99) {
    // TODO
    // Create a repeating date.
  }
  return $node_field;
}