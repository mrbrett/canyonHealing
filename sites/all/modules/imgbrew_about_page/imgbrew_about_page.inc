<?php

/**
 * @file
 * The page callback for the homepage
 */

function imgbrew_about_page() {

	$module_path = drupal_get_path('module', 'imgbrew_about_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.easing.1.3.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));
	
	// INIT vars
	$html   = '';
	$about_posts = '';
	$team_posts = '';
	$brand_posts = '';
	$count = 0;

	$query_string_about = "
		SELECT a.about_intro, f.uri, a.press
		FROM imgbrew_about_page AS a
		JOIN file_managed AS f
		ON f.fid = a.process_graphic
		ORDER BY created DESC
	";

	$about_query = db_query( $query_string_about );
	$about_results = $about_query->fetchAll();
	$imgbrew_about = reset($about_results);

	$intro = $imgbrew_about->about_intro;
	$process_uri = $imgbrew_about->uri;
	$process_pic = file_create_url($process_uri);

	$about_posts .= "
		<section id=\"about\" class=\"row-fluid\">
			<div class=\"span2\">
				<h2>About</h2>
			</div>
			<div class=\"span10 overview-wrap\">
				<h3>$intro</h3>
			</div>
		</section>
		<section id=\"process\" class=\"row-fluid\">
			<div class=\"span2\">
				<h4>Process</h4>
			</div>
			<div class=\"span10 process graphic-wrap\">
				<img src=\"$process_pic\" />
			</div>
		</section>
	";

	$query_string_team = "
		SELECT t.title, t.bio, f.uri
	  FROM imgbrew_team_member AS t
	  LEFT JOIN file_managed AS f
	  ON f.fid = t.team_photo
	  ORDER BY t.weight
	";

	$team_query = db_query( $query_string_team );
	$team_results = $team_query->fetchAll();

	foreach ( $team_results as $imgbrew_team ) {

		// Init
		$name = $imgbrew_team->title;
		$bio = $imgbrew_team->bio;
		$team_img = '';

		// Check for URI
		if ( isset($imgbrew_team->uri) ) {
		
			$team_uri = $imgbrew_team->uri;

		  $img_vars = array(
			  'style_name' => 'team_photos',
		    'path' => $team_uri, 
		    'alt' => '',
		    'title' => '',
		    'width' => '',
		    'height' => '',
		    'attributes' => array(),
		  );

			$team_img = theme_image_style($img_vars);

		}

  	if($count == 0){
		
			$team_posts .= "<div class=\"row-fluid\">";
		
		}
		
		if(($count%3) == 0 && $count !== 0){
	 	
	 		$team_posts .="</div><div class=\"row-fluid\">";
	 	
	 	}

	 	$count++;

		$team_posts .= "
			<div class=\"team-wrap span4\">
				<div class=\"team-pic-wrap\">
					$team_img
				</div>
				<div class=\"bio-name\">
					<h5>$name</h5>
				</div>
				<div class=\"bio-wrap\">
					$bio
				</div>
			</div>
		";


	}

	// Query for brands
	$query_string_brands = "
		SELECT f.uri
		FROM imgbrew_brand AS b
		JOIN file_managed AS f
		ON f.fid = b.brand_photo
		ORDER BY b.weight
	";

	$brands_query = db_query( $query_string_brands );
	$brands_results = $brands_query->fetchAll();

	// Loop through brands and create images
	foreach($brands_results as $imgbrew_brands) {
	$uri = $imgbrew_brands->uri;
	
	 $img_vars = array(
	  'style_name' => 'logos',
    'path' => $uri, 
    'alt' => '',
    'title' => '',
    'width' => '',
    'height' => '',
    'attributes' => array(),
  );

	$brand_img = theme_image_style($img_vars);

	$brand_posts .= "
		<div class=\"brand-wrap\">
			$brand_img
		</div>
	";

	}

	$about_nav = "
		<ul id=\"about-nav\">
			<li><a class=\"about\" href=\"#about\">About</a></li>
			<li><a class=\"process\" href=\"#process\">Process</a></li>
			<li><a class=\"team\" href=\"#team\">Team</a></li>
			<li><a class=\"brands\" href=\"#brands\">Brands</a></li>
			<li><a class=\"press\" href=\"#press\">Press</a></li>
		</ul>
	";

	$html = " 
		$about_posts
		<section id=\"team\" class=\"row-fluid\">
			<div class=\"span2\">
				<h4>Team</h4>
			</div>
			<div class=\"span10\">
				$team_posts
			</div>
		</section>
		<section id=\"brands\" class=\"row-fluid\">
			<div class=\"span2\">
				<h4>Brands</h4>
			</div>
			<div class=\"span10\">
				$brand_posts
			</div>
		</section>
		<section id=\"press\" class=\"row-fluid\">
			<div class=\"span2\">
				<h4>Press</h4>
			</div>
			<div class=\"span10\">
				{$imgbrew_about->press}
			</div>
		</section>
		$about_nav
	";

	return $html;

}
