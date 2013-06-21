/**
 * The Cirro JS framework
 *
 * This script is called in the header of the page after jQuery,
 * but before any plugin definitions.
 */
;(function( Cirro, $, window, document, undefined ) {

	"use strict";

	/**
	 * Cirro JS Error Reporter
	 */

	// The default error selector (where the errors get prepended to)
	var errorSelector = "div#content";

	// The Cirro error reporting function
	Cirro.error = function( error, data ) {

		// Console log the error
		console.error( error );

		// Create error message markup
		var $errorMessage = $( "<div />", {
			"class": "alert alert-error",
		}).html( "<em>Cirro JS Framework Error: </em>" + error );

		// If data was passed to the error function
		if ( typeof data === "object" ) {

			// If trace was passed to the error function
			if ( typeof data.trace === "object" ) {

				// Console log the trace
				console.log( "---TRACE---" );
				console.log( data.trace );
				console.log( "---/TRACE---" );

			}

			// If messages were passed to the error function
			if ( typeof data.messages === "object" ) {

				// Create messages markup
				var $messages = $( "<ul />" );

				// Loop through messages
				$.each( data.messages, function( index, message ) {

					// If the message is a string
					if ( typeof message === "string" ) {

						// Create the message markup
						$( "<li />" ).text( message ).appendTo( $messages );

					}

				});

				// Append the messages to the error message
				$errorMessage.append( $messages );

			}

		}

		// Prepend the error message to the main container
		$( errorSelector ).prepend( $errorMessage );

	}



	/**
	 * Cirro AJAX
	 */

	// Status codes accepted from the return of the AJAX calls
	var statusCodes = [ "pass", "error", "exception" ];

	// The Cirro.ajax public method
	Cirro.ajax = function( options, type ) {

		// If the options parameter has an $errorPlacement property
		if ( typeof options.errorSelector === "string" ) {

			// Override the default error placement
			errorSelector = options.errorSelector;
			
		}

		// If the options parameter is not an object
		if ( typeof options !== "object" ) {

			// Call error and stop the function
			Cirro.error( "Options passed to Cirro AJAX is not an object." );
			return;

		}

		// If the options parameter does not have data
		if ( typeof options.data === "undefined" ) {

			// Call error and stop the function
			Cirro.error( "No data passed to Cirro AJAX." );
			return;

		}

		// Define the ajaxOptions
		var ajaxOptions = {
			url: "/Cirro/ajax",
			type: "POST",
			data: options.data,
			error: function( jqXHR, textStatus, errorThrown ) {

				// Call error
				Cirro.error( "Cirro AJAX framework PHP page was not found." );

			},
			success: function( data, textStatus, jqXHR ) {

				// If data is not an object
				if ( typeof data !== "object" ) {

					// Call error and stop the success function
					Cirro.error( "Data returned from Cirro AJAX is not an object." );
					return;

				}

				// If the data does not contain a status
				if ( typeof data.status === "undefined" ) {

					// Call error and stop the success function
					Cirro.error( "No status returned from Cirro AJAX." );
					return;

				}

				// If the status is not a string
				if ( typeof data.status !== "string" ) {

					// Call error and stop the success function
					Cirro.error( "Status returned from Cirro AJAX is not a string." );
					return;

				}

				// If the status type is not an allowed status code
				if ( $.inArray( data.status, statusCodes ) === -1 ) {

					// Call error and stop the success function
					Cirro.error( "Status returned from Cirro AJAX is not recognized." );
					return;

				}

				// If the status type is "exception"
				if ( data.status === "exception" ) {

					// Call the error and stop the success function
					Cirro.error( "Cirro AJAX threw an exception.", data );
					return;

				}

				// If the status type is "error"
				if ( data.status === "error" ) {

					// Call the error and stop the success function
					Cirro.error( "Cirro AJAX returned an error.", data );
					return;

				}

				// If there is a success callback in the options
				if ( typeof options.success === "function" ) {

					// Execute success callback
					options.success( data );

				}

				// Console log Cirro AJAX success statement
				console.log( "Cirro AJAX Success" );

			},
		};

		// If the uploadFiles flag is true
		if ( type === "upload files" ) {

			// Add appropriate properties to the ajax options			
			ajaxOptions.processData = false;
			ajaxOptions.contentType =  'multipart/form-data';

		}

		// Run the jQuery AJAX method
		$.ajax( ajaxOptions );

	}



	/**
	 * Cirro WYSIWYG
	 */

	// The default Cirro WYSIWYG options
	Cirro.WYSIWYG = {
		fieldSelector: "textarea:not(.no-wysiwyg)",//"textarea:not(.no-wysiwyg)",
		buttons: [ 'html', 'formatting', 'unorderedlist', 'link' ],
	};



}( window.Cirro = window.Cirro || {}, jQuery, window, document ));