<?php
/**
*
* @package UltimateSocialBookmarkingPlugin
* @copyright (C)2008 JoomlaDigger.com
* @license GNU/GPL
* http://joomladigger.com
*
**/
// no direct access
defined( '_JEXEC' ) or die( 'Restricted access' );

// Import library dependencies
jimport('joomla.event.plugin');

class plgContentultimate_social_bookmarking_plugin extends JPlugin
{
    function plgContentultimate_social_bookmarking_plugin( &$subject )
    {
		parent::__construct( $subject );

		// load plugin parameters
		$this->_plugin = JPluginHelper::getPlugin( 'content', 'ultimate_social_bookmarking_plugin' );
		$this->_params = new JParameter( $this->_plugin->params );
    }
	
    function onPrepareContent(&$article, &$params, $page)
    {
		$mosConfig_absolute_path = JPATH_SITE;
		
		// Get plugin variables
		$homepageListing = $this->_params->def('homepageListing','top');
		$urlMode = $this->_params->get( 'urlMode', 'standard' );
		$excludeCategories = $this->_params->def( 'excludeCategories', '' );
		$topPrecedingHtml = $this->_params->def( 'topPrecedingHtml', ' ' );
		$topFollowingHtml = $this->_params->def( 'topFollowingHtml', ' ' );
		$bottomPrecedingHtml = $this->_params->def( 'bottomPrecedingHtml', '<br/><hr/>Add this page to your favorite Social Bookmarking websites<br/>' );
		$bottomFollowingHtml = $this->_params->def( 'bottomFollowingHtml', ' ' );
		$topSeparator = $this->_params->def( 'topSeparator', '<br/>' );
		$bottomSeparator = $this->_params->def( 'bottomSeparator', ' ' );
		$imageFolder = $this->_params->def( 'imageFolder', 'default' );
		$lookForManualSettings = $this->_params->def( 'lookForManualSettings', 'no' );
		$bgColor = $this->_params->def( 'bgColor', '#ffffff' );
		$addThisPubId = $this->_params->def( 'addThisPubId', '' );
		$validArticle = true;
		
		// Check if the full content item is currently displayed
		if (isset($article->alias))
		{
			$isHomepage = false;
		}
		else
		{
			$isHomepage = true;
		}
		
		// Invalidate if on homepage and currentUrls enabled
		if (($isHomepage == true) and ($urlMode == 'currentUrl')) $validArticle = false;
		
		// Get base URL
		$baseUrl = JURI::base();
		if ($baseUrl[strlen($baseUrl)-1] == '/')
		{
			$baseUrl = substr($baseUrl, 0, strlen($baseUrl) - 1);
		}

		// Add stylesheet to header
		JFactory::getDocument()->addStyleSheet($baseUrl .'/plugins/content/ultimate_social_bookmarking_plugin.css');
		
		// Set urlMode to currentUrl if SEF URLs are not enabled for this website
		//if ($mosConfig_sef == '0') $urlMode = 'currentUrl';
		
		// Check if this content item is in an excluded category
		$excluded = explode (",", $excludeCategories);
		foreach ($excluded as $ex)
		{
			if ($article->catid == trim($ex))
			{
				$validArticle = false;
			}
		}

		// Run the plugin if not excluded and content type is valid
		if ($validArticle == true)
		{
			// Setup image directory
			$imageLocation = $baseUrl .'/plugins/content/usbp_images/'. $imageFolder;
			
			if (($lookForManualSettings == 'yes') and (preg_match('#{socialbookmarker}(.*?){/socialbookmarker}#s', $article->text, $matches, PREG_OFFSET_CAPTURE)))
			{
				// Get $thisurl
				if (preg_match('#sburl=#s', $matches[0][0]))
				{
					$pos1 = stripos($matches[0][0], 'sburl=&quot;') + 12;
					$pos2 = stripos($matches[0][0], '&quot;', $pos1);
					$thisurl = substr($matches[0][0], $pos1, $pos2 - $pos1);
				}
				else
				{
					$thisurl = $this->usbp_createUrl($article, $urlMode, $baseUrl);
				}				
				// Get $thistitle
				if (preg_match('#sbtitle=#s', $matches[0][0]))
				{
					$pos1 = stripos($matches[0][0], 'sbtitle=&quot;') + 14;
					$pos2 = stripos($matches[0][0], '&quot;', $pos1);
					$thistitle = substr($matches[0][0], $pos1, $pos2 - $pos1);
				}
				else
				{
					$thistitle = $article->title;
				}
				// Get $thisdesc
				if (preg_match('#sbdescription=#s', $matches[0][0]))
				{
					$pos1 = stripos($matches[0][0], 'sbdescription=&quot;') + 20;
					$pos2 = stripos($matches[0][0], '&quot;', $pos1);
					$thisdesc = substr($matches[0][0], $pos1, $pos2 - $pos1);
				}
				else
				{
					$thisdesc = $article->metadesc;
				}
				// Get $enabled
				if (preg_match('#sbenabled=#s', $matches[0][0]))
				{
					$pos1 = stripos($matches[0][0], 'sbenabled=&quot;') + 16;
					$pos2 = stripos($matches[0][0], '&quot;', $pos1);
					$enabled = substr($matches[0][0], $pos1, $pos2 - $pos1);
					if ($enabled == 'true')
					{
						$enabled = true;
					}
					else
					{
						$enabled = false;
					}
				}
				else
				{
					$enabled = true;
				}	

			}
			else
			{
				$thisurl = $this->usbp_createUrl($article, $urlMode, $baseUrl);
				$thistitle = $article->title;
				$thisdesc = $article->metadesc;
				$enabled = true;
			}
			
			// Run plugin if this page was not manually set to disabled
			if ($enabled == true)
			{
				// Initialize button arrays
				$htmls = array();
				$positions = array();
				
				// Process buttons.xml, create arrays for html and position
				$xmlFileName = $mosConfig_absolute_path .'/plugins/content/ultimate_social_bookmarking_plugin_buttons.xml';
				if (file_exists($xmlFileName) == true)
				{
					// Parse XML
					$button_xml = simplexml_load_file($xmlFileName);

					// For each <button> node, get html and position if enabled
					$i = 0;
					foreach ($button_xml->button as $button)
					{
						if (trim($button->enabled) == 'true')
						{
							$htmls[$i] = trim($button->html);
							$positions[$i] = trim($button->position);
							$i++;
						}
					}
					
					// Build html for top and bottom button groups
					$topHtml = '';
					$bottomHtml = '';
					
					for ($i=0; $i<sizeof($htmls); $i++)
					{
						if ($positions[$i] == 'top')
						{
							$topHtml .= $htmls[$i];
							if ($i < sizeof($htmls)-1)
							{
								$topHtml .= $topSeparator;
							}
						}
						else
						{
							$bottomHtml .= $htmls[$i];
							if ($i < sizeof($htmls)-1)
							{
								$bottomHtml .= $bottomSeparator;
							}
						}
					}
					if ($bottomHtml != '')
					{				
						$bottomHtml = $bottomHtml . $bottomSeparator .'<a href="http://joomladigger.com/" title="Add any social bookmarking button to your blog."><img height="18px" width="18px" src="***imageDirectory***/joomladigger.png" alt="Free social bookmarking plugins and extensions for Joomla! websites!" title="Boost your website traffic with free social bookmarking scripts!" /></a>';
					}
					
					// Replace keystrings in top and bottom html
					$topHtml = $this->usbp_replaceParams($topHtml, $thisurl, $thistitle, $thisdesc, $imageLocation, $bgColor, $addThisPubId);
					$bottomHtml = $this->usbp_replaceParams($bottomHtml, $thisurl, $thistitle, $thisdesc, $imageLocation, $bgColor, $addThisPubId);
					
					// Add top and bottom html to content
					if (($isHomepage ==  true) or ($homepageListing == 'top') or ($homepageListing == 'both'))
					{
						$article->text = '<div class="ultimatesbplugin_top">'. $topPrecedingHtml . $topHtml . $topFollowingHtml .'</div>'. $article->text;
					}
					
					// If full content is displayed
					if (($isHomepage ==  false) or ($homepageListing == 'bottom') or ($homepageListing == 'both'))
					{
						 $article->text .= '<div class="ultimatesbplugin_bottom">'. $bottomPrecedingHtml . $bottomHtml . $bottomFollowingHtml .'</div>';
					}
				}
				else
				{
					// Debug
					echo 'Could not find '. $xmlFileName;
				}
			}
		}
		
		// Remove code from HTML
		$article->text = preg_replace('#{socialbookmarker}(.*?){/socialbookmarker}#s', '', $article->text);

        return true;
    }
	function usbp_createUrl($article, $urlMode, $baseUrl)
	{
		switch($urlMode)
		{
			case 'standard':
				//$link = JRoute::_('index.php?option=com_content&view=article&id='. $article->slug .'&catid='. $article->catslug .'&Itemid='. JApplication::getItemid($article->id));
				$link = JRoute::_(ContentHelperRoute::getArticleRoute($article->slug, $article->catslug, $article->sectionid));
				if (substr($link, 0, 4) != 'http:') $link = $baseUrl . $link;
				break;
			case 'currentUrl':
				$link = "' + location.href + '";
				break;
			default:
				//$link = JRoute::_('index.php?option=com_content&view=article&id='. $article->slug .'&catid='. $article->catslug .'&Itemid='. JApplication::getItemid($article->id));
				$link = JRoute::_(ContentHelperRoute::getArticleRoute($article->slug, $article->catslug, $article->sectionid));
				if (substr($link, 0, 4) != 'http:') $link = $baseUrl . $link;
				break;
		}
			
		return $link;
	}
	function usbp_replaceParams($text, $thisurl, $thistitle, $thisdesc, $imageLocation, $bgColor, $addThisPubId)
	{
		$text = str_replace('***url***', str_replace("'", "", $thisurl), $text);
		$text = str_replace('***url_encoded***', "' + encodeURIComponent('". str_replace("'", "", $thisurl) ."') + '", $text);
		$text = str_replace('***title***', str_replace("'", "", $thistitle), $text);
		$text = str_replace('***title_encoded***', "' + encodeURIComponent('". str_replace("'", "", $thistitle) ."') + '", $text);
		$text = str_replace('***description***', str_replace("'", "", $thisdesc), $text);
		$text = str_replace('***description_encoded***', "' + encodeURIComponent('". str_replace("'", "", $thisdesc) ."') + '", $text);
		$text = str_replace('***imageDirectory***', $imageLocation, $text);
		$text = str_replace('***bgcolor***', $bgColor, $text);
		$text = str_replace('***addThisPubId***', $addThisPubId, $text);
		
		return $text;
	}
}
?>