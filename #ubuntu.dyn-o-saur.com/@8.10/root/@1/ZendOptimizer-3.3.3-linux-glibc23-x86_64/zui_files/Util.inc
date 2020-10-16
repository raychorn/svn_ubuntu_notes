<?php

	/**
	* @author Michael Spector <michael@zend.com>
	* @copyright Zend Technologies Ltd 2005
	* @version $Revision: 1.19.4.9 $
	* @since $Date: 2005/09/14 14:26:37 $
	*/

	include_once ('Log.inc');

	/**
	* class Util
	* 
	* This class used for providing helper functions
	*/
	class Util
	{
		/**
		* This function makes specified path nicer
		* It removes '/./', '/../' and extra '/' from it
		*/
		function nicePath($path)
		{
			$path = preg_replace('@///*@', '/', $path); /* multiple '/' */
			$path = preg_replace('@^\./@', '', $path); /* first './' from the path */
			$path = preg_replace('@/\./@', '/', $path); /* '/./' from the path */
			$path = preg_replace('@^[^/]+/\.\./?@', '', $path); /* dir/.. from the beginning */
			$path = preg_replace('@\/[^/]+/\.\./?@', '/', $path); /* /dir/.. from the path */
			$path = preg_replace('@([^/])/+$@', '\\1', $path); /* last / from the end of path */

			return $path;
		}

		/**
		* This function returns first argument as is if its a full path,
		* otherwise it prepends second argument to it.
		* @param string input path
		* @param string root path (to prepend, in case if input path is not full)
		* @return string full path
		*/
		function fullPath($path, $root)
		{
			return $path[0] == '/' ? $path : $root.'/'.$path;
		}

		/**
		* This function performs matching file to pattern
		* @param string file
		* @param string pattern
		* @return bool True whether file matches patterm, otherwise false
		*/
		function fileMatch($file, $pattern)
		{
			if(function_exists('fnmatch')) {
				return fnmatch ($pattern, $file);
			}

			for($i=0; $i<strlen($pattern); $i++) {
				if($pattern[$i] == "*") { /* star wildcard (matches all or nothing :) */
					for($c=$i; $c<max(strlen($pattern), strlen($file)); $c++) {
						if(Util::fileMatch(substr($file, $c), substr($pattern, $i+1))) {
							return true;
						}
					}
					return false;
				}

				if($pattern[$i] == "[") { /* [] set of letters (matches something :) */
					unset($sqr_braket);
					$letter_set = array();
					for($c=$i+1; $c<strlen($pattern); $c++) {
						if($pattern[$c] != "]") {
							array_push($letter_set, $pattern[$c]);
						}
						else {
							$sqr_braket = true;
							break;
						}
					}
					if(!isset($sqr_braket)) {
						print("No closing ']' found\n");
						return false;
					}
					foreach ($letter_set as $letter) {
						if(Util::fileMatch(substr($file, $i), $letter.substr($pattern, $c+1))) {
							return true;
						}
					}
					return false;
				}

				if($pattern[$i] == "?") { /* question sign wildcard (matches only one thing :) */
					continue;
				}

				if($pattern[$i] != $file[$i]) {
					return false;
				}
			}
			return true;
		}

		/**
		* This function returns all directory contents, that
		* match specified pattern
		* @param string directory path
		* @param string pattern
		* @return array filenames that match the pattern
		*/
		function listDir($path, $pattern)
		{
			if(function_exists ('glob')) {
				return glob($path.'/'.$pattern);
			}

			$files_list = array();
			$dir = opendir($path);
			while($file = readdir($dir)){
				if($file == "." || $file == ".."){
					continue;
				}
				if(Util::fileMatch($file, $pattern)){
					array_push($files_list, realpath($path.'/'.$file));
				}
			}
			closedir($dir);

			return $files_list;
		}

		/**
		* This function applies XSLT transformation on XML document and returns the result
		* @param string path to XML file
		* @param string path to XSLT file
		* @return string result of transformation
		*/
		function xsltTransformXML($xml_file, $xslt_file)
		{
			if(function_exists('xslt_create')) {
				return Util::xsltTransformXMLwithSablotron ($xml_file, $xslt_file);
			}
			else if (function_exists('domxml_xslt_stylesheet_file')) {
				return Util::xsltTransformXMLwithDom ($xml_file, $xslt_file);
			}
			else {
				print ("None of XSLT processors is avalable\n");
				return;
			}
		}

		/**
		* This function applies XSLT transformation using Sablotron
		* @param string path to XML file
		* @param string path to XSLT file
		* @return string result of transformation
		*/
		function xsltTransformXMLwithSablotron ($xml_file, $xslt_file)
		{
			$xsltproc = xslt_create();
			$newt_code = xslt_process ($xsltproc, $xml_file, $xslt_file);
			if(empty ($newt_code)) {
				print ('XSLT processing error: '. xslt_error($xsltproc));
				return;
	  	}
	 		xslt_free($xsltproc);
			return preg_replace('@<\?xml.*\?>@U', '', $newt_code);
		}

		/**
		* This function applies XSLT transformation using DOM XSLT
		* @param string path to XML file
		* @param string path to XSLT file
		* @return string result of transformation
		*/
		function xsltTransformXMLwithDom ($xml_file, $xslt_file)
		{
			if(!$dom_xml_obj = domxml_open_file($xml_file)) {
				print("Error parsing XML file: $xml_file\n");
				return;
			}
			$dom_xslt_obj = domxml_xslt_stylesheet_file ($xslt_file);
			$dom_trans_obj = $dom_xslt_obj->process ($dom_xml_obj);
			return $dom_xslt_obj->result_dump_mem ($dom_trans_obj);
		}

		/**
		* This function can be used for sending emails with attachments
		* @param string To: recipient
		* @param string Subject: subject of the letter
		* @param string From: sender
		* @param string path to the file to attach
		* @param string mime type of the file
		* @return bool exit status of PHP function mail() - it doesn't mean the status of mail delivery
		*/
		function sendMailWithAttachment($to, $subject, $message, $from, $file_name, $mime_type="application/octet-stream")
		{
			// Generate a boundary string
			$semi_rand = md5(time());
			$mime_boundary = "Zend-$semi_rand";

			$smtp_headers = "X-Mailer: PHP mailer\n";

			$text_body = "--$mime_boundary\n".
				"Content-Type: text/plain; charset=\"us-ascii\"\n\n".
				"$message\n";

			// Open file to attach
			$data = Util::fileToStr ($file_name);

			// Prepare an encoded message
			$text_encoded = "--$mime_boundary\n".
				"Content-type: $mime_type; name=\"".basename($file_name)."\";\n".
				"Content-Transfer-Encoding: base64\n".
				"Content-disposition: attachment; filename=\"".basename($file_name)."\"\n\n".
				chunk_split(base64_encode($data))."\n".
				"--$mime_boundary--\n";

			$mime_headers = "MIME-version: 1.0\n".
				"Content-type: multipart/mixed; ".
				"boundary=\"$mime_boundary\"\n".
				"Content-transfer-encoding: 7BIT\n".
				"X-attachments: $file_name;\n\n";

			// Send the message
			$headers = (empty($from) ? "" : "From: $from \r\n") . $smtp_headers . $mime_headers;
			$message = $text_body . $text_encoded;

			return mail($to, $subject, $message, $headers);
		}

		/**
		* This function writes contents of string to file
		* @param string string to write
		* @param string filename
		* @return bool exit status
		*/
		function strToFile ($str, $file)
		{
			if (function_exists('file_put_contents')) {
				if(@file_put_contents ($file, $str)) return true;
			}
			$fp = @fopen ($file, "wb");
			if (!is_resource($fp)) {
				Log::append ("Error opening file: $file for writing");
				return false;
			}
			fwrite ($fp, $str);
			fclose ($fp);
			return true;
		}

		/**
		* This function returns contents of the file
		* @param string filename
		* @param int offset from where to start reading (default: 0)
		* @return string contents of the file
		*/
		function fileToStr ($file, $offset=0)
		{
			if (function_exists('file_get_contents') && $offset==0) {
				return @file_get_contents ($file);
			}
			$fp = @fopen ($file, "rb");
			if(is_resource($fp)) {
				if ($offset > 0) fseek ($fp, $offset, SEEK_SET);
				else if($offset < 0) fseek ($fp, $offset, SEEK_END);

				$bytes = filesize($file);
				$str = "";
				if ($bytes > 0) $str = fread ($fp, $offset < 0 ? -$offset : $bytes-$offset);
				fclose ($fp);
				return $str;
			}
			Log::append ("Error opening file: $file for reading");
			return null;
		}

		/**
		* This function executes command and returns output as string
		* @param string command
		* @param int exit status
		* @return string output from the command
		*/
		function cmdToStr ($command, &$status)
		{
			exec($command.' 2>&1', $res_arr, $status);
			Log::append ("Executing: $command (exit status: $status)");
			return implode("\n", $res_arr);
		}

		/**
		* This function executes command and returns exit status
		* @param string command
		* @return int exit status
		*/
		function execCmd ($command)
		{
			$command = preg_replace ('@\d\s*>\s*/dev/null\s*@', '', $command);
			$command = $command.' 2> /dev/null 1> /dev/null';
			passthru ($command, &$status);
			Log::append ("Executing: $command (exit status: $status)");
			return $status;
		}

		/**
		* This function returns Glibc version
		* @return string Glibc version
		*/
		function glibcVersion()
		{
			if (preg_match ("#(\d[\.\d]*\d)#", shell_exec("ls /lib/{libc,ld}-?.?.* 2>/dev/null"), $match)) {
				return $match[1];
			}
			return null;
		}

		/**
		* This functions works like 'which' command in Unix
		* @return string full path of the first command that was found, otherwise - null
		*/
		function whichCmd($command, $extra_path=array())
		{
			static $which_cache;

			if(isset($which_cache[$command])) {
				return $which_cache[$command];
			}

			$extra_path[] = '/sbin';
			$extra_path[] = '/usr/sbin';

			$path_arr = array_merge(explode(':', getenv('PATH')), $extra_path);
			foreach ($path_arr as $path) {
				$full_path = realpath ($path.'/'.$command);
				if (file_exists ($full_path) && is_executable($full_path)) {
					$which_cache[$command] = $full_path;
					return $full_path;
				}
			}
			return null;
		}

		/**
		* This function kills process with specified signal
		* @param int PID
		* @param mixed signal number
		* @return bool exit status of the operation
		*/
		function kill($pid, $sig='TERM')
		{
			$pid = trim($pid);
			if (is_numeric($pid)) {
				$kill = Util::whichCmd("kill");
				if ($kill) {
					if(Util::execCmd ("$kill -$sig $pid") == 0) {
						return true;
					}
				}
			}
			return false;
		}

		/**
		* This function finds all PIDs of specified process
		* @param string name (or part of the name) of the process
		* @param &array PIDs of all matched processes
		* @return bool exit status of the operation
		*/
		function pidof ($process, &$pids)
		{
			$ps = Util::whichCmd("ps");
			$pids = array();

			if($ps) {
				exec ($ps.' ax 2> /dev/null', $ps_table, $status);
				if (count($ps_table) == 0) {
					exec ($ps.' -ef 2> /dev/null', $ps_table, $status);
				}
				foreach ($ps_table as $line) {
					if(preg_match('@^[^\d]*(\d+)\s.*'.addcslashes($process, '@').'@', $line, $match)) {
						array_push ($pids, $match[1]);
					}
				}
				return (count($pids) > 0);
			}
			return false;
		}

		/**
		* This function kills process with specified name
		* @param string name (or part of the name) of the process
		* @param mixed signal number
		* @return bool exit status of the operation
		*/
		function killProcess ($process, $sig='TERM')
		{
			if (Util::pidof ($process, $pids)) {
				foreach ($pids as $pid) {
					if(Util::kill ($pid, $sig)) {
						Log::append ("Killed: $process with signal: $sig");
					}
				}
				// Check whether there are no still opened processes in memory
				return !Util::pidof ($process, $pids);
			}
			return false;
		}

		/**
		* This function parses URL
		* If no protocol was specified, this function assumes 'http' protocol
		* @param string URL
		* @return array output from PHP's parse_url
		*/
		function parseURL ($url)
		{
			// If protocol exists - return native call
			if(preg_match('@^[^:]+://.*$@', $url)) {
				return @parse_url($url);
			}
			// Otherwise assume 'http' protocol and call to native function
			$url = 'http://'.$url;
			$url_arr = @parse_url($url);
			unset ($url_arr['scheme']);
			return $url_arr;
		}

		/**
		* This function strips HTML tags
		* @param string HTML page
		* @return string text
		*/
		function stripHTML ($page)
		{
			foreach (array ('head', 'title', 'form', 'script') as $tag) {
				$page = preg_replace ("/<$tag.*>.+<\/\s*$tag>/U", "", $page);
			}
			$page = preg_replace ("/<.+>/U", "", $page);
			$page = html_entity_decode ($page);

			return $page;
		}

		/**
		* This function strips HTTP error response
		* @param string HTTP response
		* @return string text
		*/
		function stripHTTPError ($text)
		{
			$text = Util::stripHTML ($text);
			$text = trim ($text);
			$text = preg_replace ("/^[\r\n]+/", "", $text);
			$text = preg_replace ("/404\s+Not\s+Found\s+Not Found/si", "", $text);
			$text = preg_replace ("/^Apache\/.*Server at.*$/m", "", $text);
			$text = preg_replace ("/[\r\n]+/", "\n", $text);

			return $text;
		}

		/**
		* Checks whether the given IP is valid
		* @param string IP
		* @return boolean true - valid, false - invalid
		*/
		function IsIPValid ($ip)
		{
			if (preg_match("/^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/", $ip, $match)) {
				if($match[1] < 256 && $match[2] < 256 && $match[3] < 256 && $match[4] < 256) {
					return true;
				}
			}
			return false;
		}
	}
?>
