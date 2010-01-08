<?php
// $Id: views_handler_field_node_link_delete.inc,v 1.1 2008/09/03 19:21:29 merlinofchaos Exp $
/**
 * Field handler to present a link to delete a node.
 */
class views_handler_field_note_link_delete extends views_handler_field_note_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    // ensure user has access to edit this node.
    if (!user_access('administer devel notes')) {
      return;
    }
    $note_id = $values->{$this->aliases['note_id']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return l($text, "admin/devel_notes/note/$note_id/delete", array('query' => drupal_get_destination()));
  }
}