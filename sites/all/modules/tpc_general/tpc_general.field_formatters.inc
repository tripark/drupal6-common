<?php
/**
 * Implements hook_field_formatter_info().
 *
 * Add a text formatter similar to plain text, but with HTML line breaks.
 * Add a text formatter for formatting email addresses as mailto: links.
 */
function tpc_general_field_formatter_info() {
  return array(
    'text_plain_linebreaks' => array(
      'label' => t('Plain text with linebreaks'),
      'field types' => array('text'),
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
    'text_email' => array(
      'label' => t('Email address as "mailto:" link'),
      'field types' => array('text'),
      'multiple values' => CONTENT_HANDLE_CORE,
    ),
  );
}