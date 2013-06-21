<?php

/**
 * @file
 *   Everything related to entity weights except for hook_menu().
 */

/**
 * Weights reorder form.
 */
function cirro_core_weight_form($form, &$form_state, $type) {
	
	// Init
	$info = entity_get_info($type);
	$label = $info['label'];
	$id_field = $info['entity keys']['id'];
	$table = $info['base table'];
	$form = array();
	$form['items'] = array();
	$form['items']['#tree'] = TRUE;
	
	// Build weight options
	$weight_options = array();

	for ( $i = -40; $i <= 40; $i++ ) {

		$weight_options[$i] = $i;

	}

	// Query items
	$query_string = "
		SELECT id, title, weight
			FROM $table
			ORDER BY weight, created
	";
	
	$query = db_query($query_string);
	
	$items = $query->fetchAll();
	
	// Add type to form
	$form['type'] = array(
		'#type' => 'hidden',
		'#value' => $type,
	);
	
	// Add id_field t- form
	$form['id_field'] = array(
		'#type' => 'hidden',
		'#value' => $id_field,
	);
	
	// Loop through items
	foreach ( $items as $item ) {
		
		// Build fields
		$fields = array();
		
		// ID
		$fields[$id_field] = array(
			'#type' => 'hidden',
			'#value' => $item->$id_field,
		);

		// NAME
		$fields['name'] = array(
			'#markup' => "<h3>{$item->title}</h3>",
		);

		// WEIGHT
		$fields['weight'] = array(
			'#type' => 'select',
			'#title' => 'Weight',
			'#options' => $weight_options,
			'#default_value' => $item->weight,
		);

		// Add fields to items
		$form['items'][$item->$id_field] = $fields;

	}

	// SUBMIT
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);
	
	return $form;
	
}

/**
 * Item weights form submit.
 */
function cirro_core_weight_form_submit($form, &$form_state) {
	
	// Init
	$values = $form_state['values'];
	$type = $values['type'];

	$objects = array();

	// Loop through imgs
	foreach ( $values['items'] as $id => $data ) {

		// Cast fields object
		$fields_obj = (object) $data;

		// Update img
		entity_get_controller($type)->update($fields_obj);

	}

	// Display success message
	drupal_set_message('Item order updated');
	
}

/**
 * Theme item weights forms.
 *
 * @param $variables
 *   Themeing variables array
 */
function theme_cirro_core_weight_form($variables) {
	
	// Init
	$output = '';
	$rows = array();
	
	// Load form array
	$form = $variables['form'];
	
	// Activate tabledrag shiz
	drupal_add_tabledrag('draggable-table', 'order', 'sibling', 'weight-group');
	
	// Loop through form Elements
	foreach ( element_children($form['items']) as $key ) {
		
		// Init
		$element = array();
		$data = '';
		$row = array();
		
		// Skip none numeric keys
		if ( !is_numeric($key) ) continue;
		
		// Load element
		$element = &$form['items'][$key];
		
		// Add class to weight field
		$element['weight']['#attributes']['class'] = array('weight-group');
		
		// Put data fields together
		$data .= drupal_render($element['id']);
		
		// Build row
		$row = array();
		$row[] = drupal_render($element['name']) . $data;
		$row[] = drupal_render($element['weight']);
		
		// Add to rows
		$rows[] = array( 'data' => $row, 'class' => array('draggable'));

	}
	
	// Define headers
	$headers = array(
		'Name',
		'Weight',
	);

	// Define table vars
	$table_vars = array(
		'header' => $headers,
		'rows' => $rows,
		'attributes' => array(
			'id' => 'draggable-table',
			'class' => array(
				'table',
				'table-bordered',
				'table-striped',
			),
		),
		'caption' => 'Drag and Drop to change order',
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => 'No Items Found',
	);
	
	// Add table to output
	$table = theme_table($table_vars);
	
	// Render remaining form elements
	$form_children = drupal_render_children($form);
	
	// Build output with wrapper
	$output = "
		<div class=\"span8\">
			$table
			$form_children
		</div>
	";
	
	return $output;

}