Drupal.behaviors.views_infinite_scroll = function() {
  // Make sure that autopager pluginis loaded
  if($.autopager) {
    // Make sure that views ajax is disabled
    if(!Drupal.Views) {
      $.autopager({
        content: Drupal.settings.views_infinite_scroll.content_selector,
        link: Drupal.settings.views_infinite_scroll.next_selector,
        page: 0,
        start: function() {
          $(Drupal.settings.views_infinite_scroll.pager_selector).hide();
        }    
      });
    }
    else {
      alert(Drupal.t('Views infinite scroll pager is not compatible with Ajax Views. please disable "Use Ajax" option'));
    }
  }
  else {
    alert(Drupal.t('Autopager jquery plugin in not loaded'));
  }
};  
