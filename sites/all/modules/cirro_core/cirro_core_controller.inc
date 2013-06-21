<?php
/**
 * @file
 *   cirroCoreController
 *
 */

/**
 * Controller class for Cirro core entities.
 *
 * This extends the DrupalDefaultEntityController class.
 */
class cirroCoreController extends DrupalDefaultEntityController {

	/**
	 * Gives access to the object.
	 *
	 * @return
	 *   The object.
	 */
	public function showThis() {
		return $this;
	}

	/**
	 * Creates a new Cirro item entity.
	 *
	 * @param stdClass $new_obj
	 *   The stdClass object containing the necessary properties to create the entity.
	 */
	public function create($new_obj) {

		// Init
		global $user;
		$error = '';
	  $type = $this->entityType;
		$table = $this->entityInfo['base table'];
		$id_name = $this->entityInfo['entity keys']['id'];
		$schema = drupal_get_schema($table);

		// If the argument is not an object
		if (!is_object($new_obj)) {

			// Create watchdog error and throw exception
			$error = 'Entity Create: Argument must be object.';
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);
			throw new Exception($error);

		}

		// Add the user id and created time properties to the new object
		$new_obj->uid = $user->uid;
		$new_obj->created = time();

		// Insert the new object into the database
		if (drupal_write_record($table, $new_obj) === FALSE) {

			// Create watchdog error and throw exception
			$error = 'Entity Create: write_record FAIL';
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);			
			throw new Exception($error);

		}

		// Loop through fields looking for file fields
		foreach ( $schema['fields'] as $field => $info ) {
			
			// Check for form array and managed_file #type
			if (
				isset($info['form']) &&
				$info['form']['#type'] == 'managed_file' &&
				isset($new_obj->$field) &&
				$new_obj->$field !== 0
			) {
				
				// Load file object
				$file = file_load($new_obj->$field);
				
				// Create usage entry
				file_usage_add($file, 'cirro_core', $type, $new_obj->$id_name);
				
				// Make file permanent
				$file->status = FILE_STATUS_PERMANENT;
				
				// Save file
				file_save($file);
				
			}

		}

		// Return the boolean response from the new object database insertion
		return $new_obj;

	} // create()

	/**
	 * Updates an existing entity in the database.
	 *
	 * @param stdClass $update_obj
	 *   The stdClass object containing all of the data to be updated.
	 * @param bool $log_disable
	 *   (optional) TRUE to log individual changes. Defaults to FALSE.
	 *   Skip logging for individual property changes.
	 *
	 * @return
	 *   The created object with the primary key prooperty included, or FALSE if
	 *   the process fails.
	 */
  public function update($update_obj, $log_disable = FALSE) {

	  // Init
	  $error = NULL;
	  $id = NULL;
	  $type = $this->entityType;
		$table = $this->entityInfo['base table'];
	  $id_name = $this->entityInfo['entity keys']['id'];
	  $schema = drupal_get_schema($table);

	  // Confirm update_obj
	  if ( !is_object($update_obj) ) {

			// Create watchdog error and throw exception
			$error = 'Entity Update: Argument must be object.';
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);			
			throw new Exception($error);

	  }

	  // If the update object's id name property is empty
	  if ( empty($update_obj->$id_name) ) {

			// Create watchdog error and throw exception
			$error = 'Entity Update: Update object must include ID.';
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);			
			throw new Exception($error);

	  }
	  
	  // Load existing object
	  $item = cirro_core_single_load($update_obj->$id_name, $type);

		// Add the changed time property to the update object
	  $update_obj->changed = time();

	  // Write to db
	  if ( drupal_write_record($table, $update_obj, $id_name) === FALSE ) {

			// Create watchdog error and throw exception
			$error = "Entity Update: Could not write to db. $type : $id";
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);			
			throw new Exception($error);

	  }
	  
		// Loop through fields looking for file fields
		foreach ( $schema['fields'] as $field => $info ) {
			
			// Check for form array and managed_file #type
			if (
				isset($info['form']) &&
				$info['form']['#type'] == 'managed_file'
			) {
				
				// Check for mismatch values
				if ( isset($update_obj->$field) && $item->$field !== $update_obj->$field ) {

					// Check for old non 0 value
					if ( $item->$field != 0 ) {

						// Load old file
						$old_file = file_load($item->$field);
						
						// Delete old file
						file_delete($old_file, $force = TRUE);
						
					}
					
					// Check for new, non 0 value
					if ( isset($update_obj->$field) && $update_obj->$field !== 0 ) {
						
						// Load file object
						$file = file_load($update_obj->$field);
						
						// Create usage entry
						file_usage_add($file, 'cirro_core', $type, $update_obj->$id_name);
						
						// Make file permanent
						$file->status = FILE_STATUS_PERMANENT;
						
						// Save file
						file_save($file);
						
						
					}
					
				}
				
			}
			
		}

	  // Return updated object
	  return $update_obj;

  } // update()

  /**
   * Deletes a single entity object and its children from the database.
   *
   * Simply passes to deleteMultiple().
   *
   * @param int $id
   *   ID of object to be deleted.
   *
   * @return
   *   The updated object with the primary key prooperty included, or FALSE if
	 *   the process fails.
   */
  public function delete($id) {
	  
	  return $this->deleteMultiple(array($id));
  }

  /**
   * Deletes mulitple entity objects and their children from the database.
   *
   * Run and Injection types require additional delete method for child deletion.
   *
   * @param array $ids
   *   Array of object ids to be deleted.
   *
   * @return
   *   TRUE/FALSE depending on success.
   */
  public function deleteMultiple($ids) {
	  
	  // Init
	  $type = $this->entityType;
	  $info = $this->entityInfo;
		$table = $info['base table'];
	  $id_name = $info['entity keys']['id'];
	  $schema = drupal_get_schema($table);
	  $file_fields = array();
	  $children = array();

	  // Confirm arg is array
	  if ( !is_array($ids) ) {
		  
			// Create watchdog error and throw exception
			$error = "Entity Delete Mulitple: Arg must be array.";
			watchdog('cirroCoreController', $error, array(), WATCHDOG_ERROR);			
			throw new Exception($error);
		  
	  }
	  
	  // Check for children
	  if ( isset($info['children']) ) {

		  // Loop through children
		  foreach ( $info['children'] as $child_type => $child_label ) {

			  // Find and delete children recursively
			  $this->find_children($child_type, $type, $ids);
			  
		  }
		  
	  }

	  // Check for multi_img
	  if ( isset($schema['multi_img']) ) {
		  
		  // Query multi_img ids
		  $query_string = "
		  	SELECT id
		  		FROM cirro_multi_img
		  		WHERE parent_type = :type
		  		AND parent_id IN ( :ids )
		  ";
		  
		  $query = db_query($query_string, array(':type' => $type, ':ids' => $ids));
		  
		  $img_ids = $query->fetchCol();
		  
		  // Delete mutli_img items
		  entity_get_controller('multi_img')->deleteMultiple($img_ids);
		  
	  }

	  // Create file managed fields list
	  foreach ( $schema['fields'] as $field => $info ) {
		  
		  // Check for form and file_managed #type
		  if ( isset($info['form']) && $info['form']['#type'] === 'managed_file' ) {
			  
			  // Add field to file_fields array
			  $file_fields[] = $field;
			  
		  }
		  
	  }
	  
	  // Loop through ids
	  foreach ($ids as $id) {

		  // Load item
		  $item = cirro_core_single_load($id, $type);
		  
		  // Loop through file fields
		  foreach ( $file_fields as $field ) {
			  
			  // Check for non 0 value
			  if ( $item->$field != 0 ) {
				  
				  // Load file
				  $file = file_load($item->$field);
				  
				  // Delete File
				  file_delete($file, $force = TRUE);

			  }
			  
		  }

		  // Init transaction
		  $transaction = db_transaction();

		  // Try to delete
		  try {
			  
			  // Delete entity from table
				db_delete($table)
					->condition($id_name, $id)
					->execute();

			  // Commit transaction by destroying object
			  unset($transaction);
			  
      } catch (Exception $e) {
		  
			  // Rollback transaction
			  $transaction->rollback();
			  
			  // Log error
			  watchdog_exception('cirroCoreController', $e);
			  
			  // Pass up exception
			  throw $e;
		  }
		  
	  } // endforeach ids

		// Clear caches
		entity_get_controller($type)->resetCache();
		
		return TRUE;
 
  } // deleteMultiple()
  
  /**
   * Find children when given parent_type and parent_id.
   *
   * @param $child_type
   *   Type of items to look for and check for children
   * @param $parent_type
   *   Mach name of parent type
   * @param $parent_ids
   *   Array of parent IDs to look for
   *
   * @return
   *   Index array of IDs that match.
   */
 private function find_children($child_type, $parent_type, $parent_ids) {

	 // Init
	 $child_info = entity_get_info($child_type);
	 $table = $child_info['base table'];
	 
	 // Query child IDS
	 $query_string = "
	 	SELECT id
	 		FROM $table
	 		WHERE parent_type = :parent_type
	 		AND parent_id IN ( :parent_ids )
	 ";

	 $query_options = array(
	 	':parent_type' => $parent_type,
	 	':parent_ids' => $parent_ids,
	 );
	 
	 $query = db_query($query_string, $query_options);
	 
	 $child_ids = $query->fetchCol();
	 
	 // Check for results
	 if ( empty($child_ids) ) return TRUE;
	 
	 if ( isset($child_info['children']) ) {
		 
		 // Loop through children types and look for onese that match
		 foreach ( $child_info['children'] as $sub_child_type => $sub_child_label ) {
			 
			 // Recurse
			 $this->find_children($sub_child_type, $child_type, $child_ids);
			 
		 }
		 
	 }
	 
	 // Delete child ids
	 db_delete($table)
	 	->condition('id', $child_ids)
	 	->execute();
	 
	 return TRUE;
	 
 }

} // CirroCoreController


















