<?php

// $Id$
/**
 * The plugin to handle full pager.
 *
 * @ingroup views_pager_plugins
 */
class views_plugin_pager_infinite_scroll extends views_plugin_pager_full {
  function init(&$view, &$display, $options = array()) {
    parent::init($view, $display, $options);
  }

  function summary_title() {
    return "Infinite Scroll";
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], 'Infinite scroll pager, @count item, skip @skip', 'Infinite scroll pager, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
      return format_plural($this->options['items_per_page'], 'Infinite scroll pager, @count item', 'Infinite scroll pager, @count items', array('@count' => $this->options['items_per_page']));
  }

  function render($input) {
    $style_plugin = $this->view->display_handler->default_display->options['style_plugin']; 
    $content_selector = '';
    $items_selector = '';
    
    switch($style_plugin){
      case 'default':
        $content_selector = 'div.view-content';
        $items_selector = 'div.view-content .views-row';
        break;
      case 'grid':
        $content_selector = 'div.view-content tbody';
        $items_selector = 'div.view-content tbody tr';
        break;
      case 'list':
        $content_selector = 'div.view-content .item-list > *';
        $items_selector = 'div.view-content .views-row';
        break;
      case 'table':
        $content_selector = 'div.view-content tbody';
        $items_selector = 'div.view-content tbody tr';
        break;
    }
    $pager_theme = views_theme_functions('views_infinite_scroll_pager', $this->view, $this->display);
    return theme($pager_theme, $input, $this->options['items_per_page'], $content_selector, $items_selector, $this->options['id']);
  }
}
