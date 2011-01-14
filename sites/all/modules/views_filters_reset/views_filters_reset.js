// $Id: views_filters_reset.js,v 1.1.2.4 2010/10/21 09:14:25 kratib Exp $
(function ($) {
// START jQuery

Drupal.vfr = Drupal.vfr || {};

Drupal.behaviors.vfr = function(context) {
  $.each(Drupal.settings.vfr, function(form_id, url) {
    $('form#'+form_id+' input#edit-reset', context).click(function() {
      if (url) {
        window.location = url;
      }
      else {
        $('form#'+form_id, context).clearForm();
        $('form#'+form_id, context).submit();
      }
    });
  });
}

Drupal.vfr.ajaxViewResponse = function(target, response) {
  $('form#'+response.exposed_form_id).replaceWith(response.exposed_form);
  Drupal.attachBehaviors($('form#'+response.exposed_form_id).parent());
}

// END jQuery
})(jQuery);

