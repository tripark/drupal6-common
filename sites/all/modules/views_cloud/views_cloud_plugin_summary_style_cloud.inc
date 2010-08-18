<?php

/**
 * A tag-cloud summary style view.
 */
class views_cloud_plugin_summary_style_cloud extends views_plugin_style_summary {
  function option_definition() {
    $options = parent::option_definition();
    $options['count'] = array('default' => FALSE);
    $options['sort'] = array('default' => 'desc');
    $options['randomize'] = array('default' => TRUE);
    $options['sizes'] = array('default' => '7');
    $options['font_size'] = array('default' => '');
    $options['algorithm'] = array('default' => 'log');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['sort'] = array(
      '#type' => 'radios',
      '#title' => t('Override sort to show'),
      '#options' => array(
        'desc' => t('Most popular items by count'),
        'random' => t('Random items'),
      ),
      '#description' => t('This will ignore all normal sorting and sort instead by the item count.'),
      '#default_value' => $this->options['sort'],
    );

    $form['randomize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize the order of shown items'),
      '#description' => t("This setting respects the View's sort order when limiting large paged lists, but shuffles each list of items when displayed on the page."),
      '#default_value' => $this->options['randomize'],
    );

    $form['sizes'] = array(
      '#type' => 'select',
      '#title' => t('Number of font sizes'),
      '#options' => drupal_map_assoc(range(4,10)),
      '#default_value' => $this->options['sizes'],
      '#description' => t('Increasing the number of sizes increases the number of possible sizes an item may be.'),
    );

    $form['font_size'] = array(
      '#type' => 'select',
      '#title' => t('Font size adjustment'),
      '#options' => array(
        '0.5' => t('50%'),
        '0.6' => t('60%'),
        '0.7' => t('70%'),
        '0.8' => t('80%'),
        '0.9' => t('90%'),
        '' => t('None'),
        '1.1' => t('110%'),
        '1.2' => t('140%'),
        '1.3' => t('130%'),
        '1.4' => t('140%'),
        '1.5' => t('150%'),
      ),
      '#default_value' => $this->options['font_size'],
      '#description' => t('Note that changing this value may affect your site differently depending on your theme.'),
    );

    $form['algorithm'] = array(
      '#type' => 'select',
      '#title' => t('Weight to size algorithm'),
      '#description' => t("Select the item weight sizing algorithm, linear or logarithmic. Logarithmic emphasizes the relative weight of items."),
      '#options' => array('log' => t('Logarithmic'), 'linear' => t('Linear')),
      '#default_value' => $this->options['algorithm'],
    );
  }

  function query() {
    parent::query();

    // Add a filter to include a certain count threshold.
    if ($this->options['count_filter']) {
      $this->view->query->add_where(0, 'num_records > %d', $this->options['count_filter']);
    }

    // Override normal sorting and sort by number of results.
    if ($this->options['sort'] == 'random') {
      global $db_type;
      switch ($db_type) {
        case 'mysql':
        case 'mysqli':
          $formula = 'RAND()';
          break;
        case 'pgsql':
          $formula = 'RANDOM()';
          break;
      }
      if (!empty($formula)) {
        $this->view->query->orderby = array();
        $this->view->query->add_field(NULL, $formula, 'random', array('aggregate' => TRUE));
        $this->view->query->orderby[] = 'random DESC';
      }
    }
    elseif ($this->options['sort']) {
      $this->view->query->orderby = array();
      $this->view->query->add_orderby(NULL, NULL, $this->options['sort'], 'num_records');
    }
  }
}
