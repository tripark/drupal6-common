<?php
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
