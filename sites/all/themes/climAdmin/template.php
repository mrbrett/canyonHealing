<?php

/**
 * Override or insert variables into the maintenance page template.
 */
//function climAdmin_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // climAdmin_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  //climAdmin_preprocess_html($vars);
//}

/**
 * Override or insert variables into the page template.
 */
function climAdmin_preprocess_page(&$vars) {
  $vars['primary_local_tasks'] = $vars['tabs'];
  unset($vars['primary_local_tasks']['#secondary']);
  $vars['secondary_local_tasks'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
}

/**
 * Display the list of available node types for node creation.
 */
function climAdmin_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="clearfix">';
      $output .= '<span class="label">' . l($item['title'], $item['href'], $item['localized_options']) . '</span>';
      $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function climAdmin_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';
  if (!empty($content)) {
    $output = system_admin_compact_mode() ? '<ul class="admin-list compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (isset($item['description']) && !system_admin_compact_mode()) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Override of theme_tablesort_indicator().
 *
 * Use our own image versions, so they show up as black and not gray on gray.
 */
function climAdmin_tablesort_indicator($variables) {
  $style = $variables['style'];
  $theme_path = drupal_get_path('theme', 'climAdmin');
  if ($style == 'asc') {
    return "<i class=\" icon-chevron-up icon-white admin-table-sort\"></i>";
  }
  else {
    return "<i class=\" icon-chevron-down icon-white admin-table-sort\"></i>";
  }
}


/**
 * Rewrite the system breadcrumb to add claro admin link
 *
 */
function climAdmin_menu_breadcrumb_alter(&$active_trail, $item) {

  if(isset($active_trail[2]) && $active_trail[2]['link_path'] == 'admin/claro') {

	  	// Remove Home and Administratoin links from beginning
      $active_trail = array_slice($active_trail, 2);
  }
}
