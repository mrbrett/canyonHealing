<?php

/**
 * @file
 * The page callback for the homepage
 */

function imgbrew_contact_page() {

	$module_path = drupal_get_path('module', 'imgbrew_contact_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.easing.1.3.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));
	$js_path_maps = "https://maps.google.com/maps/api/js?sensor=false";
	drupal_add_js($js_path_maps, array('scope' => 'footer', 'weight' => 10));

	$html = '';
	$address_posts = '';
	$rentals_page_link = 'Rentals Page';
	$rentals_page_path = "rentals";
	$rentals_link = l($rentals_page_link, $rentals_page_path);

	$query_string_contact = "
	SELECT c.body, c.email, c.address, c.phone_number, f.uri
		FROM imgbrew_contact_page AS c
		LEFT JOIN file_managed AS f
			ON c.image = f.fid
	";

	$contact_query = db_query( $query_string_contact );
	$contact_results = $contact_query->fetchAll();

 foreach($contact_results as $imgbrew_contact) {

 	// Init
 	$body = $imgbrew_contact->body;
	$email = $imgbrew_contact->email;
	$address = $imgbrew_contact->address;
	$phone_number = $imgbrew_contact->phone_number;
	$image_url = file_create_url($imgbrew_contact->uri);

	$address_posts = "
		<div class = \"address-wrap\">
			$address
		</div>
		<div class = \"email-wrap\">
			<a href=\"mailto:$email\">$email</a>
		</div>
		<div class = \"phone-wrap\">
			$phone_number
		</div>
	";

 }
	
	$contact_nav = "
		<ul id=\"contact-nav\">
			<li><a class=\"contact\" href=\"#contact\">Contact</a></li>
			<li><a class=\"office\" href=\"#office\">Office</a></li>
			<li><a class=\"address\" href=\"#address\">Address</a></li>
			<li><a class=\"map\" href=\"#map\">Map</a></li>
		</ul>
	";
	
	$map = "
   <div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>
  ";

	$html = "
	<section id=\"contact\">
		<div class=\"row-fluid contact-title-wrap\">
			<div class=\"span2\">
				<h4>Contact</h4>
			</div>
		<div class=\"span10 contact-text-wrap\">
			$body
			<div class=\"rentals-link-wrap\">
				$rentals_link
			</div>
		</div>
	</section>
	<section id=\"office\">
		<img src=\"$image_url\" />
	</section>
	<section id=\"address\" class=\"row-fluid\">
		<div class=\"span2\">
			<h4>Address</h4>
		</div>
		<div class=\"span10\">
			$address_posts
		</div>
	</section>
	<section id=\"map\">
		$map
	</section>
	$contact_nav
	";

	return $html;

}

/**
 * Build Rentals page
 */
function imgbrew_contact_rentals_page() {

	// Init
	$html = '';
	$rentals = 'We do not currently have any rentals available.';

	// Query rentals
	$query_string = "
		SELECT rentals_text
			FROM imgbrew_rentals
	";

	$query = db_query($query_string);

	$rentals_obj = $query->fetchObject();

	// Check for text
	if (
		isset($rentals_obj->rentals_text) && 
		!empty($rentals_obj->rentals_text)
	) {

		// Replace text
		$rentals = $rentals_obj->rentals_text;

	}

	// Wrap rentals text
	$html = "
		<h1>Rentals</h1>
		<div class=\"rentals-wrapper\">
			$rentals
		</div>
	";

	return $html;

}



