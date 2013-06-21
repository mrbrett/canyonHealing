<?php

/**
 * @file
 * The page callback for the homepage
 */

function canyon_contact_page() {

	$module_path = drupal_get_path('module', 'canyon_contact_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.easing.1.3.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));
	$js_path_maps = "https://maps.google.com/maps/api/js?sensor=false";
	drupal_add_js($js_path_maps, array('scope' => 'footer', 'weight' => 10));



	return $html;

}



