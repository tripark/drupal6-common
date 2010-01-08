<?php
// $Id: views_handler_field_numeric.inc,v 1.6 2009/07/02 00:13:12 merlinofchaos Exp $
/**
 * Render a field as a numeric value
 *
 * Definition terms:
 * - float: If true this field contains a decimal value. If unset this field
 *          will be assumed to be integer.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_note_status extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($value) {
      case 0:
        $output = "Active";
        break;
      case 1:
        $output = "Finished";
        break;
      case 2:
        $output = "Needs Info";
        break;
      default:
      $output = NULL;
    }
    return $output;
  }
}
