/**
 * The Clim Admin Theme JS plugin instantiations
 *
 * This script is called in the footer before all other instantiations
 * within the climAdmin theme.
 */
;(function( Cirro, $, window, document, undefined ){

	"use strict";


	/**
	 * Cirro WYSIWYG Validation
	 */

	// If the Redactor jQuery plugin is not defined
	if ( typeof $.fn.redactor !== "function" ) {

		// Call error and stop the function
		Cirro.error( "The Redactor Plugin was not added to the page." );
		return;

	}

	// If the Cirro WYSIWYG object has been overridden and is now undefined
	if ( 
		typeof Cirro.WYSIWYG !== "object" ||
		typeof Cirro.WYSIWYG.fieldSelector !== "string" ||
		typeof Cirro.WYSIWYG.buttons !== "object"
	) {

		// Call error and stop the function
		Cirro.error( "The WYSIWYG options are missing." );
		return;
		
	}

	/**
	 * Document Ready and Plugin Instantiations
	 */

	// Cache references to the jQuery window and document
	var $document = $( document );

	// When the document is ready
	$document.ready(function() {

		// Redactor WYSIWYG
		// Init
		var $textareas = $( Cirro.WYSIWYG.fieldSelector );
		var buttons = Cirro.WYSIWYG.buttons;


		// Instantiate redactor
		$textareas.redactor({
			buttons: buttons,
		});

		// Core multi tools details simple slider

		// Init
		var $accButton = $('div.multi-acc-button');
		var $accContent = $('div.multi-acc-content');
		var detailsText = '<i class="icon-chevron-down"></i> show details';
		var hideText = '<i class="icon-chevron-up"></i> hide';

		// Bind click to button
		$accButton.click(function() {

			// Init
			var $this = $(this);
			var $thisContent = $this.next('div.multi-acc-content');

			// Check for content visible
			if ( $thisContent.is(':visible') ) {

				// Hide content
				$thisContent.slideUp();

				// Replace button content
				$this.html(detailsText);

			} else {

				// Slide up an other contents that are visible
				$accContent.slideUp();

				// Replace all buttons with details text
				$accButton.html(detailsText);

				// Show content
				$thisContent.slideDown();

				// Replace button content
				$this.html(hideText);

			}

		});

	});	

}( window.Cirro = window.Cirro || {}, jQuery, window, document ));