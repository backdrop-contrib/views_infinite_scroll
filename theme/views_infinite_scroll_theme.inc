<?php
// $Id$
/**
 * @file
 * Theme infinite scroll page
 */
function theme_views_infinite_scroll_pager($tags = array(), $limit = 10, $content_selector = 'div.view-content', $items_selector = 'div.view-content .views-row', $element = 0, $parameters = array(), $quantity = 9) {
  $PAGER_CLASS = 'pager';
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  $li_next = theme('pager_next', (isset($tags[3]) ? $tags[3] : t('››')), $limit, $element, 1, $parameters);
  if (empty($li_next)) {
    $li_next = "&nbsp;";
  }

  if ($pager_total[$element] > 1) {

    $items[] = array(
      'class' => 'pager-current',
      'data' => t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max)),
    );

    $items[] = array(
      'class' => 'pager-next',
      'data' => $li_next,
    );
   
    $settings = array(
      'views_infinite_scroll' => array(
        'pager_selector' => 'ul.' . $PAGER_CLASS,
        'next_selector' => 'li.pager-next a:first',    
        'content_selector' => $content_selector,        
        'items_selector' => $items_selector,        
      )
    );
    drupal_add_js($settings, 'setting' );
    drupal_add_js(drupal_get_path('module', 'views_infinite_scroll') . '/js/jquery.autopager-1.0.0.js');
    drupal_add_js(drupal_get_path('module', 'views_infinite_scroll') . '/js/views_infinite_scroll.js');
    
    return theme('item_list', $items, NULL, 'ul', array('class' => $PAGER_CLASS));
  }
}
