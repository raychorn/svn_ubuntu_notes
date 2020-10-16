<?php

// @version $Id: mod_jootabs.php 1.5
// http://www.templateplazza.com
// @based on mod_jootabs.1.0 and EASY TABS 1.2 Produced and Copyright by Koller   Juergen www.kollermedia.at | www.austria-media.at
// @package Joomla
// @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.php
// Compability: Joomla 1.5.X

// JooTabs for Joomla! CMS //
// Created by: Ricardo Sousa and Andy Sikumbang //
// Released by: TemplatePlazza.com  - Best Joomla! Templates //
// Date Release: October 2007 //
// Based on: EASY TABS 1.2 Produced and Copyright by Koller Juergen www.kollermedia.at | www.austria-media.at //
// What's this? A way to easy add tabs to Joomla CMS //
// Copyright: 2007 - TemplatePlazza and AiediLabs (Ricardo Sousa) //
defined( '_JEXEC' ) or die( 'Restricted access' );

$numbertab 		= $params->get( 'numbertab', 2) ;     // Select how many tabs you want to use -- max: 10
$firsttabopen 	= $params->get( '1tabopen', 1);    // select the 1st tab to open... If you want to open the 1st tab just need to put 1
$autochange 	= $params->get( 'autochange', 0) ; // select if you want to autochange the
$changedelay 	= $params->get( 'changedelay', 80) ; // select how many seconds should tabs wait before change automatic
$changestop 	= $params->get( 'changestop', 0) ; // Select if the autochange of tabs should stop if user mouseover any tab
$nameidlinks 	= $params->get( 'nameidlinks', tablink) ; // Individual name for the set of links. If you copy the module YOU SHOULD CHANGE THIS!
$nameidarea 	= $params->get( 'nameidarea', tabcontent) ; // Individual name for the space of tabs. If you copy the module YOU SHOULD CHANGE THIS!
$changetype 	= $params->get( 'changetype', mouseover) ; // Select if you want to select a tab by MouseOver or Click!

$tab_template 	=  $params->get( 'tab_template', 1) ; // template style
$width_tab 		=  $params->get( 'width_tab', 300) ; // tab width
$tab1 			=  $params->get( 'tab1', user1); // Module to use in the 1st Tab
$tab1name 		=  $params->get( 'tab1name', Tab1); // Title of the 1st Tab
$tab2 			=  $params->get( 'tab2', user2); // Module to use in the 2nd Tab
$tab2name 		=  $params->get( 'tab2name', Tab2); // Title of the 2nd Tab
$tab3 			=  $params->get( 'tab3', user1); // Module to use in the 3rd Tab
$tab3name 		=  $params->get( 'tab3name', Tab3); // Title of the 3rd Tab
$tab4 			=  $params->get( 'tab4', user2); // Module to use in the 4th Tab
$tab4name 		=  $params->get( 'tab4name', Tab4); // Title of 4th  Tab
$tab5 			=  $params->get( 'tab5', user1); // Module to use in the 5th Tab
$tab5name 		=  $params->get( 'tab5name', Tab5); // Title of the 5th Tab
$tab6 			=  $params->get( 'tab6', user2); // Module to use in the 6th Tab
$tab6name 		=  $params->get( 'tab6name', Tab6); // Title of 6th Tab
$tab7 			=  $params->get( 'tab7', user1); // Module to use in the 7th Tab
$tab7name 		=  $params->get( 'tab7name', Tab7); // Title of the 7th Tab
$tab8 			=  $params->get( 'tab8', user2); // Module to use in the 8th Tab
$tab8name 		=  $params->get( 'tab8name', Tab8); // Title of 8th Tab
$tab9 			=  $params->get( 'tab9', user1); // Module to use in the 9th Tab
$tab9name 		=  $params->get( 'tab9name', Tab8); // Title of the 9th Tab
$tab10 	   		=  $params->get( 'tab10', user2); // Module to use in the 10th Tab
$tab10name 		=  $params->get( 'tab10name', Tab10); // Title of 10th Tab
$titleshow 		=  $params->get( 'titleshow', 1); // Show or not the titles of the modules inside the tabs


// Include the syndicate functions only once
require( JModuleHelper::getLayoutPath( 'mod_jootabs' ) );




?>

  
