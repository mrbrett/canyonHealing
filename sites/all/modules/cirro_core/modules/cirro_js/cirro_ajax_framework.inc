<?php

/**
 * @file
 * The page callback for the cirro AJAX framework.
 */


/**
 * Accepts a basic POST from the client to call PHP functions on the server.
 *
 * @return
 *   JSON with a minimum of status => true/false.
 */
function cirro_ajax_framework() {

	ini_set('display_errors', 0);
	
	try {
	
		// Init
		$post = $_POST;
		
		// Check for callback
		if ( !isset($post['callback']) ) {
			
			throw new Exception('Callback argument missing');
	
		}

		// Check for valid callback
		$exploded_callback = explode('_', $post['callback']);

		if ( end($exploded_callback) !== 'cjax' ) {

			throw new Exception('Callback argument invalid');

		}

		// Load callback
		$callback = $post['callback'];
		
		// Run Callback
		$return = $callback($post);
			
		// Return $return as JSON
		drupal_json_output($return);
	
	} catch(Exception $e) {

		// Log exception
		watchdog_exception('cirro_ajax', $e);

		// Cleanup file name
		$doc_len = strlen($_SERVER['DOCUMENT_ROOT']);
		$file = substr($e->getFile(), $doc_len);

		// Build messages
		$messages = array(
			$e->getMessage(),
			"File: {$file}",
			"Line: {$e->getLine()}",
		);
		
		// Collect data
		$data = array(
			'status' => 'exception',
			'messages' => $messages,
			'trace' => $e->getTrace(),
		);
		
		// Return JSON
		drupal_json_output($data);

	}

}