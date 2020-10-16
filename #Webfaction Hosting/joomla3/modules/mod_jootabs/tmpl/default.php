<?php 
// no direct access
defined('_JEXEC') or die('Restricted access'); 
jimport('joomla.application.module.helper');
?>
<!-- Move this line into head section of template to make your web valid xhtml --> 
<link href="modules/mod_jootabs/jootabs/style<?php echo $tab_template; ?>.css" rel="stylesheet" type="text/css" />


<script type="text/javascript">
/*
EASY TABS 1.2 Produced and Copyright by Koller Juergen
www.kollermedia.at | www.austria-media.at
*/

var tablink_idname = new Array("<?php echo $nameidlinks; ?>")
var tabcontent_idname = new Array("<?php echo $nameidarea; ?>")
var tabcount = new Array("<?php echo $numbertab; ?>")
var loadtabs = new Array("<?php echo $firsttabopen; ?>")
var autochangemenu = <?php echo $autochange; ?>;
var changespeed = <?php echo $changedelay; ?>;
var stoponhover = <?php echo $changestop; ?>;

function easytabs(menunr, active) {if (menunr == autochangemenu){currenttab=active;}if ((menunr == autochangemenu)&&(stoponhover==1)) {stop_autochange()} else if ((menunr == autochangemenu)&&(stoponhover==0))  {counter=0;}menunr = menunr-1;for (i=1; i <= tabcount[menunr]; i++){document.getElementById(tablink_idname[menunr]+i).className='tab'+i;document.getElementById(tabcontent_idname[menunr]+i).style.display = 'none';}document.getElementById(tablink_idname[menunr]+active).className='tab'+active+' tabactive';document.getElementById(tabcontent_idname[menunr]+active).style.display = 'block';}var timer; counter=0; var totaltabs=tabcount[autochangemenu-1];var currenttab=loadtabs[autochangemenu-1];function start_autochange(){counter=counter+1;timer=setTimeout("start_autochange()",1000);if (counter == changespeed+1) {currenttab++;if (currenttab>totaltabs) {currenttab=1}easytabs(autochangemenu,currenttab);restart_autochange();}}function restart_autochange(){clearTimeout(timer);counter=0;start_autochange();}function stop_autochange(){clearTimeout(timer);counter=0;}

window.onload=function(){
var menucount=loadtabs.length; var a = 0; var b = 1; do {easytabs(b, loadtabs[a]);  a++; b++;}while (b<=menucount);
if (autochangemenu!=0){start_autochange();}
}
</script>


<?php
// preparing the new variable to be able to stop or the click or the mouseover depending of user selection //
       if($changetype == mouseover) {
                $notselected = onclick;
                       } else {
                      $notselected = mouseover;
                     }

// preparing the option to show or not the titles of the modules inside the tabs
           if($titleshow == 1) {
                $showtitle = -2;
                       } else {
                      $showtitle = -1;
                     }
                     ?>


<!-- jooTabs - Joomla tabs by http://templateplazza.com -->
<!--Start of the menu -->
<div class="menu" style="width:<?php echo $width_tab; ?>px;">
<ul>
<?php if ($numbertab >= 1)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '1');" onFocus="easytabs('1', '1');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>1"><?php echo $tab1name; ?></li>
<?php } ?>

<?php if ($numbertab >= 2)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '2');" onFocus="easytabs('1', '2');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>2"><?php echo $tab2name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 3)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '3');" onFocus="easytabs('1', '3');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>3"><?php echo $tab3name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 4)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '4');" onFocus="easytabs('1', '4');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>4"><?php echo $tab4name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 5)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '5');" onFocus="easytabs('1', '5');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>5"><?php echo $tab5name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 6)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '6');" onFocus="easytabs('1', '6');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>6"><?php echo $tab6name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 7)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '7');" onFocus="easytabs('1', '7');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>7"><?php echo $tab7name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 8)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '8');" onFocus="easytabs('1', '8');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>8"><?php echo $tab8name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 9)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '9');" onFocus="easytabs('1', '9');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>9"><?php echo $tab9name; ?> </li>
<?php } ?>

<?php if ($numbertab >= 10)  { ?>
<li on<?php echo $changetype; ?>="easytabs('1', '10');" onFocus="easytabs('1', '10');" on<?php echo $notselected; ?>="return false;"  title="" id="<?php echo $nameidlinks; ?>10"><?php echo $tab10name; ?></li>
<?php } ?>

</ul>
</div>
<!--End of the menu -->

<?php if ($numbertab >= 1)  { ?>
<!--Start Tabcontent 1 -->
<div id="<?php echo $nameidarea; ?>1"  style="width:<?php echo $width_tab; ?>px;">	
<?php 
//mosLoadModules ( $tab1 , $showtitle ); 

	$modules =& JModuleHelper::getModules($tab1); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
  
?>
</div>

<!--End Tabcontent 1-->
<?php } ?>

<?php if ($numbertab >= 2)  { ?>
<!--Start Tabcontent 2-->
<div id="<?php echo $nameidarea; ?>2"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab2 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab2); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 

?>
</div>
<!--End Tabcontent 2 -->
<?php } ?>

<?php if ($numbertab >= 3)  { ?>
<!--Start Tabcontent 3-->
<div id="<?php echo $nameidarea; ?>3"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab3 , $showtitle ); 
    $modules =& JModuleHelper::getModules($tab3); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 

?>
</div>
<!--End Tabcontent 3 -->
<?php } ?>

<?php if ($numbertab >= 4)   { ?>
<!--Start Tabcontent 4-->
<div id="<?php echo $nameidarea; ?>4" style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab4 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab4); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>

</div>
<!--End Tabcontent 4 -->
<?php } ?>

<?php if ($numbertab >= 5)  { ?>
<!--Start Tabcontent 5-->
<div id="<?php echo $nameidarea; ?>5"  style="width:<?php echo $width_tab; ?>px;">
<?php //mosLoadModules ( $tab5 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab5); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 

?>
</div>
<!--End Tabcontent 5 -->
<?php } ?>

<?php if ($numbertab >= 6)  { ?>
 <!--Start Tabcontent 6-->
<div id="<?php echo $nameidarea; ?>6"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab6 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab6); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>
</div>
<!--End Tabcontent 6 -->
<?php } ?>

<?php if ($numbertab >= 7)  { ?>
 <!--Start Tabcontent 7-->
<div id="<?php echo $nameidarea; ?>7"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab7 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab7); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>
</div>
<!--End Tabcontent 7 -->
<?php } ?>

<?php if ($numbertab >= 8)  { ?>
  <!--Start Tabcontent 8-->
<div id="<?php echo $nameidarea; ?>8"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab8 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab8); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>
</div>
<!--End Tabcontent 8-->
<?php } ?>

<?php if ($numbertab >= 9)  { ?>
  <!--Start Tabcontent 9-->
<div id="<?php echo $nameidarea; ?>9"  style="width:<?php echo $width_tab; ?>px;">
<?php 
//mosLoadModules ( $tab9 , $showtitle ); ]
	$modules =& JModuleHelper::getModules($tab9); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>
</div>
<!--End Tabcontent 9-->
<?php } ?>

<?php if ($numbertab >= 10)  { ?>
  <!--Start Tabcontent 10-->
<div id="<?php echo $nameidarea; ?>10"  style="width:<?php echo $width_tab; ?>px;">
<?php 
	//mosLoadModules ( $tab10 , $showtitle ); 
	$modules =& JModuleHelper::getModules($tab10); 
	foreach ($modules as $module) 
	{ 
		echo JModuleHelper::renderModule($module); 		
	} 
?>
</div>
<!--End Tabcontent 10-->
<?php } ?>