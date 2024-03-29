<?php

/**
 * @file
 * Contains \views_plugin_pager_infinite_scroll.
 */

/**
 * The plugin to handle the infinite scroll pager.
 *
 * @ingroup views_pager_plugins
 */
class views_plugin_pager_infinite_scroll extends views_plugin_pager_full {

  var $has_more_results = FALSE;

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options['manual_load'] = array('default' => 0);
    $options['text'] = array('default' => 'Show More', 'translatable' => TRUE);
    $options['items_per_page'] = array('default' => 10);
    $options['offset'] = array('default' => 0);
    $options['id'] = array('default' => 0);
    $options['total_pages'] = array('default' => '');
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $form['warning']['#markup'] = t('Note: The infinite scroll option requires the <em>Use AJAX</em> setting to be enabled for this views display.');
    $form['items_per_page'] = array(
      '#title' => t('Items to display'),
      '#type' => 'textfield',
      '#description' => t('The number of items to display. Enter 0 for no limit.'),
      '#default_value' => $this->options['items_per_page'],
    );
    $form['offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset'),
      '#description' => t('The number of items to skip. For example, if this field is 3, the first 3 items will be skipped and not displayed.'),
      '#default_value' => $this->options['offset'],
    );
    $form['id'] = array(
      '#type' => 'textfield',
      '#title' => t('Pager ID'),
      '#description' => t("Unless you're experiencing problems with pagers related to this view, you should leave this at 0. If using multiple pagers on one page you may need to set this number to a higher value so as not to conflict within the ?page= array. Large values will add a lot of commas to your URLs, so avoid if possible."),
      '#default_value' => $this->options['id'],
    );
    $form['total_pages'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of pages'),
      '#description' => t('The total number of pages. Leave empty to show all pages.'),
      '#default_value' => $this->options['total_pages'],
    );
    $form['manual_load'] = array(
      '#type' => 'checkbox',
      '#title' => t('Load subsequent pages manually instead of automatically'),
      '#description' => t('When checked, use a link to trigger loading of subsequent pages instead of window scroll.'),
      '#default_value' => $this->options['manual_load'],
    );
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Manual load label'),
      '#required' => FALSE,
      '#description' => t('Label to use on the manual link.'),
      '#default_value' => $this->options['text'],
      '#states' => array(
        //Hide the manual load label if manual_load is unchecked.
        'invisible' => array(
         ':input[name="pager_options[manual_load]"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  function options_validate(&$form, &$form_state) {
  }

  /**
   * {@inheritdoc}
   */
  function summary_title() {
    return t('Infinite Scroll: @parent', array('@parent' => parent::summary_title()));
  }

  /**
   * {@inheritdoc}
   */
  function use_count_query() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    parent::query();
    if ($this->get_items_per_page() > 0 && (empty($this->options['total_pages']) || ($this->get_current_page() < $this->options['total_pages']))) {
      $limit = $this->options['items_per_page'];;
      $limit += 1;
      $this->view->query->set_limit($limit);
    }
  }

  /**
   * {@inheritdoc}
   */
  function post_execute(&$result) {
    global $pager_total;

    if ($this->get_items_per_page() > 0 && count($result) > $this->get_items_per_page()) {
      array_pop($result);
      $total = PHP_INT_MAX / 2;
      $this->has_more_results = TRUE;
    }
    else {
      $total = $this->get_current_page() * $this->get_items_per_page() + count($result);
    }
    $pager_id = $this->get_pager_id();
    $pager_total[$pager_id] = $total;
  }

  /**
   * {@inheritdoc}
   */
  function render($input) {
    if (!$this->has_more_results) {
      return;
    }
    $pager_theme = views_theme_functions('views_infinite_scroll_pager', $this->view, $this->display);
    $settings = array(
      'views_infinite_scroll' => array(
        'img_path' => file_create_url(backdrop_get_path('module', 'views_infinite_scroll') . '/images/ajax-loader.gif'),
        // The threshold for how far to the bottom you should reach before reloading.
        'scroll_threshold' => 200,
      ),
    );
    backdrop_add_js($settings, array('type' => 'setting', 'scope' => JS_DEFAULT));
    backdrop_add_js(backdrop_get_path('module', 'views_infinite_scroll') . '/js/views-infinite-scroll.js', array('scope' => 'footer'));
    return theme($pager_theme, array(
      'text' => $this->options['text'],
      'manual_load' => $this->options['manual_load'],
      'element' => $this->get_pager_id(),
    ));
  }
}
