<header id="header">
    <?php include 'menu.inc'; ?>
</header>
<div class="container-fluid" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="row-fluid">
    <div class="span12">
      <?php print $messages; ?>
    	<?php print render($page['content']) ?>
    </div>
  </div>
</div>
<footer id="footer">
  <div id="social-wrap">
    <div class="social">
      <a href="https://www.facebook.com/imagebrew" target="_BLANK">
        <img src="/sites/default/files/social/facebook.png"/>
      </a>
      <a href="https://twitter.com/imagebrew" target="_BLANK">
        <img src="/sites/default/files/social/twitter.png"/>
      </a>
      <a href="http://vimeo.com/imagebrew" target="_BLANK">
        <img src="/sites/default/files/social/vimeo.png"/>
      </a>
      <a href="http://www.linkedin.com/company/image-brew" target="_BLANK">
        <img src="/sites/default/files/social/linkedin.png"/>
      </a>
  </div>
  <div class="container">
    <div class="menu bottom-wrap"></div>
  </div>
</footer> 

