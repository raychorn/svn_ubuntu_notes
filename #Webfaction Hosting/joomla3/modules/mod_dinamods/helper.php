<?php
/**
* @version		$Id: helper.php 2008 vargas $
* @package		Joomla
* @copyright	Copyright (C) 2008 Open Source Matters. All rights reserved.
* @license		GNU/GPL, see LICENSE.php
*/

// no direct access
defined('_JEXEC') or die('Restricted access');

class modDinamodsHelper {

	function addScripts ( &$params, $dinamods_id ) {
	
	if ( $params->get('write_css', 1) ) :

		jimport('joomla.filesystem.file');
	
		JFile::write(dirname(__FILE__).DS.'scripts'.DS.'dinamods_'.$dinamods_id.'.css', modDinamodsHelper::buildCSS( $params, $dinamods_id ) );
		
	endif;

	if ( $params->get('call_css', 1) ) :

		JHTML::stylesheet('dinamods_'.$dinamods_id.'.css','modules/mod_dinamods/scripts/',false);
	
	endif;

	JHTML::script('dinamods.js','modules/mod_dinamods/scripts/',false );
	
	return true;
	
	}

	function buildCSS ( &$params,$dinamods_id ) {

		$tpos     = $params->get('tabs_pos', 0);
		$width    = $params->get('width', 'auto');
		$heigh    = $params->get( 'height', 'auto');
		$border   = $params->get('border', '1px solid #CCCCCC');
		$bgcolor  = $params->get('bgcolor', '#FFFFFF');
		$padding  = $params->get('padding', '5px');
		$trmarg   = $params->get('tab_margin_right', 0);
		$tlmarg	  = $params->get('tab_margin_left', 0);
		$tbgcol	  = $params->get('tab_bgcolor', '#F6F6F6');
		$tfont    = $params->get('tab_font', 'bold 13px Arial');
		$tfcol    = $params->get('tab_font_color', '#000000');
		$tpad	  = $params->get('tab_padding', '5px 11px 5px 11px');
		$stbgcol  = $params->get('sel_tab_bgcolor', '#FFFFFF');
		$stfcol	  = $params->get('sel_tab_font_color', '#135CAE');
		$htbgcol  = $params->get('hover_tab_bgcolor', '#FFFFFF');
		$htfcol   = $params->get('hover_tab_font_color', '#135CAE');

		$style = '/** Dinamod #'.$dinamods_id.' css **/
#dm_tabs_'.$dinamods_id.' ul.dm_menu_'.$dinamods_id.' {
	width:'.$width.';
	margin:0;
	padding:0;
	list-style:none;
}

#dm_tabs_'.$dinamods_id.' ul.dm_menu_'.$dinamods_id.' li.dm_menu_item_'.$dinamods_id.' {
	display: inline;
	margin:0 '.$trmarg.' 0 '.$tlmarg.';
	padding:0;
	float:left;
	border:'.$border.';
	border-'.($tpos == 0 ? 'bottom' : 'top').':none;
	background-color:'.$tbgcol.';
	background-image:none;
}

#dm_tabs_'.$dinamods_id.' ul.dm_menu_'.$dinamods_id.' li.dm_menu_item_'.$dinamods_id.' a {
	font:'.$tfont.';
	float:left;
	border:none;
	color:'.$tfcol.';
	padding:'.$tpad.';
	text-decoration:none;
}

#dm_tabs_'.$dinamods_id.' ul.dm_menu_'.$dinamods_id.' li.dm_menu_item_'.$dinamods_id.' a.dm_selected {
	position:relative;
	'.($tpos == 0 ? 'top' : 'bottom').':'.(int)$border.'px;
	background-color:'.$stbgcol.';
	color:'.$stfcol.';
}

#dm_tabs_'.$dinamods_id.' ul.dm_menu_'.$dinamods_id.' li.dm_menu_item_'.$dinamods_id.' a:hover {
	background-color:'.$htbgcol.';
	color:'.$htfcol.';
}

#dm_container_'.$dinamods_id.' {
    border:'.$border.';
	width:'.$width.';
	height:'.$heigh.';
	background-color:'.$bgcolor.';
	padding:'.$padding.';
	overflow:hidden;
}

#dm_container_'.$dinamods_id.' .dm_tabcontent {
	display:none;
}

@media print {
	#dm_container_'.$dinamods_id.' .dm_tabcontent {
	display:block !important;
	}
}';

		return $style;

	}
	
}