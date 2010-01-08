// $Id$
/**
 * @file
 *
 * devel_notes javascript functions.
 */

(function ($) {
  Drupal.develNotes = Drupal.develNotes || {};

  Drupal.behaviors.develNotesToggleTab = function (context) {
    $('body').addClass('devel-notes');
    $('#devel-notes-tab').click(function(){
  	  if ($(this).html() == 'Notes') {
  	    $(this).html('Close');
  	    $('#devel-notes-badge').hide();
  	    Drupal.develNotes.enableSelection();
  	    Drupal.develNotes.displayInlineNotes();
  	  }
  	  else {
  	    $(this).html('Notes');
  	    $('#devel-notes-badge').show();
  	    Drupal.develNotes.disableSelection();
  	    Drupal.develNotes.hideInlineNotes()
  	  }
  		$('#devel-notes').toggleClass('expanded');
  		$('#devel-notes-inner').toggle();
  		return false;
  	});
  }

  /**
   * Bind click, mouseover and, mouseout events for highlighting and selecting
   * any div with an ID in the DOM.
   */
  Drupal.develNotes.enableSelection = function() {
    $('div[id]').bind('click.develNotes', function() {
      // Remove highlighting from any previously clicked elements.
      $('*').removeClass('devel-notes-selected').css('outline', '');
      // Only allow selection of non #devel_notes > * elements.
      if ($(this).attr('id') != 'devel-notes' && !$(this).parents('#devel-notes').length) {
        // Do not call bound click events for this elements parents.
        $(this).parents().unbind('click.develNotes');
        id_path = '#' + $(this).attr('id');
        $('#devel-notes #edit-selector').val(id_path);
        $(this).addClass('devel-notes-selected');
        return false;
      }
    });

    $('div[id]').bind('mouseover.develNotes', function(e) {
      // Only allow highlighting of non #devel_notes > * elements.
      if ($(this).attr('id') != 'devel-notes' && !$(this).parents('#devel-notes').length) {
        $(this).addClass('devel-notes-mouseover');
        e.stopPropagation();
      }
    });

    $('div[id]').bind('mouseout.develNotes', function() {
      $(this).removeClass('devel-notes-mouseover');
    });
  }

  /**
   * Removing bindings set by Drupal.develNotes.enableSelection().
   */
  Drupal.develNotes.disableSelection = function() {
    $('*').removeClass('devel-notes-selected').removeClass('devel-notes-mouseover');
    $('div[id]').unbind('click.develNotes');
    $('div[id]').unbind('mouseover.develNotes');
    $('div[id]').unbind('mouseout.develNotes');
  }

  Drupal.develNotes.displayInlineNotes = function (context) {
    if (Drupal.settings.develNotes) {
      jQuery.each(Drupal.settings.develNotes, function (key, value) {
        $(value.selector)
          .prepend('<div class="devel-notes-inline-note-badge">' + value.note_number + '</div>')
          .addClass('devel-notes-inline-note-selected')
          .bind('mouseover.develNotes', function() {
            $('#' + key).addClass('devel-notes-note-selected');
          })
          .bind('mouseout.develNotes', function() {
            $('#' + key).removeClass('devel-notes-note-selected');
          });
      });
    }
  }

  Drupal.develNotes.hideInlineNotes = function (context) {
    if (Drupal.settings.develNotes) {
      jQuery.each(Drupal.settings.develNotes, function (key, value) {
        $(value.selector).removeClass('devel-notes-inline-note-selected');
        $('.devel-notes-inline-note-badge').remove();
      });
    }
  }
})(jQuery);
