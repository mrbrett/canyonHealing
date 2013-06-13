<?php
/**
 * @file
 *   Clients page builds.
 */

/**
	* Display page build.
	*/
function imgbrew_clients_page($client_name) {

	// Init
	$html = '';
	$staff = FALSE;
	$posts_html = '';
	$notify_button = '';
	global $user;

	// Add js
	$path = drupal_get_path('module', 'imgbrew_clients');
	drupal_add_js("$path/js/imgbrew_clients_notify.js", array('scope' => 'footer'));

	// Check for user admin or staff
	if (
		$user->uid == 1 ||
		in_array('staff', $user->roles)
	) {

		$staff = TRUE;

	}

	// Query client
	$query_string = "
		SELECT id, title, short_name, notified
			FROM imgbrew_client
			WHERE short_name = :name
	";

	$query = db_query($query_string, array(':name' => $client_name));

	$client = $query->fetchObject();

	// Build title
	$title = $client->title;

	// Check for staff
	if ( $staff ) {

		// Build link
		$title = l($title, "admin/dash/client/{$client->id}/edit");

		// Find user email
		$query_string = "
			SELECT mail
				FROM users
				WHERE name = :name
		";

		$query = db_query($query_string, array(':name' => $client->short_name));

		$mail = $query->fetchField();

		// Check for notified
		if ( $client->notified == 0 ) {

			$notified = '<em>never</em>';

		} else {

			$notified = format_date($client->notified);

		}

		// Build button
		$notify_button = "
			<button
				class=\"btn notify\"
				data-mail=\"$mail\"
				data-client=\"$client->id\"
			>
				Notify
			</button>
			<div class=\"notify-status\">$notified</div>
		";


	}

	// Add client name
	$title_wrapped = "
		<div class=\"post-client\">
			<h1>$title</h1>
			$notify_button
		</div>
	";

	// Get client ID
	$cid = $client->id;

	// Query client posts
	$query_string = "
		SELECT title, id, vimeo_id, notes, created
			FROM imgbrew_client_post
			WHERE parent_id = $cid
			ORDER BY weight, created
	";

	$query = db_query($query_string);

	$posts = $query->fetchAll();

	// Loop through posts
	foreach ( $posts as $post ) {

		// Init
		$title = $post->title;
		$vimeo = '';
		$notes = '';
		$created = format_date($post->created);

		// Check for staff
		if ( $staff ) {

			$title = l($post->title, "admin/dash/client_post/{$post->id}/edit");

		}

		// Check for notes
		if ( isset($post->notes) && !empty($post->notes) ) {

			// Build notes markup
			$notes = "
				<div class=\"post-notes\">
					{$post->notes}
				</div>
			";

		}

		// Check for vimeo ID
		if ( isset($post->vimeo_id) && !empty($post->vimeo_id) ) {

			$vid = $post->vimeo_id;

			// Build vimeo markup
			$vimeo = "
				<iframe
					id=\"player\"
					class=\"post-vid\"
					src=\"http://player.vimeo.com/video/$vid?api=1&title=0&amp;byline=0&amp;portrait=0&amp;color=ffffff\" 
					frameborder=\"0\" 
					width=\"960px\"
					height=\"540px\"
					webkitAllowFullScreen
					mozallowfullscreen
					allowFullScreen
				>
				</iframe>
			";
		}

		// Build markup
		$post_html = "

			<div class=\"post-outer-wrapper\">

				<div class=\"post-title\">
					<h2>$title</h2><h5>$created</h5>
				</div>

				<div class=\"post-vid-wrapper\">
					$vimeo
				</div>

				$notes

			</div>

			<hr></hr>

		";

		// Add to page html
		$posts_html .= $post_html;

	}

	// Put it all together
	$html .= "
		$title_wrapped
		$posts_html
	";

	return $html;

}