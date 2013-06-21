<?php

/**
 * @file
 *   Everything for managing children items except for hook_menu
 */

function cirro_core_children_build_forms($item, $children_types, $type, $id) {
	
	// Init
	$children = '';
	
			// Check for Add page
		if ( $item === 'add' ) {
		
			// Loop through children
			foreach ( $children_types as $child_type => $child_label ) {
				
				// Build child form placeholder
				$child_form = "
					<div class=\"children-form-wrapper\">
						<div class=\"title-wrapper\">
							<h2>$child_label</h2>
						</div>
						<div class=\"note-wrapper\">
							<h4>Finish creating item in order to add {$child_label}s</h4>
						</div>
					</div>
				";
				
				// Add to children markup
				$children .= $child_form;

			}
		
		
		} else {
			
			// Edit page

			// Loop through children
			foreach ( $children_types as $child_type => $child_label ) {

				// Load entity info
				$child_info = entity_get_info($child_type);
				$table = $child_info['base table'];

				// Query children
				$query_string = "
					SELECT id, title, type, weight
					FROM $table
					WHERE parent_type = :parent_type
					AND parent_id = :parent_id
					ORDER BY weight, created
				";
	
				$query_options = array(
					':parent_type' => $type,
					':parent_id' => $id
				);
	
				$query = db_query($query_string, $query_options);

				$child_items = $query->fetchAll();

				// Count children
				$child_count = count($child_items);

				// Build children weights form
				$children_form_options = array(
					'parent_id' => $id,
					'parent_type' => $type,
					'child_type' => $child_type,
					'child_items' => $child_items,
				);
				$children_form_id = "cirro_core_{$child_type}_children_weights";
				$children_weights = drupal_get_form($children_form_id, $children_form_options);

				$children_weight_form = drupal_render($children_weights);

				// Check for limit
				if ( isset($info['limit']) && $info['limit'] >= $child_count ) {
					
					// Replace add button with limit warning
					$add_button = "
						<div class=\"alert alert-info\">
							You have reached the upload limit of {$info['limit']} items.<br />
							Delete some items in order to add more.
						</div>
					";
					
				} else {

					// Build create form
					$options = array(
						'parent_type' => $type,
						'parent_id' => $id,
						'child_type' => $child_type,
						'child_label' => $child_info['label'],
					);

					$create_form = drupal_get_form("cirro_core_{$child_type}_child_create", $options);
					
					$create_form_rendered = drupal_render($create_form);
					
				}
				
				// Build link to add form
				
				// Build children form element
				$children_form = "
					<div class=\"children-form-wrapper box-white\">
						<div class=\"title-wrapper\">
							<h2>{$child_info['label']}</h2>
						</div>
						<div class=\"weight-wrapper\" />
							$children_weight_form
						</div>
						<div class=\"line-seperator\"></div>
						<div class=\"add-wrapper\">
							$create_form_rendered
						</div>
					</div>
				";

				// Add to imgs markup
				$children .= $children_form;
				
			}
			
		}
	
	return $children;
	
}

/**
 * Create child item.
 *
 * @param array $options
 *   parent_id:
 *   parent_type:
 *   child_type:
 *   child_label:
 */
function cirro_core_child_create_form($form, &$form_state, $options) {
	
	// Init
	$form = array();
	$child_label = $options['child_label'];

	// Add parent and type info
	$form['parent_id'] = array(
		'#type' => 'hidden',
		'#value' => $options['parent_id'],
	);
	
	$form['parent_type'] = array(
		'#type' => 'hidden',
		'#value' => $options['parent_type'],
	);
	
	$form['type'] = array(
		'#type' => 'hidden',
		'#value' => $options['child_type'],
	);
	
	$form['type_label'] = array(
		'#type' => 'hidden',
		'#value' => $child_label,
	);
	
	// Title field
	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#required' => TRUE,
		'#default_value' => '',
	);
	
	// Submit
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => "Create $child_label",
	);	

	return $form;
	
}

/**
 * Create child form submit.
 */
function cirro_core_child_create_form_submit($form, &$form_state) {
	
	// Init
	$values = $form_state['values'];
	$child_type = $values['type'];
	$child_label = $values['type_label'];
	$parent_type = $values['parent_type'];
	$parent_id = $values['parent_id'];
	$controller = entity_get_controller($child_type);

	// Cast fields object
	$fields_obj = (object) $values;
	
	// Attempt create
	if ( ( $new_item = $controller->create($fields_obj) ) !== FALSE ) {
		
		// Set success message
		drupal_set_message("$child_label created.");
		
	} else {
		
		// Set Error
		drupal_set_message('There was an error creating the child item.', 'error');	
		
	}
	
}

/**
 * Children weights form.
 *
 * @param array $options
 *   parent_id:
 *   parent_type:
 *   child_type:
 *   child_items: array of loaded child objects
 *
 */
function cirro_core_children_weight_form($form, &$form_state, $options) {
	
	// Init
	$form = array();
	$parent_id = $options['parent_id'];
	$parent_type = $options['parent_type'];
	$child_type = $options['child_type'];
	$child_items = $options['child_items'];
	
	// Build weight options
	for ( $i = -40; $i <= 40; $i++ ) {

		$weight_options[$i] = $i;

	}

	// Add parent info to form
	$form['parent_id'] = array(
		'#type' => 'hidden',
		'#value' => $parent_id,
	);

	$form['parent_type'] = array(
		'#type' => 'hidden',
		'#value' => $parent_type,
	);
	
	$form['child_type'] = array(
		'#type' => 'hidden',
		'#value' => $child_type,
	);

	// Init items array
	$form['items'] = array();
	$form['items']['#tree'] = TRUE;

	// Loop through child items
	foreach ( $child_items as $item ) {
		
		// Init
		$id = $item->id;
		$item_type = $item->type;
		$fields = array();

		// Add ID
		$fields['id'] = array(
			'#type' => 'hidden',
			'#value' => $id,
		);

		// Build link
		$options = array(
			'attributes' => array(
				'title' => 'Edit Item',
			),
		);
		$link = l($item->title, "admin/dash/$item_type/$id/edit", $options);
		
		$fields['link'] = array(
			'#markup' => $link,
		);
		
		// Build delete link
		$fields['delete'] = array(
			'#markup' => l(
				'<i class="icon-remove"></i> ',
				"admin/dash/$child_type/{$id}/delete",
				array(
					'html' => TRUE,
					'attributes' => array(
						'class' => array('btn', 'btn-small'),
						'title' => 'Remove Item',
					),
					'query' => array(
						'destination' => "admin/dash/$parent_type/$parent_id/edit"
					),
				)
			),
		);
		
		// Weight
		$fields['weight'] = array(
			'#type' => 'select',
			'#title' => 'Weight',
			'#default_value' => $item->weight,
			'#options' => $weight_options,
		);
		
		// Add item to form items
		$form['items'][$id] = $fields;
	
	}
	if ( !empty($child_items) ) {

		// Add submit
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => "Update Order",
		);
		
	}
	
	return $form;

}

/**
 * Children weights submit
 */
function cirro_core_children_weight_form_submit($form, &$form_state) {

	// Init
	$values = $form_state['values'];
	$child_type = $values['child_type'];

	$objects = array();

	// Loop through imgs
	foreach ( $values['items'] as $id => $data ) {

		// Cast fields object
		$fields_obj = (object) $data;

		// Update img
		entity_get_controller($child_type)->update($fields_obj);

	}

	// Display success message
	drupal_set_message('Children Order Updated');

}

/**
 * Theme children weights forms.
 *
 * @param $variables
 *   Themeing variables array
 */
function theme_cirro_core_children_weight_form($variables) {

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
		$data .= drupal_render($element['link']);
		$data .= drupal_render($element['id']);
		
		// Build row
		$row = array();
		$row[] = $data;
		$row[] = drupal_render($element['weight']);
		$row[] = drupal_render($element['delete']);
		
		// Add to rows
		$rows[] = array( 'data' => $row, 'class' => array('draggable'));

	}
	
	// Define headers
	$headers = array(
		'Edit',
		'Weight',
		'',
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
				'table-striped'
			),
		),
		'caption' => 'Drag and Drop to change order',
		'colgroups' => array(),
		'sticky' => FALSE,
		'empty' => 'No Children found',
	);
	
	// Add table to output
	$output = theme_table($table_vars);
	
	// Render remaining form elements
	$output .= drupal_render_children($form);	
	
	return $output;

}