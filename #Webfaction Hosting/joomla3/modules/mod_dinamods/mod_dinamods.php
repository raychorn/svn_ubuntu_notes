<?php
/**
* @version		$Id: mod_dinamods.php 2008 vargas $
* @package		Joomla
* @copyright	Copyright (C) 2005 - 2007 Open Source Matters. All rights reserved.
* @license		GNU/GPL, see LICENSE.php
* Joomla! is free software. This version may have been modified pursuant
* to the GNU General Public License, and as distributed it includes or
* is derivative of works licensed under the GNU General Public License or
* other free or open source software licenses.
* See COPYRIGHT.php for copyright notices and details.
*/

// no direct access
defined('_JEXEC') or die('Restricted access');

// Include the syndicate functions only once
require_once (dirname(__FILE__).DS.'helper.php');

$position = trim( $params->get('position', 'dinamod') );

$dinamods = JModuleHelper::getModules( $position );

if ( !$dinamods ) :  return; endif;

global $dinamods_id;

if ( !$dinamods_id ) : $dinamods_id = 1; endif;

$tabs_pos  = $params->get('tabs_pos', 0);

$speed = 0;

if ( $params->get('slider', 1) == 1 ) : $speed = $params->get('speed', 3000 ); endif;

modDinamodsHelper::addScripts( $params, $dinamods_id );

require( JModuleHelper::getLayoutPath('mod_dinamods') );

$dinamods_id++;