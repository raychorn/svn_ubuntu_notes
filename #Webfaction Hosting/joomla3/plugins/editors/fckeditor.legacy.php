<?php
/**
* @version $Id: fckeditor.php, v 2.5.0 10:35 GMT 20.12.2007
* based on Angel Franco Calixto's fckeditor mambot
* further developed by Webxsolution Ltd
*
* Licence
* -------
* WebxSolution Ltd's work is licenced under a Creative Commons Licence. 
* 
* You are free:
* to copy, distribute, display, and perform the work
* 
* Under the following conditions:
* Attribution. You must give the original author credit. 
* No Derivative Works. You may not alter, transform, or build upon this.
* @package Joomla
**/

defined( '_VALID_MOS' ) or die( 'Direct Access to this location is not allowed.' );

$_MAMBOTS->registerFunction( 'onInitEditor', 'botInitEditor' );
$_MAMBOTS->registerFunction( 'onEditorArea', 'botEditorArea' );
$_MAMBOTS->registerFunction( 'onGetEditorContents', 'botGetEditorContents' );

function botInitEditor() {
	global $mosConfig_live_site;
return <<<EOD
<script type="text/javascript" src="$mosConfig_live_site/mambots/editors/fckeditor/fckeditor.js">
</script>
EOD;
}

function botEditorArea( $name, $content, $hiddenField, $width, $height, $col, $row ) {
	global $my, $mosConfig_live_site, $database, $option, $_MAMBOTS, $mosConfig_absolute_path;

	$gid = 20;

	$content = str_replace("&lt;", "<", $content);
	$content = str_replace("&gt;", ">", $content);
	$content = str_replace("&amp;", "&", $content);
	$content = str_replace("&nbsp;", " ", $content);
	$content = str_replace("&quot;", "\"", $content);
	
	$mainframe = new mosMainFrame( $database, $option, '.' );
	 
	$query = "SELECT id FROM #__mambots WHERE element = 'legacy.fckeditor' AND folder = 'editors'";
	$database->setQuery( $query );
	$id = $database->loadResult();
	$mambot = new mosMambot( $database );
	$mambot->load( $id );
	$params =& new mosParameters( $mambot->params );
	
	
	$toolbar = $params->get( 'toolbar', 'Advanced' );
	$toolbar_ft = $params->get( 'toolbar_ft', 'Advanced' );
	$content_css = $params->get( 'content_css', '0' );
	$editor_css = $params->get( 'editor_css', '0' );
	$content_css_custom = $params->get( 'content_css_custom', '' );
	$text_direction	= $params->get( 'text_direction', 'ltr' );
	$newlines = $params->get( 'newlines', 'false' );
	$skin = $params->get( 'skin', 'office2007' );
	$image_path = $params->get( 'imagePath', '/images/stories/' );
	$wwidth = $params->get( 'wwidth', '750' );
	$hheight = $params->get( 'hheight', '480' );
	$formatSource = $params->get('FormatSource',1);
	$add_stylesheet_path = $params->get('add_stylesheet_path','');
	$add_stylesheet = $params->get('add_stylesheet','');
	$bgcolor = $params->get('bgcolor','#FFFFFF');
	$fontcolor 	= $params->def('fontcolor','');
	$entermode = $params->def( 'entermode', 0 );
	$shiftentermode = $params->def( 'shiftentermode', 1 );
	$htmlentities = $params->def( 'htmlentities', 0 );
	$includelatinentities = $params->def( 'includelatinentities', 0 );
	$includegreekentities = $params->def( 'includegreekentities', 0 );
	$numericentities = $params->def( 'numericentities', 0 );
	$useRelativeURLPath = $params->def( 'UserRelativeFilePath', 0 );
	$showerrors = $params->def( 'showerrors', 1 );

	

	
	//set default view for toolabar
	$toolbar = $toolbar == 'Default' ? 'Advanced' : $toolbar;
	$toolbar_ft = $toolbar_ft == 'Default' ? 'Advanced' : $toolbar_ft;
		
	
	//set flag to see if Pspell should be enabled
	$enablePspell = function_exists("pspell_check")  ? 1 : 0;
	
	
	//define path_root for relative path
	
	$path_root = '../';
	
	if(strpos($_SERVER['REQUEST_URI'],'administrator'))
	{
		$logintime 	= mosGetParam( $_SESSION, 'session_logintime', '' );
		$session_id = md5( $my->id . $my->username . $my->usertype . $logintime );
		
	}
	else // frontend login
	{
		$query = 'select s.session_id from #__session s'
		. ' join #__users u on u.id = s.userid ' 
		. ' where u.id =' . $my->id . ' and s.guest = 0 and u.gid > 18 ';
		$database->setQuery( $query);
		$session_id = $database->loadResult();
		
		//set toolbar to compact mode
		$toolbar = $toolbar_ft;
		$path_root = '';
		
	}	
	
	/* Need to check to see  seesion recordd already created */
			
	$ip = md5($_SERVER['REMOTE_ADDR']);
	$query ='select session_id from #__session where session_id =\'' .$ip .'\''; 
	$database->setQuery( $query);
	$ip_recorded = $database->loadResult();
		
	if(!isset($ip_recorded)) //lets  record IP address
	{
		    
		$query = 'insert into #__session(username,time,session_id,gid) values(\'' . $session_id . '\',\'' . (time() + 7200) . '\',\'' 
		. $ip  . '\',0)';

	}
	else // update time and  user session_id 
	{
		$query = 'update #__session set time = \'' . (time() + 7200) . '\',username = \'' . $session_id . '\' ' 
		.'where session_id =\'' .$ip .'\''; 
	                     
	}
	$database->setQuery( $query);
	$database->query();

	 
	$errors = '';
	
	//Sanitize image path
	$image_path = preg_replace('/(^\s*\/|\/\s*$)/','',$image_path);
	       
	
	$xml_path="$mosConfig_absolute_path/mambots/editors/fckeditor/fckstyles_template.xml";
	$template = $mainframe->getTemplate();
	if ( $content_css || $editor_css ) {
		if($editor_css !== 0 & $content_css == 0){
			if( is_file( $mosConfig_absolute_path . '/templates/'.$template.'/css/editor_css.css' ) ){
				$content_css = 'templates/'.$template.'/css/editor_css.css';
			} else {
				if( $my->gid > $gid ){
					$errors .= '<span style="color: red;">Warning: ' . $mosConfig_absolute_path . 'templates/'.$template.'/css/editor_css.css' . ' does not appear to be a valid file. Reverting to JoomlaFCK\'s default styles</span><br/>';
				}//end if gid > 29
			}//end if valid file
		} else {
			if( is_file( $mosConfig_absolute_path . '/templates/'.$template.'/css/template_css.css' ) ){
				$content_css = 'templates/'.$template.'/css/template_css.css';
			} 
			else if( is_file( $mosConfig_absolute_path . '/templates/'.$template.'/css/template.css.php' ) ){
								
				   $content_css = 'templates/'.$template.'/css/JFCKeditor.css.php'; 
				  
				   if(!is_file( $mosConfig_absolute_path  . '/templates/'.$template.'/css/JFCKeditor.css.php') ||  
				   		filemtime($mosConfig_absolute_path  . '/templates/'.$template.'/css/template.css.php') > 
						filemtime($mosConfig_absolute_path  . '/templates/'.$template.'/css/JFCKeditor.css.php') ) 
				   {
				           
              			 $file_content = file_get_contents('../templates/'.$template.'/css/template.css.php');
						 $file_content  =  preg_replace_callback("/(.*?)(@?ob_start\('?\"?ob_gzhandler\"?'?\))(.*)/",
						   create_function(
								'$matches',
								'return ($matches[1]) .\';\';'
								
							),$file_content);
						 
					    $file_content = preg_replace("/(.*define\().*DIRECTORY_SEPARATOR.*(;?)/",'',$file_content);
						$file_content =
						 '<'. '?' . 'php' . ' function getYooThemeCSS() { ' . '?' . '>' . $file_content . '<'. '?' . 'php' .  ' } ' . '?' . '>';
					  
						$fout = fopen($path_root . $content_css,"w");
						fwrite($fout,$file_content);
						fclose($fout);
					}
					
					include($path_root . $content_css);
					$content_css = 'templates/'.$template.'/css/JFCKeditor.css'; 
									
					ob_start();
					getYooThemeCSS();
					$file_content =  ob_get_contents(); 
					ob_end_clean();
					$fout = fopen($path_root . $content_css,"w");
					fwrite($fout,$file_content);
					fclose($fout);
				
				}
				else {
				if( $my->gid > $gid ){
					$errors .= '<span style="color: red;">Warning: ' . $mosConfig_absolute_path . 'templates/'.$template.'/css/template_css.css' . ' does not appear to be a valid file. Reverting to JoomlaFCK\'s default styles</span><br/>';
				}//end if gid > 29
			}//end if valid file
		}//end if  $editor_css !== 0 & $content_css == 0

		/* Is the content_css == 0 or 1 then use FCK's default */
		if( $errors  != "" ){
			$content_css = 'mambots/editors/fckeditor/editor/css/fck_editorarea.css';
			$style_css="fckstyles.xml";
		}//end if 

		/*write to xml file and read from css asnd store this file under editors*/
		xml_writer($path_root . $content_css,$xml_path);
		$style_css="fckstyles_template.xml";

	} else {
		if ( $content_css_custom ) {
		
		
		  $hasRoot = strpos(' ' . strtolower($content_css_custom),strtolower($mosConfig_absolute_path));
			$file_path = ($hasRoot ? '' : $mosConfig_absolute_path) .  ($hasRoot || substr($content_css_custom,0,1) == DIRECTORY_SEPARATOR  ? '' : DIRECTORY_SEPARATOR) . 	$content_css_custom;
		   

			if( is_file( $file_path)) 
			{
          $content_css =  $file_path;
            $content_css = str_replace(strtolower($mosConfig_absolute_path ) . DS,'',strtolower($content_css_custom));
          xml_writer($path_root . $content_css,$xml_path);
          $style_css="fckstyles_template.xml";
			} else 
			{
          if( $my->gid > $gid ){
            $errors .= '<span style="color: red;">Warning: ' . $mosConfig_absolute_path . '/' .$content_css_custom . ' does not appear to be a valid file.</span><br/>';
          }//end if gid > $gid	
          $content_css = 'mambots/editors/fckeditor/editor/css/fck_editorarea.css';
          $style_css="fckstyles.xml";
			}//end if valid file
				
		} else
		 {
		
			$content_css = 'mambots/editors/fckeditor/editor/css/fck_editorarea.css';
			$style_css="fckstyles.xml";

		 }//end if $content_css_custom

	}//end if $content_css || $editor_css

	if ( strpos( $width, '%' ) === false )
		$WidthCSS = $width . 'px' ;
	else
		$WidthCSS = $width ;

	if ( strpos( $height, '%' ) === false )
		$HeightCSS = $height . 'px' ;
	else
		$HeightCSS = $height ;


      
  
		//if additional stylesheets specified
   		$stylesheet_name = ''; 
		
		
		
		if($add_stylesheet_path)
		{
		
		   	$hasRoot = strpos(' ' . strtolower($add_stylesheet_path),strtolower($mosConfig_absolute_path));
			 
			$add_stylesheet_path = str_replace(strtolower($mosConfig_absolute_path) . DIRECTORY_SEPARATOR,'',strtolower($add_stylesheet_path));
				
		
  		}
		else
		{
		  $add_stylesheet_path = '/templates/' . $template . '/css/';
		}
				
			   
      
		
	    $BaseAddCSSPath = (preg_match('/(^\/|^\\\\)/',$add_stylesheet_path) ? '' : '/') .$add_stylesheet_path   
				.(preg_match('/.(\/$|\\\\$)/',$add_stylesheet_path) ? '' : '/');
        
		
  
        $BaseAddCSSPath = str_replace('\\','/',$BaseAddCSSPath);
		
       
	     //echo $add_stylesheet_path;

  
	   	if($add_stylesheet_path &&  $add_stylesheet)
	   	{
	   
          if (strpos($add_stylesheet,';'))
          {
            $stylesheets =  explode(';',$add_stylesheet);
          }
          else
          {
            $stylesheets[] = $add_stylesheet;
          }
        
           
          $count = 0;
          
          foreach($stylesheets as $stylesheet)
          {
          
            if(!preg_match('/\.\w{3}$/',$stylesheet))
            {
              $stylesheet .= '.css';
                   
            }
            
          
             
            $fin =  $path_root .  substr($BaseAddCSSPath,1,strlen($BaseAddCSSPath)) . $stylesheet;
                  
            
                  
            $file =  $mosConfig_absolute_path. (preg_match('/(^\/|^\\\\)/',$add_stylesheet_path) ? '' : DIRECTORY_SEPARATOR) .$add_stylesheet_path   
            .(preg_match('/.(\/$|\\\\$)/',$add_stylesheet_path) ? '' : DIRECTORY_SEPARATOR)  . $stylesheet;
            
            $fout = $path_root . 'mambots/editors/fckeditor/' . str_replace('.css','.xml',$stylesheet);	
          
          
           
            
            if( is_file( $file)) 
            {
                  xml_writer($fin,$fout);
                     
            } else 
            {
                      if( $my->gid > $gid ){
                          $errors .= '<span style="color: red;">Warning: ' . $file . ' does not appear to be a valid file.</span><br/>';
                      }//end if gid > $gid	
                      array_splice($stylesheets, $count,1);
                      
            }//end if valid file
        
            $count ++;
          }
          
          $stylesheet_name =  str_replace('.css','',implode(';',$stylesheets));
         
	   	}
   
    
		
	$results = $_MAMBOTS->trigger( 'onCustomEditorButton' );
	$buttons = array();
	foreach ($results as $result) {
		if ( $result[0] ) {
			$buttons[] = '<img src="'.$mosConfig_live_site.'/mambots/editors-xtd/'.$result[0].'" onclick="InsertHTML(\''.$hiddenField.'\',\''.$result[1].'\')" />';
		}
	}
	$buttons = implode("", $buttons);		


	/* Lets sort out the directory issue */
	$urlDetails = parse_url($mosConfig_live_site);
	$directory = str_replace( array( $urlDetails['scheme'], $urlDetails['host'], '://' ), '', $mosConfig_live_site );
	

	if( $showerrors && $my->gid > $gid ){	
		//Version Checker
		if( function_exists( "curl_init" ) ){
			$ch = curl_init();	curl_setopt( $ch, CURLOPT_URL, 'http://www.joomlafckeditor.com/version.txt' );	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
			$version = curl_exec($ch); curl_close( $ch ); if( $version != '2.6.1' && $version ){ $errors .= 'Please beaware there is a newer version of the JoomlaFCK Editor which can be downloaded from <a href="http://www.joomlafckeditor.com" target="_blank">http://www.joomlafckeditor.com</a>.<br/>'; }//end if
		}//end if
				
		/* Check to see if the path exists. */
		if( !is_dir( $mosConfig_absolute_path . '/' . $image_path ) ){
			$errors .= '<span style="color: red;">Warning: ' . $mosConfig_absolute_path . $imagePath . ' does not appear to be a valid directory!</span><br/>';
		}//end if
		if( $errors !== "" ){
			echo $errors . '<span style="color:blue">Please note the above message will only displayed to Managers and above.</span>';
		}//end if
	}

	$content_css =   $mosConfig_live_site . '/' . $content_css; 
  	$content_css =   str_replace('\\','/',$content_css); 
  
  
	// Define Enter & Shift Enter Mode
	$enterbehavior = array();
	$enterbehavior[0] = 'br';
	$enterbehavior[1] = 'p';
	$enterbehavior[2] = 'div';

	// Define Entities
	$htmlentities	= $htmlentities ? 'true' : 'false';
	$includelatinentities	= $includelatinentities ? 'true' : 'false';
	$includegreekentities	= $includegreekentities ? 'true' : 'false';
	$numericentities	= $numericentities ? 'true' : 'false';
  


return <<<EOD
<textarea name="$hiddenField" id="$hiddenField" cols="$col" rows="$row" style="width:{$WidthCSS}; height:{$HeightCSS};">$content</textarea>
<script type="text/javascript">

	var oFCKeditor = new FCKeditor('$hiddenField');
	oFCKeditor.BasePath = "$directory/mambots/editors/fckeditor/" ;
	oFCKeditor.Config["SitePath"] =  "$mosConfig_live_site";
	oFCKeditor.Config["ImagePath"] =  "$image_path"; 
	oFCKeditor.Config["UseRelativeURLPath"] =  $useRelativeURLPath; 
	oFCKeditor.Config["CustomConfigurationsPath"] = "$mosConfig_live_site/mambots/editors/fckconfigjoomla.js";
	oFCKeditor.ToolbarSet = "$toolbar" ;
	oFCKeditor.Config['BaseAddCSSPath'] = "$BaseAddCSSPath";
	oFCKeditor.Config['EditorAreaCSS'] = "$content_css";
	oFCKeditor.Config['ContentLangDirection'] = "$text_direction" ;
	oFCKeditor.Config['SkinPath'] = oFCKeditor.BasePath + 'editor/skins/' + '$skin' + '/' ;
	oFCKeditor.Config['StylesXmlPath']=oFCKeditor.BasePath +'$style_css';
	oFCKeditor.Config['FormatSource'] = $formatSource;	
	oFCKeditor.Config['AddStylesheets'] = "$stylesheet_name";
	oFCKeditor.Config['BackgroundColor'] = "$bgcolor";
	oFCKeditor.Config["FontColor"] = "'.$fontcolor.'";	
	oFCKeditor.Config['EnterMode'] = "$enterbehavior[$entermode]";
	oFCKeditor.Config['ShiftEnterMode'] = "$enterbehavior[$shiftentermode]";
	oFCKeditor.Config['ProcessHTMLEntities'] = $htmlentities ;
	oFCKeditor.Config['IncludeLatinEntities'] = $includelatinentities ;
	oFCKeditor.Config['IncludeGreekEntities'] = $includegreekentities ;
	oFCKeditor.Config['ProcessNumericEntities'] = $numericentities ;
	oFCKeditor.Config['Pspell'] = "$enablePspell";	
	oFCKeditor.Width = "$wwidth" ;
	oFCKeditor.Style_css = "$style_css" ;
	oFCKeditor.Height = "$hheight" ;
	oFCKeditor.ReplaceTextarea() ;
   
   

	function InsertHTML(field, value) {
		// Get the editor instance that we want to interact with.
		var oEditor = FCKeditorAPI.GetInstance(field) ;
	
		// Check the active editing mode.
		if ( oEditor.EditMode == FCK_EDITMODE_WYSIWYG )	{
			// Insert the desired HTML.
			oEditor.InsertHtml( value ) ;
		} else {
			alert( 'Please switch to WYSIWYG mode.' ) ;
		}//end if
	}//end function

</script>
<br />
<p>$buttons</p>
EOD;
}

function botGetEditorContents($editorArea, $hiddenField) {

}


function xml_writer($txt_filename,$xml_filename)
 {
	
	
	

	
	
    $file_exist = is_file($xml_filename);  
	
	// Only create file if it has not already been created today
	
	
	
	if(!$file_exist  ||  ($file_exist  &&  floor((time() - filemtime($txt_filename))/60)  <= 5)) //template changed within the last 5 minutes
	{
	
		$xml_str="<?xml version=\"1.0\" encoding=\"utf-8\" ?>
				<Styles>";
		
		
	   global  $main,$elem;
		
		
		$oldumask = umask(0) ;
		@chmod( $txt_filename, 0666);
		@chmod( $xml_filename, 0666);
		umask( $oldumask ) ;			
					
		$f=file($txt_filename);
		$main=array();
		$elem=array();
		  
		extract_css_elements($f,dirname($txt_filename));
		
		if(count($main))
		{
			foreach($main as $k=>$val)
			{
				$xml_str.='<Style name="'.$val.'" element="'.$elem[$k].'">
								<Attribute name="class" value="'.$val.'" />
							</Style>';	
				$xml_str.="\n";
	
	
			}
		}
			
		$xml_str.="\n"."</Styles>"; 
		$fp=fopen($xml_filename,"w");
		fwrite($fp,$xml_str);
		fclose($fp);
		
				


		
	}
	
 }
 
  
 
 
function extract_css_elements($f,$dirname = '')
{
		
	
	   global $main, $elem,$dir; 
	   
	   $dir = $dirname;
	  
		
		for($i=0;$i<count($f);$i++)
		{
			$mm= trim($f[$i]);
			
		  
			preg_replace_callback('/@import\s*(?:url\()?["\']([^"\')]+)["\']\)?;/i',  create_function(
            '$matches', 
			'global $dir;
			 $oldumask = umask(0);
		     @chmod( $matches[1], 0666);
		     umask( $oldumask );
			 $file_array = file($dir ."/" .$matches[1]);
			 extract_css_elements($file_array,$dir);'
			),$mm);
			
			
			//for dot class
				if(preg_match("/^\./", $mm,$tt))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],1);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"p");
						}
					}
				}//end dot
				//for ids
				elseif(preg_match("/^#/", $mm,$tt))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],1);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"p");
						}
					}
				}//end ids
				//for div class
				elseif(preg_match("/^div(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],4);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"div");
						}
					}
				}//end div
				//for img class
				elseif(preg_match("/^img(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],4);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"img");
						}
					}
				}//end img
				elseif(preg_match("/^table(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],6);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"table");
						}
					}
				}//end table
				elseif(preg_match("/^tr(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],3);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"tr");
						}
					}
				}//end table row
				elseif(preg_match("/^td(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],3);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"td");
						}
					}
				}//end table cell
				elseif(preg_match("/^input(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],6);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"input");
						}
					}
				}//end input
				elseif(preg_match("/^textarea(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],9);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"textarea");
						}
					}
				}//end textarea
				elseif(preg_match("/^hr(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],3);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"hr");
						}
					}
				}//end horizontal rule
				//for span class
				elseif(preg_match("/^span(\.|#)/", $mm))
				{
				$x=explode("{",$mm);
				$xx=trim($x[0]);
				$pp=explode(",",$xx);
				foreach($pp as $val)
					{
					$ll=explode(" ",$val);
					$nn=substr($ll[0],5);
					if(!in_array($nn,$main) && $nn!="")
						{
						array_push($main,$nn);
						array_push($elem,"span");
						}
					}
				}//end img
				
				
				
	
			}	
	
	
	        
	
		
	
} 
 
?>