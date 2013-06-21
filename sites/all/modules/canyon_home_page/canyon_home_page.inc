<?php

/**
 * @file
 * The page callback for the homepage
 */

function canyon_home_page() {

	// Add JS
	$module_path = drupal_get_path('module', 'canyon_home_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.isotope-min.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'jquery.fitvids.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));

	// Init
	$html   = '';
	


	return $html;

}
