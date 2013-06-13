<?php global $user; ?>
<?php if ( $user->uid != 1 ) { ?>
  <div class="cirro-header">
    <ul class="cirro-header-links">
      <?php if ( user_access('access dashboard') ) { ?>
        <li><a href="/admin/dash">Dashboard</a></li>
      <?php } ?>
      <?php if ( user_access('administer users') ) { ?>
        <li><a href="/admin/people">People</a></li>
      <?php } ?>
      <li><a href="/user/logout">Logout</a></li>
    </ul>
  </div>
<?php } ?>

<div class="container-fluid">  
  <header id="header" class="clearfix">
    <?php print $breadcrumb; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="page-title left"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ( $user->uid ==1) { ?>
      <div class="admin_tabs right">
        <?php print render($primary_local_tasks); ?>
      </div>
    <?php } ?>
  </header>

  <div id="page">
    <?php if ($secondary_local_tasks): ?>
      <div class="tabs-secondary clearfix"><ul class="tabs secondary"><?php print render($secondary_local_tasks); ?></ul></div>
    <?php endif; ?>

    <div id="content" class="clearfix">
      <div class="element-invisible"><a id="main-content"></a></div>
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      <?php if ($page['help']): ?>
        <div id="help">
          <?php print render($page['help']); ?>
        </div>
      <?php endif; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </div>

    <div id="footer">
      <?php print $feed_icons; ?>
    </div>

  </div>
</div>
