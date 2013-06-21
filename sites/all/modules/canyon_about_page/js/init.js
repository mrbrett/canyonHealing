;(function( $, window, document, undefined ){
    
	// Init vars
	var Cirro     = window.Cirro || {},
			$window   = $(window),
			$document = $(document),
			$html     = $("html");

	// Set scroll offset
	Cirro.scrollOffset = -110;

	var aboutPage = {


		init: function(){

			var self = this;

			self.$nav = $("#about-nav a");

			// Attach click handlers
			self.$nav.on("click", self.scrollTo);
			console.log('clicked');
			//self.setActiveSection();

		},


		//
		// Scrollto easing
		//
		scrollTo: function(event){

			var self = aboutPage;

			// Init
			var $link = $(this);

			// Scroll to href of clicked link
			$("html, body").stop().animate({

        scrollTop: $($link.attr("href")).offset().top + Cirro.scrollOffset

      }, 1500,'easeInOutExpo');

			event.preventDefault();

		},


	};
	
$window.load(function() {

	aboutPage.init();
})



}( jQuery, window, document));
