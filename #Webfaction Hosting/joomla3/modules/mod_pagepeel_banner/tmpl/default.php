<?php	
		defined('_JEXEC') or die('Restricted access'); 
		//Get All Parameters
		$clientids = $params->get( 'banner_cids', '5' );
		$adsystem  = $params->get( 'adsystem', '1' );
		// peel setting
		$peelspeed = $params->get( 'peelspeed', '4' );
		$peelmirror = $params->get( 'peelmirror', '1' );
		$peelnomirrorclr = $params->get( 'peelnomirrorclr', 'FFFFFF' );
		$peellinktarget = $params->get( 'peellinktarget', 'self' );		
		$peelautoopen = $params->get( 'peelautoopen', 'false' );
		$peelcloseautoopen = $params->get( 'peelcloseautoopen', '5' );
		$peeldirection = $params->get( 'peeldirection', 'rt' );
		$alterlink = $params->get( 'alterlink', 'http://www.templateplazza.com' );
		$alterimage = $params->get( 'alterimage', 'animated_ads.jpg' );
		$peelsmallimage = $params->get( 'peelsmallimage', 'clickhere.jpg' );
		$peelsmallwidth = $params->get( 'peelsmallwidth', '100' );
		$peelbigwidth = $params->get( 'peelbigwidth', '500' );
		
		//add by remush
		$peelautoopenable = $params->get( 'peelautoopenable', 'disable' );
		$peelautoopenbehaviour = $params->get( 'peelautoopenbehaviour', 'reload' );

		
		
$database = & JFactory::getDBO();
$banner   = null;

$count_banner = count($list);
$baseurl = JURI::base();

if($adsystem ==1){ // if Static Ads
	$imageurl 	= 'modules/mod_pagepeel_banner/pagepeel_banner/'."$alterimage".'';
	$link		= $alterlink;
}
else{ //if Joomla Ads
if($count_banner > 0){
	foreach($list as $item) :
		$link = JRoute::_( 'index.php?option=com_banners&task=click&bid='. $item->bid );
		$imageurl 	= 'modules/mod_pagepeel_banner/pagepeel_banner/'."$alterimage".'';

		if (BannerHelper::isImage( $item->imageurl ))
		{
			
			$imageurl 	= $baseurl.'images/banners/'.$item->imageurl;
			
		}
		else if (BannerHelper::isFlash( $item->imageurl ))
		{
			$imageurl = $baseurl."images/banners/".$item->imageurl;
		}

	endforeach;
}else{	
	$imageurl 	= 'modules/mod_pagepeel_banner/pagepeel_banner/'."$alterimage".'';
	$link		= $alterlink;
}
}

?>

<script src="modules/mod_pagepeel_banner/pagepeel_banner/AC_OETags.js"language="javascript"></script>  
<script type="text/javascript">
/********************************************************************************************
* PageEar advertising CornerAd by Webpicasso Media
* Leave copyright notice.  
*
* @copyright www.webpicasso.de
* @author    christian harz <pagepeel-at-webpicasso.de>
*********************************************************************************************/
/*
 *  Konfiguration / Configuration
 */ 

//  URL to small image 
var pagearSmallImg = 'modules/mod_pagepeel_banner/pagepeel_banner/<?php echo $peelsmallimage; ?>'; 
// URL to small pageear swf
var pagearSmallSwf = 'modules/mod_pagepeel_banner/pagepeel_banner/pageear_s.swf'; 

// URL to big image
var pagearBigImg = '<?php echo $imageurl;?>'; 
// URL to big pageear swf
var pagearBigSwf = 'modules/mod_pagepeel_banner/pagepeel_banner/pageear_b.swf'; 

// Movement speed of small pageear 1-4 (2=Standard)
var speedSmall = <?php echo $peelspeed; ?>; 
// Mirror image ( true | false )
var mirror = '<?php if ($peelmirror == 0) { echo "false"; } elseif ($peelmirror == 1) { echo "true"; }  ?>'; 

// Color of pagecorner if mirror is false
var pageearColor = '<?php echo $peelnomirrorclr; ?>';  

// URL to open on pageear click
var jumpTo = '<?php echo $link; ?>' ;

// Browser target  (new) or self (self)
var openLink = '<?php echo $peellinktarget; ?>'; 

// Opens pageear automaticly (false:deactivated | 0.1 - X seconds to open) 
var openOnLoad = <?php echo $peelautoopen; ?>; 

// Second until pageear close after openOnLoad
var closeOnLoad = <?php echo $peelcloseautoopen; ?>; 

// Set direction of pageear in left or right top browser corner (lt: left | rt: right )
setDirection = '<?php echo $peeldirection; ?>';
 

/*
 *  Do not change anything after this line
 */ 

// Flash check vars
var requiredMajorVersion = 6;
var requiredMinorVersion = 0;
var requiredRevision = 0;

// Copyright
var copyright = 'Webpicasso Media, www.webpicasso.de';

// Size small peel 
var thumbWidth  = <?php echo $peelsmallwidth; ?>;
var thumbHeight = <?php echo $peelsmallwidth; ?>;

// Size big peel
var bigWidth  = <?php echo $peelbigwidth; ?>;
var bigHeight = <?php echo $peelbigwidth; ?>;

// Css style default x-position
var xPos = 'right';


//add by remush
var autoopen = '<?php echo $peelautoopenable; ?>';
var behaviour = '<?php echo $peelautoopenbehaviour; ?>';

// GET - Params
var queryParams = 'pagearSmallImg='+escape(pagearSmallImg); 
queryParams += '&pagearBigImg='+escape(pagearBigImg); 
queryParams += '&pageearColor='+pageearColor; 
queryParams += '&jumpTo='+escape(jumpTo); 
queryParams += '&openLink='+escape(openLink); 
queryParams += '&mirror='+escape(mirror); 
queryParams += '&copyright='+escape(copyright); 
queryParams += '&speedSmall='+escape(speedSmall); 
queryParams += '&openOnLoad='+escape(openOnLoad); 
queryParams += '&closeOnLoad='+escape(closeOnLoad); 
queryParams += '&setDirection='+escape(setDirection); 


function openPeel(){
	//document.getElementById('teks').value = 'open'; 
	document.getElementById('bigDiv').style.top = '0px'; 
	document.getElementById('bigDiv').style[xPos] = '0px';
	document.getElementById('thumbDiv').style.top = '-1000px';
		
}

function closePeel(){
	//document.getElementById('teks').value = 'close'; 
	document.getElementById("thumbDiv").style.top = "0px";
	document.getElementById("bigDiv").style.top = "-1000px";
	//setTimeout("openPeel()",3000);
}

function autoPeel(){
	openPeel();
}


function writeObjects () { 
    
    // Get installed flashversion
    var hasReqestedVersion = DetectFlashVer(requiredMajorVersion, requiredMinorVersion, requiredRevision);
    
    // Check direction 
    if(setDirection == 'lt') {
        xPosBig = 'left:-1000px';  
        xPos = 'left';      
    } else {
        xPosBig = 'right:1000px';
        xPos = 'right';              
    }
    
	
    // Write div layer for big swf
    document.write('<div id="bigDiv" style="position:absolute;width:'+ bigWidth +'px;height:'+ bigHeight +'px;z-index:9999;'+xPosBig+';top:-1000px;">');    	
    
    // Check if flash exists/ version matched
    if (hasReqestedVersion) {    	
    	AC_FL_RunContent(
    				"src", pagearBigSwf+'?'+ queryParams,
    				"width", bigWidth,
    				"height", bigHeight,
    				"align", "middle",
    				"id", "bigSwf",
    				"quality", "high",
    				"bgcolor", "#FFFFFF",
    				"name", "bigSwf",
    				"wmode", "transparent",
    				"allowScriptAccess","always",
    				"type", "application/x-shockwave-flash",
    				'codebase', 'http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab',
    				"pluginspage", "http://www.adobe.com/go/getflashplayer"
    	);
    } else {  // otherwise do nothing or write message ...    	 
    	document.write('no flash installed');  // non-flash content
    } 
    // Close div layer for big swf
    document.write('</div>'); 
    
    // Write div layer for small swf
    document.write('<div id="thumbDiv" style="position:absolute;width:'+ thumbWidth +'px;height:'+ thumbHeight +'px;z-index:9999;'+xPos+':0px;top:0px;">');
    
    // Check if flash exists/ version matched
    if (hasReqestedVersion) {    	
    	AC_FL_RunContent(
    				"src", pagearSmallSwf+'?'+ queryParams,
    				"width", thumbWidth,
    				"height", thumbHeight,
    				"align", "middle",
    				"id", "bigSwf",
    				"quality", "high",
    				"bgcolor", "#FFFFFF",
    				"name", "bigSwf",
    				"wmode", "transparent",
    				"allowScriptAccess","always",
    				"type", "application/x-shockwave-flash",
    				'codebase', 'http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab',
    				"pluginspage", "http://www.adobe.com/go/getflashplayer"
    	);
    } else {  // otherwise do nothing or write message ...    	 
    	document.write('no flash installed');  // non-flash content
    } 
    document.write('</div>');  
   
}


//added by REMUSH
//for Set cookies
<?php if($peelautoopenable == "enable"){?>
function createCookie(name,value,hours) {
  if (hours) {
    var date = new Date();
    date.setTime(date.getTime()+(hours*60*60));
    var expires = "; expires="+date.toGMTString();
  }
  else expires = "";
  document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
  var nameEQ = name + "=";
  var ca = document.cookie.split(';');
  for(var i=0;i < ca.length;i++) {
    var c = ca[i];
    while (c.charAt(0)==' ') c = c.substring(1,c.length);
    if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
  }
  return null;
}
//end added by REMUSH
<?php } ?>

</script>
<!-- PagePeel Banner - http://www.templateplazza.com -->
<script type="text/javascript">    
    
	<?php 
if($peelautoopenable == "enable"){?>
    writeObjects();	
	if(autoopen == "enable"){			
		if(behaviour == "once"){
			//if you choose behaviour once, you must enable cookies on your browser
			cookie = readCookie("auto_open_pagepeel");
			//if not IE4+ nor NS6+			
			if (typeof navigator.cookieEnabled=="undefined" && !cookieEnabled){ 
				document.cookie="testcookie";
				cookieEnabled=(document.cookie.indexOf("testcookie")!=-1)? true : false ;
			}
			if (cookie != 1 && cookieEnabled ) {				
				openPeel();	
				createCookie("auto_open_pagepeel", "1");			 
			}  
		}
		else{
			openPeel();				
		}
	
		
	}
<?php 
} else{?>
writeObjects();	
<?php 
}
?>
</script>
