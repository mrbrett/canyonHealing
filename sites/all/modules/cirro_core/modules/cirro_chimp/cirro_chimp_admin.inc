<?php

/**
 * @file
 *   Any admin functions associated with mail chimp shiz.
 */

/**
* Simple config page for storing API key.
*/
function cirro_chimp_config_form($form, &$form_state) {

	// Init
	$form = array();

	// API Key
	$form['cirro_chimp_api_key'] = array(
		'#type' => 'textfield',
		'#title' => 'API Key',
		'#default_value' => variable_get('cirro_chimp_api_key', ''),
	);

	return system_settings_form($form);

}