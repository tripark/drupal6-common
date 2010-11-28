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
  
  // Height adjustment for content blocks
  var height = $('#content-below').height();
  $('#content-below .block').height(height + 1);
  $('#content-below .block-inner').height(height + 1);

  var height = $('#content-bottom').height();
  $('#content-bottom .block').height(height + 1);
  $('#content-bottom .block-inner').height(height + 1);
}
