<?php
/**
 * @file
 *   All admin forms.
 */

/**
 * Build breadcrumb array reverse recursively in case of children.
 */
function cirro_core_build_breadcrumb($item) {
	
	// Init
	$breadcrumb = array();
	$parents = array();

	// Build default dashboard link
	$breadcrumb[] = l('Dashboard', 'admin/dash');

	// Check if item is an array
	if ( is_object($item) ) {

		// Check for title field
		if ( isset($item->title) ) {
			
			$current_title = $item->title;
			
		} else {
			
			// Use type label instead
			$current_title = $item->type_label;	
			
		}
			
		// Recurse through parent layers
		while ( isset($item->parent_type) ) {
			
			// Load parent item
			$parent_item = cirro_core_single_load($item->parent_id, $item->parent_type);
			
			// Check for title field
			if ( isset($parent_item->title) ) {
				
				$title = $parent_item->title;
				
			} else {
				
				// Use parent item type label instead
				$title = $parent_item->type_label;
				
			}
			
			// Create edit link for breadcrumb
			$parent_url = "admin/dash/{$parent_item->type}/{$parent_item->id}/edit";
			$parent_link =  l($title, $parent_url);
			
			// Prepend on pareants array
			array_unshift($parents, $parent_link);
			
			// Replace item with parent item for recursion
			$item = $parent_item;
			
		}
	
		// Merge parents into breadcrumb
		$breadcrumb = array_merge($breadcrumb, $parents);
		
		// Add plain text for current page
		$breadcrumb[] = $current_title;
	
	} else {
		
		// Type label pass in place of item
		
		$type_label = $item;
		
		// Add type label to breadcrumb
		$breadcrumb[] = "Add $type_label";
		
	}
	
	return $breadcrumb;
	
}

/**
 * Add/Edit form page callback.
 */
function cirro_core_entity_edit_page($type, $item, $action = NULL) {

	// Init
	$html = '';
	$info = entity_get_info($type);
	$label = $info['label'];
	$schema = drupal_get_schema($info['base table']);
	$id_field = $info['entity keys']['id'];
	$children = '';
	$imgs = '';
	$files = '';
	$breadcrumb_item = $item;
	$id = NULL;

	// Check for add
	if ( $item !== 'add' ) {
		
		// Load ID
		$id = $item->$id_field;	
		
	} else {
		
		// Pass type to breadcrumb
		$breadcrumb_item = $label;
		
	}
	
	// Replace breadcrumb
	$breadcrumb = cirro_core_build_breadcrumb($breadcrumb_item);
	
	drupal_set_breadcrumb($breadcrumb);

	// Load primary form
	$edit_options = array(
		'info' => $info,
		'schema' => $schema,
		'type' => $type,
		'item' => $item,
		'action' => $action,
	);
	$form_array = drupal_get_form('cirro_core_entity_edit_form', $edit_options);
	$form = drupal_render($form_array);

	// Add form to html
	$form_wrapped = "
		<div class=\"edit-form-wrapper box-border\">
			$form
		</div>
	";

	// Check for Children
	if ( isset($info['children']) ) {
		
		// Build children forms
		$children = cirro_core_children_build_forms(
			$item,
			$info['children'],
			$type,
			$id
		);

	}

	// Check for multi_img in info
	if ( isset($info['multi_img']) ) {

		// Build options
		$imgs = cirro_core_multi_img_build_forms(
			$item,
			$info['multi_img'],
			$type,
			$id
		);

	}
	
	// Check for multi_file in info
	if ( isset($info['multi_file']) ) {
		
		// Build files
		$files = cirro_core_multi_file_build_forms(
			$item,
			$info['multi_file'],
			$type,
			$id
		);
		
	}

	// Build html
	$html = "
		<div class=\"row-fluid\">
			<div class=\"span6\">
				$form_wrapped
			</div>
			<div class=\"span6\">
				$children
				$imgs
				$files
			</div>
		</div>
	";

	return $html;

}

/**
 * CRUD create/update form.
 *
 * @param $type
 *   Entity Type
 *
 * @param $id
 *   ID of item being updated OR the string 'create'
 *
 * @return
 *   Form array
 */
function cirro_core_entity_edit_form($form, &$form_state, $options) {

	// Init
	$form = array();
	$entity_info = $options['info'];
	$schema = $options ['schema'];
	$type = $options['type'];
	$item = $options['item'];
	$action = $options['action'];
	$entity_label = $entity_info['label'];
	$base_table = $entity_info['base table'];
	$id_field = $entity_info['entity keys']['id'];
	$fields = $schema['fields'];

	// Check for id
	if ( $item === 'add' ) {

		// Create empty object
		$item = new stdClass;

		// Unset id field
		unset($fields[$id_field]);

		// Set action
		$action = 'add';

	}

	// Add type as hidden element
	$form['type'] = array(
		'#type' => 'hidden',
		'#value' => $type,
	);

	// Add type label as hidden element
	$form['type_label'] = array(
		'#type' => 'hidden',
		'#value' => $entity_label,
	);

	// Add action as hidden element
	$form['action'] = array(
		'#type' => 'hidden',
		'#value' => $action,
	);

	// Add id field as hidden element
	$form['id_field'] = array(
		'#type' => 'hidden',
		'#value' => $id_field,
	);


	// Uppercase action for display
	$action_display = ucfirst($action);

	// Loop through schema fields
	foreach ( $fields as $field => $info ) {

		// Look for form array within field info
		if ( isset($info['form']) ) {

			// Add to form
			$form[$field] = $info['form'];

			// Check for default value
			if ( isset($item->$field) ) {

				// Add default value to field
				$form[$field]['#default_value'] = $item->$field;

			}

		}

	}

	// Submit
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => "Save Item",
	);

	// If the item has a value for the id_field
	if ( isset($item->id) ) {

		$delete_button = l('delete', "admin/dash/$type/$item->id/delete");

		$form['delete'] = array(
			'#markup' => $delete_button,
		);

		// Add id value as hidden element
		$form[$id_field] = array(
			'#type' => 'hidden',
			'#value' => $item->id,
		);

	}

	return $form;

}

/**
 * Submit entity create/update
 *
 */
function cirro_core_entity_edit_form_submit($form, &$form_state) {

	// Init
	$values = $form_state['values'];
	$type = $values['type'];
	$action = $values['action'];
	$id_field = $values['id_field'];
	$file_fields = array();

	// Translate action to method
	if ( $action === 'add' ) $action = 'create';
	if ( $action === 'edit' ) $action = 'update';

	// Cast fields object
	$fields_obj = (object) $values;

	if ( ( $new_obj = entity_get_controller($type)->$action($fields_obj) ) !== FALSE ) {

		// Load id
		$id = $new_obj->$id_field;

		// Add id to form_state
		$form_state['values'][$id_field] = $id;

		// Set success message
		drupal_set_message("Item $action complete.");

		// Redirect to display
		$form_state['redirect'] = "admin/dash/$type/$id/edit";

	} else {

		drupal_set_message("Controller fail.", "error");

	} 

}

/**
 * Delete confirmation form
 *
 * @param $type
 *   Entity machine name
 *
 * @param $item
 *   Item loaded object
 *
 */
function cirro_core_entity_delete_form($form, &$form_state, $type, $item) {

	// Init
	$form = array();
	$entity_info = entity_get_info($type);
	$entity_label = $entity_info['label'];
	$base_table = $entity_info['base table'];
	$schema = drupal_get_schema($base_table);
	$primary_key = reset($schema['primary key']);
	$id = $item->$primary_key;
	$title = $entity_label;
	$children = '';
	$yes = 'Yes, I am sure. Delete this item forever!';

	// Check for item name
	if ( isset($item->title) ) {

		// Load item name
		$title = $item->title;

	}

	// Get breadcrumb
	$breadcrumb = cirro_core_build_breadcrumb($item);
	
	// Replace item text with link
	array_pop($breadcrumb);
	$url = "admin/dash/{$item->type}/$id/edit";
	$breadcrumb[] = l($title, $url);
	
	// Add delete text
	$breadcrumb[] = 'DELETE';
	
	drupal_set_breadcrumb($breadcrumb);
	
	// Check for children
	if ( isset($entity_info['children']) ) {
		
		$children = "
			<div class=\"alert alert-error \">
				<strong>This will also delete ALL child items!</strong>
			</div>
		";
		
	}

	// Add type to form
	$form['type'] = array(
		'#type' => 'hidden',
		'#value' => $type,
	);

	// Add id to form
	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $id,
	);

	// Build confirm args
	$question = "Confirm Delete: $title";
	$path = "admin/dash/$type/$id/edit";
	$description = "
		
		Are you sure you want to delete <strong>$title</strong>?<br />
		<div class=\"alert\">This cannot be undone!</div>
		$children	
	";

	// Build confirm form
	$form = confirm_form($form, $question, $path, $description, $yes);

	return $form;

}

/**
 * Delete submit
 */
function cirro_core_entity_delete_form_submit($form, &$form_state) {

	// Init
	$values = $form_state['values'];
	$type = $values['type'];
	$id = $values['id'];
	$type_info = entity_get_info($type);

	// Attempt delete
	if ( entity_get_controller($type)->delete($id) !== FALSE ) {

		// Display message
		drupal_set_message('Successfully deleted.');

		// Redirect to dashboard
		$form_state['redirect'] = 'admin/dash';

	}

}