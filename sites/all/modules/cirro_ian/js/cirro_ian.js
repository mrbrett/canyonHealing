/*
Cirro.ajax({
	$errorPlacement: jQuery( "div#content" ),
	data: {
		callback: "valid_ajax_callback_cjax",
		test: "testing valid callback",
	},
	success: function( data ) {

		console.log( data );

	},
});

Cirro.ajax({
	$errorPlacement: jQuery( "div#content" ),
	data: {
		callback: "invalid_ajax_callback",
		test: "testing invalid callback",
	},
	success: function( data ) {

		console.log( data );

	},
});
*/