Drupal.behaviors.TPCTheme = function(context) {
  
  // Search box text replacement
  if ($('#search-box .form-text').val() == "") {
    $('#search-box .form-text').val('Search');
  }

  $('#search-box .form-text').focus(function(){
    if (jQuery(this).val() == "Search") {
      jQuery(this).val('');
    }
  });
  $('#search-box .form-text').blur(function(){
    if (jQuery(this).val() == "") {
      jQuery(this).val('Search');
    }
  });

}
