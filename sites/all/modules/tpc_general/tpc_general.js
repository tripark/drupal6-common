Drupal.behaviors.tpc_general = function(context) {
  // Traverse the DOM looking for any links with a title of "new-window" and
  // set a mouse click listener to open those links in a new window when
  // clicked.
  // This is done so that links in textile like the following can open
  // in a new window.
  // "example page(new-window)":http://example.com
  $("a[title^='new-window']").each(function() {
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
  
  
  // hide unwanted disabled system blocks from the block administration form.
  if (Drupal.settings.tpc_general.q = 'admin/build/block' &&
      Drupal.settings.tpc_general.core_blocks_disable == 1) {
    $('#blocks .region--1').nextAll().addClass('disabled');
    $('#edit-menu-devel-weight').parents('.disabled').hide(); // Development
    $('#edit-menu-features-weight').parents('.disabled').hide(); // Features
    $('#edit-menu-primary-links-weight').parents('.disabled').hide(); // Primary links
    $('#edit-menu-secondary-links-weight').parents('.disabled').hide(); // Secondary links
    $('#edit-devel-0-weight').parents('.disabled').hide();    // Switch user
    $('#edit-devel-2-weight').parents('.disabled').hide();    // Execute PHP
    $('#edit-node-0-weight').parents('.disabled').hide();     // Syndicate
    $('#edit-user-2-weight').parents('.disabled').hide();     // Who's new
    $('#edit-user-3-weight').parents('.disabled').hide();     // Who's online
    $('#edit-system-0-weight').parents('.disabled').hide();   // Powered by Drupal
  }
  
  // hide unused themes.
  if (Drupal.settings.tpc_general.q = 'admin/build/themes' &&
      Drupal.settings.tpc_general.core_themes_disable == 1) {
    $('#edit-status-bluemarine').not(':checked').parents('tr').hide();
    $('#edit-status-chameleon').not(':checked').parents('tr').hide();
    $('#edit-status-garland').not(':checked').parents('tr').hide();
    $('#edit-status-marvin').not(':checked').parents('tr').hide();
    $('#edit-status-minnelli').not(':checked').parents('tr').hide();
    $('#edit-status-pushbutton').not(':checked').parents('tr').hide();
    $('#edit-status-zen').not(':checked').parents('tr').hide();
    $('#edit-status-zen-classic').not(':checked').parents('tr').hide();
    $('#edit-status-STARTERKIT').not(':checked').parents('tr').hide();
    $('#system-themes-form .form-radio:checked').not(':checked').parents('tr').show();
  }
  
};
