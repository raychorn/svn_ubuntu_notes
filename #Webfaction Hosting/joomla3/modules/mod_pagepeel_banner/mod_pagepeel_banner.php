<?php 

// @version $Id: mod_pagepeel_banner.php 1.2
// Reni Mustika
// http://www.templateplazza.com
// @based on www.webpicasso.de pageear script and mod_banner.php

// @version $Id: mod_banners.php 6087 2006-12-24 18:59:57Z robs $
// @package Joomla
// @copyright Copyright (C) 2005 Open Source Matters. All rights reserved.
// @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.php

// no direct access
defined( '_JEXEC' ) or die( 'Restricted access' );
// Include the syndicate functions only once
require_once( dirname(__FILE__).DS.'helper.php' );
$list = modPeelBannersHelper::getList($params);
require( JModuleHelper::getLayoutPath( 'mod_pagepeel_banner' ));

?>


