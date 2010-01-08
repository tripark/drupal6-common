<?php
/**
 * Implementation of hook_views_data().
 */
function devel_notes_views_data() {
  $data = array();
  $data['devel_notes']['table']['group'] = t('Devel Notes');
  
  // Define devel_notes as a base table.
  $data['devel_notes']['table']['base'] = array(
    'field' => 'note_id',
    'title' => t('Devel Notes'),
    'help' => t('Developer notes and in-site help text.'),
    'weight' => 0,
  );
  
  // Make devel_notes information available on the "Node" base table.
  $data['devel_notes']['table']['join']['node'] = array(
    'handler' => 'views_join_node_path',
    'left_field' => 'nid',
    'field' => 'pg',
  );
  
  // Field descriptions.
  $data['devel_notes']['note_id'] = array(
    'title' => t('Note ID'),
    'help' => t('The unique ID of this note.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['devel_notes']['pg'] = array(
    'title' => t('Path'),
    'help' => t('The Drupal path on which this note is displayed.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['devel_notes']['status'] = array(
    'title' => t('Status'),
    'help' => t('A flag that describes the note as active, pending or finished.'),
    'field' => array(
      'handler' => 'views_handler_field_note_status',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_note_status',
    ),
  );
  $data['devel_notes']['note'] = array(
    'title' => t('Body'),
    'help' => t('The text of the note.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['devel_notes']['view_note'] = array(
    'title' => t('Link'),
    'help' => t('A simple link to the note.'),
    'field' => array(
      'handler' => 'views_handler_field_note_link',
    ),
  );
  $data['devel_notes']['edit_note'] = array(
    'title' => t('Edit Link'),
    'help' => t('A simple link to edit the note.'),
    'field' => array(
      'handler' => 'views_handler_field_note_link_edit',
    ),
  );
  $data['devel_notes']['delete_note'] = array(
    'title' => t('Delete Link'),
    'help' => t('A simple link to delete the note.'),
    'field' => array(
      'handler' => 'views_handler_field_note_link_delete',
    ),
  );
  $data['devel_notes']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The unique user ID of the note author.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['devel_notes']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the note was written.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 *
 * Expose user information to Devel Note views by default.
 */
function devel_notes_views_data_alter(&$data) {
  $data['users']['table']['join']['devel_notes'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
    'type' => 'INNER',
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function devel_notes_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'devel_notes') . '/views',
    ),
    'handlers' => array(
      // field handlers
      'views_handler_field_note_link' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_note_link_edit' => array(
        'parent' => 'views_handler_field_note_link',
      ),
      'views_handler_field_note_link_delete' => array(
        'parent' => 'views_handler_field_note_link',
      ),
      'views_handler_field_note_status' => array(
        'parent' => 'views_handler_field',
      ),
      // filter handlers
      'views_handler_filter_note_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Join handler for nids in node link paths.
 */
class views_join_node_path extends views_join {
 function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT') {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
  }
  /**
   * Just like the parent join(), but with a different initial $output string.
   */
  function join($table, &$query) {
    if (empty($this->definition['table formula'])) {
      $right_table = "{" . $this->table . "}";
    }
    else {
      $right_table = $this->definition['table formula'];
    }

    if ($this->left_table) {
      $left = $query->get_table_info($this->left_table);
      $left_field = "$left[alias].$this->left_field";
    }
    else {
      // This can be used if left_field is a formula or something. It should be used only *very* rarely.
      $left_field = $this->left_field;
    }

    $output = " $this->type JOIN $right_table $table[alias] ON CONCAT('node/',$left_field) = $table[alias].$this->field";
    // Tack on the extra.
    if (isset($this->extra)) {
      if (is_array($this->extra)) {
        $extras = array();
        foreach ($this->extra as $info) {
          $extra = '';
          // Figure out the table name. Remember, only use aliases provided
          // if at all possible.
          $join_table = '';
          if (!array_key_exists('table', $info)) {
            $join_table = $table['alias'] . '.';
          }
          elseif (isset($info['table'])) {
            $join_table = $info['table'] . '.';
          }

          // And now deal with the value and the operator.  Set $q to
          // a single-quote for non-numeric values and the
          // empty-string for numeric values, then wrap all values in $q.
          $raw_value = $this->db_safe($info['value']);
          $q = (empty($info['numeric']) ? "'" : '');

          if (is_array($raw_value)) {
            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            // Transform from IN() notation to = notation if just one value.
            if (count($raw_value) == 1) {
              $value = $q . array_shift($raw_value) . $q;
              $operator = $operator == 'NOT IN' ? '!=' : '=';
            }
            else {
              $value = "($q" . implode("$q, $q", $raw_value) . "$q)";
            }
          }
          else {
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            $value = "$q$raw_value$q";
          }
          $extras[] = "$join_table$info[field] $operator $value";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $output .= ' AND ' . array_shift($extras);
          }
          else {
            $output .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      else if ($this->extra && is_string($this->extra)) {
        $output .= " AND ($this->extra)";
      }
    }
    return $output;
  }
}
