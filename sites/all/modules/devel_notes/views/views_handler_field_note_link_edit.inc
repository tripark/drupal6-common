<?php
// $Id: views_handler_field_node_link_edit.inc,v 1.1 2008/09/03 19:21:29 merlinofchaos Exp $
/**
 * Field handler to present a link node edit.
 */
class views_handler_field_note_link_edit extends views_handler_field_note_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    // ensure user has access to edit this node.
    if (!user_access('administer devel notes')) {
      return;
    }
    $note_id = $values->{$this->aliases['note_id']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return l($text, "admin/devel_notes/note/$note_id/edit", array('query' => drupal_get_destination()));
  }
}

