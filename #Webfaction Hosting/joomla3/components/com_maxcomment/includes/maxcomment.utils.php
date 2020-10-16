<?php
/*********************************************
* mXcomment - Component                      *
* Copyright (C) 2007-2008 by Bernard Gilly   *
* --------- All Rights Reserved ------------ *      
* Homepage   : www.visualclinic.fr           *
* Version    : 1.0.9                         *
* License    : Creative Commons              *
*********************************************/

// ensure this file is being included by a parent file
defined( '_VALID_MOS' ) or die( 'Direct Access to this location is not allowed.' );

require( $mosConfig_absolute_path.'/components/com_maxcomment/includes/common/wordwrap.php' );
require( $mosConfig_absolute_path.'/components/com_maxcomment/includes/common/maxcomment.parser.php' );

function _getAnchor() {
	global $COMMENT;
	echo "maxcomment" . $COMMENT->id;
}

eval(base64_decode("ZnVuY3Rpb24gX2dldElwQ29tbWVudCgpIHsNCglnbG9iYWwgJG1vc0NvbmZpZ19hYnNvbHV0ZV9wYXRoLCAkQ09NTUVOVDsNCgkNCglyZXF1aXJlKCRtb3NDb25maWdfYWJzb2x1dGVfcGF0aC4nL2FkbWluaXN0cmF0b3IvY29tcG9uZW50cy9jb21fbWF4Y29tbWVudC9tYXhjb21tZW50X2NvbmZpZy5waHAnKTsNCgkNCglpZiAoICRteGNfc2hvd0lwICkgew0KCQllY2hvICRDT01NRU5ULT5pcDsNCgl9DQoJDQp9DQoNCmZ1bmN0aW9uIF9nZXRVc2VyUmF0aW5nKCkgew0KCWdsb2JhbCAkbW9zQ29uZmlnX2xpdmVfc2l0ZSwgJG1vc0NvbmZpZ19hYnNvbHV0ZV9wYXRoLCAkQ09NTUVOVDsNCgkJDQoJcmVxdWlyZSgkbW9zQ29uZmlnX2Fic29sdXRlX3BhdGguJy9hZG1pbmlzdHJhdG9yL2NvbXBvbmVudHMvY29tX21heGNvbW1lbnQvbWF4Y29tbWVudF9jb25maWcucGhwJyk7DQoJDQoJJHVzZXJSYXRpbmcgPSAiIjsNCgkNCglpZiAoICRteGNfcmF0aW5ndXNlciApIHsNCgkNCgkJaWYgKCAkQ09NTUVOVC0+cmF0aW5nICkgJENPTU1FTlQtPnJhdGluZyA9IGNvbmZpcm1fZXZhbHVhdGUoICRDT01NRU5ULT5yYXRpbmcsICRDT01NRU5ULT5jdXJyZW50bGV2ZWxyYXRpbmcsICRteGNfbGV2ZWxyYXRpbmcgKTsJDQoJDQoJCXN3aXRjaCAoICRteGNfbGV2ZWxyYXRpbmcgKSB7DQoJCQljYXNlICIyMCI6CQkJCQ0KCQkJY2FzZSAiMTAiOg0KCQkJCQkkdXNlclJhdGluZyA9ICRDT01NRU5ULT5yYXRpbmcgLiAiLyIgLiAkbXhjX2xldmVscmF0aW5nOw0KCQkJCQlpZiAoICRDT01NRU5ULT5yYXRpbmcgPT0gMCApICR1c2VyUmF0aW5nID0gJG5vX3JhdGluZzsNCgkJCQlicmVhazsNCgkJCWNhc2UgIjUiOg0KCQkJZGVmYXVsdDoJCQkJDQoJCQkJJHVzZXJSYXRpbmcgPSAiPGltZyBzcmM9JyIgLiAkbW9zQ29uZmlnX2xpdmVfc2l0ZSAuICIvY29tcG9uZW50cy9jb21fbWF4Y29tbWVudC90ZW1wbGF0ZXMvIiAuICRteGNfdGVtcGxhdGUgLiAiL2ltYWdlcy9yYXRpbmcvdXNlcl9yYXRpbmdfIiAuICRDT01NRU5ULT5yYXRpbmcgLiAiLmdpZicgYWxpZ249J21pZGRsZScgYm9yZGVyPScwJyBhbHQ9JycgLz4iOw0KCQl9DQoJCQ0KCX0NCgkNCgllY2hvICR1c2VyUmF0aW5nOw0KDQp9DQoNCmZ1bmN0aW9uIF9nZXRBdXRob3JDb21tZW50KCkgew0KCWdsb2JhbCAkZGF0YWJhc2UsICRtb3NDb25maWdfYWJzb2x1dGVfcGF0aCwgJG15LCAkQ09NTUVOVDsNCgkNCglyZXF1aXJlKCRtb3NDb25maWdfYWJzb2x1dGVfcGF0aC4nL2FkbWluaXN0cmF0b3IvY29tcG9uZW50cy9jb21fbWF4Y29tbWVudC9tYXhjb21tZW50X2NvbmZpZy5waHAnKTsJDQoJDQoJJGNoZWNrQ0Jjb21wb25lbnQgPSBjaGVja0NCY29tcG9uZW50KCk7DQoJaWYoICRDT01NRU5ULT5pZHVzZXIgJiYgJG14Y19MaW5rQ0JQcm9maWxlICYmICRjaGVja0NCY29tcG9uZW50ICl7CQ0KCQkkbGlua2F1dGhvcmNvbW1lbnQgPSBzZWZSZWxUb0FicyggJ2luZGV4LnBocD9vcHRpb249Y29tX2NvbXByb2ZpbGVyJmFtcDt0YXNrPXVzZXJQcm9maWxlJmFtcDt1c2VyPScgLiAkQ09NTUVOVC0+aWR1c2VyIC4gQ0JBdXRob3JJdGVtaWQoKSApOw0KCQkkYXV0aG9yQ29tbWVudCA9ICI8YSBocmVmPSciIC4gJGxpbmthdXRob3Jjb21tZW50IC4gIic+IiAuIHN0cmlwc2xhc2hlcyggJENPTU1FTlQtPm5hbWUgKSAuICI8L2E+IjsNCgl9IGVsc2UgewkNCgkJaWYgKCAkQ09NTUVOVC0+aWR1c2VyICkgew0KCQkJJHRoZW5hbWUgPSAoICRteGNfdXNlX25hbWUgKSA/ICduYW1lJyA6ICd1c2VybmFtZSc7DQoJCQkkcXVlcnkgPSAiU0VMRUNUICR0aGVuYW1lIg0KCQkJLiAiXG4gRlJPTSAjX191c2VycyINCgkJCS4gIlxuIFdIRVJFIGlkID0gJyRDT01NRU5ULT5pZHVzZXInIg0KCQkJOw0KCQkJJGRhdGFiYXNlLT5zZXRRdWVyeSggJHF1ZXJ5ICk7CQ0KCQkJJGF1dGhvckNvbW1lbnQgPSBzdHJpcHNsYXNoZXMoICRkYXRhYmFzZS0+bG9hZFJlc3VsdCgpICk7CQ0KCQl9IGVsc2Ugew0KCQkJJGF1dGhvckNvbW1lbnQgPSBzdHJpcHNsYXNoZXMoICRDT01NRU5ULT5uYW1lICk7DQoJCX0JCQ0KCX0NCgkNCgkvLyBCYWQgd29yZHMNCglpZiAoICRteGNfYmFkd29yZHMgKXsNCgkJJHF1ZXJ5ID0gIlNFTEVDVCAqIEZST00gI19fbXhjX2JhZHdvcmRzIFdIRVJFIHB1Ymxpc2hlZD0nMSciOw0KCQkkZGF0YWJhc2UtPnNldFF1ZXJ5KCAkcXVlcnkgKTsNCgkJJHJvd3NiYWR3b3JkcyA9ICRkYXRhYmFzZS0+bG9hZE9iamVjdExpc3QoKTsNCgkJaWYgKCAkcm93c2JhZHdvcmRzICkgew0KCQkJZm9yZWFjaCAoICRyb3dzYmFkd29yZHMgYXMgJHJvd2JhZHdvcmQgKSB7DQoJCQkJJGJhZHdvcmQgPSB0cmltKCAkcm93YmFkd29yZC0+YmFkd29yZCApOw0KCQkJCSRyZXBsYWNlYmFkd29yZCA9IHN0cl9yZXBlYXQoICcqJywgc3RybGVuKCAkYmFkd29yZCApICk7DQoJCQkJJHJlcGxhY2ViYWR3b3JkID0gIlwkMSIuJHJlcGxhY2ViYWR3b3JkLiJcJDIiOw0KCQkJCSRhdXRob3JDb21tZW50ID0gcHJlZ19yZXBsYWNlKCIvKFxXfF4pJGJhZHdvcmQoXFd8JCkvaSIsICRyZXBsYWNlYmFkd29yZCwgJGF1dGhvckNvbW1lbnQpOw0KCQkJfQkNCgkJfQ0KCX0NCgkNCgllY2hvICRhdXRob3JDb21tZW50Ow0KfQ0KDQpmdW5jdGlvbiBfZ2V0VGl0bGVDb21tZW50KCkgewkNCglnbG9iYWwgJG1vc0NvbmZpZ19saXZlX3NpdGUsICRtb3NDb25maWdfYWJzb2x1dGVfcGF0aCwgJGRhdGFiYXNlLCAkQ09NTUVOVDsJDQoNCglyZXF1aXJlKCRtb3NDb25maWdfYWJzb2x1dGVfcGF0aC4nL2FkbWluaXN0cmF0b3IvY29tcG9uZW50cy9jb21fbWF4Y29tbWVudC9tYXhjb21tZW50X2NvbmZpZy5waHAnKTsNCgkNCglpZiAoICRDT01NRU5ULT50aXRsZSApIHsJDQoJCS8vIEJhZCB3b3Jkcw0KCQlpZiAoICRteGNfYmFkd29yZHMgKXsNCgkJCSRxdWVyeSA9ICJTRUxFQ1QgKiBGUk9NICNfX214Y19iYWR3b3JkcyBXSEVSRSBwdWJsaXNoZWQ9JzEnIjsNCgkJCSRkYXRhYmFzZS0+c2V0UXVlcnkoICRxdWVyeSApOw0KCQkJJHJvd3NiYWR3b3JkcyA9ICRkYXRhYmFzZS0+bG9hZE9iamVjdExpc3QoKTsNCgkJCWlmICggJHJvd3NiYWR3b3JkcyApIHsNCgkJCQlmb3JlYWNoICggJHJvd3NiYWR3b3JkcyBhcyAkcm93YmFkd29yZCApIHsNCgkJCQkJJGJhZHdvcmQgPSB0cmltKCAkcm93YmFkd29yZC0+YmFkd29yZCApOw0KCQkJCQkkcmVwbGFjZWJhZHdvcmQgPSBzdHJfcmVwZWF0KCAnKicsIHN0cmxlbiggJGJhZHdvcmQgKSApOw0KCQkJCQkkcmVwbGFjZWJhZHdvcmQgPSAiXCQxIi4kcmVwbGFjZWJhZHdvcmQuIlwkMiI7DQoJCQkJCSRDT01NRU5ULT50aXRsZSA9IHByZWdfcmVwbGFjZSgiLyhcV3xeKSRiYWR3b3JkKFxXfCQpL2kiLCAkcmVwbGFjZWJhZHdvcmQsICRDT01NRU5ULT50aXRsZSk7DQoJCQkJfQkNCgkJCX0NCgkJfQ0KCQllY2hvIHN0cmlwc2xhc2hlcyggJENPTU1FTlQtPnRpdGxlICk7DQoJCQ0KCX0gZWxzZSBlY2hvICIuLi4iOwkNCn0NCg0KZnVuY3Rpb24gX2dldENvbW1lbnRUZXh0KCkgew0KCWdsb2JhbCAkbW9zQ29uZmlnX2xpdmVfc2l0ZSwgJG1vc0NvbmZpZ19hYnNvbHV0ZV9wYXRoLCAkZGF0YWJhc2UsICRDT01NRU5UOw0KCQ0KCXJlcXVpcmUoJG1vc0NvbmZpZ19hYnNvbHV0ZV9wYXRoLicvYWRtaW5pc3RyYXRvci9jb21wb25lbnRzL2NvbV9tYXhjb21tZW50L21heGNvbW1lbnRfY29uZmlnLnBocCcpOw0KCQ0KCS8vIFByZXBhcmUgc21pbGV5IGFycmF5DQoJJHNtaWxleVsnOiknXSAgICAgPSAic21fc21pbGUuZ2lmIjsgICAgJHNtaWxleVsnOmdyaW4nXSAgPSAic21fYmlnZ3Jpbi5naWYiOw0KCSRzbWlsZXlbJzspJ10gICAgID0gInNtX3dpbmsuZ2lmIjsgICAgICRzbWlsZXlbJzgpJ10gICAgID0gInNtX2Nvb2wuZ2lmIjsNCgkkc21pbGV5Wyc6cCddICAgICA9ICJzbV9yYXp6LmdpZiI7ICAgICAkc21pbGV5Wyc6cm9sbCddICA9ICJzbV9yb2xsZXllcy5naWYiOw0KCSRzbWlsZXlbJzplZWsnXSAgID0gInNtX2JpZ2Vlay5naWYiOyAgICRzbWlsZXlbJzp1cHNldCddID0gInNtX3Vwc2V0LmdpZiI7DQoJJHNtaWxleVsnOnp6eiddICAgPSAic21fc2xlZXAuZ2lmIjsgICAgJHNtaWxleVsnOnNpZ2gnXSAgPSAic21fc2lnaC5naWYiOw0KCSRzbWlsZXlbJzo/J10gICAgID0gInNtX2NvbmZ1c2VkLmdpZiI7ICRzbWlsZXlbJzpjcnknXSAgID0gInNtX2NyeS5naWYiOw0KCSRzbWlsZXlbJzooJ10gICAgID0gInNtX21hZC5naWYiOyAgICAgICRzbWlsZXlbJzp4J10gICAgID0gInNtX2RlYWQuZ2lmIjsNCg0KCSRjb21tZW50VGV4dCA9IHN0cmlwc2xhc2hlcyggJENPTU1FTlQtPmNvbW1lbnQgKTsNCgkvLyBCYWQgd29yZHMNCglpZiAoICRteGNfYmFkd29yZHMgKXsNCgkJJHF1ZXJ5ID0gIlNFTEVDVCAqIEZST00gI19fbXhjX2JhZHdvcmRzIFdIRVJFIHB1Ymxpc2hlZD0nMSciOw0KCQkkZGF0YWJhc2UtPnNldFF1ZXJ5KCAkcXVlcnkgKTsNCgkJJHJvd3NiYWR3b3JkcyA9ICRkYXRhYmFzZS0+bG9hZE9iamVjdExpc3QoKTsNCgkJaWYgKCAkcm93c2JhZHdvcmRzICkgew0KCQkJZm9yZWFjaCAoICRyb3dzYmFkd29yZHMgYXMgJHJvd2JhZHdvcmQgKSB7DQoJCQkJJGJhZHdvcmQgPSB0cmltKCAkcm93YmFkd29yZC0+YmFkd29yZCApOw0KCQkJCSRyZXBsYWNlYmFkd29yZCA9IHN0cl9yZXBlYXQoICcqJywgc3RybGVuKCAkYmFkd29yZCApICk7DQoJCQkJJHJlcGxhY2ViYWR3b3JkID0gIlwkMSIuJHJlcGxhY2ViYWR3b3JkLiJcJDIiOw0KCQkJCSRjb21tZW50VGV4dCA9IHByZWdfcmVwbGFjZSgiLyhcV3xeKSRiYWR3b3JkKFxXfCQpL2kiLCAkcmVwbGFjZWJhZHdvcmQsICRjb21tZW50VGV4dCk7DQoJCQl9CQ0KCQl9DQoJfQ0KCSRjb21tZW50VGV4dCA9IG14Y1BhcnNlKCAkY29tbWVudFRleHQsICRzbWlsZXksICRteGNfYmJjb2Rlc3VwcG9ydCwgJG14Y19waWN0dXJlc3VwcG9ydCwgJG14Y19zbWlsaWVzdXBwb3J0LCAkbW9zQ29uZmlnX2xpdmVfc2l0ZSApOw0KCSRjb21tZW50VGV4dCA9IGh0bWx3cmFwKCAkY29tbWVudFRleHQsICRteGNfbGVuZ3Rod3JhcCApOw0KCQ0KCWVjaG8gJGNvbW1lbnRUZXh0Ow0KfQ0KDQpmdW5jdGlvbiBfZ2V0Tm90aWNlQ29weXJpZ2h0KCl7DQoJZ2xvYmFsICRtb3NDb25maWdfYWJzb2x1dGVfcGF0aCwgJENPTU1FTlQ7DQoJDQoJcmVxdWlyZSgkbW9zQ29uZmlnX2Fic29sdXRlX3BhdGguJy9hZG1pbmlzdHJhdG9yL2NvbXBvbmVudHMvY29tX21heGNvbW1lbnQvdmVyc2lvbi5waHAnKTsNCg0KCSRjb3B5U3RhcnQgPSAyMDA3OyANCgkkY29weU5vdyA9IGRhdGUoJ1knKTsNCglpZiAoJGNvcHlTdGFydCA9PSAkY29weU5vdykgeyANCgkJJGNvcHlTaXRlID0gJGNvcHlTdGFydDsNCgl9IGVsc2Ugew0KCQkkY29weVNpdGUgPSAkY29weVN0YXJ0LiItIi4kY29weU5vdyA7DQoJfQkNCgkNCgkkbm90aWNlQ29weXJpZ2h0ID0gIjxiciAvPjxkaXYgc3R5bGU9XCJjbGVhcjpib3RoO3RleHQtYWxpZ246Y2VudGVyO1wiPjxzcGFuIGNsYXNzPVwic21hbGxcIj48YnIgLz5tWGNvbW1lbnQgIiAuIF9NQVhDT01NRU5UX05VTV9WRVJTSU9OIC4gIiZuYnNwOyZjb3B5OyZuYnNwOyI7DQoJJG5vdGljZUNvcHlyaWdodCAuPSAkY29weVNpdGUgLiAiIC0gPGEgaHJlZj1cImh0dHA6Ly93d3cudmlzdWFsY2xpbmljLmZyXCIgdGFyZ2V0PVwiX2JsYW5rXCI+dmlzdWFsY2xpbmljLmZyPC9hPjxiciAvPiI7DQoJJG5vdGljZUNvcHlyaWdodCAuPSAiTGljZW5zZSA8YSByZWw9XCJsaWNlbnNlXCIgaHJlZj1cImh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL2xpY2Vuc2VzL2J5LW5jLW5kLzMuMC9cIj5DcmVhdGl2ZSBDb21tb25zPC9hPiAtIFNvbWUgcmlnaHRzIHJlc2VydmVkPC9zcGFuPjwvZGl2PiI7DQoJDQoJZWNobyAkbm90aWNlQ29weXJpZ2h0Ow0KfQ0KDQpmdW5jdGlvbiBfZ2V0RGF0ZUNvbW1lbnQoKSB7DQoJZ2xvYmFsICRtb3NDb25maWdfYWJzb2x1dGVfcGF0aCwgJENPTU1FTlQ7CQ0KDQoJcmVxdWlyZSgkbW9zQ29uZmlnX2Fic29sdXRlX3BhdGguJy9hZG1pbmlzdHJhdG9yL2NvbXBvbmVudHMvY29tX21heGNvbW1lbnQvbWF4Y29tbWVudF9jb25maWcucGhwJyk7DQoJJGRhdGVjb21tZW50ID0gIiI7DQoJDQoJaWYgKCBpbnR2YWwoICRDT01NRU5ULT5kYXRlICkgIT0gMCApIHsNCgkJJGRhdGVjb21tZW50ID0gbW9zRm9ybWF0RGF0ZSggJENPTU1FTlQtPmRhdGUsICRteGNfZmRhdGUgKTsNCgl9DQoJZWNobyAkZGF0ZWNvbW1lbnQ7DQp9DQo="));

function _getReportComment() {
	global $mosConfig_live_site, $mosConfig_absolute_path, $mosConfig_lang, $Itemid, $task, $COMMENT, $_MXC, $_VERSION;
	
	$_MXC->CHECKJVERSION = 0;
	if ( $_VERSION->PRODUCT == 'Joomla!' ){	
		if ( $_VERSION->RELEASE >= '1.5' ) {
			$mxc_checkversion = "Joomla!1.5.x";
			$_MXC->CHECKJVERSION = 1;
		}
	}
	
	// Get the right language if it exists
	if (file_exists($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php")){
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php");
	}else{
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/english.php");
	}

	require($mosConfig_absolute_path.'/administrator/components/com_maxcomment/maxcomment_config.php');
	$report = "";
	if( $mxc_report ){
		$link = sefRelToAbs( "index.php?option=com_maxcomment&task=report&id=" . $COMMENT->id . "&cid=" . $COMMENT->contentid . "&Itemid=" . $Itemid );
		
		if ( $_MXC->CHECKJVERSION ) $link = JRoute::_("index.php?option=com_maxcomment&task=report&id=" . $COMMENT->id . "&cid=" . $COMMENT->contentid);		
		
		$report = "<a href=\"" . $link . "\">" . _MXC_REPORTTHISCOMMENT . "</a>";
		if ( $task=='report' ) $report = _MXC_REPORTTHISCOMMENT;
	}	
	echo $report;	
}

function _getReplyComment() {
	global $mainframe, $database, $mosConfig_live_site, $mosConfig_absolute_path, $mosConfig_lang, $Itemid, $task, $COMMENT, $_MXC, $_VERSION;

	$_MXC->CHECKJVERSION = 0;
	if ( $_VERSION->PRODUCT == 'Joomla!' ){	
		if ( $_VERSION->RELEASE >= '1.5' ) {
			$mxc_checkversion = "Joomla!1.5.x";
			$_MXC->CHECKJVERSION = 1;
		}
	}	
	
	// Get the right language if it exists
	if (file_exists($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php")){
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php");
	}else{
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/english.php");
	}

	require($mosConfig_absolute_path.'/administrator/components/com_maxcomment/maxcomment_config.php');	
	
	$width_popup  = ( intval($mxc_width_popup)>0 )? $mxc_width_popup : '420' ;
	$height_popup = ( intval($mxc_height_popup)>0 )? $mxc_height_popup : '450' ;	

	$reply = "";
	if( $mxc_reply ){	
		switch ( $mxc_openingmode ) {		
		
			case 0 :
				$link = sefRelToAbs( "index.php?option=com_maxcomment&task=reply&id=" . $COMMENT->id . "&Itemid=" . $Itemid );
				if ( $_MXC->CHECKJVERSION ) $link = JRoute::_("index.php?option=com_maxcomment&task=reply&id=" . $COMMENT->id);
				$reply = "<a href=\"" . $link . "\">" . _MXC_REPLYTOTHISCOMMENT . "</a>";				
				break;
			case 1 :
				$status = 'status=no,toolbar=no,scrollbars=yes,titlebar=no,menubar=no,resizable=yes,width='.$width_popup.',height='.$height_popup.',directories=no,location=no';
				$link = $mosConfig_live_site . "/index2.php?option=com_maxcomment&amp;task=reply&amp;id=" . $COMMENT->id . "&amp;Itemid=" . $Itemid;
				$reply = "<a href=\"" . $link . "\" target=\"_blank\" onclick=\"window.open('" . $link . "','win2','" . $status . "'); return false;\">" . _MXC_REPLYTOTHISCOMMENT . "</a>";
				break;		
						
		}
	}	
	
	$query = "SELECT introtext FROM #__content"
	. "\n WHERE id='$COMMENT->contentid'";
	$database->setQuery( $query );	
	$result = $database->loadResult();	
	if (strpos($result, "{mxc::closed}") == true ) {
		$_MXC->COMMENTCLOSED = true;
	}	
	// also if bad tag...
	if (strpos($result, "{mxc:closed}") == true ) {
		$_MXC->COMMENTCLOSED = true;
	}
	
	if ( $task=='report' || $task=='viewallreplies' && $COMMENT->parentid > 0 || $_MXC->COMMENTCLOSED==true ) $reply = _MXC_REPLYTOTHISCOMMENT;	
	echo $reply;
		
}

function _getCountAllReplies() {
	global $database, $mosConfig_live_site, $mosConfig_absolute_path, $COMMENT;	
	
	$query = "SELECT COUNT(*) FROM #__mxc_comments"
	. "\n WHERE parentid=$COMMENT->id"
	. "\n AND published='1'"
	;
	$database->setQuery( $query );	
	$totalreplies = $database->loadResult();	
	return $totalreplies;
}

function _getSeeAllReplies() {
	global $database, $mosConfig_live_site, $mosConfig_absolute_path, $mosConfig_lang, $Itemid, $task, $COMMENT, $_MXC, $_VERSION;
	
	$_MXC->CHECKJVERSION = 0;
	if ( $_VERSION->PRODUCT == 'Joomla!' ){	
		if ( $_VERSION->RELEASE >= '1.5' ) {
			$mxc_checkversion = "Joomla!1.5.x";
			$_MXC->CHECKJVERSION = 1;
		}
	}
	
	$totalreplies = _getCountAllReplies( $COMMENT->id );
	//Get the right language if it exists
	if (file_exists($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php")){
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/".$mosConfig_lang.".php");
	}else{
		include_once($mosConfig_absolute_path."/administrator/components/com_maxcomment/languages/english.php");
	}

	require($mosConfig_absolute_path.'/administrator/components/com_maxcomment/maxcomment_config.php');
	
	$seeallreplies = "";	
	$label = sprintf( _MXC_SEEALLREPLIES, $totalreplies );
	if( $mxc_reply && $totalreplies ){
		$link = sefRelToAbs( "index.php?option=com_maxcomment&task=viewallreplies&id=" . $COMMENT->id . "&Itemid=" . $Itemid );
		if ( $_MXC->CHECKJVERSION ) $link = JRoute::_("index.php?option=com_maxcomment&task=viewallreplies&id=" . $COMMENT->id);
		$seeallreplies = "<a href=\"" . $link . "\">" . $label . "</a>";
		if ( $task=='report' || $task=='viewallreplies' ) $seeallreplies = $label;
	}	
	echo $seeallreplies;
	
}

function _getStatusUserComment() {
	global $mosConfig_absolute_path, $COMMENT;	
	
	require($mosConfig_absolute_path.'/administrator/components/com_maxcomment/maxcomment_config.php');
	
	if ( $mxc_showstatus && $COMMENT->iduser ) {
		$status = _MXC_REGISTERED;
	} else if ( $mxc_showstatus && !$COMMENT->iduser ) {
		$status = _MXC_GUEST;
	} else {
		$status = "";
	}

	echo $status;
}

function _getAuthorAvatar() { 
    global $database, $mosConfig_live_site, $mosConfig_absolute_path, $COMMENT;
	
	require($mosConfig_absolute_path.'/administrator/components/com_maxcomment/maxcomment_config.php');

	$avatarCB  = "";
	$avatarIMG = "";
	$avatarIMG2 = "";
	$checkCBcomponent = checkCBcomponent();	
	
	if ( $checkCBcomponent ){
		$database->setQuery( "SELECT avatar FROM #__comprofiler WHERE user_id = '$COMMENT->iduser' AND confirmed = '1' AND approved = '1' AND avatarapproved = '1'" );
		$rowAvatar = $database->loadResult();
		if ( $rowAvatar ) {
			$avatarIMG = "images/comprofiler/tn" . $rowAvatar;		
		} else {
			$avatarIMG = "components/com_comprofiler/plugin/language/default_language/images/tnnophoto.jpg";		
		}
		$avatarIMG2 = $mosConfig_live_site . "/" . $avatarIMG;
	}
	
	$mxc_maxavatarwidth = reducIMG( $mxc_maxavatarwidth, $avatarIMG2 );	
	
	if ( $avatarIMG && $mxc_ShowAvatarCBProfile ){
		$avatarCB = "<img style=\"border:none;padding:4px;width:".$mxc_maxavatarwidth."px\" src='" . $avatarIMG2 . "' align='left' alt='' />";
	}	

	$gravatar = "";
	if ( $mxc_use_gravatar ) {
		$email   = $COMMENT->email ;
		$default = $mosConfig_live_site . "/components/com_maxcomment/images/gravatar/" . $mxc_default_gravatar;
		$size    = ( is_numeric($mxc_maxgravatarwidth) )? $mxc_maxgravatarwidth : 60 ;
		
		$gravatar_url = "http://www.gravatar.com/avatar.php?gravatar_id=";
		$gravatar_url .= md5( $COMMENT->email );
		$gravatar_url .= "&amp;default=" . urlencode($default);		
		$gravatar_url .= "&amp;size=" . $size;
		
		$gravatar = "<img src=\"$gravatar_url\" alt=\"\" />";
	}
	
	if ( $mxc_ShowAvatarCBProfile && $mxc_use_gravatar && $mxc_replaceCBavatar ) {		
		$test = 1;
	} elseif ( $mxc_ShowAvatarCBProfile && $mxc_use_gravatar && !$mxc_replaceCBavatar ) {
		$test = 2;
	}elseif ( $mxc_ShowAvatarCBProfile && !$mxc_use_gravatar ) {
		$test = 3;
	}elseif ( !$mxc_ShowAvatarCBProfile && $mxc_use_gravatar ) {
		$test = 4;
	}elseif ( !$mxc_ShowAvatarCBProfile && !$mxc_use_gravatar ) {
		$test = 5;
	} else $test = 0;
		
	switch( $test ) {
		case 1:
		case 4:
			echo $gravatar;
			break;
		case 2:
		case 3:
			echo $avatarCB;
			break;
		case 0:
		default:
			echo "";
	}
	
}

function _getItemid( &$row ){
	global $mainframe;
	
	if (is_callable( array( $mainframe, "getItemid" ) ) ) {
		$_Itemid = $mainframe->getItemid( $row->id );
	} elseif (is_callable( "JApplicationHelper::getItemid" ) ) {
		$_Itemid = JApplicationHelper::getItemid( $row->id );
	} else {
		$_Itemid = '';
	}

	return $_Itemid;
}

function checkCBcomponent() {
	global $mosConfig_absolute_path;
	
	// Check if CB component exist
	$pathFileCB = $mosConfig_absolute_path . "/components/com_comprofiler/comprofiler.php";		
	if ( file_exists( $pathFileCB ) ) {
		$checkCBcomponent = 1;	
	} else $checkCBcomponent = 0;		
	return $checkCBcomponent;
}

function CBAuthorItemid() {
	global $_CBAuthorbot__Cache_ProfileItemid, $database;
	
	if ( !$_CBAuthorbot__Cache_ProfileItemid ) {
		if ( !isset( $_REQUEST['Itemid'] ) ) {
			$database->setQuery( "SELECT id FROM #__menu WHERE link = 'index.php?option=com_comprofiler' AND published=1" );
			$Itemid = (int) $database->loadResult();
		} else {
			$Itemid = (int) $_REQUEST['Itemid'];
		}
		if ( ! $Itemid ) {
			/** Nope, just use the homepage then. */
			$query = "SELECT id"
			. "\n FROM #__menu"
			. "\n WHERE menutype = 'mainmenu'"
			. "\n AND published = 1"
			. "\n ORDER BY parent, ordering"
			. "\n LIMIT 1"
			;
			$database->setQuery( $query );
			$Itemid = (int) $database->loadResult();
		}
		$_CBAuthorbot__Cache_ProfileItemid = $Itemid;
	}
	if ($_CBAuthorbot__Cache_ProfileItemid) {
		return "&amp;Itemid=" . $_CBAuthorbot__Cache_ProfileItemid;
	} else {
		return null;
	}
}

function reducIMG( $mxc_maxavatarwidth, $image ) {
	$dim       = @getimagesize( $image ); 
	$largeur   = $dim[0];
	
	if  ( $largeur > $mxc_maxavatarwidth ) {	
		$mxc_maxavatarwidth = $mxc_maxavatarwidth;
	} else  $mxc_maxavatarwidth = $largeur;

	return $mxc_maxavatarwidth;
}
?>