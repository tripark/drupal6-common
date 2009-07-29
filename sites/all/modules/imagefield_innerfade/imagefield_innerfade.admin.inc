<?php
// $Id:
/**
 * Adminstration pages for configuring Imagefield Innerfade settings.
 */

/**
 * Form builder; Menu callback for imagefield_innerfade settings form.
 */
function imagefield_innerfade_settings() {
  $form = array();
  $form['imagefield_innerfade_help'] = array(
    '#value' => t('Settings for the imagefield innerfade slideshow.'),
  );
  $form['imagefield_innerfade_animationtype'] = array(
    '#type' => 'select',
    '#title' => t('Animation Type'),
    '#description' => t('Animation style that the innerfade javascript will use when transitioning between images.'),
    '#options' => array('fade' => t('fade'), 'slide' => t('slide')),
    '#default_value' => variable_get('imagefield_innerfade_animationtype', 'fade'),
  );
  $form['imagefield_innerfade_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('Fading/sliding speed in milliseconds or keywords (slow, normal or fast).'),
    '#size' => 12,
    '#default_value' => variable_get('imagefield_innerfade_speed', 'normal'),
  );
  $form['imagefield_innerfade_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('Time between fades in milliseconds.'),
    '#size' => 12,
    '#default_value' => variable_get('imagefield_innerfade_timeout', '2000'),
  );
  $form['imagefield_innerfade_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of slideshow'),
    '#options' => array('sequence' => t('sequence'), 'random' => t('random'), 'random_start' => t('random start')),
    '#default_value' => variable_get('imagefield_innerfade_type', 'sequence'),
  );
  $form['imagefield_innerfade_containerheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t("Height of the containing element in any css-height-value. 'auto' will calculate the height of the largest image."),
    '#size' => 12,
    '#default_value' => variable_get('imagefield_innerfade_containerheight', 'auto'),
  );
  $form['imagefield_innerfade_runningclass'] = array(
    '#type' => 'textfield',
    '#title' => t('Running class'),
    '#description' => t('CSS class applied to the container while the slideshow is running.'),
    '#default_value' => variable_get('imagefield_innerfade_runningclass', 'innerfade'),
  );
  $form['imagefield_innerfade_children'] = array(
    '#type' => 'textfield',
    '#title' => t('Children'),
    '#description' => t('Optional jQuery selector for children.'),
    '#default_value' => variable_get('imagefield_innerfade_children', NULL),
  );
  return system_settings_form($form);
}