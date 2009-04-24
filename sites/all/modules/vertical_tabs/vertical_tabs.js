Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.behaviors.verticalTabs = function() {
  if (!$('.vertical-tabs-list').size()) {
    var ul = $('<ul class="vertical-tabs-list"></ul>');
    var fieldsets = $('<div class="vertical-tabs-fieldsets"></div>');
    $.each(Drupal.settings.verticalTabs, function(k, v) {
      var description = '', cssClass = 'vertical-tabs-list-' + k;
      if (v.callback && Drupal.verticalTabs[v.callback]) {
        description = '<span class="description">'+ Drupal.verticalTabs[v.callback].apply(this, v.args) +'</span>';
      }
      else {
        cssClass += ' vertical-tabs-nodescription';
      }

      // Add a list item to the vertical tabs list.
      $('<li><a href="#' + k + '" class="' + cssClass + '">'+ v.name + description +'</a></li>').appendTo(ul)
        .find('a')
        .bind('click', function() {
          $(this).parent().addClass('selected').siblings().removeClass('selected');
          $('.vertical-tabs-' + k).show().siblings('.vertical-tabs-div').hide();
          return false;
        });

      // Find the contents of the fieldset (depending on #collapsible property).
      var fieldsetContents = $('.vertical-tabs-' + k + ' > .fieldset-wrapper');
      if (fieldsetContents.size() == 0) {
        fieldsetContents = $('<div class="fieldset-wrapper"></div>');
        $('.vertical-tabs-' + k).children('div').appendTo(fieldsetContents);
      }

      // Add the fieldset contents to the toggled fieldsets.
      fieldsetContents.appendTo(fieldsets)
      .addClass('vertical-tabs-' + k)
      .addClass('vertical-tabs-div')
      .find('input, select, textarea').bind('change', function() {
        if (v.callback && Drupal.verticalTabs[v.callback]) {
          $('a.vertical-tabs-list-' + k + ' span.description').html(Drupal.verticalTabs[v.callback].apply(this, v.args));
          Drupal.behaviors.verticalTabsResize();
        }
      });
      $('.vertical-tabs-' + k).remove();
    });

    $('div.vertical-tabs').html(ul).append(fieldsets);

    Drupal.behaviors.verticalTabsResize();
    
    // Activate the first tab.
    $('.vertical-tabs-div').hide();
    $('.vertical-tabs-div:first').show();
    $('.vertical-tabs ul li:first').addClass('selected');
  }
}

Drupal.behaviors.verticalTabsReload = function() {
  $.each(Drupal.settings.verticalTabs, function(k, v) {
    if (v.callback && Drupal.verticalTabs[v.callback]) {
      $('a.vertical-tabs-list-' + k + ' span.description').html(Drupal.verticalTabs[v.callback].apply(this, v.args));
    }
  });
}

Drupal.behaviors.verticalTabsResize = function() {
  // Adjust the min-height property of each field area to match the height of
  // the vertical tabs list, then hide them.
  if ($.browser.msie && $.browser.version.substr(0,1) == "6") {
    // IE6 min-height doesn't work, so use 'height' instead
    $('.vertical-tabs-div').add('.vertical-tabs').css('height', $('.vertical-tabs-list').height() - 25);
  } else {
    $('.vertical-tabs-div').css('minHeight', $('.vertical-tabs-list').height() - 25);
  }
}
