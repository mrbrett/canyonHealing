;(function( $, window, document, undefined ){
    
	// Init vars
	var $window   = $(window),
			$document = $(document),
			$html     = $("html");

	var contactPage = {


		init: function(){

			var self = this;

			self.$nav = $("#contact-nav a");
			self.googleMap();

			// Attach click handlers
			self.$nav.on("click", self.scrollTo);
			console.log('clicked');


		},


		//
		// Scrollto easing
		//
		scrollTo: function(event){

			var self = contactPage;

			// Init
			var $link = $(this);

			// Scroll to href of clicked link
			$("html, body").stop().animate({

        scrollTop: $($link.attr("href")).offset().top - 120

      }, 1500,'easeInOutExpo');

			event.preventDefault();

		},


		googleMap: function(){

			var self = this;

			// Lat long of store
				var Latlng = new google.maps.LatLng(39.772131,-104.981948);

				// Google map styles array
				var styles = [
				  {
				    "stylers": [
				      { "saturation": -100 },
				      { "gamma": 0 },
				      { "weight": 0.3 },
				      { "visibility": "on" }
				    ]
				  }
				];

			  // Create a new StyledMapType object, passing it the array of styles,
			  // as well as the name to be displayed on the map type control.
			  var styledMap = new google.maps.StyledMapType(styles);

			  // Create a map object, and include the MapTypeId to add
			  // to the map type control.
			  var mapOptions = {

			    zoom: 15,
			    center: Latlng,
			    scrollwheel: false,
			    mapTypeControl: false,
			    mapTypeId: google.maps.MapTypeId.ROADMAP

			  };

			  var map = new google.maps.Map(document.getElementById('map_canvas'),mapOptions);

			  // Map marker
			  var marker = new google.maps.Marker({
			      position: Latlng,
			      map: map
			      //animation: google.maps.Animation.DROP
			      //title:"TAXI"
			  });

			  //Associate the styled map with the MapTypeId and set it to display.
			  map.mapTypes.set('map_style', styledMap);
			  map.setMapTypeId('map_style');

		},


	};
	
$window.load(function() {

	contactPage.init();
})



}( jQuery, window, document));
