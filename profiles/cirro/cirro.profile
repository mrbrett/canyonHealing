<?php
/**
 * @file
 * Enables modules and site configuration for a standard site installation.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function cirro_form_install_configure_form_alter(&$form, $form_state) {

  // Pre-populate the site name.
  $form['site_information']['site_name']['#default_value'] = 'Cirro Dist';
  $form['site_information']['site_mail']['#default_value'] = 'admin@gocirro.com';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@gocirro.com';
  $form['update_notifications']['update_status_module']['#default_value'] = array(0, 0);
  $form['server_settings']['site_default_country']['#default_value'] = 'US';
}