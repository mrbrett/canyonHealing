<?php

/**
 * @file
 * The page callback for the homepage
 */

function canyon_about_page() {

	$module_path = drupal_get_path('module', 'canyon_about_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.easing.1.3.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));
	
	// INIT vars
	$html   = '';

	return $html;

}
