;(function( $, window, document, undefined ) {

	$(document).ready( function() {

		// Init
		var $button = $("button.notify"),
		$status = $("div.notify-status"),
		loadGIF = '<img src="/sites/all/themes/clim/images/loading_small.gif" />';

		$button.on('click', function( event ) {

			// Load status with sending message
			$status.html('sending notification... ' + loadGIF);

			// Post to cirro ajax
			Cirro.ajax({
				data: {
					callback: "imgbrew_clients_notify_cjax",
					mail: $button.data('mail'),
					client: $button.data('client'),
				},
				success: function( data ) {

					// Replace status with new notified text
					$status.html(data.notified);

				}

			});

		});

	});

}( jQuery, window, document ));