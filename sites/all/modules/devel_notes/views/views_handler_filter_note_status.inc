<?php
/**
 * Filter by node type
 */
class views_handler_filter_note_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Note status');
      $options = array(t("Active"), t("Finished"), t("Needs Info"),);
      $this->value_options = $options;
    }
  }
}
