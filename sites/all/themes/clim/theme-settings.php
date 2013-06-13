<?php
// Form override fo theme settings
function clim_form_system_theme_settings_alter(&$form, $form_state) {

  $form['options_settings']['clim_like'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Do you like me?'),
  );
  
}