// $Id: features.js,v 1.1.2.5 2009/09/13 23:23:23 yhahn Exp $

Drupal.behaviors.features = function() {
  // Features management form
  $('table.features:not(.processed)').each(function() {
    $(this).addClass('processed');

    // Check the overridden status of each feature
    Drupal.features.checkStatus();

    // Add some nicer row hilighting when checkboxes change values
    $('input', this).bind('change', function() {
      if (!$(this).attr('checked')) {
        $(this).parents('tr').removeClass('enabled').addClass('disabled');
      }
      else {
        $(this).parents('tr').addClass('enabled').removeClass('disabled');
      }
    });
  });

  // Export form component selector
  $('form.features-export-form select.features-select-components:not(.processed)').each(function() {
    $(this)
      .addClass('processed')
      .change(function() {
        var target = $(this).val();
        $('div.features-select').hide();
        $('div.features-select-' + target).show();
        return false;
    });
  });

  // Export form machine-readable JS
  $('.feature-name:not(.processed)').each(function() {
    $('.feature-name')
      .addClass('processed')
      .after(' <small class="feature-project-suffix">&nbsp;</small>');
    if ($('.feature-project').val() === $('.feature-name').val().toLowerCase().replace(/[^a-z0-9]+/g, '_').replace(/_+/g, '_') || $('.feature-project').val() === '') {
      $('.feature-project').parents('.form-item').hide();
      $('.feature-name').keyup(function() {
        var machine = $(this).val().toLowerCase().replace(/[^a-z0-9]+/g, '_').replace(/_+/g, '_');
        if (machine !== '_' && machine !== '') {
          $('.feature-project').val(machine);
          $('.feature-project-suffix').empty().append(' Machine name: ' + machine + ' [').append($('<a href="#">'+ Drupal.t('Edit') +'</a>').click(function() {
            $('.feature-project').parents('.form-item').show();
            $('.feature-project-suffix').hide();
            $('.feature-name').unbind('keyup');
            return false;
          })).append(']');
        }
        else {
          $('.feature-project').val(machine);
          $('.feature-project-suffix').text('');
        }
      });
      $('.feature-name').keyup();
    }
  });
};

Drupal.features = {
  'checkStatus': function() {
    $('table.features tbody tr').not('.processed').filter(':first').each(function() {
      var elem = $(this);
      $(elem).addClass('processed');
      var uri = $(this).find('a.admin-check').attr('href');
      if (uri) {
        $.get(uri, [], function(data) {
          $(elem).find('.admin-loading').hide();
          if (data.status == 1) {
            $(elem).find('.admin-overridden').show();
          }
          else {
            $(elem).find('.admin-default').show();
          }
          Drupal.features.checkStatus();
        }, 'json');
      }
      else {
          Drupal.features.checkStatus();
        }
    });
  }
};
