$(document).ready(function() {
  // Traverse the DOM looking for any links with a title of "new-window" and
  // set a mouse click listener to open those links in a new window when
  // clicked.
  // This is done so that links in textile like the following can open
  // in a new window.
  // "example page(new-window)":http://example.com
  $("a[@title^='new-window']").each(function() {
    $(this).attr("title", '');
    $(this).click(function(){
      window.open(this.href);
      return false;
    });
  });
  
  // Alter CKEditor's link dialog to make the protocol setting more obvious.
  if (typeof(CKEDITOR) !== "undefined") {
    CKEDITOR.on( 'dialogDefinition', function( ev ) {
      // Take the dialog name and its definition from the event data.
      var dialogName = ev.data.name;
      var dialogDefinition = ev.data.definition;
    
      // Check if the definition is from the dialog we're
      // interested on (the "Link" dialog).
      if ( dialogName == 'link' ) {
        // Get a reference to the "Link Info" tab.
        var infoTab = dialogDefinition.getContents( 'info' );
    
        // Set the default value for the protocol field.
        var protocolField = infoTab.get( 'protocol' );
        protocolField['label'] = "Path type";
        protocolField['items'][4][0] = "(relative)";
        protocolField['default'] = '';
      }
    });
  }
});


/**
 * NOTE:
 * The function below overrides the function of the same name in tabledrag.js.
 * The code below has a flaw which breaks the use of table drag for CCK
 * multi-value fields inside of a collapsed fieldset.
 *
 * http://drupal.org/node/339105
 */

/**
 * Hide the columns containing form elements according to the settings for
 * this tableDrag instance.
 */
/*
Drupal.tableDrag.prototype.hideColumns = function(){
  for (var group in this.tableSettings) {
    // Find the first field in this group.
    for (var d in this.tableSettings[group]) {
      var field = $('.' + this.tableSettings[group][d]['target'] + ':first', this.table);
      if (field.size() && this.tableSettings[group][d]['hidden']) {
        var hidden = this.tableSettings[group][d]['hidden'];
        var cell = field.parents('td:first');
        break;
      }
    }

    // Hide the column containing this field.
    if (hidden && cell[0]) {
      // Add 1 to our indexes. The nth-child selector is 1 based, not 0 based.
      // Match immediate children of the parent element to allow nesting.
      var i = $('td', cell.parent()).index(cell.get(0)) + 1;
      $('> thead > tr, > tbody > tr, > tr', this.table).each(function(){
        var row = $(this);
        var parentTag = row.parent().get(0).tagName.toLowerCase();
        var index = i;

        // Adjust the index to take into account colspans.
        row.children().each(function(n) {
          if (n < index) {
            index -= (this.colSpan && this.colSpan > 1) ? this.colSpan - 1 : 0;
          }
        });
        if (index > 0) {
          cell = row.children(':nth-child(' + index + ')');
          if (cell[0].colSpan > 1) {
            // If this cell has a colspan, simply reduce it.
            cell[0].colSpan = cell[0].colSpan - 1;
          }
          else {
            cell.css('display', 'none');
          }
        }
      });
    }
  }
};
*/
