<?php // no direct access
defined('_JEXEC') or die('Restricted access');

if ( $tabs_pos == 0 ) : ?>

<div id="dm_tabs_<?php echo $dinamods_id; ?>">
  <ul class="dm_menu_<?php echo $dinamods_id; ?>">
    <?php
$k=1;
foreach ( $dinamods as $dinamod ) {
?>
    <li class="dm_menu_item_<?php echo $dinamods_id; ?>"><a href="#" rel="dm_tab_<?php echo $dinamods_id; ?>_<?php echo $k; ?>"<?php echo $k == 1 ? ' class="dm_selected"' : ''; ?>><?php echo $dinamod->title; ?></a></li>
    <?php
$k++; 
} ?>
  </ul>
</div>
<br style="clear:left;" />
<?php endif; ?>
<div id="dm_container_<?php echo $dinamods_id; ?>">
  <?php
$k=1;
foreach ( $dinamods as $dinamod ) {
  $modParams = new JParameter( $dinamod->params );
  $class_sfx = $modParams->get( 'moduleclass_sfx' );?>
  <div id="dm_tab_<?php echo $dinamods_id; ?>_<?php echo $k; ?>" class="dm_tabcontent">
    <div class="moduletable<?php echo $class_sfx; ?>">
      <?php
        echo JModuleHelper::renderModule($dinamod); ?>
    </div>
  </div>
  <?php
$k++;
}
?>
</div>
<?php if ( $tabs_pos == 1 ) : ?>
<div id="dm_tabs_<?php echo $dinamods_id; ?>">
  <ul class="dm_menu_<?php echo $dinamods_id; ?>">
    <?php
$k=1;
foreach ( $dinamods as $dinamod ) {
?>
    <li class="dm_menu_item_<?php echo $dinamods_id; ?>"><a href="#" rel="dm_tab_<?php echo $dinamods_id; ?>_<?php echo $k; ?>"<?php echo $k == 1 ? ' class="dm_selected"' : ''; ?>><?php echo $dinamod->title; ?></a></li>
    <?php
$k++; 
} ?>
  </ul>
</div>
<?php endif; ?>
<script type="text/javascript" language="javascript">
<!--
var Dinamods=new dinamods("dm_tabs_<?php echo $dinamods_id; ?>")
Dinamods.setpersist(true)
Dinamods.setselectedClassTarget("link")
Dinamods.init(<?php echo $speed; ?>,<?php echo $params->get('change', 0); ?>)
-->
</script>