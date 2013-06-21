;(function( $, Cirro, window, document, undefined ){

var $window   = $( window );
var $document = $( document );
var $loader   = $("div#loader");
var isIphone  = false;

// Detect iPhone
if( navigator.userAgent.match(/iPhone/i) ) {
  isIphone = true;
}

var homePage = {

	init: function(){

		var self = this;

		self.$html = $('html');
		self.$header = $('header#header');
		self.$container = $('.isotope-container');
		self.$filters = $('#filters a');
		self.$menu = $('.menu');
		self.$videoWrap = $('.video-wrap');
		self.$vidBlock = $('.vidblock');
		self.$videoEmbed = $('.video-embed');
		self.$closeButton = $('.close-button');
		self.$videoBar = $('.video-bar');
		self.$descriptionButton = $('.description-button');
		self.$fullDescription = $('.full-description');
		self.$vimeoTitleWrap = $('.vimeo-title-wrap');
		self.$hideDescriptionButton = $('.hide-description-button');
		self.$social = $('#social-wrap');

		self.margin = self.$menu.height();
		self.colW = 10;
		self.colCount = 3;

		// Initialize click handlers
		self.$vidBlock.click(self.closeVideo);
		self.$closeButton.click(self.closeVideo);
		self.$videoWrap.click(self.openVideo);
		self.$descriptionButton.click(self.showDescript);
		self.$hideDescriptionButton.click(self.hideDescript);

		// Attach keyboard shortcuts to window
    $( window ).keydown( self.keyboardShortcuts );

		// Initialize window event handlers
		if( window.addEventListener ){
	    window.addEventListener('message', self.receiveVimeoMessage, false);
		}
		else {
			window.attachEvent('onmessage', self.receiveVimeoMessage, false);
		}

		self.setVideoContainerSize();
		self.initIsotope();

	},

	setVideoContainerSize: function(){

		var self = this;

		self.wW = $window.width();
		self.wH = $window.height();

		// Find the height of the header and footer
		var $topMenu = $('#header');
		var $botMenu = $('#footer');
		var topMargin = $topMenu.outerHeight(true);
		var botMargin = $botMenu.outerHeight(true);
		var actualHeight = self.wH - (topMargin + botMargin);
		self.videoWidth = self.wW;
		self.videoHeight = self.videoWidth * .5618;

		if ( self.videoHeight >= actualHeight ) {
			self.videoHeight = actualHeight;
			self.videoWidth = self.videoHeight * 1.78;
		}

		self.videoMarginWidth = (self.wW - self.videoWidth)/2;
		self.videoMarginHeight = (self.wH - self.videoHeight)/2;

		var fixVideoHeightPosition = (self.wH-self.videoHeight)/2-50; 
		self.$videoEmbed
			.css('height', self.videoHeight)
			.css('width', self.videoWidth)
			.css('margin-left', self.videoMarginWidth)
			.css('margin-top', fixVideoHeightPosition);

	},

	openVideo: function(){

		var self = homePage;

		var $this = $(this);
		var vidID = $this.data('vimeo');
		var vidTitle = $this.data('title');
		var vidDescription = $.parseJSON($this.data('description'));

		self.$vidBlock.show();
		self.$videoBar.show();
		self.$filters.hide();
		self.$social.hide();

		self.$fullDescription.html(vidDescription);
		self.$vimeoTitleWrap.text(vidTitle);

		self.$header.css('background', 'rgba(255,255,255,1.0)');

		// Override html touchmove
		self.$html.on('touchmove', function( event ) {
			event.preventDefault();
		});

		Cirro.ajax({
			errorSelector: 'div.region-content',
			data: {
				callback: 'imgbrew_items_build_iframe_cjax',
				vidID: vidID,
				vidTitle: vidTitle,
				vidDescription: vidDescription,
				vidW: self.videoWidth,
				vidH: self.videoHeight
			},
			success: function(data){
				
				self.$iframe = $(data.iframe);		
				self.$videoEmbed.prepend(self.$iframe);

				self.$iframe.ready(function(){
					self.initFitVid();
				});

			}	
		});

	},

	closeVideo: function( event ){

		//console.log( "closeVideo" );

		// Stop propagation since close button overlaps vidblock (both close)
		if( typeof event === 'object' ){
			event.stopPropagation();
		}

		var self = homePage;

		// Return touchmove to html
		self.$html.off('touchmove');

		self.$header.css('background', 'rgba(255,255,255,0.7)');
		self.$vidBlock.hide();
		self.$videoBar.hide();
		self.$fullDescription.slideDown();
		self.$fullDescription.html('');
		self.$videoEmbed.html('');
		self.$vimeoTitleWrap.html('');
		self.$filters.show();
		self.$social.show();
		self.$iframe = undefined;
		self.hideDescript();
		
	},

	showDescript: function(){

		console.log( "showDescript" );

		var self = homePage;

		self.$fullDescription.show();
		self.$descriptionButton.hide();
		self.$hideDescriptionButton.show();

	},

	hideDescript: function(){

		//console.log( "hideDescript" );

		var self = homePage;

		self.$fullDescription.hide();
		self.$descriptionButton.show();
		self.$hideDescriptionButton.hide();

		if( isIphone ){
			self.$descriptionButton.hide();
		}

	},

	initIsotope: function(){

		var self = this;

		// init iso
		self.$container.isotope({
	    resizable: false,
	    masonry: { columnWidth: self.colCount }
		});

		self.$filters.click( function() {
		
			var $this = $(this);

			// Dont' proceed if its already selected
			if ( $this.hasClass('active') ) {
				return;
			}

			// Clear out any other selected classes
			var $filters = $this.parents('#filters');
			$filters.find('.active').removeClass('active');

			// Add class to this filter
			$this.addClass('active');

			// Get filter selector and pass to isotope
			var selector = $this.attr('data-filter');
			self.$container.isotope({ filter: selector});

			return false;

		});

	},

	// centerIsotope: function(){

	// 	var self = this,
	// 			columns = null;
	// 	self.$body = $('body');
	// 	self.colWid = self.$videoWrap.width()+(2*self.colW);

	// 	// check if columns has changed
 //    var currentColumns = Math.floor( ( self.$body.width() -10 ) / (self.colWid));

 //    if ( currentColumns !== self.columns ) {

 //      // set new column count
 //      self.columns = currentColumns;

 //      // apply width to container manually, then trigger relayout
 //      self.$container.width( self.columns * self.colWid)
 //      	.isotope('reLayout');

	//   }
    
	// },

	sendVimeoMessage: function(action, value){

		var self = this;

		var data, url, message;
		url = self.$iframe.attr('src').split('?')[0];
		data = {
			method: action
		};
		if (value) {
      data.value = value;
    }
		message = JSON.stringify(data);

		// Post message to the vimeo iframe
		self.$iframe[0].contentWindow.postMessage(message, url);

	},

	receiveVimeoMessage: function( event ) {
		
		var self = homePage;

    var data = JSON.parse(event.data);
    
    switch (data.event) {
			case 'ready':
			    self.vimeoReady();
			    break;
			   
			// case 'playProgress':
			//     self.vimeoProgress(data.data);
			//     break;
			    
			case 'pause':
			    self.vimeoPause();
			    break;
			   
			case 'finish':
			    self.vimeoFinish();
			    break;
    }

	},

	vimeoReady: function() {

		var self = homePage;
		self.sendVimeoMessage('play', '')
		self.sendVimeoMessage('addEventListener', 'pause');
    self.sendVimeoMessage('addEventListener', 'finish');
    self.sendVimeoMessage('addEventListener', 'playProgress');

	},

	vimeoFinish: function() {
		
		var self = homePage;
    
    self.closeVideo();

	},

	vimeoPause: function() {
		// console.log('paused');
	},

	initFitVid: function(){

		var self = this;

		self.$videoEmbed.fitVids();

	},

	// Keyboard shortcuts
	keyboardShortcuts: function( event ) {

		var self = homePage;

		// Check for ESC key 27
		if ( event.keyCode == 27 ) {

			// Close movie
			self.closeVideo();

		}

	}

};

// Attach homePage to the Cirro namespace
Cirro.homePage = homePage;

// On window load
$window.load(function() {

	setTimeout(function() {

		$loader.fadeOut();

	}, 1000);

	homePage.init();

	$window.smartresize(function(){

		//homePage.centerIsotope();

	}).smartresize(); 

});

// On window resize

$window.resize(function(){

	homePage.setVideoContainerSize();

	homePage.$container.isotope({
    masonry: { columnWidth: self.colCount }
	});

});


}( jQuery, Cirro = Cirro || {}, window, document ));


