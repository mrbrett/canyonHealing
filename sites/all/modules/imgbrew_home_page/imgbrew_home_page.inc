<?php

/**
 * @file
 * The page callback for the homepage
 */

function imgbrew_home_page() {

	// Add JS
	$module_path = drupal_get_path('module', 'imgbrew_home_page');
	$js_path = "$module_path/js/";
	drupal_add_js($js_path . 'jquery.isotope-min.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'jquery.fitvids.js', array('scope' => 'header'));
	drupal_add_js($js_path . 'init.js', array('scope' => 'footer'));

	// Init
	$html   = '';
	$posts = '';
	$filters = '';
	
	// Query video types in use
	$query_string = "
		SELECT DISTINCT video_type
			FROM imgbrew_video
	";

	$query = db_query($query_string);

	$types = $query->fetchCol();

	// Loop through types
	foreach ( $types as $type ) {

		// Capitalize type
		$cap_type = ucfirst($type);

		// Build type filter list item
		$filter = "
			<li>
				<a href=\"#\" data-filter=\".{$type}\">$cap_type</a>
			</li>
		";

		// Add to filters
		$filters .= $filter;

	}

	// Filter links for isotope
	$filters_list = "
		<ul id=\"filters\">
			<li><a href=\"#\" class=\"active\" data-filter=\"*\">Show all</a></li>
			$filters
		</ul>
	";

	// Query videos
  $query_string = "
  SELECT s.title, s.vimeo_id, s.description, s.video_type, f.uri
    FROM imgbrew_video AS s
    JOIN file_managed AS f
    ON f.fid = s.thumbnail
    ORDER BY s.weight DESC
	";


	// Use Drupal's db_query function
	$query = db_query( $query_string );
	$results = $query->fetchAll();

	foreach($results as $imgbrew_video) {
  
  	$title = $imgbrew_video->title;
  	$vimeo_id = $imgbrew_video->vimeo_id;
  	$description = drupal_json_encode($imgbrew_video->description);
  	$type = $imgbrew_video->video_type;
  	$video_type = $imgbrew_video->video_type;
  	$uri = $imgbrew_video->uri;
   
	  $img_vars = array(
		  'style_name' => 'video',
	    'path' => $uri, 
	    'alt' => '',
	    'title' => '',
	    'width' => '',
	    'height' => '',
	    'attributes' => array(),
    );

	// Style the video thumbnails
	$video_img = theme_image_style($img_vars);

		$posts .= "
			<div class=\"item $video_type toggle\">	
				<div class=\"item-inner\">	
					<div
						class=\"video-wrap\"
						data-vimeo=\"$vimeo_id\"
						data-title=\"$title\"
						data-description='$description'
					>
						<div id=\"hover-wrap\">
							<div class=\"font-wrap\">
								$title
							</div>
							<div class=\"type-wrap\">
								$type
							</div>
						</div>
						$video_img
					</div>
				</div>
			</div>
		";
	 
  }

	$html .= "
	  <div class=\"isotope-container\">
	  	$posts
	  </div>
	  $filters_list
	  <div class=\"video-bar\">
			<div class=\"vimeo-title-wrap\"></div>
			<div class=\"description-button\">
				Description
			</div>
			<div class=\"hide-description-button\">
				Hide Description
			</div>
		</div>
	  <div class=\"vidblock\">
	  	<div class=\"video-embed\"></div>
	  	<div class=\"close-button\">
	  		<img src=\"/sites/default/files/close.png\" />
	  	</div>
	  </div>
		<div class = \"full-description\"></div>
	 ";


	return $html;

}
