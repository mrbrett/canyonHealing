<?php

/**
 * function that unsets all Drupal added css
 */
function clim_css_alter(&$css) {
  
  // Create exclude array to pass
  $exclude = array(
    'misc/vertical-tabs.css' => FALSE,
    'modules/block/block.css' => FALSE,
    'modules/comment/comment.css' => FALSE,
    'modules/dblog/dblog.css' => FALSE,
    'modules/field/theme/field.css' => FALSE,
    'modules/file/file.css' => FALSE,
    'modules/filter/filter.css' => FALSE,
    'modules/forum/forum.css' => FALSE,
    'modules/help/help.css' => FALSE,
    'modules/menu/menu.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'modules/openid/openid.css' => FALSE,
    'modules/poll/poll.css' => FALSE,
    'modules/profile/profile.css' => FALSE,
    'modules/search/search.css' => FALSE,
    'modules/statistics/statistics.css' => FALSE,
    'modules/syslog/syslog.css' => FALSE,
    'modules/system/admin.css' => FALSE,
    'modules/system/maintenance.css' => FALSE,
    'modules/system/system.css' => FALSE,
    'modules/system/system.admin.css' => FALSE,
    'modules/system/system.base.css' => FALSE,
    'modules/system/system.maintenance.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.theme.css' => FALSE,
    'modules/taxonomy/taxonomy.css' => FALSE,
    'modules/tracker/tracker.css' => FALSE,
    'modules/update/update.css' => FALSE,
    'modules/user/user.css' => FALSE,
  );

  $css = array_diff_key($css, $exclude);

}

/**
 * function that unsets all Drupal added js and updates jQuery
 */
function clim_js_alter(&$javascript) {

  $jquery_new = '//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js';
  $jquery_version = '1.8.3';
  $jquery_newest = '//code.jquery.com/jquery-latest.min.js';

  // Swap out jQuery to use an updated version and move to footer.
  $javascript['misc/jquery.js']['data']   = $jquery_new;
  $javascript['misc/jquery.js']['version']   = $jquery_version;
  $javascript['misc/jquery.js']['scope']  = 'header';
  $javascript['misc/jquery.js']['weight'] = 0;

  // Unset the rest of Drupal added js
  unset($javascript['settings']);
  unset($javascript['misc/jquery.once.js']);
  unset($javascript['misc/drupal.js']);
  unset($javascript['sites/all/modules/devel/devel_krumo_path.js']);
  unset($javascript['misc/jquery.cookie.js']);
  unset($javascript['modules/toolbar/toolbar.js']);

  // Check out $javascript 
  //dsm($javascript);

}