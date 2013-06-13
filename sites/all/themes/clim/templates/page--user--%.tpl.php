<?php
	// Check for staff and client users and redirect as appropriate
	global $user;

	// Don't bother for anon or admin
	if ( $user->uid  && $user->uid != 1 ) {

		// Check for staff
		if ( in_array('staff', $user->roles) ) {

			// Redirect to dashboard
			drupal_goto('admin/dash');

		}

		// Query for client with matching cuid
		$query_string = "
			SELECT short_name
				FROM imgbrew_client
				WHERE cuid = :uid
		";

		$query = db_query($query_string, array(':uid' => $user->uid));

		$client = $query->fetchObject();

		// Check for empty
		if ( !empty($client) ) {

			// Redirect to client page
			drupal_goto("client/{$client->short_name}");

		} else {

			// Redirect to logout
			drupal_goto('user/logout');

		}

	}

?>
<div class="container <?php print $classes; ?>"<?php print $attributes; ?> >
	 
  <section id="main" class="clearfix">
     <?php print $messages; ?>

      <div id="content-area">
        <?php print render($page['content']) ?>
      </div>
   
  </section> <!-- /main -->
  
</div> <!-- /page -->
