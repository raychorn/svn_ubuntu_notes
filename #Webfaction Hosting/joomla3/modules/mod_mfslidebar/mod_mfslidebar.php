<?php
/**
* mod_mfslidebar.php ,v 1.0
* @copyright (C) 2008 Marcofolio.net
* http://www.marcofolio.net/
*
* Joomla 1.5.x Module
* MF SlideBar allows you to display another module
*  in a fancy sliding sidebar
*/

/** ensure this file is being included by a parent file */
defined( '_JEXEC' ) or die( 'Direct Access to this location is not allowed.' );

	/**
	* Initalize the variables
	**/
	
	// Load the parameters
	$loadmodule = $params->def( 'loadmodule', 'user3' );
	$pos = $params->def( 'pos', 'left' );
	$toppos = $params->def( 'toppos', '140' );
	$theme = $params->def( 'theme', 'dark' );
	$concolor = $params->def( 'concolor', '0E1C2F' );
	$conwidth = $params->def( 'conwidth', '200' );
	$conheight = $params->def( 'conheight', '320' );
	$conpadding = $params->def( 'conpadding', '10' );
	$imgwidth = $params->def( 'imgwidth', '0' );
	$imgheight = $params->def( 'imgheight', '137' );
	$imgalt = $params->def( 'imgalt', 'mfSlideBar' );
	
	// Show the Slider
	?>
	<!-- Start mfSlideBar - Joomla! Module by Marcofolio.net -->
	<script type="text/javascript" src="modules/mod_mfslidebar/prototype.js"></script>
	<script type="text/javascript" src="modules/mod_mfslidebar/effects.js"></script>
	<script type="text/javascript" src="modules/mod_mfslidebar/mfslidebar.js"></script>
	<div style="text-align:left;position: absolute;width: auto;height: auto;top: <?php echo($toppos); ?>px;<?php echo($pos); ?>:0px;background-color:#<?php echo($concolor); ?>" id="mfslideBar">
	<div style="display:none;float:<?php echo($pos); ?>;overflow:hidden !important;width:<?php echo($conwidth); ?>px;height:<?php echo($conheight); ?>px;" id="mfslideBarContents">
		<div style="width:<?php echo($conwidth); ?>px;padding:<?php echo($conpadding); ?>px;" id="mfslideBarContentsInner">
	        <jdoc:include type="modules" name="<?php echo($loadmodule); ?>" />
		</div>
	</div>
	<a href="#" style="float:<?php echo($pos); ?>;height:<?php echo($imgheight); ?>px;width:<?php echo($imgwidth); ?>px;" id="mfslideBarTab"><img src="modules/mod_mfslidebar/templates/<?php echo($theme); ?>-<?php echo($pos); ?>/slide-button.gif" alt="<?php echo($imgalt);?>" title="<?php echo($imgalt);?>" border="0" /></a>
	</div>
	<!-- End mfSlideBar - Joomla! Module by Marcofolio.net -->
<?php

?>