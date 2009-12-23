<?php
// $Id$

/**
 * @file
 * Administrative functions for devel_notes.module.
 */

/**
 * Build a form for editing a note. Callback for admin/devel_notes/note/%devel_notes_note/edit
 */
function devel_notes_note_edit($form_state, $note) {
  $form = array();
  $form['#note'] = $note;
  
	$form['pg'] = array(
	  '#type' => 'text',
	  '#default_value' => $note->pg,
	  '#required' => TRUE,
	);
	$form['status'] = array(
	  '#type' => 'select',
	  '#title' => t('Status'),
	  '#options' => _devel_notes_get_status_ops(),
	  '#default_value' => $note->status,
	);
	$form['note'] = array(
		'#type' => 'textarea',
		'#title' => t('Note'),
		'#rows' => 3,
		'#required' => TRUE,
		'#default_value' => $note->note
	);
	$form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
	$form['delete'] = array(
	  '#type' => 'submit',
	  '#value' => t('Delete'),
	  '#submit' => array('devel_notes_delete_form_submit'),
	  '#weight' => 15,
	);
	return $form;
}

/**
 * Callback for devel_notes_note_edit form
 *
 * Saves update note information to the database.
 */
function devel_notes_note_edit_submit($form, &$form_state) {
  $note = $form['#note'];
  $note->status = $form['status']['#value'];
  $note->note = $form['note']['#value'];

  module_invoke_all('noteapi', 'update', $note);

  db_query('UPDATE {devel_notes} SET uid=%d, pg="%s", status=%d, note="%s", info="%s" WHERE note_id=%d', $note->uid, $note->pg, $note->status, $note->note, $note->info, $note->note_id);
  drupal_set_message(t('The note has been updated'));
}

/**
 * Menu callback - confirm intente to delete $note.
 */
function devel_notes_delete_confirm(&$form_state, $note) {
  $form = array();
  $form['#note'] = $note;
  return confirm_form($form,
    t('Are you sure you want to delete the note %note?', array('%note' => $note->short)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/devel_notes/note/' . $note->note_id,
    t('This action cannot be undone'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Callback for devel_notes_delete_confirm.
 */
function devel_notes_delete_confirm_submit($form, &$form_state) {
  $note = $form['#note'];
  if ($form_state['values']['confirm']) {
    devel_notes_note_delete($note->note_id);
    drupal_set_message(t('The note %note has been deleted.', array('%note' => $note->short)));
  }

  $form_state['redirect'] = $note->pg;
}

/**
 * Callback for delete button on note edit form.
 */
function devel_notes_delete_form_submit($form, &$form_state) {
  $destination = '';
  if (isset($_REQUEST['destination'])) {
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }
  $note = $form['#note'];
  $form_state['redirect'] = array('admin/devel_notes/note/'. $note->note_id .'/delete', $destination);
}