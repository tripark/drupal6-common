<?php // $Id: views-cycle.tpl.php,v 1.1.2.3 2010/01/21 23:13:13 crell Exp $ ?>
<?php echo $js_settings; ?>
<div class="views-cycle item-list">
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <<?php print $options['type']; ?> class="views-cycle-container js-hide" id="<?php echo $cycle_id; ?>">
    <?php foreach ($rows as $id => $row): ?>
      <li class="<?php print $classes[$id]; ?>"><?php print $row; ?></li>
    <?php endforeach; ?>
  </<?php print $options['type']; ?>>
  <ul id='<?php print $cycle_id; ?>-thumb-data' style='display: none;'><?php print $thumbs_data; ?></ul>
</div>
