;(function ( $, window, document, undefined ) {
	
	var $button = $('button.cjax');
	var data = {
		callback: "cirro_will_cjax",
		message: "This is a test by Will",
	};
console.log($button);
	$button.click( function(event) {

		Cirro.ajax({
			data: data,
			success: function( data ) {

				console.log(data);

			}


		});

	});
	

	
}( jQuery, window, document ));