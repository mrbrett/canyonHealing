<div class="container <?php print $classes; ?>"<?php print $attributes; ?> >
  <div class="login-form">
  	<div id="login-logo">
	  	<a href="/"><img src="/sites/default/files/logos/imgBrewLogo.png"></a>
	  </div>
    <?php print $messages; ?>
    <?php print render($page['content']) ?>
    <div id="forgot"><a href="user/password">forgot?</a></div> 
  </div>
</div>
<div class="barrr top"></div>
<div class="barrr left"></div>
<div class="barrr bottom"></div>
<div class="barrr right"></div>