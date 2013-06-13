<?php

// Init
$nav = '';
$logo = '';
$dashboard_markup = '';
global $user;

$logo = "<a href=\"/\"><img src=\"/sites/default/files/logos/imgBrewLogo.png\"></a>";

// Nav Links
$about_link = l('About', 'about');
$contact_link = l('Contact', 'contact');
$login = l('Login', 'user');
$logout = l('Logout', 'user/logout');

if ( $user->uid == 0 ) {

	$user_link = $login;

} else {

	$user_link = $logout;

}

// Check for dashboard access
if ( user_access('access dashboard') ) {

	// Build dashboard link
	$dashboard_link = l('Dashboard', 'admin/dash');
	$dashboard_markup = "<li>$dashboard_link</li>";

}

// Nav markup
$nav = "	
	<div class=\"logo-wrap\">$logo</div>
	<nav id=\"main-menu\">
		<ul>
			<li>$about_link</li>
			<li>$contact_link</li>
			$dashboard_markup
			<li>$user_link</li>
		</ul>
	</nav>
";

print $nav;