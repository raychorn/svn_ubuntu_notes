<?php
/*
   +----------------------------------------------------------------------+
   | Zend installation class                                              |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2005 Zend Technologies Ltd.                       |
   +----------------------------------------------------------------------+
   | The contents of this source file is the sole property of             |
   | Zend Technologies Ltd.  Unauthorized duplication or access is        |
   | prohibited.                                                          |
   +----------------------------------------------------------------------+
   | Authors: Michael Spector <michael@zend.com>                          |
   |          Anya Tarnyavsky <anya@zend.com>                             |
   +----------------------------------------------------------------------+
*/

error_reporting (E_ALL);

include_once ('basic_install.inc');
include_once("gaugebox.inc");
include_once ('license_downloader.inc');

if(defined('SERIAL_NUMBER')){
	include_once("forms.inc");
}

if (!defined ('LOG_FILE')) {
	define ('LOG_FILE', getcwd()."/install.log");
}

class Install extends BasicInstall
{
	var $forms;  /* forms for displaying more complex dialogs */

	function Install ($product, $version="")
	{
		set_time_limit(0);
		error_reporting(E_ALL ^ E_NOTICE);

		print ("Please, wait ...\n");

		parent::BasicInstall();
		global $_SERVER;

		# Parse command line arguments:
		$argc = count ($_SERVER["argv"]);
		$argv = $_SERVER["argv"];
		$this->cmdline_arg = array();
		$this->cmdline_arg["__options"] = array();
		for ($i=1; $i < $argc; $i++) {
			if (preg_match ('/^\-\-?(.*)$/', $argv[$i], $match)) {
				if (isset ($argv[$i+1]) && !preg_match ('/^\-\-?/', $argv[$i+1])) {
					$this->cmdline_arg[strtolower($match[1])] = $argv[$i+1];
					$argc++;
				} else {
					$this->cmdline_arg[strtolower($match[1])] = true;
				}
			} else {
				$this->cmdline_arg["__options"][] = $argv[$i];
			}
		}
		$this->cmdline_args = array (
			"help" => "Show this help", 
			"answers-file 'file'" => "Read installation configuration from file",
			"save-answers 'file'" => "Save installation configuration to file"
		);

		// the following overriddes logger and error handler from the basic class
		$this->logger = new InstallLogger (LOG_FILE);
		$this->error_handler = new InstallErrorHandler ($this);

		$this->conf['product'] = $product;
		//$this->conf['version'] = $version;
		//$this->conf['title'] = "$product $version";
		$this->conf['backtitle'] = "$product Installation";
		$this->conf['install_guide'] = 1;  // by default, there is Installation Guide

		$this->conf['backup_suffix'] = "-". strtolower(preg_replace("/[\s\/]+/", "_", $product)).".bak";
		$this->conf['progress_bar_started'] = false;
		$this->conf['wrap_size'] = 75;
		$this->conf['php_ini_opened'] = false;

		$this->logger->log ("Starting installation of $product $version ...");

		// define system information
		// $uname = posix_uname();
		$this->conf['uname'] = $this->my_uname();
		if(empty($this->conf['uname']['nodename'])){
			$this->conf['uname']['nodename'] = "localhost";
		}

		$this->logger->log ('System: ' . $this->conf['uname']['sysname'] .
				   ' ' . $this->conf['uname']['release'] .
				   ' ' . $this->conf['uname']['machine']);

		$this->logger->log ('Hostname: ' . $this->conf['uname']['nodename']);

		$this->conf['nl'] = "\n";
		$this->conf['zend_ext_directive'] = "zend_extension";

		if ($this->conf['uname']['sysname'] == "HP-UX") {
			$this->conf['so_ext'] = ".sl";
		} else {
			$this->conf['so_ext'] = ".so";
		}

		// determine temporary directory
		$this->conf['tmp_dir'] = getenv("ZEND_TMPDIR");
		unset ($_ENV["ZEND_TMPDIR"]);
		if(empty($this->conf['tmp_dir'])){
			$this->conf['tmp_dir'] = "/tmp".$this->conf['slash']."zend_install.". getmypid();
		}
		$this->logger->log ('Setting TEMP=' . $this->conf['tmp_dir']);

		// and create it
		$this->my_delete($this->conf['tmp_dir']);
		$this->my_mkdir($this->conf['tmp_dir']);
		chmod($this->conf['tmp_dir'],0700);

		$this->conf['interactive'] = true;

		# Read default config values
		$answers_file = null;
		if (isset ($this->cmdline_arg["answers-file"])) {
			$answers_file = $this->cmdline_arg["answers-file"];
			if (!$this->my_file_exists ($answers_file)) {
				$file = $this->search_file(array("../", "../data"), array($answers_file));
				if (!empty ($file)) {
					$answers_file = $file;
				} else {
				  $this->on_error ("{${answers_file}}: file doesn't exist!");
				}
			}
		} else {
			$config_file = strtolower(preg_replace("/\s+/", "_", $product)).".conf";
			$answers_file = $this->search_file(array("../", "../data"), array($config_file));
		}
		if(!empty ($answers_file)) {
			$default_config = parse_ini_file($answers_file);
			foreach ($default_config as $name => $val) {
				$this->conf[$name] = $val;
			}
			$this->conf['interactive'] = false;
		}

		$this->conf['webserver'] = "Web server";

		$this->conf['comp_dirs_arr'] = array( /* possible directories where to get install components  */
				1 =>  "../data",
				2 =>  "../install",
				3 =>  "../",
				4 =>  "/"
				);

		// initialize components array
		$this->conf['components'] = array();

		$this->conf['webserver_docroot_arr'] = array();

		// initialize file permissions
		if($this->my_file_exists("permissions")){
			$file_perms = file("permissions");
			foreach ($file_perms as $num => $line){
				if(preg_match("/^\s*\#/", $line) || preg_match("/^\s*$/", $line)){
					continue;
				}
				list($file, $mode, $owner, $group) = preg_split("/\s+/", $line); // file mode owner group
				$this->conf['file_perms'][$num] = array($file, $mode, $owner, $group);
			}
		}
		else{
			$this->conf['file_perms'] = array();
		}

		$this->conf['build_info'] = array();
		$inventory_file = $this->search_file(array("../", "../data"), array("Inventory.xml"));
		if($this->my_file_exists($inventory_file)) {
			$inventory = $this->file2str ($inventory_file);
			if(preg_match ("/\<Package\s+.*version=\"([^\"]+)\".*\>/iU", $inventory, $match)) {
			  //$this->conf['build_info']['package.version'] = rtrim(preg_replace("/RC.*$/", "", $this->tag2version ($match[1])), '_');
			  $this->conf['build_info']['package.version'] = $this->tag2version ($match[1]);
			}
			if(preg_match_all ("/\<Component\s([^\>]+)\>/iU", $inventory, $match)) {
				for ($i=0; $i<count($match[0]); $i++) {
					if(preg_match ("/version\=\"([^\"]+)\"/i", $match[1][$i], $ver_match)
						&& preg_match ("/name\=\"([^\"]+)\"/i", $match[1][$i], $name_match)) {
					  $name = preg_replace ("/-php-.*$/", "", $name_match[1]);
					  
					  $this->conf['build_info']["${name}.version"] = 
					    $this->tag2version ($ver_match[1], $name);
					}
				}
			}
		}

		// if version is specified as argument to ctor, take it, otherwise rely
		// on one automatically determined from Inventory.xml
		$this->conf['version'] = (empty ($version) ? 
					  $this->conf['build_info']['package.version'] : $version);
		$this->conf['title'] = "$product " . $this->conf['version'];

		// compose version, as should be submitted for license download mechanism
		if (empty ($this->conf['release']))
		  {
		    // $this->on_error("Undefined release name");
		    $this->conf['release'] = 'release';
		    $release = '0';
		  }
		elseif (preg_match ("/^beta(.)$/", $this->conf['release'], $arr))
		  {
		    $release = $arr[1];
		  }
		else
		  {
		    $release = '99'; // invalid build_id
		  }

		// this thingy is needed for license download; probably we should
		// catch it later, somewhere in get_license() or in license_downloader.inc;
		// otherwise (like for zend core) it is just not needed
		/*
		if(empty($this->conf['product_id'])){
			$this->on_error("Undefined product ID");
		}
		*/
		$this->conf['version_for_license'] = $this->conf['version'] . '-' . $release;
		
		// get the basename of the calling script
		$basename = basename(getenv("CALLING_SCRIPT"));
		unset ($_ENV["CALLING_SCRIPT"]);

		if ($basename == "install-tty" || !$this->conf['interactive']) {
			unset($this->conf['dialog']);
		} else {
			$dialog = "./dialog";
			if(!$this->my_file_exists($dialog)){
			  $this->on_error ("File: $dialog does not exist!");
			}
			if(!is_executable($dialog)){
				chmod($dialog, 0555);
			}
			$this->conf['dialog'] = $dialog;
		}

		if(defined('SERIAL_NUMBER')){
			$this->forms =& new Forms($this, !empty($this->conf['dialog']));
		}

		if(!empty($this->conf['dialog'])){ // set common dialog setup
			$this->conf['dialog'] .= sprintf(" --clear --backtitle %s --title %s",
				escapeshellarg ($this->conf['backtitle']),
				escapeshellarg ($this->conf['title'])
			);

			$this->conf['max_x'] = 78; // default size
			$this->conf['max_y'] = 24;
			$this->conf['dialog_tmp'] = $this->conf['tmp_dir'] . "/dialog.tmpfile";

			$this->conf['dialog_error'] = "Error while executing the dialog utility.\n".
                                                "Probable reason - wrong terminal settings.\n".
				                "Please use install-tty instead.";
		}

		$this->conf['php_ini_backup_msg'] = "";

		$this->conf['lic_error_file'] = "error.txt";
		$this->conf['license_installed'] = false;

		$this->restore_config();
		foreach ($this->conf['saved_config'] as $key => $val) {
			$this->conf[$key] = $val;
		}

		$this->conf['min_space_size'] = $this->my_du("../data");

		$this->conf['installer_dir'] = dirname(getcwd());
		$this->conf['installer_path'] = $this->conf['installer_dir'].":".$this->make_path($this->conf['installer_dir'], "data").":".$this->make_path($this->conf['installer_dir'], "zui_files");

		// needed for searching zendid in path
		putenv ('PATH=' . $this->conf['installer_path'] . ':' . getenv ('PATH'));
	} // end of ctor


	// ===================================
	// Command line arguments functions
	// ===================================

	function add_cmdline_args ($options)
	{
		$this->cmdline_args = array_merge ($this->cmdline_args, $options);

		$this->check_bad_arguments();

		if ($this->get_cmdline_arg ("help") || $this->get_cmdline_arg ("h")) {
			$this->help_and_die();
		}
	}

	function help_and_die ($err=null)
	{
		$prog = getenv("CALLING_SCRIPT");
		if ($err) {
			$err = "ERROR: {$err}\n";
		}
		$help = <<<EOF
{$err}
USAGE: {$prog} [OPTIONS]

Where OPTIONS are:


EOF;
		$space = 0;
		foreach ($this->cmdline_args as $opt => $descr) {
			if (strlen ($opt) > $space) {
				$space = strlen ($opt);
			}
		}
		$space += 12;
		foreach ($this->cmdline_args as $opt => $descr) {
			$help .= "--{$opt}".str_repeat (" ", $space-strlen($opt)).$descr."\n";
		}

		$this->msgbox ($help);
		$this->cleanup();
		die(-1);
	}

	function get_cmdline_arg($name)
	{
		return (isset($this->cmdline_arg[$name]) ? $this->cmdline_arg[$name] : null);
	}

	function check_bad_arguments()
	{
		$args = array();
		foreach ($this->cmdline_args as $arg => $descr) {
			$args[preg_replace ('/\s.*/', '', $arg)] = true;
		}

		foreach ($this->cmdline_arg as $arg => $val) {
			if ($arg == "__options") continue;
			if (!isset ($args[$arg])) {
				$this->help_and_die ("Bad option: --{$arg}");
			}
		}
	}

	// ------------------
	// SYSTEM FUNCTIONS
	// ==================


	// parms: 0 - tag (like OPTIMIZER_2_6_2_RC4_20060104)
	//        1 - (optional) component name (like 'optimizer')
	// returns: version (like 2.6.2)
	function tag2version ($tag, $name=null)
	{
	  if (isset ($name))
	    {
	      // don't take into account '-ts' suffix in name, if any
	      $name = preg_replace ('/-ts$/', '', $name);
	      // strip the component name from the beginning of the tag, if any
	      $tag = preg_replace ("/^$name/i", '', $tag);
	    }

	  if (preg_match ('/\w?_(\d+)_(\d+)_(\d+[a-z]?|[a-zA-Z0-9]+?)_?((ALPHA|BETA)\d*?)?_?RC(\d+)_/', $tag, $match)) 
	    {
	      $version = "{$match[1]}.{$match[2]}.{$match[3]}";
	    } 
	  else 
	    {
	      $version = $tag;
	    }

	  // it still could be something like 2.1.2RC7, so stripping out RC part
	  return (preg_replace ('/RC.*$/', '', $version));

	  //$version = preg_replace ('/_/', '.', $version);
	  //return ($version);
	} // end of tag2version()



	function my_uname()
	{
		$uname = array();
		$uname["sysname"]   = $this->cmd2str("uname -s 2>/dev/null");

		$uname["machine"]   = ($uname["sysname"] == "Linux" ?
			$this->cmd2str("uname -m 2>/dev/null") : $this->cmd2str("uname -p 2>/dev/null"));

		$uname["release"]   = $this->cmd2str("uname -r 2>/dev/null");
		$uname["nodename"]  = $this->cmd2str("uname -n 2>/dev/null");

		if ($uname["sysname"]=="AIX") {
			$major = $this->cmd2str("uname -v 2>/dev/null");
			$uname["release"] = $major.'.'.$uname["release"];
		}

		foreach ($uname as $name => $val) {
			if($val == "") {
				$uname[$name] = "unknown";
			}
		}

		return $uname;
	}

	// GUESS_DOMAINNAME
	function guess_domainname()
	{
		$cmd = $this->search_cmd_in_path ("domainname", true);
		if ($cmd) {
			return str_replace ("(none)", "", $this->cmd2str ("{$cmd} 2>/dev/null"));
		}
		return "";
	}

	// GUESS_GLIBC_VERSION
	function guess_glibc_version()
	{
		$glibc_ver = "";
		if($this->conf['uname']['sysname'] == "Linux") {
		  $glibc_path = $this->cmd2str ('/bin/ls /lib{,64}/{libc,ld}-?.?.* 2>/dev/null');
			if(@preg_match("/(\d)\.(\d)/", $glibc_path, $match)) {
				$glibc_ver = "$match[1].$match[2]";
				$this->logger->log ("Detected GLIBC version: $glibc_ver");
			}
		}
		return $glibc_ver;
	}

	// GET_RELEASE_VERSION
	function get_release_version()
	{
		$release = $this->conf['uname']['release'];
		if(preg_match("/^(\d+\.\d+)/", $this->conf['uname']['release'], $match)) {
			$release = $match[1];
		}

		$this->logger->log ("Detected release version: $release");
		return $release;
	}

	// CHECK_SYSTEM_SUPPORTED
	function check_system_supported()
	{
		if(!isset($this->conf['supported_systems'][$this->conf['uname']['sysname']])) {
			$error = "OS type: ".$this->conf['uname']['sysname'];
		}
		else {
			$sysconf = $this->conf['supported_systems'][$this->conf['uname']['sysname']];
			if(count($sysconf) > 0) {

				if(isset($sysconf["glibc"])) {
					$glibc_arr = $sysconf["glibc"];
					$glibc_OK = true;
					foreach($glibc_arr as $glibc) {
						$glibc_OK = false;
						if(preg_match("/^([\>\<]?)(.*)$/", $glibc, $match)) {
							$installed_glibc = $this->guess_glibc_version();
							if(($match[1] == ">" && version_compare($installed_glibc, $match[2]) >= 0)
									|| ($match[1] == "<" && version_compare($installed_glibc, $match[2]) <= 0)
									|| $this->ver_equal($installed_glibc, $glibc)) {

								// Glibc version is OK
								$glibc_OK = true;
								break;
							}
						}
					}
					if(!$glibc_OK) {
						$error = "GLIBC version: $installed_glibc";
					}
				}

				if(!isset($error) && isset($sysconf["release"])) {
					$release_arr = $sysconf["release"];
					$release_OK = true;
					foreach($release_arr as $release) {
						$release_OK = false;
						if(preg_match("/^([\>\<]?)(.*)$/", $release, $match)) {
							$installed_release = $this->get_release_version();
							if(  ($match[1] == ">" && version_compare($installed_release, $match[2]) >= 0)
									|| ($match[1] == "<" && version_compare($installed_release, $match[2]) <= 0)
									|| $this->ver_equal($installed_release, $release)) {

								// Release is OK
								$release_OK = true;
								break;
							}
						}
					}
					if(!$release_OK) {
						$error = "OS release: $installed_release";
					}
				}

				if(!isset($error) && isset($sysconf["machine"])) {
					$machine_arr = $sysconf["machine"];
					$machine_OK = true;
					foreach($machine_arr as $machine) {
						$machine_OK = false;
						if($machine == $this->conf['uname']['machine']) {
							// Processor type is OK
							$machine_OK = true;
							break;
						}
					}
					if(!$machine_OK) {
						$error = "processor type: ".$this->conf['uname']['machine'];
					}
				}
			}
		}

		if(isset($error)) {
			$this->yesnobox("The installation detected that this package is not compatible with your\nsystem configuration.\n".
					"It seems that you have an unsupported $error\n".
					"Do you still wish to continue the installation?\n".
					"For list of supported configurations, please refer to the Zend Product\ncompatibility table.", true, true);
		}
	}

	// ------------------
	// BASIC FUNCTIONS
	// ==================

	// CLRSCR
	function clrscr()
	{
		$this->run_command("clear");
	}
	
	// WELCOME_BOX
	function welcome_box($msg="", $addtext="")
	{
		if(!isset($this->conf['dialog'])) { // Clear screen before showing the "welcome" message
			$this->clrscr();
		}

		if($msg == ""){
			$msg = "Welcome to the ".$this->conf['product']." ".$this->conf['version']." Installation!\n\n";

			// by default, we assume there is Installation Guide
			if ($this->conf['install_guide'] == 1)
			  {
			    $msg .= "For more information regarding this procedure, please see the\n";
			    $msg .= $this->conf['product']." Installation Guide.\n";
			  }
			if(!empty($addtext)){
				$msg .= "\n$addtext";
			}
		}
		$this->msgbox($msg);
	}

	// BYE_BYE
	function bye_bye($url="", $msg="", $need_restart=true, $know_port=false)
	{
		$this->save_answers_file();
		
		$this->clrscr();

		print "\n\n\n\n\n\n\n";
		print "******************************************************************************\n";
		print "\n";
		if(!empty($msg)){
			print "{$msg}\n";
		}
		if(!$this->conf['license_installed']){
			print "  [7;1mYou must [0m[37;44;1mINSTALL the license[0m[7;1m files from ".
				"Zend.com to your installation directory[0m\n";
			print "\n";
		}
		if($need_restart){
			if(isset($this->conf['php_type']) && $this->conf['php_type'] == "fastcgi"){
				print "  [7;1mYou must [0m[37;44;1mRESTART your FastCGI[0m[7;1m for the ".
					"modifications to take effect[0m\n";
				print "\n";
			}
			else{
				print "  [7;1mYou must [0m[37;44;1mRESTART your ".$this->conf['webserver']."[0m[7;1m for the ".
					"modifications to take effect[0m\n";
				print "\n";
			}
		}
		if(!empty($url)) {
			print "  You can access the ".$this->conf['product']."'s User Interface using the URL:\n";
			print "  $url\n";
			print "\n";

			if(!$know_port && !preg_match("@:\d+/@", $url)){
				$url_with_port = preg_replace("@://([^/]*)/@", "://\\1:<port-number>/", $url);

				print "  If your Web server is running on port different from the default\n";
				print "  port 80, you should access the ".$this->conf['product']." as\n";
				print "  $url_with_port\n";
				print "\n";
			}
		}
		print "\n";
		print "******************************************************************************\n\n\n\n\n\n";

		$this->logger->log ('Bye bye ... :)');
	}

	// RUN_COMMAND
	function run_command($cmd, $err_msg="", $need_logging=true)
	{
		$tmpfile = tempnam ($this->conf['tmp_dir'], "cmd");
		if ($tmpfile) {
			$fp = popen("$cmd 2> {$tmpfile}", "w");
		} else {
			$fp = popen("$cmd 2> /dev/null", "w");
		}
		if(empty($fp)){
			$this->on_error 
			  ("Can't execute command: $cmd" . 
			   (strlen($err_msg) > 0 ? "\nERROR: $err_msg" : ''));
		}

		$status = $this->safe_pclose($fp);
		if ($need_logging) {
			$this->logger->log ("Executing: \"$cmd\" (status: $status)");
			if ($status != 0 && $tmpfile) {
				$error = @trim($this->file2str ($tmpfile));
				if (@strlen ($error) > 0) {
				  $this->logger->log ("Error was: {$error}");
				}
				@unlink ($tmpfile);
			}
		}
		return $status;
	}

	// NICE_PATH
	function nice_path($path) /* removes all multiple slashes */
	{
		/* remove multiple slashes from the path */
		$path = preg_replace("/\/\/\/*/", "/", $path);				/* multiple '/' */
		$path = preg_replace("/^\.\//", "", $path);	         /* first './' from the path */
		$path = preg_replace("/\/\.\//", "/", $path);        /* '/./' from the path */
		$path = preg_replace("/^[^\/]+\/\.\.\/?/", "", $path);   /* dir/.. from the beginning */
		$path = preg_replace("/\/[^\/]+\/\.\.\/?/", "/", $path); /* /dir/.. from the path */
		$path = preg_replace("/([^\/])\/+$/", "\\1", $path); /* last / from the end of path */

		return $path;
	}

	// ADD_DASHES
	function add_dashes($msg)
	{
		$dashes_len = 0; /* define dashes line under the menu title */
		$tmp_len = 0;

		for($i=0; $i<strlen($msg); $i++){
			if($msg[$i] == "\n" || $msg[$i] == "\r"){
				if($tmp_len > $dashes_len){
					$dashes_len = $tmp_len;
					$tmp_len = 0;
				}
			}
			else{
				$tmp_len ++;
			}
		}

		if($dashes_len == 0){
			$dashes_len = $tmp_len;
		}

		if($dashes_len > $this->conf['wrap_size']){
			$dashes_len = $this->conf['wrap_size'];
		}

		$dashes = str_repeat("-", $dashes_len);
		return "$msg\n$dashes";
	}

	// MY_DU
	function my_du($filename)
	{
		$total_size = 0;
		if($this->my_dir_exists($filename)) {
			$dir = $this->my_opendir($filename);
			while($file = readdir($dir)){
				if($file == "." || $file == ".."){
					continue;
				}
				$total_size += $this->my_du($this->make_path($filename, $file));
			}
			closedir($dir);
		}
		else if($this->my_file_exists($filename)) {
			$total_size = filesize($filename);
		}

		return $total_size;
	}

	// SIZE_HUMAN
	function size_human($size)
	{
		if($size > 1048576) {
			return round($size/1048576, 1)."MB";
		}
		else if($size > 1024) {
			return round($size/1024, 1)."KB";
		}
		else return round($size, 1)." bytes";
	}

	// EXPAND_TILDE
	function expand_tilde($path) /* converts tilde to a real path */
	{
		if(preg_match("/^~(.*)$/", $path, $match1)) {
			if(preg_match("/^([\w]+)(.*)$/", $match1[1], $match2)) {
				$pwd = $this->run_silently("posix_getpwnam", $match2[1]);
				if(!empty($pwd)) {
					$path = $this->make_path($pwd["dir"], $match2[2]);
				}
			}
			else {
				$path = $this->make_path(getenv("HOME"), $match1[1]);
			}
		}

		return $path;
	}

	// IS_EMAIL_VALID
	function is_email_valid($email)
	{
		return preg_match("/^(.+)\@(.+)\.(.+)$/", $email);
	}

	// IS_IP_VALID
	function is_ip_valid($ip)
	{
		if (preg_match ('/^\d+\.\d+\.\d+\.\d+$/', $ip) && @ip2long ($ip) !== -1) {
			return true;
		}
		return false;
	}

	// GET_BUILD_INFO
	function get_build_info()
	{
		return $this->conf['build_info'];
	}

	// INI_ARRAY_REBUILD
	function ini_array_rebuild($ini_arr,$translation_arr) {
		if (!is_array($ini_arr) or !is_array($translation_arr)) return array();
		if (empty($ini_arr) or empty($translation_arr)) return array();
		$result=array();
		foreach ($ini_arr as $directive=>$value) {
			foreach ($translation_arr as $prefix=>$translated_prefix) {
				$prefix_length=strlen($prefix);
				if (strncmp($prefix,$directive,$prefix_length)==0) {
					$translated_directive=$translated_prefix.substr($directive,$prefix_length);
					$result[$translated_directive]=$value;
				}
			}
		}
		return $result;
	}

	// VER_EQUAL
	function ver_equal($version1, $version2, $from=0)
	{
		$ver_arr1 = preg_split("/[\.\_]/", $version1);
		$ver_arr2 = preg_split("/[\.\_]/", $version2);

		if(count($ver_arr1) != count($ver_arr2)){
			return false;
		}
		for($i = 0; $i < count($ver_arr1); $i++){
			if(($ver_arr1[$i]!=$ver_arr2[$i]) && ($ver_arr1[$i]!='x') && ($ver_arr2[$i]!='x')){
				return false;
			}
			if(($ver_arr1[$i]!=$ver_arr2[$i]) && ($ver_arr1[$i]=='x') &&
				 ($from != 0) && ($ver_arr2[$i] < $from)){
				return false;
			}
		}

		return true;
	}

	// ARRAY2STRING
	function array2string($array, $index=-1)
	{
		$str = "";
		foreach ($array as $num => $data){
			if($index==-1){
				$str .= $data.$this->conf['nl'];
			}
			else{
				$str .= $data[$index].$this->conf['nl'];
			}
		}
		return $str;
	}

	// ARRAY2PHP_SUPPORT_STRING
	function array2php_support_string($array, $index=-1, $from_index=-1)
	{
		$str = "";
		foreach ($array as $num => $data){

			if($index==-1){
				$str .= $data.$this->conf['nl'];
			}
			else{
				if($from_index != -1 && $data[$from_index] != 0){
					$sub_str=substr($data[$index],0 ,(strlen($data[$index])-1) );
					$str .= $sub_str.$data[$from_index]." and above".$this->conf['nl'];
				}else{
					$str .= $data[$index].$this->conf['nl'];
				}
			}
		}
		return $str;
	}

	// WARN
	function warn($msg)
	{
		$this->infobox($msg);
	}

	// ON_ABORT
	function on_abort($msg = "", $dont_ask=false) /* die with error message */
	{
		if(!$this->conf['interactive']) {
			$dont_ask = true;
		}

		if(!$dont_ask && !$this->yesnobox("Do you really wish to abort the installation?", true)) {
			return;
		}

		$this->logger->log ("ABORT: $msg");
		$this->save_config();

		$this->php_ini_restore();

		if(strlen($msg) > 0) {
			$msg = "\n$msg";
		}
		$this->msgbox($this->conf['product']." installation was aborted.$msg");

		$this->cleanup();
		die(-1);
	}

	// DEFAULT_ABORT
	function default_abort()
	{
		$this->on_abort("The installation was NOT completed successfully.", true);
	}

	// CLEANUP
	function cleanup()
	{
		$this->logger->log ('Cleaning up ...');

		/* Save installation log */
		if (isset($this->conf['prefix'])) {
			$this->my_mkdir_long ($this->conf['prefix'].'/logs');
			//$this->logger->store ($this->conf['prefix'].'/logs');
			$this->my_copy ($this->logger->logfile_name, $this->conf['prefix'] . '/logs');
		}

		$this->my_delete($this->conf['tmp_dir']);
	}

	// SAVE_ANSWERS_FILE
	function save_answers_file()
	{
		$answers_file = $this->get_cmdline_arg("save-answers");
		$save_file = '';
		if ($answers_file) {
			foreach ($this->conf as $key => $val) {
				if (is_numeric ($val) || is_string ($val) && strlen ($val) < 20) {
					$save_file .= "{$key}={$val}\n";
				}
			}
			$this->str2file ($save_file, $answers_file);
		}
	}

	// SAVE_CONFIG
	function save_config()
	{
		if(!empty($this->conf['saved_config'])) {
			$this->str2file(serialize($this->conf['saved_config']), "install.sav");
		}
	}

	// RESTORE_CONFIG
	function restore_config()
	{
		if($this->my_file_exists("install.sav")) {
			$this->conf['saved_config'] = unserialize($this->file2str("install.sav"));
		}
		else $this->conf['saved_config'] = array();
	}

	// SEARCH_DIR
	function search_dir($dirs_array)
	{
		foreach ($dirs_array as $dir){  /* searches for an existent directory
																			 in an array of paths */
			if($this->my_dir_exists($dir)){
				return $dir;
			}
		}
		return "";
	}

	// SEARCH_FILE
	function search_file($dirs_array, $files_arr)
	{
		foreach ($dirs_array as $dir){  /* searches for an existent directory and file
																			 in an array of paths */
			if($this->my_dir_exists($dir)){

				foreach($files_arr as $file){
					if($this->my_file_exists($this->make_path($dir, $file))){
						return $this->make_path($dir, $file);
					}
				}
			}
		}
		return "";
	}

	// SELECT_DIR
	function select_dir($msg, $default="", $create=false)
	{
		while(true){
			$dir = $this->choose_dir($msg, $default);
			$dir = $this->nice_path($dir);

			if(!$this->my_dir_exists($dir)){
				if($this->my_file_exists($dir)){
					$this->msgbox("\"$dir\" must be a directory!");
				}
				else{
					if ($create) {
						if ($this->yesnobox ("Directory: \"$dir\" doesn't exist!\nWould you like this directory to be created?")) {
							$this->my_mkdir_long ($dir);
							break;
						}
					}
					else $this->msgbox("Directory: \"$dir\" doesn't exist!");
				}
			}
			else{
				break;
			}
		}

		return $dir;
	}
	
	// SELECT_FILE_NOSTRIP
	function select_file_nostrip($msg, $default="", $accept_empty=false)
	{
		while(true) {
			$file = $this->inputbox($msg, $default, $accept_empty);
			if ((empty($file) || $file == -1) && $accept_empty) {
				return null;
			}
			$file = $this->nice_path($file);

			if(!$this->my_file_exists($file)){
				if($this->my_dir_exists($file)){
					$this->msgbox("\"$file\" must be a file!");
				}
				else{
					$this->msgbox("File: \"$file\" doesn't exist!");
				}
			}
			else{
				break;
			}
		}
		return $file;
	}
	
	// SELECT_FILE
	function select_file($msg, $default="", $accept_empty=false)
	{
		return $this->strip_symlink ($this->select_file_nostrip($msg, $default, $accept_empty));
	}
	
	// CHOOSE_DIR
	function choose_dir($msg, $default, $min_space_size=0, $illegal_chars=" \;\$\#\'\"")
	{
		while(true){
			$dir = $this->inputbox($msg, $default);
			$dir = $this->expand_tilde($dir);
			if(file_exists($dir)) {
				$dir = $this->strip_symlink($dir);
			}
			if(!preg_match("/^\//", $dir)){
				$this->msgbox("The path must be absolute!");
			}
			else if($illegal_chars && preg_match("/[".$illegal_chars."]/", $dir)) {

				$chars_str = implode(",", preg_split("//", trim(stripslashes($illegal_chars)), -1, PREG_SPLIT_NO_EMPTY));
				if($chars_str != "") {
					$chars_str = ": $chars_str and";
				}
				$chars_str .= " the whitespace character";

				$this->msgbox("The path contains illegal characters!\n"
					."It must not contain".addslashes($chars_str).".");
			}
			else{
				$dir = $this->nice_path($dir);
				if($min_space_size != 0) {
					$install_target = $dir;
					while(strlen($install_target) > 1 && !$this->my_dir_exists($install_target)) {
						$install_target = $this->my_dirname($install_target);
					}
					$actual_free_space = disk_free_space($install_target);
					if($actual_free_space < $min_space_size) {
					  $this->logger->log ("Directory $dir has a free disk space: " .
							     $this->size_human ($actual_free_space) . 
							     ' , but at least ' . 
							     $this->size_human ($min_space_size) . 
							     ' is needed');
					  $this->msgbox("The directory must have at least ".$this->size_human($min_space_size).
								" of free disk space.\nPlease provide another directory.");
						continue;
					}
				}
				break;
			}
		}
		return $dir;
	}

	// FSELECT
	function fselect ($path="/", $msg=null)
	{
		$this->progress_bar_stop();

		if (isset ($this->conf['dialog'])) {
			$file = $this->rundialog ('fselect', $path);
		} else {
			$file = $this->select_file ($msg, $file);
		}
		return $file;
	}
	
	// CHOOSE_PREFIX
	function choose_prefix($what, $default, $min_space_size=0, $illegal_chars=" \;\$\#\'\"")
	{
		return $this->choose_dir("Please specify the location for installing $what:\n", $default,
			$min_space_size, $illegal_chars);
	}

	// CHOOSE_INSTALL_PREFIX
	function choose_install_prefix($default)
	{
		if(isset($this->conf['default_prefix'])) {
			if(!$this->conf['interactive']) {
				return;
			}
			$default = $this->conf['default_prefix'];
		}
		$this->logger->log ('Choosing install prefix ...');

		$this->conf['prefix'] = $this->choose_prefix($this->conf['product'], $default,
				isset($this->conf['min_space_size']) ? $this->conf['min_space_size'] : 0);

		$this->conf['saved_config']['default_prefix'] = $this->conf['prefix'];
	}

	// USER2UID
	function user2uid($user)
	{
		$pw = posix_getpwnam($user);
		return $pw["uid"];
	}

	// UID2USER
	function uid2user ($uid)
	{
		$userinfo = @posix_getpwuid($uid);
		if($userinfo !== false) {
			return $userinfo["name"];
		}
		return false;
	}

	// LINK_STARTUP_DAEMON
	/* program must understand 'start' and 'stop' arguments */
	function link_startup_daemon($script, $name=null, $descr=null)
	{
		if (!$name) {
			$name = basename($script);
			$name = preg_replace ('@\..*$@', '', $name);
		}
		if (!$descr) {
			$descr = $name;
		}

		$err = null;

		if (@file_exists ('/etc/gentoo-release')) {
			$init_script = <<<EOF
#!/sbin/runscript

depend() {
	need net
}

start() {
	{$script} start
}

stop() {
	{$script} stop
}
 
restart() {
	{$script} restart
}

EOF;
			$target_script = "/etc/init.d/{$name}.Zend";
			$this->str2file ($init_script, $target_script);
			$this->set_permissions ($target_script, "0755");
			$cmd = "rc-update add {$name}.Zend default";
			if ($this->run_command ("{$cmd} 1>/dev/null") != 0) {
				$err = "Cannot run: {$cmd}";
			}
		}
		else if (PHP_OS == "Linux" || PHP_OS == "SunOS") {
			// Detect default init level to determine the RC directory where to install the script
			if(preg_match ('/(\d+):initdefault/', $this->file2str('/etc/inittab'), $match)) {
				$rc_dir = '/etc/rc'.$match[1].'.d';
				if (!$this->my_dir_exists ($rc_dir)) {
					$rc_dir = '/etc/init.d/rc'.$match[1].'.d';
				}
				if (!$this->my_dir_exists ($rc_dir)) {
					$rc_dir = '/etc/rc.d/rc'.$match[1].'.d';
				}
			}
			if (isset ($rc_dir)) {
				$target_script = $rc_dir.'/S'.$name.".Zend";
				if (!$this->my_symlink ($script, $target_script)) {
					$err = "Cannot symlink {$target_script} to {$script}";
				}
			} else {
				$err = "Cannot find init scripts directory";
			}
		}
		else if (PHP_OS == "FreeBSD") {
			$rc_dir = '/usr/local/etc/rc.d';
			$target_script = $rc_dir.'/S'.$name.".Zend.sh";
			if (!$this->my_symlink ($script, $target_script)) {
				$err = "Cannot symlink {$target_script} to {$script}";
			}
		}
		else if (PHP_OS == "Darwin") {
			$rc_dir = '/Library/StartupItems/Zend'.$name;
			if(!$this->my_mkdir_long ($rc_dir)) {
				$err = "Cannot create directory {$rc_dir}";
			}	
                        $this->my_copy ($script, '/Library/StartupItems/Zend'.$name.'/'.basename($script));

                        $this->str2file (
                                "{\n".
                                "       Description     = \"$descr\";\n".
                                "       Provides        = (\"$script\");\n".
				"       Requires        = (\"Network\");\n".
                                "       OrderPreference = \"None\";\n".
                                "       Messages =\n".
                                "       {\n".
                                "               start = \"Starting $name\";\n".
                                "               stop  = \"Stopping $name\";\n".
                                "       };\n".
                                "}",
                                '/Library/StartupItems/Zend'.$name.'/StartupParameters.plist');
		} else{
			$err = "Automatic loading is not supported for this platform";
		}

		if ($err !== null) {
			$this->msgbox (
				"Cannot register $script to start automatically when the system boots\n\n".
				"ERROR: {$err}\n\n".
				"You will have to fix this problem after the installation is complete"
			);
		}
	}

	// CONSTRUCT_CRONTAB_LINE
	function construct_crontab_line($min_frequency, $args)
	{
		if($min_frequency <= 0 || $min_frequency > 60){
		  $this->on_error ('Frequency must be in range between 1 and 60');
		}

		$crontab_line = "";
		for($i=0; $i<60; $i += $min_frequency){
			$crontab_line .= "$i";
			if($i < 60-$min_frequency){
				$crontab_line .= ",";
			}
		}

		$crontab_line .= " * * * *  $args";
		$this->logger->log ("Constructing a crontab line: $crontab_line");

		return $crontab_line;
	}

	
	// --------------------------
	// FILE FUNCTIONS
	// ==========================

	// MY_MOVE
	function my_move($oldname, $newname, $delete=false)
	{
		if($delete){ /* delete destination before moving */
			$this->my_delete($newname);
		}

		$this->my_mkdir_long (dirname($newname));

		if($this->my_file_exists($this->strip_symlink($oldname))){
			$this->logger->log ("Moving: $oldname to: $newname");
			return @rename($oldname, $newname);
		}
		else if($this->my_dir_exists($oldname)){
			$this->my_dir_move($oldname, $newname);
		}
		else{
		  $this->on_error 
		    ("Cannot rename file: \"$oldname\" (it's neither a file nor a directory).");
		}
	}

	// MY_DIR_MOVE
	function my_dir_move($oldname, $newname)
	{
		if(!$this->my_dir_exists($oldname)){
		  $this->on_error ("File: $oldname should be a directory.");
		}
		$this->run_command("mv $oldname $newname");
	}

	// MY_TOUCH
	function my_touch($file)
	{
		$dir = dirname($file);
		if(!$this->my_dir_exists($dir)){
			$this->my_mkdir_long($dir);
		}

		if(!$this->my_file_exists($file)){
			$this->logger->log ("Creating file: $file");
			if(!$this->run_silently("touch", $file)){
			  $this->on_error ("Cannot create file: $file");
			}
		}
	}

	// MY_COPY
	function my_copy ($oldname, $newname, $follow_symlinks=true, $overrite=true)
	{
		if($this->my_dir_exists($newname)){
			$newname = $newname.$this->conf['slash'].basename($oldname);
		}

		if ($this->my_file_exists ($newname)) {
		  if (! $overrite)
		    {
		      $this->logger->log ("Not overriting $newname with $oldname");
		      return;
		    }
			$this->my_rename ($newname, "$newname.old", false);
			$this->my_unlink ("$newname.old", false); // Unlink the old file
		} else {
			$this->my_delete ($newname);
		}

		$this->my_mkdir_long (dirname($newname));

		if (!$follow_symlinks && $this->my_link_exists($oldname)) {
			$dest = readlink ($oldname);
			if ($dest !== false) {
				$this->my_symlink ($dest, $newname);
			}
		}
		else if($this->my_file_exists($this->strip_symlink($oldname))){
			$perms = fileperms($oldname);
			$owner = fileowner($oldname);
			$group = filegroup($oldname);

			if(!$this->my_is_writable($newname)){
				$this->infobox("Cannot overwrite: $newname");
			}
			else{
				if (!$this->run_silently("copy", $oldname, $newname)) {
					$this->infobox("Cannot copy: $oldname to: $newname");
				}
				else{
					$this->logger->log ("Copying: $oldname to: $newname");

					$this->set_permissions($newname, decoct($perms), $owner, $group);
				}
			}
		}
		else if($this->my_dir_exists($oldname)){
			$this->my_dir_copy($oldname, $newname, $follow_symlinks);
		}
		else{
		  $this->on_error
		    ("Cannot copy file: \"$oldname\" (it's neither a file nor a directory).");
		}
	}

	// MY_DIR_COPY
	function my_dir_copy($oldname, $newname, $follow_symlinks=true)
	{
		$this->my_mkdir($newname);
		$dir = $this->my_opendir($oldname);
		while($file = readdir($dir)){
			if($file == "." || $file == ".."){
				continue;
			}
			if($this->my_dir_exists($oldname.$this->conf['slash'].$file)
				&& $this->my_dir_exists($newname.$this->conf['slash'].$file)) {
				$this->my_dir_copy($oldname.$this->conf['slash'].$file, $newname.$this->conf['slash'].$file, $follow_symlinks);
			}
			else $this->my_copy($oldname.$this->conf['slash'].$file, $newname.$this->conf['slash'].$file, $follow_symlinks);
		}
		closedir($dir);
	}

	// MY_IS_WRITABLE
	function my_is_writable($file)
	{
		if(!$this->my_file_exists($file)){
			return true;
		}

		$fp = $this->run_silently("fopen", $file, "w");
		if($fp == false){
			return false;
		}
		else{
			fclose($fp);
			return true;
		}
	}

	// MY_SYMLINK
	function my_symlink($from, $to)
	{
		$this->my_unlink($to);

		$this->logger->log ("Symlinking $to -> $from");
		return $this->run_silently("symlink", $from, $to);
	}

	// MY_DELETE
	function my_delete($file) /* recursive deletion of directories and files */
	{
		if($this->my_file_exists($file) || $this->my_link_exists($file)){
			$this->logger->log ("Deleting: $file");
			$this->my_unlink($file);
		}
		else if($this->my_dir_exists($file)){
			$this->logger->log ("Removing directory: $file");
			$this->run_command("rm -rf $file");
		}
	}

	// MY_MKDIR_SILENTLY
	function my_mkdir_silently($dir)
	{
		return $this->my_mkdir_long($dir, true);
	}

	// MY_MKDIR_LONG
	function my_mkdir_long($dir, $silent = false)
	{
		if(!$this->my_dir_exists($dir)){
			if ($this->is_broken_link ($dir)) {
				$this->my_unlink ($dir);
			}
			$this->run_command("mkdir -p '$dir'", "Cannot create directory: $dir");
		}
		return true;
	}

	// MY_MKDIR
	function my_mkdir($dir, $silent = false)
	{
		if(!$this->my_dir_exists($dir)){
			$this->logger->log ("Creating directory: $dir");
			if(!$this->run_silently("mkdir", $dir)){
				if(!$silent){
				  $this->on_error ("Cannot create directory: $dir");
				}
				return false;
			}
		}

		return true;
	}

	// IS_BROKEN_LINK
	function is_broken_link($file)
	{
		return ($this->strip_symlink($file) == "");
	}

	// MY_LIST_DIR
	function my_list_dir($path, $pattern)
	{
		if(function_exists('glob')) {
			return glob($this->make_path($path, $pattern));
		}

		$files_list = array();
		$dir = $this->my_opendir($path);
		while($file = readdir($dir)){
			if($file == "." || $file == ".."){
				continue;
			}
			if($this->my_fnmatch($pattern, $file)){
				array_push($files_list, $this->nice_path("$path/$file"));
			}
		}
		closedir($dir);

		return $files_list;
	}

	// MY_BACKUP
	function my_backup($file, $backup_link_dest=false)
	{
		if($this->my_link_exists($file)){ /* don't backup links */
			if($backup_link_dest){
				$dest = $this->strip_symlink($file);
				if(empty($dest)){
					$this->my_unlink($file);
				}
				else{
					$this->logger->log ("Backing-up link destination: $dest");
					$this->my_copy($dest, $dest.$this->conf['backup_suffix']);
					$new_file = $dest.$this->conf['backup_suffix'];
				}
			}
			else{
				$this->logger->log ("Not backing-up a link: $file");
			}
		}
		elseif($this->my_file_exists($file)){
			$this->logger->log ("Backing-up file: $file");
			$this->my_copy($file, $file.$this->conf['backup_suffix']);
			$new_file = $file.$this->conf['backup_suffix'];
		}
		elseif($this->my_dir_exists($file)){
			$this->logger->log ("Backing-up directory: $file");
			$this->my_move($file, $file.$this->conf['backup_suffix'], true);
			$new_file = $file.$this->conf['backup_suffix'];
		}

		return isset($new_file) ? $new_file : null;
	}

	// MY_FNMATCH
	function my_fnmatch($pattern, $file)
	{
		if(function_exists('fnmatch')) {
			return fnmatch ($pattern, $file);
		}

		$lenpattern = strlen($pattern);
		$lenfile    = strlen($file);

		for($i=0 ; $i<$lenpattern ; $i++)
		{
		 if($pattern[$i] == "*")
		 {
			 for($c=$i ; $c<max($lenpattern, $lenfile) ; $c++)
			 {
				 if($this->my_fnmatch(substr($pattern, $i+1), substr($file, $c)))
						 return true;
			 }
			 return false;
		 }

		 if($pattern[$i] == "[")
		 {
			 $letter_set = array();
			 for($c=$i+1 ; $c<$lenpattern ; $c++)
			 {
				 if($pattern[$c] != "]")
					 array_push($letter_set, $pattern[$c]);
				 else
					 break;
			 }
			 foreach($letter_set as $letter)
			 {
				 if($this->my_fnmatch($letter.substr($pattern, $c+1), substr($file, $i)))
					 return true;
			 }
			 return false;
		 }

		 if($pattern[$i] == "?") continue;
		 if($pattern[$i] != $file[$i]) return false;
		}

		if(($lenpattern != $lenfile) && ($pattern[$i - 1] == "?")) return false;
		return true;
	}
	
	// MY_SCANDIR
	function my_scandir($dir)
	{
		$files = array();
		if($this->my_dir_exists ($dir)) {
			$fh = opendir($dir);
			while (false !== ($filename = readdir($fh))) {
				array_push($files, $filename);
			}
			closedir($fh);
		}
		return $files;
	}

	// FIND_FILE
	function find_file($dir, $pattern, $ret_arr=null)
	{
		$num_files = 0;
		if($this->my_dir_exists ($dir)) {
			$files = preg_grep ("/^\.+$/", $this->my_scandir ($dir), PREG_GREP_INVERT);;
			foreach ($files as $file) {
				$num_files += $this->find_file ($this->make_path($dir, $file), $pattern, $ret_arr);
			}
		}
		else if($this->my_file_exists ($dir)) {
			if($this->my_fnmatch($pattern, basename($dir))) {
				if(is_array($ret_arr)) {
					array_push ($ret_arr, $dir);
				}
				return 1;
			}
		}
		return $num_files;
	}

	// FILECMP
	function filecmp($file1, $file2)
	{
		return strcmp(file2str($file1), file2str($file2));
	}

	// STR2FILE
	function str2file($str , $file)
	{
		$fp = $this->my_fopen($file, "w");
		$ret = fwrite($fp, $str);
		fclose($fp);

		return $ret;
	}

	// FILE_WORD_WRAP
	function file_word_wrap($file)
	{
		$this->logger->log ("Wrapping text from file: $file");

		$tmp_file = $this->conf['tmp_dir'].$this->conf['slash'].basename($file).".wrapped";
		$this->str2file(wordwrap($this->file2str($file), $this->conf['wrap_size']), $tmp_file);
		return $tmp_file;
	}

	// MY_OPENDIR
	function my_opendir($path)
	{
		if(!$this->my_dir_exists($path)){
			$this->on_error("No such directory: $path");
		}
		$dir = $this->run_silently("opendir", $path);
		if(empty($dir)){
			$this->on_error("Cannot open directory: $path for reading");
		}

		return $dir;
	}

	// PUSHD
	function pushd($dir)
	{
		$cwd = getcwd();
		if(empty($cwd)){
			$this->on_error("Unable to store the current working directory.");
		}
		if(!isset($this->conf['cwd'])) {
			$this->conf['cwd'] = array();
		}
		array_push($this->conf['cwd'], $cwd);

		$this->my_chdir($dir);
	}

	// POPD
	function popd()
	{
		if(!isset($this->conf['cwd'])) {
			$error = "Current directory is not stored";
		}
		$cwd = array_pop($this->conf['cwd']);
		if(!chdir($cwd)){
			$error = "Can't change directory to: $cwd";
		}
		if(isset($error)) {
			$this->on_error("Unable to restore the current working directory.\n$error");
		}
	}

	// MY_CHDIR
	function my_chdir($dir, $log=false)
	{
		if ($log) {
			$this->logger->log ("Changing directory to: $dir");
		}
		if(!chdir($dir)){
			$this->on_error("Unable to change directory to: $dir");
		}
	}

	// GET_FILE_TREE
	function get_file_tree($dir, &$file_tree)
	{
		$totalsize = 0;
		$file_tree = array();
		$this->get_file_tree_a($dir, $file_tree, $totalsize);
		return $totalsize;
	}

	function get_file_tree_a($path, &$file_tree, &$totalsize) /* returns number of files */
	{
		$dir = $this->my_opendir($path);
		$num_files = 0;

		while($file = readdir($dir)){
			if($file != "." && $file != ".."){
				if($this->my_file_exists("$path/$file")){
					$size = filesize($path.$this->conf['slash'].$file);
					$file_tree[$path.$this->conf['slash'].$file] = $size;
					$totalsize += $size;
				}
				elseif($this->my_dir_exists($path.$this->conf['slash'].$file)){
					if($this->get_file_tree_a($path.$this->conf['slash'].$file, $file_tree, $totalsize)==0){
						$file_tree[$path.$this->conf['slash'].$file] = 0; /* if directory is empty put it into
																																 components array */
					}
				}
				$num_files ++;
			}
		}

		closedir($dir);
		return $num_files;
	}


	// ------------------------------
	// DIALOG FUNCTIONS
	// ==============================

	// MY_READLINE
	function my_readline($msg = "[To continue, press Enter]")
	{
		print($msg);
		flush();
		if($this->conf['interactive']) {
			exec("read OUTPUT\necho \$OUTPUT", $output);
		}
		print ("\n");
		return isset($output[0]) ? $output[0] : "";
	}

	// RUNDIALOG
	function rundialog($mode, $msg, $extra_args=null)
	{
		$this->progress_bar_stop();

		$command = $this->conf['dialog'];

		# Strip not always displayable HTML and PHP tags:
		$msg = strip_tags ($msg);

		# remove ascii escape sequences(not suitable for dialog)
		$msg = preg_replace('/\\033[0-9;\[]*m/', '', $msg);

		# If message is empty escapeshellarg() returns empty string, instead of expected ''
		$msg = strlen($msg) ? $msg : " ";

		switch($mode)
		{
			case 'fselect':
			case 'msgbox':
			case 'inputbox':
			case 'passwordbox':
			case 'noyes':
				if($mode == "noyes") {
					$command .= " --defaultno ";
					$mode = "yesno";
				}
			case 'yesno':
			case 'infobox':
				if ($mode == "infobox") $command .= " --sleep 1 ";
			case 'menu':
			case 'checklist':
				$command .= sprintf(" --%s %s 0 0", $mode, escapeshellarg("{$msg}\n"));
				break;
			case 'textbox':
				$command .= sprintf(" --%s %s 0 0", $mode, escapeshellarg($msg));
				break;
			default:
				print("Unsupported dialog option\n");
				return('');
		}

		if (is_array ($extra_args)) {
			foreach ($extra_args as $arg) {
				$arg = strlen($arg) ? $arg : " ";
				$command .= " ".escapeshellarg ($arg);
			}
		} else if ($extra_args !== null) {
			$extra_args = strlen($extra_args) ? $extra_args : " ";
			$command .= " ".escapeshellarg ($extra_args);
		}

		$rc = $this->dialog_execute($command);
		if((($mode == 'passwordbox') || ($mode == 'inputbox')) && ($rc>0)) {
			// Cancel pressed.
			return -1;
		}

		$dialog_output = $this->dialog_get_output();
		if(preg_match ('/Error opening terminal/', $dialog_output)) {
			unset($this->conf['dialog']);
			$this->on_error($this->conf['dialog_error']);
		}

		return($mode == 'yesno' ? 1 - $rc : $dialog_output);
	}

	function floating_box($msg) {
	}
	
	// MSGBOX
	function msgbox($msg)
	{
		$this->progress_bar_stop();

		$this->logger->log ("MSGBOX: \n\"$msg\"");

		if(!empty($this->conf['dialog'])){
			$this->rundialog("msgbox", "\n{$msg}");
		}
		else{
			print("\n$msg\n");
			$this->my_readline();
		}
	}

	// INPUTBOX
	function inputbox($msg, $init="", $accept_empty=false, $show_empty_msg=true)
	{
		$this->progress_bar_stop();

		$this->logger->log ("ASK: $msg");
		do
		{
			if(!empty($this->conf['dialog'])){
				while(true) {
					if ($show_empty_msg && $accept_empty && $init == "") {
						$msg .= "\n\nPress OK to leave empty.";
					}
					$input = $this->rundialog("inputbox", "\n{$msg}", $init);
					if(!$accept_empty && $input == -1) {
						$this->on_abort();
						continue;
					}
					break;
				}
			}
			else{
				// replace last ':' if it exits and trim :
				$msg = preg_replace("/\:\s*$/", "", $msg);

				if($show_empty_msg && $accept_empty && $init == ""){
					$init = "Press ENTER to leave empty";
				}

				$prompt = "{$msg}: ";
				if (!empty ($init)) {
					if (preg_match ('/\n$/', $msg)) {
						$prompt = "{$msg}[{$init}]: ";
					} else {
						$prompt = "{$msg} [{$init}]: ";
					}
				}
				$input = $this->my_readline ($prompt);

				if($input == "" && $init != "Press ENTER to leave empty") {
					$input = $init;
				}
			}

			$this->logger->log ("INPUTBOX: $input");
		}
		while((empty($input) || preg_match("/^\s*$/", $input)) && !$accept_empty);

		$input = trim($input);        /* trim the line */
		$input = addslashes($input);  /* add the back slashes to illegal characters */

		return $input;
	}

	// INPUTBOX_BOUNDED
	function inputbox_bounded($msg, $what, $min_chars, $max_chars, $init="", $accept_empty=false)
	{
		while(true)
		{
			$output = $this->inputbox($msg, $init, $accept_empty);

			if(strlen($output) < $min_chars) {
				$this->msgbox("A $what should contain at least $min_chars characters. Please try again.");
				continue;
			}
			if(strlen($output) > $max_chars) {
				$this->msgbox("A $what should contain at most $max_chars characters. Please try again.");
				continue;
			}

			break;
		}

		return $output;
	}

	// YESNOBOX
	function yesnobox($msg, $defaultno=false, $abort_on_no=false)
	{
		$this->progress_bar_stop();

		$this->logger->log ("YESNO: $msg");

		$answer = false;
		$default = ($defaultno ? "NO" : "YES");

		while(true) {
			if(!empty($this->conf['dialog'])){
				if($defaultno) {
					$answer = $this->rundialog("noyes", "\n{$msg}");
				} else {
					$answer = $this->rundialog("yesno", "\n{$msg}");
				}
			}
			else{
				print("\n$msg\n");
				do{
					$output = $this->my_readline("Answer (yes or no) [$default]: ");
					if(preg_match("/^\s*$/", $output)){
						$output = $default;
					}
					if(strcasecmp($output, "yes") == 0 ||
					   strcasecmp($output, "y") == 0){
						$answer = true;
						break;
					}
					if(strcasecmp($output, "no") == 0 ||
					   strcasecmp($output, "n") == 0){
						$answer = false;
						break;
					}
				}while(true);
			}

			if($abort_on_no && !$answer) {
				$this->on_abort();
				continue;
			}

			print("\n");
			break;
		}

		$result = $answer ? "YES" : "NO";
		$this->logger->log ("User choose: $result");
		return $answer;
	}

	// INFOBOX
	function infobox($msg)
	{
		$this->progress_bar_stop();

		$this->logger->log ("INFOBOX: \n\"$msg\"");

		if(!empty($this->conf['dialog'])){
			$this->rundialog("infobox", "\n{$msg}");
		}
		else{
			print("\n$msg\n\n");
		}
	}

	// PASSWDBOX
	function passwdbox($msg, $accept_cancel=false)
	{
		$this->progress_bar_stop();

		do{
			if(!empty($this->conf['dialog'])){
				while(true) {
					$passwd = $this->rundialog("passwordbox", $msg);
					if($passwd == -1 && !$accept_cancel) {
						$this->on_abort();
						continue;
					}
					break;
				}
			}else{
				print("\n$msg");
				exec("stty -echo");
				$passwd = $this->my_readline("");
				exec("stty echo");
				//print("\n");
			}
		}
		while(empty($passwd) || preg_match("/^\s*$/", $passwd));

		$this->logger->log ("PASSWDBOX: " . preg_replace ("/\w/", "X", $passwd));

		return($passwd);
	}

	// PASSWORD_CHECK
	function password_check($passwd, $num_chars=4)
	{
		if(strlen($passwd) < $num_chars){
			$this->msgbox("A password should contain at least $num_chars characters. Please try again.");
			return false;
		}
		if(0 && (urlencode($passwd) != $passwd) or (strpos($passwd, "_") !== false)){
			$this->msgbox("Illegal password. The password may contain only:\n\n"
			."a) The alphanumeric characters 'a' through 'z',\n   'A' through 'Z' and '0' through '9'\n"
			."b) The special characters '.' and '-'\n\n"
			."Please try again.\n");
			return false;
		}
		return true;
	}

	// PASSWORD_GET
	function password_get($msg="", $num_chars=4, $accept_cancel=false)
	{
		while(true) {
			$passwd = $this->passwdbox("$msg\n\nPassword: ", $accept_cancel);
			if($passwd != -1 && !$this->password_check($passwd, $num_chars)) {
				continue;
			}
			break;
		}
		return $passwd;
	}

	// PASSWORD_CHOOSE
	function password_choose($msg="", $num_chars=4, $accept_cancel=false)
	{
		while(true){
			$passwd = $this->password_get($msg, $num_chars, $accept_cancel);
			if($passwd == -1) {
				return $passwd;
			}
			$checkpasswd = $this->passwdbox("Verify the password: ");

			if(strcmp($passwd, $checkpasswd) == 0){
				printf ("\n");
				break;
			}
			$this->msgbox("Passwords did not match, please try again.");
		}
		return $passwd;
	}

	// MENUBOX
	function menubox($msg, $menu_arr_orig, $allow_blank=false, $abort_msg="", $default=null)
	{
		$this->progress_bar_stop();

		$this->logger->log ("MENU: $msg\n:" . var_export ($menu_arr_orig, true));

		$temp_hash = array();
		$menu_arr = array();
		$index = 1;
		foreach($menu_arr_orig as $key=>$val){
			if($default !== null && $default == $key) {
				$default_index = $index;
			}
			$temp_hash[$index] = $key;
			$menu_arr[$index] = $val;
			$index ++;
		}
		$default = isset($default_index)?$default_index:null;

		if(!empty($this->conf['dialog'])) {
			$menu_params = array ('0');
			$item_len = 0;
			foreach ($menu_arr as $tag => $menu_item){
				$menu_params[] = $tag;
				$menu_params[] = $menu_item;
				if(strlen($menu_item) > $item_len){
					$item_len = strlen($menu_item);
				}
			}
			if($item_len > strlen($msg)-6){ /* message must be longer than items */
				$len_add = $item_len - strlen($msg);
				$len_add = (strlen($msg)+10 > $this->conf['wrap_size'])?
					$this->conf['wrap_size'] - strlen($msg) : $len_add + 10;
				$msg .= str_repeat(" ", $len_add);
			}

			$msg = wordwrap($msg, $this->conf['wrap_size']);
			while(true) {
				$choice = $this->rundialog("menu", "\n{$msg}", $menu_params);

				if(!$choice) {
					if($allow_blank) {
						$choice = "";
					}
					else {
						$this->on_abort($abort_msg);
						continue;
					}
				}
				else {
					$choice = $temp_hash[$choice];
				}
				break;
			}
		}
		else {
			$msg = $this->add_dashes($msg);

			$count_lines = 0;
			$str = "\n$msg\n\n";
			foreach ($menu_arr as $tag => $menu_item){
				$str .= "[$tag] $menu_item\n";
				$count_lines ++;
			}

			do
			{
				if($count_lines > 20){
					$tmp_file = $this->make_path($this->conf['tmp_dir'], "tmp.scroll");
					$this->str2file($str, $tmp_file);
					$this->show_file($tmp_file, false);
				}
				else{
					print($str);
				}

				while(true){

					$quest = "\nPlease select ";
					if ($default !== null) {
						$quest .= "[$default]";
					} else if ($allow_blank) {
						if($count_lines > 20){
							$quest .= "[S - show again, ENTER - leave empty]";
						} else {
							$quest .= "[Press ENTER to leave empty]";
						}
					} else if ($count_lines > 20) {
						$quest .= "[S - show again]";
					}
					$quest .= ": ";

					$output = $this->my_readline($quest);
					if($output == "S" || $output == "s") {
						break;
					}

					if(empty($output) && $default !== null) {
						$output = $default;
					}

					if(empty($output) && $allow_blank){ /* if ENTER was pressed */
						$choice = "";
						break;
					}
					if(isset($menu_arr[$output])){ /* if this value exists */
						$choice = $temp_hash[$output];
						break;
					}
					else {
						if(!empty($output)) {
							$this->infobox("Illegal input: $output");
						}
					}
				}
			}
			while(!isset($choice));

			print("\n");
		}

		$this->logger->log ("User's choice: $choice");
		return $choice;
	}

	// CHECKLIST
	/* 
	* @param string Text to display in the checklist box
	* @param array choices (key => choice)
	* @param array fired choices (choice => 1)
	* @return array fired choices (choice => 1)
	*/
	function checklist ($text, $choices, $default_choices=null)
	{
		if ($default_choices === null || !is_array($default_choices)) {
			$default_choices = array();
		}
		$retval = array();

		// Use integer counter for defining keys:
		$new_key = 1;
		$new_default_choices = array();
		foreach ($choices as $orig_key => $choice) {
			$new_choices[$new_key] = $choice;
			$keys_f[$new_key] = $orig_key;
			if (isset($default_choices [$orig_key])) {
				$new_default_choices [$new_key] = 1;
			}
			$new_key ++;
		}
		$default_choices = $new_default_choices;
		$choices = $new_choices;

		if (empty($this->conf["dialog"])) {
			// Display text:
                        print "\n{$text}\n\n";

                        // Until a legal option will be choosen
                        do {
                                // Display choices:
                                $line = "";
                                foreach ($choices as $key => $choice) {
                                        if (isset($default_choices [$key])) {
                                                $line .= sprintf ("[X] %d. %s\n", $key, $choice);
                                        } else $line .= sprintf ("[ ] %d. %s\n", $key, $choice);
                                }
                                print "{$line}\n";

                                // Read the answer:
                                $res = $this->my_readline ("[Change setting or press ENTER to continue]: ");

                                // On empty answer - finish setting values
                                if ($res === "") {
                                        foreach ($default_choices as $key => $value) {
                                                $retval [$keys_f[$key]] = $value;
                                        }
                                        break;
                                }

                                if (isset($keys_f[$res])) {
                                        if(isset($default_choices[$res])) {
                                                unset ($default_choices[$res]);
                                        } else $default_choices[$res] = 1;

                                        continue;
                                }
                                print "Illegal input: $res\n";
                        }
                        while (true);

		} else {
			$checklist_params = array('0');
			$item_len = 0;
			foreach ($choices as $key => $choice) {
				$checklist_params[] = $key;
				$checklist_params[] = $choice;
				$checklist_params[] = isset($default_choices [$key]) ? "On" : "Off";
				if(strlen($choice) > $item_len){
					$item_len = strlen($choice);
				}
			}
			if($item_len > strlen($text)-8){ /* message must be longer than the longer of items */
				$len_add = $item_len - strlen($text);
				$len_add = (strlen($text)+10 > $this->conf['wrap_size'])?
					$this->conf['wrap_size'] - strlen($text) : $len_add + 10;
				$text .= str_repeat(" ", $len_add);
			}

			$text = wordwrap($text, $this->conf['wrap_size']);
			while(true) {
				$res = $this->rundialog("checklist", "\n{$text}", $checklist_params);
				if(!$res) {
					return array();
				} else {
					$res = trim ($res, "\" \t");
					foreach (array_flip (explode ("\" \"", $res)) as $key => $val) {
						$key = trim ($key, "\" \t");
						$retval[$keys_f[$key]] = 1;
					}
				}
				break;
			}
		}

		return $retval;
	}

	// SHOW_FILE
	function show_file($file, $clean=true)
	{
		$this->progress_bar_stop();

		if(!$this->conf['interactive']) {
			return;
		}
		$file = $this->file_word_wrap($file);

		if(!empty($this->conf['dialog'])){
			$this->rundialog("textbox", $file);
		}
		else{
			$this->clrscr();
			popen("cat $file | more", "w");
			if($clean){
				$this->clrscr();
			}
		}
	}

	// PROGRESS_BAR_START
	/* start external process with dialog progress bar */
	function progress_bar_start($seconds, $message, $message_tty=null)	
	{
		$message = "\n\n".$message;
		if(!$message_tty){
			$message_tty = $message;
		}

		$pb_proc_env = array();
		$pb_proc_env["TIME"] = $seconds; /* set progress bar timeout */
		$pb_proc_env["MESSAGE"] = $message;
		$pb_proc_env["MESSAGE_TTY"] = $message_tty;
		$pb_proc_env["TITLE"] = $this->conf['title'];

		if(!empty($this->conf['dialog'])){
			$pb_proc_env["DIALOG"] = $this->conf['dialog']; /* set path to dialog */
		}

		$this->conf['progress_pid_file'] = $this->conf['tmp_dir'].$this->conf['slash']."progress_bar.pid";
		$this->conf['progress_fatal_file'] = $this->conf['tmp_dir'].$this->conf['slash']."kill_progress_bar";

		if (@file_exists ($this->conf['progress_fatal_file'])) {
			@unlink ($this->conf['progress_fatal_file']);
		}

		$pb_proc_env["PID_FILE"] = $this->conf['progress_pid_file'];
		$pb_proc_env["FATAL_FILE"] = $this->conf['progress_fatal_file'];

		$dspec = array(
				0 => array("pipe", "r"),
				2 => array("pipe", "w")
				);

		$proc_env = "";
		foreach ($pb_proc_env as $key => $val) {
			$val = strlen($val) ? $val : " ";
			$proc_env .= "{$key}=".escapeshellarg($val)." ";
		}

		if (isset($this->conf['php_run_cmd'])) {
			$php_cmd = $this->conf['php_run_cmd'];
		} else {
			$php_cmd = "./php -c ./php-install.ini";
		}
		touch ($this->conf['progress_pid_file']);
		$this->conf['progress_fp'] = proc_open ("{$proc_env} {$php_cmd} -q ".dirname(__FILE__)."/progress_bar.php", $dspec, $pipes);
		$this->conf['progress_bar_started'] = true;
	}

	// PROGRESS_BAR_STOP
	function progress_bar_stop() /* stop the progress bar by killing it */
	{
		if (!$this->conf['progress_bar_started']) {
			return;
		}

		while(!file_exists($this->conf['progress_pid_file'])); // Wait for process to create a file
		sleep(1);

		@touch($this->conf['progress_fatal_file']);
		@proc_close($this->conf['progress_fp']);

		if(empty($this->conf['dialog'])){
			print("\n");
		}
		$this->conf['progress_bar_started'] = false;

		sleep(1);
	}

	// DIALOG_CALC_MAX_SIZE
	function dialog_calc_max_size()
	{
		if(empty($this->conf['dialog'])){
			return -1;
		}

		$this->dialog_execute($this->conf['dialog']." --print-maxsize");

		$out = $this->dialog_get_output();

		if(preg_match("/.*:\s(.*),\s(.*)$/m", $out, $match)){
			$this->conf['max_y'] = $match[1];
			$this->conf['max_x'] = $match[2];
		}
	}

	// DIALOG_EXECUTE
	function dialog_execute($dialog_cmd)
	{
		if(empty($this->conf['dialog'])){
			$this->logger->log ('Tried to run a dialog utility, while it doesn\'t exist!');
			return -1;
		}

		$this->logger->log ($dialog_cmd);

		$fd = $this->my_popen("$dialog_cmd 2>". $this->conf['dialog_tmp'], "w", false);
		if(empty($fd)){
			$this->on_error($this->conf['dialog_error']);
		}
		$status = $this->safe_pclose($fd);
		return $status;
	}

	// DIALOG_GET_OUTPUT
	function dialog_get_output()
	{
		if(!empty($this->conf['dialog'])){
			return $this->file2str($this->conf['dialog_tmp']);
		}
		return false;
	}


	// -----------------------------
	// WEBSERVER FUNCTIONS
	// =============================

	// WEBSERVER_SUPPORTED_CHECK
	function webserver_supported_check($name)
	{
		if(empty($this->conf['supported_webservers'])){
			return true;
		}
		else{
			return array_search($name, $this->conf['supported_webservers']);
		}
	}

	// WEBSERVER_CHOOSE
	function webserver_choose($errmsg="")
	{
		if($this->conf['webserver'] != "Web server") {
			return true;
		}

		if(empty($this->conf['supported_webservers'])){
			return false;
		}

		if(count($this->conf['supported_webservers']) == 1){
			if($this->yesnobox("Are you using ".$this->conf['supported_webservers'][1]." Web server?")){
				$this->conf['webserver'] = $this->conf['supported_webservers'][1];
			}
			else{
				if(empty($errmsg)){
					$errmsg = "For now ".$this->conf['product']." supports only ".
						$this->conf['supported_webservers'][1]. " on ". $this->conf['uname']['sysname'];
				}
				$this->on_abort($errmsg, true);
			}
		}
		else{
			$rc = $this->menubox("Choose one of supported Web servers:", $this->conf['supported_webservers'], false, $errmsg);
			$this->conf['webserver'] = $this->conf['supported_webservers'][$rc];
		}
	}
	
	// WEBSERVER_ROOT_GUESS
	function webserver_root_guess()
	{
		if(empty($this->conf['webserver_root_dir'])){

			$webserver_root = "";
			if($this->conf['webserver'] == "Apache"){
				$webserver_root = $this->apache_root_guess();
			}
			else if($this->conf['webserver'] == "Zeus" && $this->my_dir_exists("/usr/local/zeus")) {
				$webserver_root = "/usr/local/zeus";
			}
			else if($this->conf['webserver'] == "SunONE" && $this->my_dir_exists("/opt/SUNWwbsvr")) {
				$webserver_root = "/opt/SUNWwbsvr";
			}

			$word = empty($webserver_root) ? "enter" : "confirm";
			$this->conf['webserver_root_dir'] = $this->select_dir("Please $word your ".$this->conf['webserver'].
					" root directory", $webserver_root);

			if($this->conf['webserver'] == "Zeus"){
				$this->conf['zeus_root'] = $this->conf['webserver_root_dir'];
			}
			else if($this->conf['webserver'] == "SunONE") {
				$this->conf['sunone_root'] = $this->conf['webserver_root_dir'];
			}
		}

		return $this->conf['webserver_root_dir'];
	}

	// WEBSERVER_DOCROOT_GUESS
	function webserver_docroot_guess()
	{
		if(isset($this->conf['webserver_doc_root']) && $this->my_dir_exists($this->conf['webserver_doc_root'])){
			return $this->conf['webserver_doc_root'];
		}

		$this->logger->log ('Trying to guess ' . $this->conf['webserver'] . ' docroot ...');

		$default = "";
		if($this->conf['webserver'] == "Apache"){
			$default = $this->apache_docroot_guess();
		}
		elseif($this->conf['webserver'] == "Zeus"){
			$default = $this->zeus_docroot_guess();
		}
		elseif($this->conf['webserver'] == "SunONE"){
			$default = $this->sunone_docroot_guess();
		}
		else{
			$default = "/var/www/html";
		}

		$manually_set = false;
		if(!isset($this->conf['webserver_doc_root'])) {
			$manually_set = true;
		}
		else if(!$this->my_dir_exists($this->conf['webserver_doc_root'])) {
			$this->msgbox("Directory:\n".$this->conf['webserver_doc_root']."\ndoesn't exist!");
			$manually_set = true;
		}

		if($manually_set) {
			$word = empty($default) ? "enter" : "confirm";
			$this->conf['webserver_doc_root'] = $this->select_dir(
					"Please $word the location of your ".$this->conf['webserver']." document root directory.\n".
					"This is the location where your web documents are stored", $default);

		}
		$this->logger->log ('Guessed: ' . $this->conf['webserver_doc_root']);
		return $this->conf['webserver_doc_root'];
	}

	function webserver_docroot_set ($docroot)
	{
		$this->conf['webserver_doc_root'] = $docroot;
	}

	// WEBSERVER_GET_USER
	function webserver_get_user($msg="")
	{
		$this->logger->log ('Trying to guess ' . $this->conf['webserver'] . '\'s UID');

		$user = "";
		if($this->conf['webserver'] == "Apache"){
			$user = $this->apache_user_guess();
		}
		else if($this->conf['webserver'] == "Zeus"){
			$user = $this->zeus_user_guess();
		}
		else if($this->conf['webserver'] == "SunONE"){
			$user = $this->sunone_user_guess();
		}

		$word = empty($user) ? "enter" : "confirm";
		if(empty($msg)){
			$msg = "\nPlease $word the username that your PHP scripts run as under ".$this->conf['webserver'];
		}

		$default = $user;
		while(true){
			$user = $this->inputbox($msg, $default);
			// Verify whether such user really exists:
			if (posix_getpwnam($user) !== false) {
				break;
			}
			if(!$this->yesnobox("Cannot find user: '$user'\nDo you want to try again?")){
				$this->on_error("The installation cannot continue.");
			}
		}

		if($this->conf['webserver'] == "Apache"){
			$this->conf['apache_user'] = $user;
		}

		return $user;
	}

	// WATCH_CONFIG_FILES
	function watch_config_files ()
	{
		$config_files = func_get_args();

		foreach ($config_files as $config) {
			$this->conf['config_files'][$config] = @filemtime ($config);
		}
	}

	// WEBSERVER_NEEDS_RESTART
	function webserver_needs_restart()
	{
		$needs_restart = false;

		if (@isset ($this->conf['config_files'])) {
			foreach ($this->conf['config_files'] as $config => $atime) {
				if ($atime != @filemtime ($config)) {
					$needs_restart = true;
					break;
				}
			}
		}
		return $needs_restart;
	}

	// WEBSERVER_RESTART
	function webserver_restart($silent=false)
	{
		$this->logger->log ('Trying to restart ' . $this->conf['webserver'] . ' ...');

		$restarted=false;

		if($this->conf['webserver'] == "Apache"){
			$restarted = $this->apache_restart($silent);
		}
		elseif($this->conf['webserver'] == "Zeus"){
			$restarted = $this->zeus_restart($silent);
		}
		elseif($this->conf['webserver'] == "SunONE"){
			$restarted = $this->sunone_restart($silent);
		}
		else{
			$this->warn("Don't know how to restart ". $this->conf['webserver']);
		}

		if($restarted){
			if (@isset ($this->conf['config_files'])) {
				foreach ($this->conf['config_files'] as $config => $atime) {
					$this->conf['config_files'][$config] = @filemtime ($config);
				}
			}
			if (!$silent) {
				$this->msgbox($this->conf['webserver']." has restarted successfully.");
			}
			return true;
		}

		$this->msgbox("Installation failed to restart your Web server,\nplease restart it manually.");
		return false;
	}

	// WEBSERVER_VERIFY_RUNNING
	function webserver_verify_running($abort_on_cancel=false)
	{
		$running = false;
		if($this->conf['webserver'] == "Apache") {
			$running = $this->apache_is_running();
		} elseif ($this->conf['webserver'] == "Zeus") {
			$running = $this->zeus_is_running();
		}

		if(!$running) {
			$this->yesnobox (
				"The installation has detected that your ".$this->conf['webserver']." Web server\n".
				"is currently not running. In order to continue with the installation,\n".
				"it must be started. Do you want to start it now?", false, $abort_on_cancel);

			$running = false;
			if($this->conf['webserver'] == "Apache") {
				$running = $this->apache_start();
			} elseif ($this->conf['webserver'] == "Zeus") {
				$running = $this->zeus_start();
			}
			if(!$running) {
				$button = empty ($this->conf['dialog']) ? "ENTER" : "OK";
				$this->msgbox ("The installation could not start your ".$this->conf['webserver']." Web server.\n".
					"Please do it manually, then press {$button}");
				$this->webserver_verify_running($abort_on_cancel);
			}
		}
	}

	// PHP_VERSION_DETECT_FROM_WEBSERVER
	function php_version_detect_from_webserver($silent = false)
	{
		$this->logger->log ('Detecting PHP version ...');

		if($this->is_using_apache($silent)) {
			$this->php_version_detect_from_apache($silent);
		}
		elseif ($this->conf['webserver'] == "Zeus") {
			$this->php_version_detect_from_zeus($silent);
		}
		else {
			return false;
		}

		if(isset($this->conf['php_version'])){
			if($silent) {
				if(!$this->php_version_substitute($silent)) {
					$this->conf['php_version'] = "unsupported";
					return false;
				}
				else return true;
			}
			return $this->php_version_confirm();
		}

		return false;
	}

	// WEBSERVER_GET_VIRTUAL_HOST_URL
	function webserver_get_virtual_host_url ($default=null, $use_default_host=false)
	{
		if($this->conf['webserver'] == "Apache") {
			return $this->apache_get_virtual_host ($default, $use_default_host);
		}
		elseif ($this->conf['webserver'] == "Zeus") {
			return $this->zeus_get_virtual_host_url ($default);
		}
		else return false;
	}

	// -----------------------------
	// APACHE FUNCTIONS
	// =============================

	// APACHE_ROOT_SET
	function apache_root_set($apache_root)
	{
		$this->conf['apache_root_dir'] = $apache_root;
		$this->conf['webserver_root_dir'] = $apache_root;
	}

	// APACHE_CONF_SET
	function apache_conf_set($apache_conf)
	{
		$this->conf['apache_conf_dir'] = $apache_conf;
		$this->conf['apache_conf_file'] = $this->make_path($apache_conf, "httpd.conf");
	}

	// APACHE_EXEC_SET
	function apache_exec_set($apache_exec)
	{
		$this->conf['apache_exec_dir'] = $apache_exec;
		$this->conf['apache_exec_file'] = $this->make_path($apache_exec, "httpd");
	}

	// APACHE_LIBEXEC_SET
	function apache_libexec_set($apache_libexec)
	{
		$this->conf['apache_libexec_dir'] = $apache_libexec;
	}

	// APACHE_CONF_OPEN
	function apache_conf_open()
	{
		$this->apache_conf_guess();

		if(!empty($this->conf['apache_conf_cont'])) {
			return $this->conf['apache_conf_cont'];
		}

		if(empty($this->conf['apache_conf_cont']) && $this->my_file_exists ($this->conf['apache_conf_file'])) {

			$this->conf['apache_conf_cont'] = "";
			$this->logger->log ('Opening Apache configuration file.');

			$apache_cont = $this->file2str($this->conf['apache_conf_file']);
			foreach(explode("\n", $apache_cont) as $line) {

				if(preg_match("@^\s*Include\s+\"?([^\s\"]+)\"?@", $line, $match)) { /* insert include file */

					$include_path = $match[1];

					/* Guess Apache root directory */
					if(preg_match("#^\s*ServerRoot\s+\"?([^\s\"]+)\"?#m", $apache_cont, $match)){
						$this->conf['apache_root_dir'] = $match[1];
					}
					if(empty($this->conf['apache_root_dir'])) {
						$this->apache_guess_from_binary();
					}

					$include_path = $this->make_full_path($include_path,
							isset($this->conf['apache_root_dir']) ? $this->conf['apache_root_dir'] : "");

					$include_files_arr = array();
					if(preg_match("@\*@", $include_path)) {
						$include_files_arr = $this->my_list_dir($this->my_dirname($include_path), basename($include_path));
					}
					else if($this->my_dir_exists($include_path)) {
						$include_files_arr = $this->my_list_dir($include_path, "*");
					}
					else {
						$include_files_arr[0] = $include_path;
					}

					if(is_array ($include_files_arr)) {
						foreach ($include_files_arr as $include_file) {
							$this->logger->log ('Found "Include" directive in '
									   . 
									   $this->conf['apache_conf_file']
									   . ": $include_file");
							if($this->my_file_exists($include_file)) {
								$this->conf['apache_conf_cont'] .= "# zend_open<$include_file>\n";
								$this->conf['apache_conf_cont'] .= $this->file2str($include_file);
								$this->conf['apache_conf_cont'] .= "# zend_close<$include_file>\n";
							}
							else {
								$this->logger->log 
								  ("File: $include_file doesn't exist");
							}
						}
					}
				}

				$this->conf['apache_conf_cont'] .= "$line\n";
			}
		} else {
			$this->conf['apache_conf_cont']="";
		}

		$this->conf['apache_conf_modified'] = false;

		return $this->conf['apache_conf_cont'];
	}

	// APACHE_CONF_CLOSE
	function apache_conf_close()
	{
		if(empty($this->conf['apache_conf_cont'])){
			return false;
		}
		
		// Save only modified httpd.conf
		if($this->conf['apache_conf_modified']) { 
			// Backup the old configuration
			$this->logger->log ('Closing Apache configuration file.');

			/* backup httpd.conf */
			$apache_conf_backup = $this->my_backup($this->conf['apache_conf_file'], true);
			if(isset($apache_conf_backup)) {
				$this->msgbox("Backed up Apache configuration file to:\n$apache_conf_backup");
			}

			if(preg_match_all("@# zend_open<(.*)>\n(.*)# zend_close<\\1>\n@Us", $this->conf['apache_conf_cont'], $match)) {
				for($i=0; $i<count($match[0]); $i++) {
					if(strcmp($match[2][$i], $this->file2str($match[1][$i])) != 0) { /* If changed this file - backup the original */
						$this->my_backup($match[1][$i], true);
					}
					$this->str2file($match[2][$i], $match[1][$i]); /* Write all includes to their destinations */
				}
			}

			$this->conf['apache_conf_cont'] = preg_replace("@# zend_open<(.*)>\n.*# zend_close<\\1>\n@Us", "",
					$this->conf['apache_conf_cont']);
			$this->str2file($this->conf['apache_conf_cont'], $this->conf['apache_conf_file']);
		}
	}

	// IS_USING_APACHE
	function is_using_apache($silent = false)
	{
		if($this->conf['webserver'] == "Apache"){
			return true;
		}
		if($this->conf['webserver'] != "Web server"){
			return false;
		}

		if(isset($this->conf['using_apache'])){
			return ($this->conf['using_apache'] == "YES") ? true: false;
		}

		if($silent) {
			return false;
		}

		if($this->yesnobox("Are you using Apache Web server?")){
			$this->conf['using_apache'] = "YES";
			$this->conf['webserver'] = "Apache";
			return true;
		}

		$this->conf['using_apache'] = "NO";
		return false;
	}

	// APACHE_CONF_GUESS
	function apache_conf_guess($silent = false) /* guess configuration file */
	{
		if(!$this->is_using_apache($silent)){
			return false;
		}

		$this->apache_exec_guess();

		if(!empty($this->conf['apache_conf_file'])){
			return $this->conf['apache_conf_file'];
		}

		$this->logger->log ('Trying to guess Apache configuration file ...');

		$possible_configs = array(
				"httpd.conf",   /* Regular */
				"httpsd.conf",  /* For Apache compiled with SSL support */
				"httpd2.conf",   /* Some systems (like Mandrake 9.1) called so to Apache 2 configuration file */
				"apache.conf",   /* Some systems (like Mandrake 9.1)*/
				"apache2.conf" /* Some systems (like Mandrake 9.1)*/
				);

		$apache_conf_default = array( /* default paths to config dir */
				"/usr/local/apache/conf",
				"/usr/local/apache2/conf",
				"/etc/httpd/conf",
				"/usr/local/apache/etc/",
				"/usr/local/apache2/etc/",
				"/usr/local/etc/apache/",
				"/etc/apache",
				"/etc/httpd"
				);

		if(@file_exists($this->conf['apache_conf_dir'])) {
			array_unshift ($apache_conf_default, $this->conf['apache_conf_dir']);
		}
		if(isset($this->conf['apache_root_dir'])) {
			array_unshift ($apache_conf_default, $this->make_path($this->conf['apache_root_dir'], "conf"));
		}

		$conf_default = $this->search_file($apache_conf_default, $possible_configs);

		if(isset($this->conf['default_apache_conf_file'])) {
			$conf_default = $this->conf['default_apache_conf_file'];
		}

		if(!$silent) {
			$this->conf['apache_conf_file'] = $this->select_file("Please specify the full path to the Apache configuration file (httpd.conf)", $conf_default);
		}
		else {
			$this->conf['apache_conf_file'] = $conf_default;
		}

		if(@file_exists($this->conf['apache_conf_file']))
		{
			$this->apache_guess_from_conf();

			$this->conf['apache_conf_dir'] = dirname($this->conf['apache_conf_file']);
			$this->conf['saved_config']['default_apache_conf_file'] = $this->conf['apache_conf_file'];
			$this->logger->log ('Found: ' . $this->conf['apache_conf_file']);
		}

		return $this->conf['apache_conf_file'];
	}

	// APACHE_EXEC_GUESS
	function apache_exec_guess($silent = false) /* guess exec. dir */
	{
		if(!$this->is_using_apache($silent)){
			return false;
		}

		if(!empty($this->conf['apache_exec_file'])){
			return $this->conf['apache_exec_file'];
		}

		$this->logger->log ('Trying to guess Apache executable file ...');

		$apachectls = array("apachectl", "httpdctl", "httpsdctl", "apache2ctl", "apache-sslctl");
		$apache_dirs = array( /* default paths to bin dir */
				1 =>  "/usr/local/apache/bin",
				2 =>  "/usr/local/apache2/bin",
				3 =>  "/usr/sbin",
				4 =>  "/usr/bin",
				5 =>  "/usr/local/sbin",
				6 =>  "/usr/local/bin"
				);

		if(!empty($this->conf['apache_exec_dir'])) {
			array_unshift ($apache_dirs, $this->conf['apache_exec_dir']);
		}
		if(!empty($this->conf['apache_root_dir'])) {
			array_unshift ($apache_dirs, $this->make_path($this->conf['apache_root_dir'], "bin"));
		}

		$apachectl_def = $this->search_file($apache_dirs, $apachectls);

		do {
			$this->conf['apachectl_path'] = 
				$this->select_file("Specify the full path to the Apache control utility (apachectl)", $apachectl_def);
				
			$file_type = $this->file_mime_type ($this->conf['apachectl_path']);
			$err = null;
			if(($file_type !== false && !strstr($file_type, "application/x-sh"))
				|| ($file_type === false && !strstr($this->file2str ($this->conf['apachectl_path']), '#!/bin/sh'))) {
				$err = "Apache control utility should be a shell script";
			}
			if(!@is_executable($this->conf['apachectl_path'])) {
				$err = "Apache control utility should be an executable shell script";
			}
			if (!$err || $this->yesnobox ("{$err}!\nDo you still wish to use this path?")) {
				break;
			}
		}
		while (true);

		$this->logger->log ('Found apachectl: ' . $this->conf['apachectl_path']);
		
		$apachectl_cont = $this->file2str ($this->conf['apachectl_path']);
		if(preg_match ("@^\s*HTTPD\s*\=(.*)$@m", $apachectl_cont, $match)) {
			$arr = preg_split('@\s+@', trim($match[1], " \t\'\""));
			if (isset($arr[0]) && $this->my_file_exists ($arr[0])) {
				$this->conf['apache_exec_file'] = $arr[0];
			}
		}
		if(preg_match ("@^\s*PIDFILE\s*\=(.*)$@m", $apachectl_cont, $match)) {
			$this->conf['apache_pid_file'] = trim($match[1], " \t\'\"");
			$this->logger->log ('Found Apache PID file: ' . $this->conf['apache_pid_file']);
		}

		if(!isset($this->conf['apache_exec_file']) || !$this->my_file_exists($this->conf['apache_exec_file'])) {
			$this->conf['apache_exec_file'] = $this->select_file("Installation couldn't detect the location of Apache executable.\nPlease specify the full path to the Apache binary (httpd)", isset($this->conf['apache_exec_file']) ? $this->conf['apache_exec_file'] : "");
		}

		$this->logger->log ('Found httpd: ' . $this->conf['apache_exec_file']);

		$this->conf['apache_exec_dir'] = dirname($this->conf['apache_exec_file']);

		$this->apache_guess_from_binary();

		return $this->conf['apache_exec_file'];
	}

	// APACHE_GET_VIRTUAL_HOST
	function apache_get_virtual_host($default_host=null, $use_default_host=false)
	{
		$this->apache_guess_from_conf();
		$host = $this->conf['uname']['nodename'];

		if($default_host) {
			$host = $default_host;
		}

		$docroot = $this->webserver_docroot_guess();

		if(@is_array($this->conf['apache_vhosts'])) {
			foreach ($this->conf['apache_vhosts'] as $key => $arr) {
				if($arr['docroot'] == $docroot && $arr['port'] != "443") {
					$found_key = $key;
					break;
				}
			}
		}

		if(isset($found_key)) {
			if(!$use_default_host) {
				if(isset($this->conf['apache_vhosts'][$found_key]["server"])) {
					$host = $this->conf['apache_vhosts'][$found_key]["server"];
				}
				else if(isset($this->conf['apache_vhosts'][$found_key]["ip"])
						&& preg_match("/^[\d+\.]+$/", $this->conf['apache_vhosts'][$found_key]["ip"])) {
					$host = $this->conf['apache_vhosts'][$found_key]["ip"];
				}
			}
			if(!preg_match("/\:\d+$/", $host)) {
				$host .= ":".$this->conf['apache_vhosts'][$found_key]["port"];
			}
			if($this->conf['apache_vhosts'][$found_key]['ssl']) {
				$host = "https://{$host}";
			} else {
				$host = "http://{$host}";
			}
		}
		return $host;
	}
	
	// APACHE_LIBEXEC_GUESS
	function apache_libexec_guess()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if(!empty($this->conf['apache_libexec_dir'])){
			return $this->conf['apache_libexec_dir'];
		}

		$this->apache_guess_from_conf();

		$this->conf['apache_libexec_dir'] = $this->select_dir(
				"Please specify the location of your Apache modules\n(usually: HTTPD_ROOT/libexec):",
				$this->make_path($this->conf['apache_root_dir'],
					($this->apache_get_version() == "2" ? "modules" : "libexec")));

		$this->logger->log ('Found Apache libexec directory at: ' . 
				   $this->conf['apache_libexec_dir']);
	}

	// APACHE_ROOT_GUESS
	function apache_root_guess()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if(!empty($this->conf['apache_root_dir'])){
			return $this->conf['apache_root_dir'];
		}

		$this->apache_guess_from_conf();
		if(empty($this->conf['apache_root_dir'])){
			$this->apache_guess_from_binary();
		}

		if(empty($this->conf['apache_root_dir'])){ /* set default apache root directory */
			$this->conf['apache_root_dir'] = $this->nice_path($this->conf['apache_conf_dir']."../");
		}

		return $this->conf['apache_root_dir'];
	}

	// APACHE_DOCROOT_GUESS
	function apache_docroot_guess()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if(!empty($this->conf['webserver_doc_root'])){
			return $this->conf['webserver_doc_root'];
		}

		$default = "";
		$this->apache_guess_from_conf();
		if(count($this->conf['webserver_docroot_arr']) == 0) {
			$possible_dirs = array(
					1 =>  '/usr/local/apache/htdocs',
					2 =>  '/usr/local/apache2/htdocs',
					3 =>  '/var/www/html',
					4 =>  '/var/lib/apache/htdocs',
					5 =>  '/var/lib/apache2/htdocs',
					6 =>  '/usr/local/httpd/htdoc',
					7 =>  '/home/httpd/html'
					);
			if(isset($this->conf['apache_root_dir'])) {
				array_unshift($possible_dirs, $this->make_path($this->conf['apache_root_dir'], "htdocs"));
			}
			$default = $this->search_dir($possible_dirs);
		}
		else if(count($this->conf['webserver_docroot_arr']) >1 && count($this->conf['webserver_docroot_arr']) <5) {
			array_push($this->conf['webserver_docroot_arr'], "other");

			while(true) {
				$num = $this->menubox("Please select the location of your Apache document root directory.\n".
						"This is the location where your web documents are stored.", $this->conf['webserver_docroot_arr']);

				if($this->conf['webserver_docroot_arr'][$num] == "other") {
					break;
				}
				else if($this->my_dir_exists($this->conf['webserver_docroot_arr'][$num])) {
					$this->conf['webserver_doc_root'] = $this->conf['webserver_docroot_arr'][$num];
					break;
				}

				$this->msgbox("Broken Apache configuration!\n".
						"Document root: \"".$this->conf['webserver_docroot_arr'][$num]."\" doesn't exist");
			}
		}
		else {
			$default = array_pop($this->conf['webserver_docroot_arr']);
		}

		return $default;
	}

	// APACHE_USER_GUESS
	function apache_user_guess()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if(!empty($this->conf['apache_user'])){
			return $this->conf['apache_user'];
		}

		$this->logger->log ('Trying to guess a User ID apache runs with ...');

		$this->apache_guess_from_conf();
		return empty($this->conf['apache_user'])? null : $this->conf['apache_user'];
	}
	
	// APACHE_ERRORLOG_GUESS
	function apache_errorlog_guess()
	{
		if(!empty($this->conf['apache_error_log'])){
			return $this->conf['apache_error_log'];
		}

		$this->apache_guess_from_conf();
		if(empty($this->conf['apache_error_log'])){
			$this->apache_guess_from_binary();
		}
	}

	// APACHE_GUESS_FROM_BINARY
	function apache_guess_from_binary()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		$this->apache_exec_guess();

		$msg = "Guessed from Apache executable:";

		$httpd_res = $this->cmd2str($this->conf['apache_exec_file']. " -V 2>/dev/null");

		// Apache root
		if(empty($this->conf['apache_root_dir']) &&
				preg_match("#HTTPD_ROOT=\"?([^\s\"]+)\"?\s*$#m", $httpd_res, $match)){
			$this->conf['apache_root_dir'] = $match[1];
			$this->logger->log ("$msg HTTPD_ROOT=" . $this->conf['apache_root_dir']);
		}
		// Apache version
		if(empty($this->conf['apache_version']) &&
				preg_match("#^\s*Server version\:\s+Apache\/(\d+)#m", $httpd_res, $match)){
			$this->conf['apache_version'] = $match[1];
			$this->logger->log ("$msg APACHE_VERSION=" . $this->conf['apache_version']);
		}
		// Apache error log
		if(empty($this->conf['apache_error_log']) &&
				preg_match("#DEFAULT_ERRORLOG=\"?([^\s\"]+)\"?\s*$#m", $httpd_res, $match)){
			$this->conf['apache_error_log'] = $this->make_full_path($match[1], $this->conf['apache_root_dir']);
			$this->logger->log ("$msg ERROR_LOG=" . $this->conf['apache_error_log']);
		}
		// Apache config file
		if(empty($this->conf['apache_conf_file']) &&
				preg_match("#SERVER_CONFIG_FILE=\"?([^\s\"]+)\"?\s*$#m", $httpd_res, $match)){
			$this->conf['apache_conf_file'] = $this->make_full_path($match[1], $this->conf['apache_root_dir']);
			$this->conf['apache_conf_dir'] = dirname($this->conf['apache_conf_file']);
			$this->logger->log ("$msg APACHE_CONFIG=" . $this->conf['apache_conf_file']);
		}
		// Apache PID file
		if(empty($this->conf['apache_pid_file']) &&
				preg_match("#DEFAULT_PIDLOG=\"?([^\s\"]+)\"?\s*$#m", $httpd_res, $match)){
			$this->conf['apache_pid_file'] = $this->make_full_path($match[1], $this->conf['apache_root_dir']);
			$this->logger->log ("$msg DEFAULT_PIDLOG=".$this->conf['apache_pid_file']);
		}

		// guess Apache2 worker
		$httpd_l_res = $this->cmd2str($this->conf['apache_exec_file']. " -l 2>/dev/null");
		if(empty($this->conf['apache2_worker']) &&
				preg_match("#^\s*worker\.c#m",$httpd_l_res,$match)){
			$this->conf['apache2_worker'] = "yes";
			$this->install_log("$msg APACHE2 worker=".$this->conf['apache2_worker']);
		} else {
			$this->conf['apache2_worker'] = "no";
		}

		return true;
	}

	// APACHE_GUESS_FROM_CONF
	function apache_guess_from_conf()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if (@isset($this->conf['apache_guessed_from_conf'])) {
			return true;
		}
		$this->conf['apache_guessed_from_conf'] = true;
			

		if(!@isset($this->conf['apache_root_dir'])){
			$this->conf['apache_root_dir']="";
		}

		$this->apache_conf_open();

		$msg = "Guessed from Apache httpd.conf:";

		// Apache root
		if(empty($this->conf['apache_root_dir']) &&
				preg_match("#^\s*ServerRoot\s+\"?([^\s\"]+)\"?#m", $this->conf['apache_conf_cont'], $match)){
			$this->conf['apache_root_dir'] = $match[1];
			$this->install_log("$msg HTTPD_ROOT=".$this->conf['apache_root_dir']);
		}
		// Error log
		if(empty($this->conf['apache_error_log']) &&
				preg_match("#^\s*ErrorLog\s+\"?([^\s\"]+)\"?#m", $this->conf['apache_conf_cont'], $match)){
			$this->conf['apache_error_log'] = $this->make_full_path($match[1], $this->conf['apache_root_dir']);
			$this->install_log("$msg ERROR_LOG=".$this->conf['apache_error_log']);
		}
		// User
		if(empty($this->conf['apache_user']) &&
				preg_match("#^\s*User\s+([^\s]*)\s*$#m", $this->conf['apache_conf_cont'], $match)){
			$this->conf['apache_user'] = $match[1];
			$this->install_log("$msg USER=".$this->conf['apache_user']);
		}
		// Port
		if(empty($this->conf['apache_port'])){
			if(preg_match("#^\s*Port\s+([\d]+)\s*$#m", $this->conf['apache_conf_cont'], $match)){
				$port = $match[1];
			}
			else if(preg_match("#^\s*Listen\s+([\d]+)\s*$#m", $this->conf['apache_conf_cont'], $match)){
				$port = $match[1];
			}
			if(isset($port)){
				$this->conf['apache_port'] = $port;
				$this->install_log("$msg PORT=".$this->conf['apache_port']);
			}
		}
		// Group
		if(empty($this->conf['apache_group']) &&
				preg_match("#^\s*Group\s+([^\s]*)\s*$#m", $this->conf['apache_conf_cont'], $match)){
			$this->conf['apache_group'] = $match[1];
			$this->install_log("$msg GROUP=".$this->conf['apache_group']);
		}
		// MaxClients
		if(empty($this->conf['max_clients']) &&
				preg_match("@^\s*MaxClients\s+([\d]+)\s*$@m", $this->conf['apache_conf_cont'], $match)) {
			$this->conf['max_clients'] = $match[1];
			$this->install_log("$msg MaxClients=".$this->conf['max_clients']);
		}
		// PidFile
		if(preg_match("@^\s*PidFile\s+(.*)$@m", $this->conf['apache_conf_cont'], $match)) {
			$this->conf['apache_pid_file'] = $this->make_full_path(trim($match[1], " \t\"\'"), $this->conf['apache_root_dir']);
			$this->install_log("$msg PidFile=".$this->conf['apache_pid_file']);
		}
		// Server Name
		if(empty($this->conf['server_name']) &&
			preg_match("@^\s*ServerName\s+([^\s]*).*<VirtualHost[^\>]+>.*</VirtualHost>@sU", $this->conf['apache_conf_cont'], $match)) {
		}

		// Virtual hosts
		if(empty($this->conf['apache_vhosts']) &&
			preg_match_all("@^\s*<VirtualHost([^\>]+)>(.*)</VirtualHost>@msU", $this->conf['apache_conf_cont'], $match)) {

			for($i=0; $i<count($match[0]); $i++) {

				if(preg_match("@\s+([^\:\s]+):?(\d*)@", $match[1][$i], $match2)) {

					$vhost_ip = $match2[1];
					$vhost_port = empty($match2[2]) ? "80" : $match2[2];

					$is_ssl = preg_match ('@^\s*SSLEngine\s+.*?on.*?$@mi', $match[2][$i]);

					if(preg_match("@^\s*DocumentRoot\s+\"?([^\s\"]+)\"?@m", $match[2][$i], $match2)) {
						$vhost_docroot = $match2[1];
					}
					if(preg_match("@^\s*ServerName\s+\"?([^\s\"]+)\"?@m", $match[2][$i], $match2)) {
						$vhost_servername = preg_replace("/:\d+/", "", $match2[1]);
					}

					if(isset($vhost_docroot) && $this->my_dir_exists($vhost_docroot) && isset($vhost_ip) && isset($vhost_servername) &&
						!preg_match("/\\\$/",$vhost_ip) && !preg_match("/\\\$/",$vhost_servername) )
					{

						$vhost_docroot = $this->nice_path($vhost_docroot);
						$this->conf['apache_vhosts'][$vhost_docroot.":".$vhost_ip] = array(
								'ip'     => $vhost_ip,
								'port'   => $vhost_port,
								'server' => $vhost_servername,
								'docroot'=> $vhost_docroot,
								'ssl'    => $is_ssl
								);
						$this->install_log("$msg VHOST=$vhost_ip:$vhost_port ($vhost_servername) => $vhost_docroot");
					}
				}
			}
		}
		// Document root
		if(empty($this->conf['webserver_docroot_arr']) &&
				preg_match_all("#^\s*DocumentRoot\s+\"?([^\s\"]+)\"?#m", $this->conf['apache_conf_cont'], $match)){

			$multiples = array();
			for($i=0, $c=1; $i<count($match[0]); $i++) {
				$docroot = $this->make_full_path($match[1][$i], $this->conf['apache_root_dir']);
				if(!strstr($docroot,'$') && $this->my_dir_exists($docroot)) {
					$docroot = $this->nice_path($docroot);
					if(!isset($multiples[$docroot])) {
						$this->conf['webserver_docroot_arr'][$c] = $docroot;
						$this->install_log ("$msg DOCUMENT_ROOT=$docroot");
						$multiples[$docroot] = true;
						$c++;
					}
				}
			}
		}
		// Global document root
		if (empty ($this->conf['webserver_global_docroot'])) {
			$global_http_conf = preg_replace ('@^\s*<VirtualHost([^\>]+)>(.*)</VirtualHost>@msU', '',
				$this->conf['apache_conf_cont']);

			if (preg_match ('@^\s*DocumentRoot\s+([^\#]+)@m', $global_http_conf, $match)) {
				$this->conf['webserver_global_docroot'] = trim ($match[1], "\t \r\n\"\'");
				$this->install_log ("{$msg} GLOBAL_DOCUMENT_ROOT={$this->conf['webserver_global_docroot']}");
			}
		}
		// Libexec
		if(empty($this->conf['apache_libexec_dir']) &&
			preg_match("#^\s*LoadModule\s+[^\s]+\s+\"?([^\s\"]+)\"?#m", $this->conf['apache_conf_cont'], $match)) {

			$path = $this->make_full_path(dirname($match[1]), $this->conf['apache_root_dir']);
			if($this->my_dir_exists($path)) {
				$this->conf['apache_libexec_dir'] = $path;
				$this->install_log("$msg APACHE_LIBEXEC=".$this->conf['apache_libexec_dir']);
			}
		}
		// PHP (module or executable) path
		if(empty($this->conf['php_path'])){
			// Script aliases
			preg_match_all("#^\s*ScriptAlias\s+\"?([^\s\"]+)\"?\s+\"?([^\s\"]+)\"?#m",
					$this->conf['apache_conf_cont'], $match);

			array_shift($match);

			$script_aliases = array();
			for($i=0; $i<count($match[0]); $i++){
				$script_aliases[$match[0][$i]] = $match[1][$i];
			}

			if(preg_match("#^\s*LoadModule\s+php\d_module\s+\"?([^\s\"]+)\"?\s*$#m",
						$this->conf['apache_conf_cont'], $match)){

				$this->conf['php_type'] = "module";
				$this->conf['php_path'] = $match[1];
			}
			elseif(preg_match("#^\s*Action\s+application\/x-httpd-php\s+\"?([^\s\"]+)\"?\s*$#m",
						$this->conf['apache_conf_cont'], $match)){
				$this->conf['php_type'] = "executable";
				$this->conf['php_path'] = $match[1];
			}
			if(!empty($this->conf['php_path'])){
				foreach($script_aliases as $alias => $path){
					$this->conf['php_path'] = preg_replace("#^$alias#", $path, $this->conf['php_path'], 1);
				}
				$this->conf['php_path'] = $this->make_full_path($this->conf['php_path'], $this->conf['apache_root_dir']);
				$this->install_log("$msg PHP_PATH=".$this->conf['php_path']);
				$this->install_log("$msg PHP_TYPE=".$this->conf['php_type']);
			}
		}

		return true;
	}

	function apache_pidfile_detect()
	{
		if (!isset($this->conf['apache_pid_file'])) {
			$this->apache_guess_from_binary();
		}
		$this->apache_guess_from_conf();

		return (@isset($this->conf['apache_pid_file']) ? $this->conf['apache_pid_file'] : null);
	}

	// APACHE_HAS_MODULE
	function apache_has_module($mod_name)
	{
		$this->apache_exec_guess();

		if(preg_match("/$mod_name/", $this->cmd2str($this->conf['apache_exec_file']." -l"))) {
			return true;
		}
		return false;
	}

	// APACHE_GET_VERSION
	function apache_get_version()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		if(!empty($this->conf['apache_version'])){
			return $this->conf['apache_version'];
		}

		$this->apache_guess_from_binary();

		if(empty($this->conf['apache_version'])){
			$version = $this->menubox(
					"The installation has failed to detect the version of Apache you use.".
					"Please choose the correct version.",
					array(1 => 'Apache 1.x.x', 2 => 'Apache 2.x.x')
					);
			$this->conf['apache_version'] = preg_replace("/Apache\s+(\d).*/", "\\1", $version);
		}

		return $this->conf['apache_version'];
	}

	// APACHE_CONFIGURE_INSTALL
	function apache_configure_install()
	{
		$apache_install_dir=dirname($this->conf['apache_root_dir']);
		if(empty($apache_install_dir) or $apache_install_dir == "/") {
			$this->on_error("Cannot install Apache into / (not permitted)");
		}
		if(!$this->my_dir_exists($apache_install_dir)){
			$this->on_error("Cannot install Apache into ".$apache_install_dir." (not a directory)");
		}

		$this->infobox("Configuring Apache installation ...");

		// Fix apachectl
		$this->conf['apachectl_path'] = $this->conf['apache_exec_dir']."/apachectl";
		$this->conf['apache_conf_file'] = $this->conf['apache_conf_dir']."/httpd.conf";

		$new_apachectl = preg_replace(
				"@(\s*HTTPD=).*$@m", "\\1\"".$this->conf['apache_exec_dir']."/httpd -f ".$this->conf['apache_conf_file']."\"",
				$this->file2str($this->conf['apachectl_path']));

		$new_apachectl = preg_replace(
				"@(\s*PIDFILE=).*$@m", "\\1".$this->conf['apache_root_dir']."/logs/httpd.pid", $new_apachectl);

		$this->my_fwrite($this->conf['apachectl_path'], "w", $new_apachectl);

		# Configure httpd.conf
		$this->apache_conf_open();

		// Fix httpd.conf
		$this->conf['apache_conf_cont'] = preg_replace(
				"@/usr/local/apache@m", $this->conf['apache_root_dir'], $this->conf['apache_conf_cont']);

		// fix "Group" directive
		$group_is_set = false;
		foreach (array('nobody', 'nogroup') as $grp) {
			if (posix_getgrnam($grp) !== false) {
				$this->conf['apache_conf_cont'] =
					preg_replace ("@^\s*Group\s+.*@m", "Group {$grp}", $this->conf['apache_conf_cont']);
				$group_is_set = true;
				break;
			}
		}
		if(!$group_is_set){
			$this->conf['apache_conf_cont'] =
				preg_replace ("@(^\s*Group\s+.*)@m", "#\\1", $this->conf['apache_conf_cont']);
		}

		$this->link_startup_daemon($this->conf['apachectl_path']);

		$this->conf['apache_conf_modified'] = true;
		return true;
	}

	// APACHE_CONFIGURE_PHP_INSTALL
	function apache_configure_php_install($php_version)
	{
		$this->apache_conf_guess();
		$this->infobox("Configuring Apache for PHP installation...");

		$this->apache_libexec_guess();

		if(!$this->my_dir_exists($this->conf['apache_libexec_dir'])) {
			$this->on_error("Broken Apache configuration: The needed directory:\n".
				$this->conf['apache_libexec_dir']."\nwhere Apache modules should be placed could not be found.\n".
				"The installation cannot proceed.\n");
		}

		$apache_libphp_so_path = $this->conf['apache_libexec_dir']."/libphp".$php_version.".so";

		if($this->my_file_exists($apache_libphp_so_path)){
			$this->my_backup($apache_libphp_so_path);
		}
		if(!$this->my_symlink($this->conf['prefix'].'/lib/libphp'.$php_version.'.so', $apache_libphp_so_path)){
			$this->on_error("Can't place a link to PHP library in your Apache ".
					"libexec directory at: ".$apache_libphp_so_path);
		}
		
		# Configure httpd.conf:
		$this->apache_conf_open();

		// install AddType entries
		if(!preg_match("@^\s*(AddType\s+application.*php)@m", $this->conf['apache_conf_cont'])) {
			$this->conf['apache_conf_cont'] .= "\n".
				"<IfModule mod_mime.c>\n".
				"   AddType application/x-httpd-php .php\n".
				"   AddType application/x-httpd-php-source .phps\n".
				"</IfModule>\n\n";
		}

		// Install LoadModule entry
		$load_module_new_entry = "\nLoadModule php".$php_version."_module $apache_libphp_so_path\n";
		if(preg_match("@^\s*LoadModule\s+php\d_module\s+@m", $this->conf['apache_conf_cont']))
		{
			$this->conf['apache_conf_cont'] = preg_replace(
				"@^\s*(LoadModule\s+php\d_module\s.*)$@m",
				"#Zend# \\1$load_module_new_entry",
				$this->conf['apache_conf_cont']);
		}
		elseif (preg_match("@^\s*Action\s+application/x-httpd-php@m", $this->conf['apache_conf_cont']))
		{
			$this->conf['apache_conf_cont'] = preg_replace(
				"@^\s*(Action\s+application/x-httpd-php.*)$@m",
				"#Zend# \\1$load_module_new_entry",
				$this->conf['apache_conf_cont']);
		}
		else {
			$this->conf['apache_conf_cont'] .= $load_module_new_entry;
		}
			
		$this->conf['apache_conf_modified'] = true;
	}

	// APACHE_DO_CONFIG
	function apache_check_php_config()
	{
		$this->apache_conf_open();
		$config_exists = preg_match("/^\s*AddType\s+application.*php/m",
			$this->conf['apache_conf_cont']);
		
		if(!$config_exists){
			$this->msgbox("No existing PHP configuration found in your ".$this->conf['apache_conf_file']."!\n".
					"Your Apache must be configured to enable PHP scripts running on it,\n".
					"otherwise this product is useless!");
		}
	}

	// APACHE_STOP
	function apache_stop()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		$this->apache_exec_guess();

		$this->progress_bar_start(20, "Stopping Apache ...\n".
		"Please wait... (this may take a few seconds)", "Stopping ...");

		$status = $this->run_command($this->conf['apachectl_path']." stop 1>/dev/null");
		sleep(4);

		$this->progress_bar_stop();
		if($status==0){
			return true;
		}
		return false;
	}

	// APACHE_START
	function apache_start()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		$this->apache_exec_guess();

		$apache_ssl = false;

		if (strstr($this->file2str($this->conf['apachectl_path']), "startssl")) {
			$cmd = $this->inputbox ("Please confirm your Apache Web server startup command",
				$this->conf['apachectl_path']." startssl");

			if (strstr($cmd, "startssl")) {
				$apache_ssl = true;
			}
		} else {
			$cmd = $this->conf['apachectl_path']." start";
		}

		if ($apache_ssl) {
			$status = $this->run_command($cmd);
		}
		else {
			$this->progress_bar_start(20, "Starting Apache ...\n".
			"Please wait... (this may take a few seconds)", "Starting ...");

			$status = $this->run_command("{$cmd} 1>/dev/null");
		}

		sleep(4);

		if(!$this->apache_is_running()) {
			if ($apache_ssl) {
				$status = $this->run_command($cmd);
			} else {
				$status = $this->run_command("{$cmd} 1>/dev/null");
			}
			sleep(4);
		}

		if (!$apache_ssl) {
			$this->progress_bar_stop();
		}
	
		if(!$this->apache_is_running()) {
			$status = -1;
		}

		return ($status == 0);
	}
	
	// APACHE_RESTART
	function apache_restart($silent=false)
	{
		if(!$this->is_using_apache()){
			return false;
		}

		$this->apache_exec_guess();

		$stop_cmd = "stop";
		$start_cmd = "start";

		if (!$silent) {
			$this->progress_bar_start(20, "Restarting Apache ...\n".
			"Please wait... (this may take a few seconds)", "Restarting ...");
		}

		if(strstr($this->file2str($this->conf['apachectl_path']), "startssl")) {
			$status = $this->run_command($this->conf['apachectl_path']." graceful 1>/dev/null");
			sleep(4);
		}
		else {
			$this->run_command($this->conf['apachectl_path']." $stop_cmd 1>/dev/null");
			sleep(4);
			$status = $this->run_command($this->conf['apachectl_path']." $start_cmd 1>/dev/null");
			sleep(4);

			if(!$this->apache_is_running()) {
				$status = $this->run_command($this->conf['apachectl_path']." $start_cmd 1>/dev/null");
				sleep(4);
			}
		}

		if (!$silent) {
			$this->progress_bar_stop();
		}

		if(!$this->apache_is_running()) {
			$status = -1;
		}

		if($status==0){
			return true;
		}
		return false;
	}

	// APACHE_IS_RUNNING
	function apache_is_running()
	{
		if(!$this->is_using_apache()){
			return false;
		}

		$pidfile = $this->apache_pidfile_detect();

		if($pidfile) {
			if ($this->my_file_exists($pidfile) 
				&& $this->run_command ("kill -0 ".$this->file2str($pidfile)." 1>/dev/null") == 0) {
				return true;
			}
			/* PID file doesn't exists - it means Apache Web server is not running */
			return false;
		}

		# Else try to find PID by name of the Apache executable
		$pids = $this->pidof (basename($this->conf['apache_exec_file']));
		if(count($pids) == 0) {
			return false;
		}
		return true;
	}

	// ----------------------
	// ZEUS FUNCTIONS
	// ======================

	// ZEUS_RESTART
	function zeus_restart($silent=false)
	{
		$this->webserver_root_guess();
		if (!$silent) {
			$this->infobox("Restarting Zeus ...");
		}

		if($this->my_file_exists($this->conf['zeus_root']."/restart-zeus")){
			$status = $this->run_command($this->conf['zeus_root']."/restart-zeus 2>&1 1>/dev/null");
			if($status == 0){
				return true;
			}
		}
		return false;
	}

	// ZEUS_START
	function zeus_start()
	{
		$this->webserver_root_guess();
		$this->infobox("Starting Zeus ...");

		if($this->my_file_exists($this->conf['zeus_root']."/start-zeus")){
			$status = $this->run_command($this->conf['zeus_root']."/start-zeus 2>&1 1>/dev/null");
			if($status == 0){
				return true;
			}
		}
		return false;
	}

	// ZEUS_CHOOSE_VIRTUAL_HOST
	function zeus_choose_virtual_host()
	{
		$this->webserver_root_guess();

		if(!isset($this->conf['zeus_vhost'])) {

			$conf_dir = $this->conf['zeus_root']."/webadmin/conf/virtual_servers/sites";

			$vhosts = array();
			if($this->my_dir_exists($conf_dir)) {
				foreach ($this->my_list_dir($conf_dir, "*") as $file) {
					$file = basename($file);
					if (!preg_match ("/^(_default|_skel.*)$/", $file)) {
						array_push ($vhosts, basename($file));
					}
				}
			}
			if(count($vhosts) > 0) {
				$i = $this->menubox("Please select the name of the virtual host,\n"
					."where you wish to setup the product", $vhosts);
				$vhost = $vhosts[$i];
			} elseif ($this->my_dir_exists($conf_dir)) {
				do {
					$vhost = $this->inputbox ("Please enter the name of the virtual host,\n"
						."where you wish to setup the product");
					if(!$this->my_file_exists ("$conf_dir/$vhost")) {
						$this->msgbox ("Virtual host: $vhost doesn't exist in the Zeus configuration!\nPlease try again!");
						continue;
					}
				} while (false);
			}	
			else return false;

			/* Guess everything from the virtual host */
			$this->conf['zeus_vhost'] = $vhost;
			$cont = $this->file2str ("$conf_dir/$vhost");

			if(preg_match ("/^\s*docroot\s+(\S.+)$/m", $cont, $match)) {
				$this->conf['webserver_doc_root'] = trim ($match[1], "\t \'\"");
				$this->install_log ("Guessed Zeus document root: ".$this->conf['webserver_doc_root']);
			}
			if(preg_match ("/^\s*ip_name\s+(\S.+)$/m", $cont, $match)) {
				$this->conf['zeus_ip_name'] = trim ($match[1], "\t \'\"");
				$this->install_log ("Guessed Zeus hostname: ".$this->conf['zeus_ip_name']);
			}
			if(preg_match ("/^\s*port\s+(\d+)$/m", $cont, $match)) {
				$this->conf['zeus_port'] = trim ($match[1], "\t \'\"");
				$this->install_log ("Guessed Zeus port: ".$this->conf['zeus_port']);
			}
			if(preg_match ("/zeus-php-handler\!filepath\s+(\S.+)$/m", $cont, $match)) {
				$this->conf['php_path'] = str_replace("%zeushome%", $this->conf['zeus_root'], trim ($match[1], "\t \'\""));
				$this->install_log ("Guessed Zeus PHP path: ".$this->conf['php_path']);
			}
			if(preg_match ("/zeus-php-handler\!type\s+(\S.+)$/m", $cont, $match)) {
				$this->conf['php_type'] = trim ($match[1], "\t \'\"");
				if($this->conf['php_type'] != 'fastcgi') {
					$this->conf['php_type'] = 'unknown';
				}
				$this->install_log ("Guessed Zeus PHP type: ".$this->conf['php_type']);
			}
			if(preg_match ("/fastcgi!uid\s+(\d+)$/m", $cont, $match)) {
				$user = $this->uid2user(trim ($match[1], "\t \'\""));
				if($user !== false) {
					$this->conf['zeus_user'] = $user;
					$this->install_log ("Guessed Zeus user: ".$this->conf['zeus_user']);
				}
			}
			if(preg_match ("/fastcgi!gid\s+(\d+)$/m", $cont, $match)) {
				$groupinfo = posix_getgrgid(trim ($match[1], "\t \'\""));
				if($groupinfo !== false) {
					$this->conf['zeus_group'] = $groupinfo['name'];
					$this->install_log ("Guessed Zeus group: ".$this->conf['zeus_group']);
				}
			}
		}

		return true;
	}

	// ZEUS_DOCROOT_GUESS
	function zeus_docroot_guess()
	{
		$this->zeus_choose_virtual_host();

		if(!empty($this->conf['webserver_doc_root'])){
			return $this->conf['webserver_doc_root'];
		}

		$this->webserver_root_guess();

		$multiples = array();
		$c = 1;
		if(!empty($this->conf['zeus_root'])){
			if($this->my_dir_exists($this->conf['zeus_root']."/webadmin/conf/virtual_servers/sites")) {
				foreach ($this->my_list_dir($this->conf['zeus_root']."/webadmin/conf/virtual_servers/sites", "*") as $file) {
					if(preg_match("/^docroot\s+(.*)$/m", $this->file2str($file), $match)) {
						if($this->my_dir_exists($match[1]) && !isset($multiples[$match[1]])) {
							$this->conf['webserver_docroot_arr'][$c] = $match[1];
							$c++;
						}
					}
				}
			}
		}

		$default = "";
		if(count($this->conf['webserver_docroot_arr']) == 0) {
			$default = $this->search_dir(array(
						1	=>	"/usr/local/zeus/web/docroot",
						2	=>	"/var/www/html"
						));
		}
		else if(count($this->conf['webserver_docroot_arr']) > 1) {
			array_push($this->conf['webserver_docroot_arr'], "other");
			while(true) {
				$num = $this->menubox("Please select the location of your document root directory.\n".
						"This is the location where your web documents are stored.", $this->conf['webserver_docroot_arr']);

				if($this->conf['webserver_docroot_arr'][$num] == "other") {
					break;
				}
				else if($this->my_dir_exists($this->conf['webserver_docroot_arr'][$num])) {
					$this->conf['webserver_doc_root'] = $this->conf['webserver_docroot_arr'][$num];
					break;
				}

				$this->msgbox("Broken Zeus configuration!\n".
						"Document root: \"".$this->conf['webserver_docroot_arr'][$num]."\" doesn't exist");
			}
		}
		else {
			$default = array_pop($this->conf['webserver_docroot_arr']);
		}

		return $default;
	}

	// ZEUS_USER_GUESS
	function zeus_user_guess()
	{
		$this->zeus_choose_virtual_host();

		if(isset($this->conf['zeus_user'])) {
			return $this->conf['zeus_user'];
		}

		$this->webserver_root_guess();

		if(!empty($this->conf['zeus_root'])){
			if($this->my_file_exists($this->conf['zeus_root']."/web/global.cfg")) {
				if(preg_match("/^uid\s+(.*)$/m", $this->file2str($this->conf['zeus_root']."/web/global.cfg"), $match)) {
					$this->conf['zeus_user'] = $match[1];
				}
			}
		}

		return isset($this->conf['zeus_user']) ? $this->conf['zeus_user'] : null;
	}

	// ZEUS_IS_RUNNING
	function zeus_is_running()
	{
		$pids = $this->pidof ("zeus");
		if(count($pids) == 0) {
			return false;
		}
		return true;
	}

	// PHP_VERSION_DETECT_FROM_ZEUS
	function php_version_detect_from_zeus($silent = false)
	{
		$this->zeus_choose_virtual_host();

		$this->webserver_root_guess();
		$this->php_type_guess ();
		if($this->conf['php_type'] == "fastcgi" && file_exists ($this->conf['zeus_root']."/php/php.fcgi")) {
			$this->php_version_search_in($this->conf['zeus_root']."/php/php.fcgi");
		}
	}

	// ZEUS_GET_VIRTUAL_HOST_URL
	function zeus_get_virtual_host_url($default = null)
	{
		$this->zeus_choose_virtual_host();
		
		$host = $this->conf['uname']['nodename'];
		
		if(isset($this->conf['zeus_ip_name'])) {
			$host = $this->conf['zeus_ip_name'];
			if(isset($this->conf['zeus_port'])) {
				$host .= ":".$this->conf['zeus_port'];
			}
		} elseif ($default) {
			$host = $default;
		}

		return $host;
	}

	// ----------------------
	// SUNONE FUNCTIONS
	// ======================

	// SUNONE_RESTART
	function sunone_restart($silent=false)
	{
		$this->sunone_root_guess();
		if (!$silent) {
			$this->infobox("Restarting SunONE ...");
		}

		if($this->my_file_exists($this->conf['sunone_root']."/restart")){
			$status = $this->run_command($this->conf['sunone_root']."/restart");
			if($status == 0){
				return true;
			}
		}
		return false;
	}

	// SUNONE_ROOT_GUESS
	function sunone_root_guess()
	{
		if(!empty($this->conf['sunone_root'])) {
			return $this->conf['sunone_root'];
		}

		$this->webserver_root_guess();

		$c = 1;
		$servers = array();
		foreach ($this->my_list_dir($this->conf['sunone_root'], "https-*") as $dir) {
			$server_id = substr($dir, strrpos($dir, "-")+1);
			if($server_id == "admserv") {
				continue;
			}
			$servers[$c++] = $server_id;
		}

		if(count($servers) > 1) {
			$i = $this->menubox("Please select the server instance you wish to use.", $servers);
			$server_id = $servers[$i];
		}
		else $server_id = $servers[1];

		$this->conf['sunone_root'] = $this->make_path($this->conf['sunone_root'], "https-".$server_id);
	}

	// SUNONE_GUESS_FROM_CONF
	function sunone_guess_from_conf()
	{
		$this->sunone_root_guess();

		$sunone_conf = $this->conf['sunone_root']."/config/server.xml";
		if(!$this->my_file_exists($sunone_conf)) {
			$this->install_log("Can't find SunONE configuration file: $sunone_conf");
			return false;
		}
		$sunone_conf_cont = $this->file2str($sunone_conf);

		$msg = "Guessed from SunONE configuration:";

		// User
		if(empty($this->conf['sunone_user']) &&
				preg_match("#PROPERTY\s+name=\"user\"\s+value=\"([^\"]*)\"#mi", $sunone_conf_cont, $match)){
			$this->conf['sunone_user'] = $match[1];
			$this->install_log("$msg USER=".$this->conf['sunone_user']);
		}
		// Port
		if(empty($this->conf['sunone_port']) &&
				preg_match("#port=\"(\d+)\"#mi", $sunone_conf_cont, $match)){
			$this->conf['sunone_port'] = $match[1];
			$this->install_log("$msg PORT=".$this->conf['sunone_port']);
		}
		// Group
		if(empty($this->conf['sunone_group']) &&
				preg_match("#PROPERTY\s+name=\"group\"\s+value=\"([^\"]*)\"#mi", $sunone_conf_cont, $match)){
			$this->conf['sunone_group'] = $match[1];
			$this->install_log("$msg GROUP=".$this->conf['sunone_group']);
		}
		// Server Name
		if(empty($this->conf['server_name']) &&
			preg_match("#servername=\"[^\"]*\"#mi", $sunone_conf_cont, $match)){
		}
		// Document root
		if(empty($this->conf['webserver_docroot_arr']) &&
				preg_match_all("#PROPERTY\s+name=\"docroot\"\s+value=\"([^\"]*)\"#mi", $sunone_conf_cont, $match)){

			$multiples = array();
			for($i=0, $c=1; $i<count($match[0]); $i++) {
				$docroot = $match[1][$i];
				if(!strstr($docroot,'$') && $this->my_dir_exists($docroot) && !isset($multiples[$docroot])) {
					$this->conf['webserver_docroot_arr'][$c] = $docroot;
					$this->install_log("$msg DOCUMENT_ROOT=$docroot");
					$multiples[$docroot] = true;
					$c++;
				}
			}
		}

		return true;
	}
	
	// SUNONE_DOCROOT_GUESS
	function sunone_docroot_guess()
	{
		if(!empty($this->conf['webserver_doc_root'])){
			return $this->conf['webserver_doc_root'];
		}

		$this->sunone_guess_from_conf();

		if(count($this->conf['webserver_docroot_arr']) > 1) {
			array_push($this->conf['webserver_docroot_arr'], "other");
			while(true) {
				$num = $this->menubox("Please select the location of your document root directory.\n".
						"This is the location where your web documents are stored.", $this->conf['webserver_docroot_arr']);

				if($this->conf['webserver_docroot_arr'][$num] == "other") {
					break;
				}
				else if($this->my_dir_exists($this->conf['webserver_docroot_arr'][$num])) {
					$this->conf['webserver_doc_root'] = $this->conf['webserver_docroot_arr'][$num];
					break;
				}

				$this->msgbox("Broken SunONE configuration!\n".
						"Document root: \"".$this->conf['webserver_docroot_arr'][$num]."\" doesn't exist");
			}
		}
		else {
			$default = array_pop($this->conf['webserver_docroot_arr']);
		}

		return $default;
	}

	// SUNONE_USER_GUESS
	function sunone_user_guess()
	{
		if(!empty($this->conf['sunone_user'])) {
			return $this->conf['sunone_user'];
		}

		$this->sunone_guess_from_conf();

		if(!empty($this->conf['sunone_root'])) {
			$conf_file = $this->conf['sunone_root']."/config/magnus.conf";
			if($this->my_file_exists($conf_file)) {
				if(preg_match("/^\s*User\s+(.*)$/m", $this->file2str($conf_file), $match)) {
					$this->conf['sunone_user'] = $match[1];
				}
			}
		}

		return (isset($this->conf['sunone_user']) ? $this->conf['sunone_user'] : null);
	}



	// ----------------------
	// PHP FUNCTIONS
	// ======================

	// DONT_HAVE_PHP
	function dont_have_php()
	{
		if (@isset($this->conf['compatibility_page'])) {
			$compatibility_info = <<<EOF

For compatibility information, please refer to the {$this->conf['product']}
system requirements page at:

{$this->conf['compatibility_page']}
EOF;
		}
		$this->on_error(
			<<<EOF
In order to install the {$this->conf['product']} you must have supported PHP installed
on your computer. Please install a valid PHP, then restart the installation.
{$compatibility_info}
EOF
		);
	}

	// PHP_VERSION_DETECT
	function php_version_detect($silent = false, $warn_unsupported_php = false)
	{
		if(!$this->php_version_detect_from_webserver($silent)){
			if(!$silent) {
				if(!$this->php_version_ask()){
					$this->php_version_select_manually();
				}
			}
		}

		if(isset($this->conf['php_version'])) {
			$this->install_log("Detected PHP version: ".$this->conf['php_version']);

			if($this->conf['php_version'] == "unsupported" && $warn_unsupported_php) {
				$this->msgbox(
					"Installation has detected PHP version ".$this->conf['detected_unsupported_php_version'].".\n".
					"This version is not compatible with the ".$this->conf['product']." ".$this->conf['version'].".\n".
					"The installation process will continue but in order for the ".$this->conf['product']."\n".
					"to load you must switch to a compatible PHP version.\n\n".
					"For a detailed compatibility table please refer to\n".
					(isset($this->conf['compatibility_page']) ? $this->conf['compatibility_page'] : "http://www.zend.com/store/products/product_compatibility.php"));
			}
		}
	}

	// PHP_VERSION_DETECT_FROM_APACHE
	function php_version_detect_from_apache($silent = false)
	{
		if($this->is_using_apache($silent)){

			$this->apache_conf_guess($silent);
			$this->php_path_guess($silent);

			if(isset($this->conf['php_type'])) {
				if($this->conf['php_type'] == "unknown"){
					$this->apache_exec_guess($silent);
					if($this->php_version_search_in($this->conf['apache_exec_file'])){
						$this->conf['php_type'] = "builtin";
					}
				}
				elseif($this->conf['php_type'] == "executable" && isset($this->conf['php_path'])){
					if(preg_match("/(\d+\.\d+\.\d+)/", $this->cmd2str($this->conf['php_path']. " -v 2>/dev/null"), $match)){
						$this->conf['php_version'] = $match[1];
					}
				}
				elseif(($this->conf['php_type'] == "module" || $this->conf['php_type'] == "fastcgi")
					&& isset($this->conf['php_path'])){
					$this->php_version_search_in($this->conf['php_path']);
				}
			}
		}
	}

	// PHP_VERSION_ASK
	function php_version_ask($msg=null, $type=null)
	{
		$this->php_versions_array_init();
		
		if(!$this->conf['interactive']) {
			return false;
		}	

		if($msg == null){
			$msg = "Setup has failed to determine your PHP version.";
		}

		$menu = array(
				1 => "Detection of PHP version from a binary.",
				2 => "Manually select my PHP version."
				);
		$choice = $this->menubox($msg, $menu);

		if($choice == 1){
			return $this->php_version_detect_from_binary();
		}
		elseif($choice == 2){
			$this->php_version_select_manually();
			return true;
		}
	}

	// PHP_VERSION_DETECT_FROM_BINARY
	function php_version_detect_from_binary()
	{
		$this->php_versions_array_init();

		$this->php_type_guess();

		$confirm_msg = "Setup will attempt to determine your PHP version from a binary.\n";

		if($this->conf['php_type'] == "module") // detect PHP version from a PHP module
		{
			$default = empty($this->conf['php_path'])? "" : $this->conf['php_path'];
			$this->conf['php_path'] = $this->select_file(
					$confirm_msg."Please enter the full path to your PHP module", $default);

			$this->php_version_search_in($this->conf['php_path']);
		}
		elseif($this->conf['php_type'] == "executable") // detect from a PHP binary
		{
			$default = empty($this->conf['php_path'])? "" : $this->conf['php_path'];
			$this->conf['php_path'] = $this->select_file(
					$confirm_msg."Please enter the full path to your PHP executable file", $default);

			if(preg_match("/(\d+\.\d+\.\d+)/", $this->cmd2str($this->conf['php_path']. " -v 2>/dev/null"), $match)){
				$this->conf['php_version'] = $match[1];
			}
		}
		elseif($this->conf['php_type'] == "builtin") // detect from an Apache executable
		{
			$default = empty($this->conf['apache_exec_file'])? "": $this->conf['apache_exec_file'];
			$php_search_path = $this->select_file(
					$confirm_msg."Please enter the full path to your ".$this->conf['webserver'], $default);

			$this->php_version_search_in($php_search_path);
		}
		elseif($this->conf['php_type'] == "fastcgi") // detect from a PHP FastCGI SAPI executable
		{
			$php_search_path = $this->select_file(
					$confirm_msg."Please enter the full path to your FastCGI PHP application", "");

			$this->php_version_search_in($php_search_path);
		}

		if(empty($this->conf['php_version'])){
			$this->msgbox("The installation still fails to determine your PHP version.");
			return false;
		}

		return $this->php_version_confirm();
	}

	// PHP_VERSION_SELECT_MANUALLY
	function php_version_select_manually($msg="")
	{
		$this->php_versions_array_init();

		if(!$this->conf['interactive']) {
			return false;
		}

		foreach ($this->conf['php_versions_array'] as $num => $php_arr){
			if($php_arr[2] != 0){
					$sub_str=substr($php_arr[0],0 ,(strlen($php_arr[0])-1) );
					$php_ver_arr[$num]= $php_arr[0]." (".$sub_str.$php_arr[2]." and later)";
			}else{
				$php_ver_arr[$num]= $php_arr[0];
			}
			$last_i = $num+1;
		}

		$msg = "$msg\nIn order to install the correct version of the Zend Modules\n".
			"you must specify which PHP version you are using.";

		$php_ver_arr[$last_i] = "Other PHP version";
		$php_ver_arr[$last_i+1] = "I don't have PHP";
		$choice = $this->menubox($msg , $php_ver_arr);

		if($choice == $last_i || $choice == $last_i+1) {
			if(isset($this->conf['doesnt_need_php'])) {
				$this->conf['php_version'] = "donthave";
			}
			else $this->dont_have_php();
		}
		else $this->conf['php_version'] = $this->conf['php_versions_array'][$choice][1];

		return true;
	}

	// PHP_TYPES_SET
	function php_types_set($php_types_arr)
	{
		$this->install_log("PHP types was set to: ". var_export($php_types_arr, true));
		$this->conf['php_types_supported'] = $php_types_arr;
	}

	// PHP_TYPE_ADD
	function php_type_add($new_php_type) // PHP type is an array that consists of two elements:
	                                     // 1) PHP type description
																			 // 2) PHP type internal use name
	{
		$this->install_log("Added new PHP type: ". $new_php_type[0]);
		array_push($this->conf['php_types_supported'], $new_php_type);
	}

	// PHP_TYPE_EXISTS
	function php_type_exists($php_type)
	{
		$this->php_types_array_init();

		if(!empty($this->conf['php_types_supported'])){
			foreach($this->conf['php_types_supported'] as $php_type_arr){
				if($php_type_arr[1] == $php_type){
					return true;
				}
			}
		}
		return false;
	}

	// PHP_TYPE_GUESS
	function php_type_guess()
	{
		$this->install_log("Trying to guess PHP type ...");

		$this->php_types_array_init();
		$this->apache_guess_from_conf();

		if(!empty($this->conf['php_type']) && $this->conf['php_type'] != "unknown"){
			$this->php_type_supported_check();
			return true;
		}
		else {
			if($this->is_using_apache() && $this->apache_has_module("mod_php")) {
				$this->conf['php_type'] = "builtin";
				$this->php_type_supported_check();
				return true;
			}
		}

		$old_php_type = isset($this->conf['php_type'])? $this->conf['php_type'] : "unknown";

		$this->install_log("Old PHP type was: $old_php_type");

		while(1) {
			if(count($this->conf['php_types_supported'])==1){
				if($this->yesnobox("Are you using ".$this->conf['php_types_supported'][1][0]."?")){
					$this->conf['php_type'] = $this->conf['php_types_supported'][1][1];
				}
				else{
					$this->on_abort("For now ".$this->conf['product']." supports only ".
							$this->conf['php_types_supported'][1][0]." with ".$this->conf['webserver'], true);
				}
			}
			else {
				foreach($this->conf['php_types_supported'] as $num => $data){
					$php_types[$num] = $data[0];
					$last_i = $num+1;
				}

				$php_types[$last_i] = "I don't have PHP";
				$choice = $this->menubox("Please choose the type of PHP you use", $php_types, false,
						"The ".$this->conf['product']." supports the following PHP configurations:\n\n".
						$this->array2string($this->conf['php_types_supported'], 0));

				if($choice == $last_i) {
					if(isset($this->conf['doesnt_need_php'])) {
						$this->conf['php_type'] = "donthave";
					}
					else $this->dont_have_php();
				}
				else $this->conf['php_type'] = $this->conf['php_types_supported'][$choice][1];
			}

			if($this->conf['php_type'] != "executable") { /* Dummy check */
				if(isset($this->conf['php_path'])) {
					if(preg_match("/^([\d+\.]+)$/", $this->get_binary_strings($this->conf['php_path']))) {
						$this->msgbox("You have specified that you are using PHP ".$this->conf['php_type']."\n".
								"But the installation detected usage of PHP CGI.\nPlease don't try to cheat the installation.");
						continue;
					}
				}
			}
			break;
		}
		$this->install_log("Guessed PHP type: ".$this->conf['php_type']);

		if($this->conf['php_type'] != $old_php_type){
			unset($this->conf['php_path']);
		}

		return true;
	}
	
	// PHP_TYPE_SUPPORTED_CHECK
	function php_type_supported_check()
	{
		$this->install_log("Checking eigther PHP type (".$this->conf['php_type'].") is supported.");

		$this->php_types_array_init();

		if(empty($this->conf['php_type']) || $this->conf['php_type'] == "unknown") {
			$this->php_type_guess();
		}

		if(!$this->php_type_exists($this->conf['php_type'])) {

			$this->on_abort("ERROR: Unsupported PHP configuration.\n".
					"The ".$this->conf['product']." supports the following PHP configurations:\n\n".
					$this->array2string($this->conf['php_types_supported'], 0), true);
		}
	}

	// PHP_VERSIONS_ARRAY_INIT
	function php_versions_array_init()
	{
		if(@isset($this->conf['php_versions_array_initialized'])){
			return;
		}
		/*
		$this->conf['php_versions_array'] = array(
				1 =>  array("PHP 4.0.5/6", "4.0.6", 0),
				2 =>  array("PHP 4.1.x", "4.1.x", 0),
				3 =>  array("PHP 4.2.0", "4.2.0", 0),
				4 =>  array("PHP 4.2.x (other than 4.2.0)", "4.2.x", 0),
				5 =>  array("PHP 4.3.x", "4.3.x", 0),
				6 =>  array("PHP 5.0.x", "5.0.x", 0)
		);
		*/
		$this->conf['php_versions_array'] = array();
		$this->conf['php_ts_versions_array'] = array();
		$php_vers = glob("../data/*_comp");
		if (is_array($php_vers)) {
			foreach ($php_vers as $php_ver) {
				$ts = file_exists ("{$php_ver}/TS");
				$php_ver = basename ($php_ver);
				$php_ver = str_replace ("_comp", "", $php_ver);
				$php_ver = str_replace ("_", ".", $php_ver);
				if ($php_ver == "4.0.6") {
					$ver = array("PHP 4.0.5/6", "4.0.6", 0);
				} else if ($php_ver == "4.2.x") {
					$ver = array("PHP 4.2.x (other than 4.2.0)", "4.2.x", 0);
				} else {
					$ver = array("PHP {$php_ver}", $php_ver, 0);
				}
				$this->conf['php_versions_array'][] = $ver;
				if ($ts) {
					$this->conf['php_ts_versions_array'][] = $ver;
				}
			}
		}
		$this->conf['php_versions_array_initialized'] = true;
	}

	// PHP_TYPES_ARRAY_INIT
	function php_types_array_init()
	{
		if(!empty($this->conf['php_types_supported'])){
			return;
		}
		$this->conf['php_types_supported'] = array(
				1 => array("PHP module (e.g libphp4.so)", "module"),
				2 => array("PHP executable (also called CGI)", "executable"),
				3 => array("PHP compiled into ". $this->conf['webserver'], "builtin"),
				4 => array("Other", "unknown")
				);
	}

	// PHP_VERSION_SUBSTITUTE
	function php_version_substitute($silent = false)
	{
		$this->php_versions_array_init();

		if(!isset($this->conf['php_version'])){
			if($silent) {
				return false;
			}
			$this->php_version_select_manually();
		}

		foreach ($this->conf['php_versions_array'] as $num => $php_arr){ /* look for an exact match */
			if($php_arr[1] == $this->conf['php_version']){
				return true;
			}
			elseif (($php_arr[1] == "4.0.6" ) && ("4.0.5" == $this->conf['php_version'])){
				$this->conf['php_version'] = $php_arr[1];
				return true;
			}
		}
		foreach ($this->conf['php_versions_array'] as $num => $php_arr){
			if($this->ver_equal($php_arr[1], $this->conf['php_version'], $php_arr[2])){ /* look for a corresponding match */
				$this->conf['php_version'] = $php_arr[1];
				return true;
			}
		}

		$this->conf['detected_unsupported_php_version'] = $this->conf['php_version'];
		return false;
	}

	// PHP_VERSION_CONFIRM
	function php_version_confirm()
	{
		$this->php_versions_array_init();

		$this->php_type_supported_check();

		if($this->yesnobox("Please confirm the PHP version you use: ".$this->conf['php_version'])){
			if($this->php_version_substitute()){
				$this->install_log("Confirmed PHP version: ".$this->conf['php_version']);
				return true;
			}

			if(count($this->conf['php_versions_array']) != 1) {
				$vers_msg = "on of the following versions of PHP:\n\n"
					.$this->array2php_support_string($this->conf['php_versions_array'], 0, 2);
			}
			else {
				$vers_msg = $this->array2php_support_string($this->conf['php_versions_array'], 0, 2);
			}

			$this->on_error($this->conf['product']." does not support PHP ".$this->conf['php_version'].
					"\nIn order to install ".$this->conf['product']." on ".$this->conf['uname']['sysname']."\n".
					"you must have $vers_msg");
		}

		unset($this->conf['php_version']);
		return false;
	}

	// PHP_VERSIONS_GET_ARRAY
	function php_versions_get_array($ts=false)
	{
		$this->php_versions_array_init();

		$key = $ts ? 'php_ts_versions_array' : 'php_versions_array';

		$php_vers_array = array();
		foreach($this->conf[$key] as $php_ver) {
			array_push($php_vers_array, $php_ver[1]);
		}

		return $php_vers_array;
	}
	
	// PHP_VERSION_ADD
	function php_version_add($descr, $ver, $from=0) /* description of version , PHP version */
	{
		$this->php_versions_array_init();

		$descr = trim($descr);
		$ver = trim($ver);

		$found = false;
		foreach($this->conf['php_versions_array'] as $num => $php_arr) {
			if($php_arr[1] == $ver) {
				$found = true;
				$this->conf['php_versions_array'][$num] = array($descr, $ver, $from);
				break;
			}
		}
		if(!$found) {
			if(count($this->conf['php_versions_array']) == 0) {
				// Always begin the index from 1
				$this->conf['php_versions_array'][1] = array($descr, $ver, $from);
			}
			else array_push($this->conf['php_versions_array'], array($descr, $ver, $from));
		}

		$this->install_log("Added $ver ($descr) from version $from to the array of PHP versions");
	}

	// PHP_VERSION_REMOVE
	function php_version_remove($ver)
	{
		$this->php_versions_array_init();

		$new_array = array();
		$index = 1;

		foreach($this->conf['php_versions_array'] as $num => $php_arr){
			if($php_arr[1] != $ver){
				$new_array[$index++] = $php_arr;
			}
			else{
				$this->install_log("Removed $ver from array of PHP versions");
			}
		}

		$this->conf['php_versions_array'] = $new_array;
	}

	// PHP_VERSIONS_SET
	function php_versions_set($php_arr)
	{
		$this->install_log("PHP versions set as: ". var_export($php_arr, true));
		$this->conf['php_versions_array'] = $php_arr;
	}

	// PHP_VERSION_SEARCH_IN
	function php_version_search_in($file)
	{
		$this->progress_bar_start(100, "Trying to determine the version of PHP.\n".
				"Please wait... (this may take a few seconds)", "Detecting ...");

		$ret = false;
		if(!$this->my_file_exists($file)){
			$this->install_log("Path to PHP: $file is broken");
		}
		else if($this->is_broken_link($file)) {
			$this->warn("Link: $file is broken");
		}
		else {
			$file_cont = $this->get_binary_strings($file);
			if(preg_match("/PHP\/(\d+\.\d+\.\d+)/", $file_cont, $match)){
				$this->conf['php_version'] = $match[1];
				$ret = $match[1];
			}
		}

		$this->progress_bar_stop();
		return $ret;
	}

	// PHP_PATH_GUESS
	function php_path_guess()
	{
		if(empty($this->conf['php_type'])){
			$this->conf['php_type'] = "unknown";
		}
		if(!empty($this->conf['apache_conf_file'])){
			$this->apache_guess_from_conf();
		}
	}

	// PHP_INI_FILE_PARSE_ERROR_CB
	function php_ini_file_parse_error_cb ($errno, $errstr, $errfile, $errline, $errcontext)
        {
		$this->conf['php_ini_parse_error'] = $errstr;
        }

	// PHP_INI_FILE_TEST
        function php_ini_file_test ($php_ini)
        {
		unset ($this->conf['php_ini_parse_error']);
                $old_err_handler = set_error_handler (array(&$this, "php_ini_file_parse_error_cb"));
                parse_ini_file ($php_ini);
                restore_error_handler();
        }

	// PHP_INI_LOCATION_GUESS
	function php_ini_location_guess()
	{
		if(isset($this->conf['php_ini_path'])) {
			return $this->conf['php_ini_path'];
		}

		$this->install_log("Trying to guess PHP.ini location ...");

		$default = array (
			"/usr/local/lib",
			"/usr/local/Zend/etc",
			"/usr/local/etc",
			"/etc",
			"/etc/php",
			"/etc/php4/apache",
			"/etc/php/apache1-php4",
			"/etc/php/apache1-php5",
			"/etc/php/apache2-php4",
			"/etc/php/apache2-php5"
		);

		if(isset($this->conf['prefix'])) {
			array_push ($default, $this->conf['prefix']."/etc");
		}

		foreach ($default as $path){
			if($this->my_file_exists($this->strip_symlink("$path/php.ini"))) {
				$this->install_log("Found php.ini in: $path");
				$php_ini_dir = $path;
				break;
			}
		}

		if(empty($php_ini_dir)){
			$php_ini_dir = "";
		}

		if(isset($this->conf['default_php_ini_path']) && @file_exists ($this->conf['default_php_ini_path'])) {
			$php_ini_dir = $this->conf['default_php_ini_path'];
		}

		while(true){
			$default_path = $php_ini_dir;
			$word = ($default_path == "") ? "Enter" : "Confirm";
			$php_ini_dir = $this->select_dir("$word the location of your php.ini file", $default_path);

			if(!$this->my_file_exists($this->strip_symlink("$php_ini_dir/php.ini"))) {
				if(!$this->yesnobox ("php.ini was not found in $php_ini_dir.\n".
					"Do you wish that an empty php.ini will be created in that location?")) {
					continue;
				}
				else {
					$this->my_unlink ("$php_ini_dir/php.ini");
					$this->my_touch ("$php_ini_dir/php.ini");
				}
			}

			$php_ini_path = "$php_ini_dir/php.ini";

        		$this->php_ini_file_test ($php_ini_path);
			if (@isset ($this->conf['php_ini_parse_error'])) {
				$this->msgbox (
					"PHP configuration file: {$php_ini_path} is corrupted:\n".
					$this->conf['php_ini_parse_error'].
					"\nPlease try again!"
				);
				continue;
			}
			
			$this->conf['php_ini_path'] = $php_ini_path;
			$this->strip_symlink($this->conf['php_ini_path']);
			break;
		}

		$this->conf['saved_config']['default_php_ini_path'] = $php_ini_dir;
		$this->install_log("Confirmed the location of php.ini at: $php_ini_dir");
	}

	// PHP_INI_FIX
	function php_ini_fix()
	{
		$this->php_ini_open();
		if($this->php_ini_replace_entry("\' at MYSQL_PORT", "; at MYSQL_PORT")) {
			$this->install_log("PHP.ini fixed");
		}
	}

	// GUESS_CONFLICT
	function guess_conflict()
	{
		$this->install_log("Trying to guess a conflict in PHP.ini ...");

		$not_asked=TRUE;

		foreach ($this->conf['php_ini_cont'] as $num => $line){
			$line = chop($line);
			if(preg_match("#^\s*zend_extension(?:_ts)?=.*ZendAccelerator.*#", $line) ||
				preg_match("#^\s*zend_extension(?:_ts)?=.*ZendPerformanceSuite.*#", $line)){

				if($not_asked){
					$not_asked=FALSE;
					$config_choose = $this->menubox(
						<<<EOF
You appear to have an old version of the Zend Performance Suite (ZPS)
installed, that is incompatible with this version of the Zend Studio
Server.  If you choose to continue the installation, ZPS will be
disabled.  To re-enable it  please download and install the Zend
Performance Suite version 3.5.0a or later.
EOF
, 						array(
							1 => "Disable ZPS and continue",
							2 =>  "Abort installation")
						);
				}

				if($config_choose == 1){
					$this->conf['php_ini_cont'][$num] = preg_replace(
						"@^\s*(.*)@", ";Zend; \\1", $this->conf['php_ini_cont'][$num]);
						$this->install_log("Commenting PHP.ini entry: $line");
				}else{
					$this->default_abort();
				}
			}
		}

		$this->install_log("PHP.ini checked for a conflict");
	}

	// PHP_INI_GUESS_BAD_DIRECTIVES
	function php_ini_guess_bad_directives ($bad_directives, $add_msg="")
	{
		$existing_bad_directives = array();
		foreach ($bad_directives as $entry => $pattern) {
			$value = $this->php_ini_get_entry ($entry, $pattern);
			if (!empty ($value)) {
				$existing_bad_directives[] = "{$entry} = {$value}";
			}
		}
		if (count($existing_bad_directives) > 0) {
			$this->on_abort(
				"\nThe installation has detected one or more PHP configuration directives that\n".
				"will conflict with {$this->conf['product']}.  These directive(s) are located in the\n".
				"server's php.ini file.\n\n".
				"The conflicting directive(s) are:\n\n".
				implode ("\n", $existing_bad_directives)."\n\n".
				"To resolve this problem, you can comment the above directive(s), restart\n".
				"your Web server, and re-run the {$this->conf['product']} installation.\n\n$add_msg"
				, true
			);
		}
	}

	// PHP_INI_REORDER
	function php_ini_reorder()
	{
		$this->php_ini_open();

		$zend_ext_order_table = array(
				"ZendExtensionManager"    => "",
				"ZendExtensionManager_TS" => "",
				"ZendOptimizer"		        => "",
				"ZendOptimizer_TS"        => "",
				"ZendAccelerator"         => "",
				"ZendDebugger"            => "",
				"ZendDebugger_TS"         => ""
				);

		foreach($this->conf['php_ini_cont'] as $line_num => $line){
			$line = chop($line);
			if(preg_match("/^\s*zend_extension(?:_ts)?=([a-zA-Z0-9_\/\"-\.]*Zend[a-zA-Z0-9_]+\.[a-z]+)/",
						$line, $match)){
				$zend_module = basename($match[1]);
				$zend_module = preg_replace("/\.[^\.]+$/", "", $zend_module);
				$zend_ext_order_table[$zend_module] = $line;
				$this->conf['php_ini_cont'][$line_num] = "";
			}
		}

		foreach($zend_ext_order_table as $module => $line){
			if(strlen($line) > 0) {
				array_push($this->conf['php_ini_cont'], "$line\n");
			}
		}

		$this->install_log("PHP.ini re-ordered");
	}

	// PHP_INI_REMOVE_ENTRY
	function php_ini_remove_entry($value, $just_comment=true)
	{
		$this->php_ini_open();

		reset($this->conf['php_ini_cont']);
		foreach ($this->conf['php_ini_cont'] as $num => $line){

			if(!preg_match("/^\s*;/", $line) && preg_match("#$value#", $line)){

				if ($just_comment) {
					$this->conf['php_ini_cont'][$num] = preg_replace("@^\s*(.*$value)@m", ";Zend; \\1", $this->conf['php_ini_cont'][$num]);
					$this->install_log("Commenting PHP.ini entry: $line");
				}
				else {
					unset($this->conf['php_ini_cont'][$num]);
					$this->install_log("Removing PHP.ini entry: $line");
				}
			}
		}
	}

	// IN_PHP_INI
	function in_php_ini($key)
	{
		$this->php_ini_open();

		reset($this->conf['php_ini_cont']);
		foreach ($this->conf['php_ini_cont'] as $num => $line){

			$line = preg_replace("/;.*$/", "", $line);

			if(preg_match("/^\s*$key\s*=/", $line)){
				return true;
			}
		}
		return false;
	}

	// PHP_INI_GET_ENTRY
	function php_ini_get_entry($key, $pattern="")
	{
		$this->php_ini_open();

		reset($this->conf['php_ini_cont']);
		foreach ($this->conf['php_ini_cont'] as $num => $line){
			$line = preg_replace("/;.*$/", "", $line);
			if(preg_match("@^\s*$key\s*=(.*".addcslashes($pattern, '@').".*)@", $line, $match)){
				return trim($match[1], "\"\' \t");
			}
		}

		if ($key != "zend_ini_file" && !strstr($this->conf['product'], 'ZendCore')) {
			# Try to search for value in zend.ini:
			if (!isset($this->conf['zend_ini_file_parsed'])) {
				$zend_ini_file = $this->php_ini_get_entry ("zend_ini_file");
				if (@file_exists ($zend_ini_file)) {
					include_once ('Config.inc');
					$this->zend_ini_config_cache =& new Config ($zend_ini_file, CONFIG_INI);
				}
				$this->conf['zend_ini_file_parsed'] = true;
			}
			if (isset($this->zend_ini_config_cache)) {
				$val = $this->zend_ini_config_cache->get ($key, null, $pattern);
				if ($val !== null) {
					return $val;
				}
			}
		}
		return "";
	}

	// PHP_INI_MOVE_ENRY_TO_ZEND_INI
	function php_ini_move_entry_to_zend_ini ($entry, &$zend_ini_conf)
	{
		if ($this->in_php_ini ($entry)) {
			$value = $this->php_ini_get_entry ($entry);
			$zend_ini_conf->set ($entry, $value);
			$this->php_ini_remove_entry ($entry, false);
		}
	}

	// PHP_INI_ADD_ENTRY
	function php_ini_add_entry($key, $value, $replace_old_entries=true, $commented=false)
	{
		$this->php_ini_open();

		$new_line = "$key=$value". $this->conf['nl'];
		if($commented){
			$new_line = ";$new_line";
		}
		reset($this->conf['php_ini_cont']);
		$replaced = false;

		foreach ($this->conf['php_ini_cont'] as $num => $line){
			$line = preg_replace("/;.*$/", "", $line);
			if(preg_match("/^\s*$key\s*=/", $line)){
				$replaced = true;
				if($replace_old_entries) {
					$this->conf['php_ini_cont'][$num] = $new_line;
					$this->install_log("Replaced line: ".$this->conf['php_ini_cont'][$num]." by: $new_line in php.ini");
				}
			}
		}

		if (!$replaced) {
			array_push($this->conf['php_ini_cont'], $new_line);
			$this->install_log("Added new line to php.ini: $new_line");
		}
	}

	// PHP_INI_UNCOMMENT_ENTRY
	function php_ini_uncomment_entry ($key)
	{
		$this->php_ini_open();
		foreach ($this->conf['php_ini_cont'] as $num => $line){
			if(preg_match("/^\s*\;\s*($key\s*=.*)$/", $line, $match)){
				$this->conf['php_ini_cont'][$num] = $match[1].$this->conf['nl'];
				$this->install_log("Uncommented entry: {$key} in php.ini");
			}
		}
	}

	// PHP_INI_ADD_PATH
	function php_ini_add_path($entry_name, $value)
	{
		$this->php_ini_open();

		$sep = ":";
		$old_value = $this->php_ini_get_entry($entry_name);
		$path_arr = explode($sep, $old_value);
		array_unshift ($path_arr, $value);
		$this->php_ini_add_entry ($entry_name, trim(implode($sep, array_unique ($path_arr)), " \t:"));
	}

	// PHP_INI_REPLACE_ENTRY
	function php_ini_replace_entry($old, $new)
	{
		$this->php_ini_open();

		$replaced = false;
		reset($this->conf['php_ini_cont']);
		foreach ($this->conf['php_ini_cont'] as $num => $line){
			if(preg_match("#$old#", $line)){
				$this->conf['php_ini_cont'][$num] = preg_replace("#$old#", $new, $line);
				$replaced = true;
			}
		}
		return $replaced;
	}

	// PHP_INI_ADD_EXTENSION
	function php_ini_add_extension($path, $directive, $commented=false)
	{
		$this->php_ini_open();

		$ext = basename($path);
		reset($this->conf['php_ini_cont']);

		$new_line = "$directive=$path". $this->conf['nl'];
		if($commented){
			$new_line = ";$new_line";
		}

		$found = false;
		foreach ($this->conf['php_ini_cont'] as $num => $line){
			if(preg_match("/^\s*;\s*$directive\s*=.*$ext/", $line)){
				$found = true;
				$this->conf['php_ini_cont'][$num] = $new_line;
				break;
			}
			$line = preg_replace("/;.*$/", "", $line);
			if(preg_match("/^\s*$directive\s*=.*$ext/", $line)){
				$found = true;
				$this->conf['php_ini_cont'][$num] = $new_line;
				break;
			}
		}

		if(!$found){
			array_push($this->conf['php_ini_cont'], $new_line);
		}

		$this->install_log("Added new extension to php.ini: $new_line");
	}

	// PHP_INI_ADD_PHP_EXTENSION
	function php_ini_add_php_extension($path, $commented=false)
	{
		$this->php_ini_open();

		$extension_dir = $this->php_ini_get_entry ("extension_dir");
		if (empty($extension_dir) || !preg_match("/^\//", $extension_dir)) {
			$extension_dir = dirname($path);
			$this->php_ini_add_entry ("extension_dir", $extension_dir);

			$this->msgbox (
			  "Directive extension_dir in your PHP.ini was empty or wasn't set\n".
			  "to absolute path. This directive has been changed to:\n$extension_dir\n\n".
			  "If you intend to change this directive in the future,\n".
			  "please copy all PHP extensions from:\n".
			  "$extension_dir\nto the new location.\n"
			);
		}
		
		$this->php_ini_add_extension (basename($path), "extension", $commented);

		if ($this->nice_path ($extension_dir) != $this->nice_path (dirname($path))) {
			$this->my_symlink ($path, $this->make_path ($extension_dir, basename($path)));
		}
	}

	// PHP_INI_ADD_ZEND_EXTENSION
	function php_ini_add_zend_extension($path, $directive="", $commented=false)
	{
		$this->php_ini_open();

		if($directive == ""){
			$directive = $this->conf['zend_ext_directive'];
		}
		$this->php_ini_add_extension($path, $directive, $commented);
	}

	// PHP_INI_ADD_ZEND_SECTION
	function php_ini_add_zend_section()
	{
		$this->php_ini_open();

		$found = false;
		foreach ($this->conf['php_ini_cont'] as $num => $line){
			$line =  preg_replace("/;.*$/", "", $line);
			if(preg_match("/^\s*\[Zend\]\s*/", $line)){
				$found = true;
			}
		}
		if(!$found){
			array_push($this->conf['php_ini_cont'], str_repeat($this->conf['nl'], 2)."[Zend]".$this->conf['nl']);
		}else{
			array_push($this->conf['php_ini_cont'], $this->conf['nl']);
		}
	}

	// PHP_INI_OPEN
	function php_ini_open()
	{
		if(!isset($this->conf['php_ini_cont'])){
			$this->install_log("Reading PHP.ini: ".$this->conf['php_ini_path']);
			if($this->my_file_exists($this->conf['php_ini_path'])) {
				$this->conf['php_ini_cont'] = file($this->conf['php_ini_path']);
			}
			else {
				$this->install_log("Opened new (empty) PHP.ini");
				$this->conf['php_ini_cont'] = array();
			}
			$this->php_ini_set_unmodified();
			$this->conf['php_ini_opened'] = true;

			$this->conf['php_ini_saved_file'] = $this->make_path(
				$this->conf['tmp_dir'], "saved", strtr($this->conf['php_ini_path'], "/\\: ", "____"));

			$this->my_mkdir (dirname($this->conf['php_ini_saved_file']));

			$this->str2file (join("", $this->conf['php_ini_cont']), $this->conf['php_ini_saved_file']);

			$this->watch_config_files ($this->conf['php_ini_path']);
		}
	}

	// PHP_INI_RESTORE
	function php_ini_restore()
	{
		if(isset($this->conf['php_ini_saved_file'])) {
			if(@filesize($this->conf['php_ini_path']) < @filesize($this->conf['php_ini_saved_file'])) {
				$this->my_unlink ($this->conf['php_ini_path']);
				$this->my_move ($this->conf['php_ini_saved_file'], $this->conf['php_ini_path']);
			}
		}
		unset ($this->conf['php_ini_saved_file']);
	}

	// PHP_INI_CLOSE
	function php_ini_close($nobackup=false)
	{
		$this->php_ini_save($nobackup);
		unset($this->conf['php_ini_cont']);
		$this->conf['php_ini_opened'] = false;
	}

	// PHP_INI_MODIFIED
	function php_ini_modified()
	{
		if(isset($this->conf['php_ini_cont']) && isset($this->conf['php_ini_md5'])) {
			return (md5(implode("", $this->conf['php_ini_cont'])) != $this->conf['php_ini_md5']);
		}
		return false;
	}

	// PHP_INI_SET_UNMODIFIED
	function php_ini_set_unmodified()
	{
		if(isset($this->conf['php_ini_cont'])) {
			$this->conf['php_ini_md5'] = md5(implode("", $this->conf['php_ini_cont']));
		}
	}

	// PHP_INI_SAVE
	function php_ini_save($nobackup=false)
	{
		if(isset($this->conf['php_ini_cont'])) {
			if(!$nobackup && $this->php_ini_modified())
			{
				$this->php_ini_backup();
					
				$this->install_log ("Saving PHP.ini file: ".$this->conf['php_ini_path']);
				$this->my_fwrite($this->conf['php_ini_path'], "w", join("", $this->conf['php_ini_cont']));
				$this->php_ini_set_unmodified();
			}
		}
		if (isset($this->conf['moved_pack_ini'])) {
			@unlink ($this->conf['moved_pack_ini']);
			unset ($this->conf['moved_pack_ini']);
		}
	}

	// PHP_INI_BACKUP
	function php_ini_backup()
	{
		if(!isset($this->conf['php_ini_backed_up'])) {
			$php_ini_backup = $this->my_backup($this->conf['php_ini_path'], true);
			if(isset($php_ini_backup)) {
				$this->conf['php_ini_backup_msg'] = "The original php.ini was backed up to\n $php_ini_backup";
			}
			$this->conf['php_ini_backed_up'] = true;
		}
	}

	// PHP_INI_RELOCATE
	function php_ini_relocate()
	{
		$new_ini_path = $this->conf['prefix']."/etc/php.ini";
		$this->my_mkdir($this->my_dirname($new_ini_path));

		$old_ini_path = $this->strip_symlink($this->conf['php_ini_path']);

		$msg = "The following configuration changes have been made:\n\n";

		if(realpath($old_ini_path) != realpath($new_ini_path)) {

			$this->install_log("Relocating php.ini from: $old_ini_path to: $new_ini_path");

			$this->my_move($old_ini_path, $new_ini_path);
			$this->my_unlink($this->conf['php_ini_path']);

			$this->my_symlink($new_ini_path, $this->conf['php_ini_path']);

			$msg.= "- The php.ini file has been relocated from ".dirname($old_ini_path)." to ".dirname($new_ini_path)."\n\n".
						 "- A symbolic link for the php.ini file has been created in ".dirname($this->conf['php_ini_path']).".\n";

			$added_message = true;
		}
		if(!empty($this->conf['php_ini_backup_msg'])) {
			$msg .= "\n- ".$this->conf['php_ini_backup_msg']."\n";
			$added_message = true;
		}
	
		if(!isset($added_message)) {
			return;
		}

		if(!preg_match("/^\s*$/s", $msg)) {
			$this->msgbox($msg);
		}
	}

	// PHP_INI_GET_PRODUCT_VERSION
	/* $product_suffix - product name suffix in Zend Extension Manager PHP.ini entry */
	function php_ini_get_product_version($product_suffix)
	{
		$this->php_ini_open();

		$product_path = $this->php_ini_get_entry ("zend_extension_manager.$product_suffix");
		if(@file_exists ($product_path)) {
			return substr ($product_path, strrpos($product_path, "-")+1);
		}
		return false;
	}

	// ------------------
	// INSTALL FUNCTIONS
	// ==================

	// GET_COMPONENT_VERSION
	function get_component_version($name)
	{
		$name .= ".version";
		if(!isset($this->conf['build_info'][$name])) {
			$this->on_error("Cannot find version of $name in the package.");
		}

		return $this->conf['build_info'][$name];
	}

	// ADD_PACKAGE_INFO
	function add_package_info()
	{
		$this->move_pack_ini_to_php_ini();

		$this->php_ini_add_entry (strtolower(str_replace(" ", "_", $this->conf['product'])).".version", 
			$this->get_component_version("package"));
	}

	// MOVE_PACK_INI_TO_PHP_INI
	function move_pack_ini_to_php_ini ($pack_ini=null)
	{
		if (@file_exists ($pack_ini)) {
			$pack_conf = @parse_ini_file($pack_ini);
			foreach ($pack_conf as $name => $val) {
				$this->php_ini_add_entry ($name, $val);
			}
			$this->conf['moved_pack_ini'] = $pack_ini;
		}
	}

	// ADD_COMPONENT
	function add_component($name, $destination, $make_backup)
	{
		if (!isset ($this->conf['components'][$name])) {
			$this->conf['components'][$name] = array($destination, $make_backup);
		}
	}

	// GET_COMPONENT_DEST
	function get_component_dest ($name)
	{
		foreach ($this->conf['components'] as $comp_name => $data) {
			if(count ($data) == 2 && $comp_name == $name || count ($data) == 4 && $data[2] == $name) {
				return $data[0];
			}
		}
		return null;
	}

	// SET_COMPONENTS
	function set_components($new_components)
	{
		$this->conf['components'] = $new_components;
	}

	// REMOVE_COMPONENT
	function remove_component($name)
	{
		unset($this->conf['components'][$name]);
	}

	// COMPONENT_EXISTS
	function component_exists($name)
	{
		return isset($this->conf['components'][$name]);
	}

	// SET_VAR_COMPONENT
	function set_var_component($pattern, $value) /* substitute undefined variables in components destinations */
	{
		foreach($this->conf['components'] as $name => $data){
			$this->conf['components'][$name][0] = preg_replace("#$pattern#", $value, $data[0]);
		}
	}

	// START_INSTALL
	function start_install ($stop_webserver=true, $progress_delay=0)
	{
		ini_set("max_execution_time", 100000000); /* give reasonable execution time */

		if(false && $this->is_using_apache()) {
			# If there are old copies of files in the installation prefix
			# try to stop the Web server before overwritting them
			foreach (array_keys($this->conf['components']) as $file) {
				if(preg_match("/\.so$/", $file)) {
					if($this->find_file ($this->conf['prefix'], basename($file)) > 0) {
						$msg = 
						"The installation has found installation of Zend product in ".
						$this->conf['prefix']."\n\n".
						"Its recommended to stop your Web server before proceeding with copying\n".
						"new files onto your system. Your Web server will be started automatically\n".
						"by the installation after it finishes to copy files.\n\n".
						"Do you want to stop your Web server now?";

						if($this->yesnobox($msg)) {
							$webserver_stopped = $this->apache_stop ();
						}
						break;
					}
				}
			}
		}

		$percent = 0;
		$size_done = 0;

		$backup_dirs = array(); // array of the directories to backup
		$total_size = $this->gather_components_info($backup_dirs);
		if($total_size == 0) { $total_size = 100; }

		//$this->dialog_calc_max_size();
		$gb = new Gaugebox(
			empty ($this->conf['dialog']) ? null: $this->conf['dialog'],
			isset ($this->conf['max_x'])  ? $this->conf['max_x'] : null,
			isset ($this->conf['max_y'])  ? $this->conf['max_y'] : null,
			null, 0, $this->conf['title']
		);

		foreach($backup_dirs as $dir){
			$this->my_backup($dir);
		}

		foreach($this->conf['components'] as $what => $data){

			$where = $data[0];	      // path where to install
			$make_backup = $data[1];	// either make a backup or not
			$comp_name = $data[2];		// name of the component
			$file_size = $data[3];		// file size of the component

			$size_done += $file_size;
			$percent = (int)(($size_done/$total_size)*100);

			$gb->update("Installing $comp_name", $percent);

			if($make_backup){
				$this->my_backup("$where/".basename($comp_name));
			}

			if(!$this->my_dir_exists($where)){ /* create the destination directory */
				$this->my_mkdir_long ($where);
			}
			$this->my_copy($what, $where, false /* dont follow symbolic links */ ); /* perform copy */

			if ($progress_delay > 0) {
				usleep ($progress_delay);
			}
		}
		$gb->close();

		if (empty($this->conf['dialog'])) {
			print ("\n");
		}

		if(isset($webserver_stopped)) {
			$this->apache_start();
		}
	}

	// GATHER_COMPONENTS_INFO
	function gather_components_info(&$backup_dirs)
	{
		$this->install_log("Gathering components info ...");
		$total_size = 0;

		foreach($this->conf['components'] as $what => $data){
			$where = $data[0];
			$backup = $data[1];
			$comp_found = false;

			foreach ($this->conf['comp_dirs_arr'] as $dir){

				$comp = $this->nice_path("$dir/$what");

				if($this->my_file_exists($comp)){

					$file_size = filesize($comp);
					$total_size += $file_size;
					$new_comp_arr[$comp] = array($where, $backup, $what, $file_size);
					$comp_found = true;
					break;
				}
				elseif($this->my_dir_exists($comp)){

					$this->pushd($comp);
					$this->get_file_tree(".", $files_arr);

					if($backup){
						array_push($backup_dirs, $where);
					}

					foreach($files_arr as $file => $file_size){
						$total_size += $file_size;
						$dir_dest = $this->nice_path("$where/".$this->my_dirname($file));
						$new_comp_arr[$this->nice_path("$comp/$file")] = array($dir_dest, false, $what, $file_size);
					}

					$this->popd();
					$comp_found = true;
					break;
				}
			}

			if(!$comp_found) {
				$this->on_error("Installation component: '$what' was not found.");
			}
		}

		$this->conf['components'] = $new_comp_arr;
		return $total_size;
	}

	// ADD_GROUP
	/**
	* @param string group name
	* @param int suggested group ID
	*/
	function add_group($group, $gid=null)
	{
		$err = null;

		// First check whether such group already exists:
		if (posix_getgrnam($group) !== false) {
			return true;
		}

		if ($this->conf['uname']['sysname'] == "AIX") {
			if ($this->run_command ("mkgroup {$group}") == 0) {
				if ($gid !== null) {
					$this->run_command ("chgroup id={$gid} {$group}");
				}
			}
			else $err = "Bad exit status from command: \"mkgroup {$group}\"";
		}
		else if ($this->conf['uname']['sysname'] == "Darwin") {
			if ($this->run_command ("nicl . -create /groups/{$group}") == 0) {
				if ($gid !== null) {
					// Try to change the group ID:
					$this->run_command ("nicl . -createprop /groups/{$group} gid {$gid}");
					$this->run_command ("nicl . -createprop /groups/{$group} passwd '*'");
				}
				$err = null;
			}
			else $err = "Bad exit status from command: \"nicl . -read /groups/{$group}\"";
		}
		else {
			$cmd = $this->search_cmd_in_path ("pw", true);
			if ($cmd) {
				if ($this->run_command ("{$cmd} groupadd {$group}") == 0) {
					if ($gid !== null) {
						// Try to change the group ID:
						$this->run_command ("{$cmd} groupmod {$group} -g {$gid}");
					}
					$err = null;
				}
				else $err = "Bad exit status from command: \"{$cmd} groupadd {$group}\"";
			}
			else {
				foreach (array ('groupadd', 'addgroup') as $cmd) {
					$cmd = $this->search_cmd_in_path ($cmd, true);
					if ($cmd) break;
				}
				if ($cmd) {
					if ($this->run_command ("{$cmd} {$group}") == 0) {
						if ($gid !== null) {
							// Try to change the group ID:
							$modcmd = $this->search_cmd_in_path ("groupmod", true);
							if ($modcmd) {
								$this->run_command ("{$modcmd} -g {$gid} {$group}");
							}
						}
						$err = null;
					}
					else $err = "Bad exit status from command: \"{$cmd} {$group}\"";
				}
				else $err = "Couldn't find program utility for adding new group to this system";
			}
		}

		if ($err) {
			$this->msgbox("The group {$group} couldn't be added! The error was:\n\n{$err}\n\nPlease add the group {$group} manually.");
			return false;
		}
		return true;
	}

	// REMOVE_GROUP
	/**
	* @param string group name
	*/
	function remove_group($group)
	{
		$err = null;

		if (posix_getgrnam($group) === false) {
			return;
		}

		if ($this->conf['uname']['sysname'] == "Darwin") {
			if ($this->run_command ("nicl . -delete /groups/{$group}") != 0) {
				$err = "Bad exit status from command: \"nicl . -delete /groups/{$group}\"";
			}
		}
		else {
			$cmd = $this->search_cmd_in_path ("pw", true);
			if ($cmd) {
				if ($this->run_command ("{$cmd} groupdel {$group}") != 0) {
					$err = "Bad exit status from command: \"{$cmd} groupdel {$group}\"";
				}
			}
			else {
				foreach (array ('groupdel', 'delgroup', 'rmgroup') as $cmd) {
					$cmd = $this->search_cmd_in_path ($cmd, true);
					if ($cmd) break;
				}
				if ($cmd) {
					if ($this->run_command ("{$cmd} {$group}") != 0) {
						$err = "Bad exit status from command: \"{$cmd} {$group}\"";
					}
				}
				else $err = "Couldn't find program utility for deleting a group from this system";
			}
		}

		if ($err) {
			$this->msgbox("The group {$group} couldn't be deleted! The error was:\n\n{$err}\n\nPlease delete the group {$group} manually.");
		}
	}


	// ADD_USER
	/**
	* @param string user name
	* @param string home directory path
	* @param string shell (default: /bin/false)
	* @param int suggested user ID (optional)
	*/
	function add_user($user, $home_dir, $uid=null)
	{
		$err = null;

		// First check whether such user already exists:
		if (posix_getpwnam($user) !== false) {
			return true;
		}

		if ($this->conf['uname']['sysname'] == "AIX") {
			if ($this->run_command ("mkuser home={$home_dir} {$user}") == 0) {
				if ($uid !== null) {
					$this->run_command ("chuser id={$uid} {$user}");
				}
			}
			else $err = "Bad exit status from command: \"mkuser {$user}\"";
		}
		else if ($this->conf['uname']['sysname'] == "Darwin") {
			if ($this->run_command ("nicl . -create /users/{$user}") == 0) {
				if ($uid !== null) {
					// Try to change the user ID:
					$this->run_command ("nicl . -createprop /users/{$user} uid {$uid}");
					$this->run_command ("nicl . -createprop /users/{$user} passwd '*'");
					$this->run_command ("nicl . -createprop /users/{$user} home {$home_dir}");
				}
				$err = null;
			}
			else $err = "Bad exit status from command: \"nicl . -read /users/{$user}\"";
		}
		else {
			$cmd = $this->search_cmd_in_path ("pw", true);
			if ($cmd) {
				if ($this->run_command ("{$cmd} useradd {$user} -d {$home_dir}") == 0) {
					if ($uid !== null) {
						// Try to change the user ID:
						$this->run_command ("{$cmd} usermod {$user} -g {$uid}");
					}
					$err = null;
				}
				else $err = "Bad exit status from command: \"{$cmd} useradd {$user}\"";
			}
			else {
				foreach (array ('useradd', 'adduser') as $cmd) {
					$cmd = $this->search_cmd_in_path ($cmd, true);
					if ($cmd) break;
				}
				if ($cmd) {
					if ($this->run_command ("{$cmd} -d {$home_dir} {$user}") == 0) {
						if ($uid !== null) {
							// Try to change the user ID:
							$modcmd = $this->search_cmd_in_path ("usermod", true);
							if ($modcmd) {
								$this->run_command ("{$modcmd} -g {$uid} {$user}");
							}
						}
						$err = null;
					}
					else $err = "Bad exit status from command: \"{$cmd} {$user}\"";
				}
				else $err = "Couldn't find program utility for adding new user to this system";
			}
		}

		if ($err) {
			$this->msgbox("The user {$user} couldn't be added! The error was:\n\n{$err}\n\nPlease add the user {$user} manually.");
			return false;
		}
		return true;
	}

	// REMOVE_USER
	/**
	* @param string user name
	*/
	function remove_user($user)
	{
		$err = null;

		if (posix_getpwnam($user) === false) {
			return;
		}
		if ($this->conf['uname']['sysname'] == "Darwin") {
			if ($this->run_command ("nicl . -delete /users/{$user}") != 0) {
				$err = "Bad exit status from command: \"nicl . -delete /users/{$user}\"";
			}
		}
		else {
			$cmd = $this->search_cmd_in_path ("pw", true);
			if ($cmd) {
				if ($this->run_command ("{$cmd} userdel {$user}") != 0) {
					$err = "Bad exit status from command: \"{$cmd} userdel {$user}\"";
				}
			}
			else {
				foreach (array ('userdel', 'deluser', 'rmuser') as $cmd) {
					$cmd = $this->search_cmd_in_path ($cmd, true);
					if ($cmd) break;
				}
				if ($cmd) {
					if ($this->run_command ("{$cmd} {$user}") != 0) {
						$err = "Bad exit status from command: \"{$cmd} {$user}\"";
					}
				}
				else $err = "Couldn't find program utility for deleting a user from this system";
			}
		}

		if ($err) {
			$this->msgbox("The user {$user} couldn't be deleted! The error was:\n\n{$err}\n\nPlease delete the user {$user} manually.");
		}
	}
	
	// SET_PERMISSIONS
	function set_permissions($file, $mode, $owner="", $group="")
	{
		$this->install_log("Setting permissions on $file: mode=$mode".
			(empty($owner) ? "":", owner=$owner").(empty($group) ? "":", group=$group"));

		if(!$this->my_file_exists($file) && !$this->my_dir_exists($file)){
			$this->on_error("File: $file does not exist.");
		}

		if(!empty($owner)){
			if(!chown($file, $owner)){
				$this->on_error("Cannot set ownership for: $file");
			}
		}
		if(!empty($group)){
			if($group == "zendtech"){
				$this->add_group($group, 200);
			}
			if(!@chgrp($file, $group)){
				$this->warn("Cannot set group $group for file $file");
			}
		}
		if(!empty($mode)){
			if(!chmod($file, octdec($mode))){
				$this->on_error("Cannot set permissions for: $file");
			}
		}
	}
										

	// ADD_PERMISSIONS
	function add_permissions($file, $mode, $owner="", $group="")
	{
		array_push($this->conf['file_perms'], array($file, $mode, $owner, $group));
	}

	// SET_FILE_PERMISSIONS
	function set_file_permissions()
	{
		foreach ($this->conf['file_perms'] as $perms){
			if(isset($this->conf['prefix'])) {
				$file = $this->make_full_path($perms[0], $this->conf['prefix']);
			}
			$this->set_permissions ($file, $perms[1], $perms[2], $perms[3]);
		}
	}

	// FILE_TYPE
	function file_mime_type($filename)
	{
		if(!function_exists('mime_content_type')) {
			$file_cmd = $this->search_cmd_in_path ("file");
			if($file_cmd) {
				$mime_type = trim($this->cmd2str("$file_cmd -bi $filename 2>/dev/null"));
				if(preg_match("/(\w+\/[^\s\(\)\<\>\@\,\;\:\\\"\/\[\]\?\=]+)/", $mime_type, $match)) {
					return $match[1];
				}
				return false;
			}
		}
		else return mime_content_type($filename);
		return false;
	}

	// CRONTAB_ERROR
	function crontab_error($cron_err)
	{
		$answer = $this->yesnobox("WARNING!\n\n$cron_err\n\nWould you like to continue setup anyway?", true);
		if(!$answer) {
			$this->default_abort();
		}
	}

	// ADD_TO_CRONTAB
	function add_to_crontab(
		$cron_msg, $user, $line, $cron_warn, $cron_err, $ident = "Zend crontab entry")
	{
		if (!$line) {
			$this->install_log("Removing {$ident} from the $user's crontab file.");
		} else {
			$this->install_log("Trying to add the line: $line to the $user's crontab file.");
		}

		$cron_deny_files = array(
				"/usr/lib/cron/cron.deny",   /* IRIX 5.3  */
				"/etc/cron.d/cron.deny",     /* SunOS 5.X */
				"/var/spool/cron/cron.deny", /* SunOS 4.X */
				"/etc/cron.deny",            /* Linux */
				"/var/adm/cron/cron.deny"    /* HP-UX */
				);

		foreach ($cron_deny_files as $file) {
			if($this->my_file_exists($file)) {
				if(preg_match("/^$user$/m", $this->file2str($file))) {
					$this->crontab_error($cron_err);
					return;
				}
			}
		}

		$crontab_opt = $this->cmd2str("crontab -H 2>&1"); // get crontab options (guess type)
		if(preg_match("@-u\s+user@", $crontab_opt)){
			$crontab_type = 2;
		}
		else if(preg_match("@user@", $crontab_opt)){
			$crontab_type = 1;
		}

		if(isset($crontab_type)){

			if($crontab_type == 1){
				$crontab_cont = $this->cmd2str("crontab -l $user 2>/dev/null");
			}
			else{
				$crontab_cont = $this->cmd2str("EDITOR=cat VISUAL=cat crontab -u $user -e 2>/dev/null");
			}

			$error = "Failed to execute command: ";

			// remove the old settings
			$crontab_cont = preg_replace("@[\r\n]*# ".$ident.":.*# End of ".$ident."[\r\n]*@s",
					"\n", $crontab_cont);

			if ($line) {
				// add the new line
				$crontab_cont .= "\n# $ident:\n$line\n# End of $ident\n";

				while(true) {
					if(!$this->yesnobox(
						"$cron_msg\n\nWe are going to add scheduled execution to the crontab of user $user:\n\n".
						wordwrap($line, $this->conf['wrap_size'])."\n\nDo you accept?"))
					{
						if($this->yesnobox($cron_warn)) {
							$this->default_abort();
						}
						continue;
					}
					break;
				}
			}

			$fp = $this->my_fopen($this->conf['tmp_dir']."/crontab", "w");
			fwrite($fp, $crontab_cont);
			fclose($fp);

			if($crontab_type == 1){ // First type of crontab

				$proc_env = "EDITOR=\"cat ". $this->conf['tmp_dir'] ."/crontab >\" ";
				$proc_env .= "VISUAL=\"cat ". $this->conf['tmp_dir'] ."/crontab >\"";

				if($this->run_command($proc_env." crontab -e $user")==0){
					unset($error);
				}
				else { $error .= "crontab -e $user"; }
			}
			else { // Second type
				if($this->run_command("crontab -u $user ". $this->conf['tmp_dir'] ."/crontab")==0){
					unset($error);
				}
				else { $error .= "crontab -u $user ". $this->conf['tmp_dir'] ."/crontab"; }
			}
		}
		else {
			$error = "Failed to execute crontab.";
		}

		if(isset($error)){
			//$this->warn("Couldn't add a crontab scheduled execution.\n$error");
			$this->crontab_error($cron_err);
		}
	}


	// --------------------------
	// LICENSE FUNCTIONS
	// ==========================

	// SHOW_LICENSE
	function show_license($file, $message="\n")
	{
		while(true) {
			$this->show_file($file);
			if(!$this->yesnobox($message."Do you accept the terms of this license?")) {
				if($this->yesnobox("In order to install ".$this->conf['product']." you must accept this agreement!\n".
							"Do you really want to cancel the installation?", true)) {
					$this->on_abort("", true);
				}
			}
			else {
				break;
			}
		}
	}

	// LICENSE_AGMNT_BOX
	function license_agmnt_box()
	{
		$license_agr_file = $this->search_file(array("../", "../data"), array("LICENSE"));
		if(!@file_exists ($license_agr_file)) {
			$this->on_error("Can't find file LICENSE in the package.");
		}

		$this->install_log("Showing license: {$license_agr_file}");

		$license = "IMPORTANT:\n".
			"BY SELECTING THE 'YES' OPTION BELOW, DOWNLOADING, INSTALLING, OR\n".
			"OTHERWISE USING THIS SOFTWARE, YOU ACKNOWLEDGE THAT YOU HAVE READ THE\n".
			"LICENSE AGREEMENT, AND THAT YOU AGREE TO BE BOUND BY ITS TERMS AND\n".
			"CONDITIONS.\n".
			"IF YOU DO NOT AGREE TO ALL OF THE TERMS AND CONDITIONS OF SUCH AGREEMENT,\n".
			"YOU ARE NOT AN AUTHORIZED USER OF THE SOFTWARE AND IT IS YOUR\n".
			"RESPONSIBILITY TO EXIT THIS DOWNLOADING/INSTALLATION PROCESS WITHOUT\n".
			"DOWNLOADING OR INSTALLING THE SOFTWARE BY SELECTING THE 'NO' OPTION BELOW,\n".
			"AND TO DELETE THE SOFTWARE FROM YOUR COMPUTER.\n\n\n";

		$this->show_license ($license_agr_file, $license);
	}

	// VERIFY_KEY
	function verify_key($user, $key)
	{
		$rc = $this->run_command("./verifykey \"$user\" $key");
		if($rc == 0){
			$this->on_error("Wrong parameters passed to verifykey.");
		}
		if($rc == 127){
			$this->on_error("Could not execute verifykey.");
		}

		return $rc;
	}

	// DLG_VERIFY_LICENSE_KEY
	function dlg_verify_license_key(&$user, &$key)
	{
		if(isset($this->conf['license_user'])) {
			$user = $this->conf['license_user'];
		}
		if(isset($this->conf['license_key'])) {
			$key = $this->conf['license_key'];
		}

		$rc = 0;
		do{
			switch($rc){
				case    0:  $error = "";
										break;
				case  255:  $error = "The name and/or key you have entered are incorrect !\n\n";
										break;
				case  254:  $error = "The key you have entered has expired !\n\n";
										break;
				default  :  $error = "Unrecognized return value from verifykey !\n\n";
			}
			$user = $this->inputbox(
					$error."Please enter the Registration Name you received for the ".$this->conf['product'], $user);

			while(true){
				$key = $this->inputbox("Please enter the License Key you received for the ".$this->conf['product'], $key);
				if(!preg_match("/^[A-Za-z0-9]+$/", $key)){
					$this->msgbox("The key you entered contains invalid characters.  Please try again!");
				}
				else{
					$key = preg_replace("/^LK/i", "", trim($key));
					if(!preg_match("/^[A-Za-z0-9]+$/", $key)){
						$this->msgbox("Invalid key.  Please try again!");
					}else{
						break;
					}
				}
			}
		}
		while(($rc = $this->verify_key($user, $key)) != 1);
	}


	// GET_LMUTIL_HOST_ID
	/*
	function get_lmutil_host_id()
	{
		$line = $this->cmd2str("../lmutil lmhostid");
		if(!preg_match("/^.+\"(.+)\"/m", $line, $hostid)){
			$this->on_error("Can't determine host ID.");
		}

		return $hostid[1];
	}
	*/


	function generate_trial_license($install_trial_utility = true) {
		// generate trial license with special secret utility
		$this->infobox ('Generating trial license...');
		preg_match ('/(M:[A-Z0-9\-]+)/', $this->get_host_id(), $matches);
		$m_host_id = $matches[1];
		$trial_utility = $this->generate_trial_utility();
		$license_file = (isset($this->conf['trial_license_file']) ?
			$this->conf['trial_license_file'] : $this->conf['license_file']);
		$rc = $this->run_command ($trial_utility . ' ../data/' . $license_file . " $m_host_id");
		if ($rc != 0)
		  {
		    $msg = '';
		    switch ($rc)
		      {
		      case 1: $msg = 'Utility not installed'; break;
		      case 2: $msg = 'Utility expired'; break;
		      case 3: case 31: case 32: case 33:
			$msg = 'Utility broken'; break;
		      case 4: $msg = 'Invalid host id'; break;
		      case 5: $msg = 'Signature failed'; break;
		      case 7: $msg = 'Random geberator failed'; break;
		      case 8: $msg = 'Utility expired'; break;
		      }
		    $this->msgbox ("Error generating trial license: ${msg}.");
		    return (-1);
		  }
		if ($install_trial_utility) {
			$this->add_component ($trial_utility, $this->conf['prefix']."/sbin", false);
		}
	}

	function get_license(&$passwd, &$uid, $license_file = "")
	{
		if($license_file == ""){
			$license_file = $this->conf['license_file'];
		}

		// check for license in package
		if($this->my_file_exists("../data/$license_file")){
			return 0;
		}

		$args = array();
		static $save_args;

		//if(extension_loaded("curl") && $this->conf['interactive']) {
		if ($this->conf['interactive'])
		  {
		  //$m1 = 'Download a license file from ' . $license_downloader->conf['license_host'];
		    $m1 = 'Get a license from Zend';
			$m2 = "Search for a license file on my disk";
			$menu = array(1 => $m1, 2 => $m2);

			$msg_add = isset($this->conf['accept_licenses']) ? "" : " The license file is called: \"{$license_file}\".";

			$msg = "In order to proceed, setup needs a valid license file.{$msg_add} If you don't have a valid license file, select \"{$m1}\". Otherwise select \n\"{$m2}\".\n\nThe license file can also be downloaded manually from:\nhttp://www.zend.com/store/pickup.php\n\n";
			$msg = wordwrap ($msg, $this->conf['wrap_size']);
			$rc = $this->menubox($msg, $menu);
		}
		else {
			$rc = 2;
		}

		if($rc == 2){
			$this->find_license_on_disk($license_file);
		}
		elseif($rc == 1){
			if(defined('SERIAL_NUMBER'))
			  {
			    // pasha nov 28 2005: as of now, it is possible to download from zend.com
			    // a commerical license only
			    $trial_commerc = "c";
			    if ($trial_commerc == "c"){
			      $serial = $this->dlg_ask_serial_number();

			      // It's impossible, that user won't be registered on the site,
			      // if he has a commercial license:
			      list ($username, $passwd) = $this->dlg_username_password();

			      // download license from zend.com
			      $this->progress_bar_start 
				(300, 
				 "Downloading license. Please wait...\n(this may take a few minutes)",
				 'Downloading ...');
			      $license_downloader = 
				new LicenseDownloader ($this->logger, $this->error_handler);
			      list ($ret_value, $license, $error_msg) = $license_downloader->download 
				($username, md5 ($passwd), $this->conf['product_id'], 
				 $this->conf['version_for_license'], $serial);
			      $this->progress_bar_stop();
			      
			      // write license to a file
			      if( ! $this->my_fwrite ($license_file, "w", $license))
				{
				  $this->error_handler->on_error ('Could not write license file.');
				}
			      $new_license_file = $license_file;
			      if (preg_match ('@^\s*Product-Name\s*=(.*)$@m', 
					      $this->file2str($license_file), $match)) 
				{
				  $new_license_file = 
				    str_replace (' ', '_', strtolower (trim ($match[1], " \t\r\n"))) . '.zl';
				}
			      if ($license_file != $new_license_file) 
				{
				  $this->my_rename ($license_file, $new_license_file);
				  $license_file = $new_license_file;
				}
			      
			      if (($ret_value == -1) && (! empty ($error_msg)))
				{
				  $this->license_errmsg ($error_msg);
				}
			      if ($ret_value != -1)
				{
				  // license file needs to end up in 'data' for install_license()
				  $this->my_copy($license_file, "../data/$license_file");
				  $this->conf['license_file'] = $license_file;
				}
			      
			      if ($ret_value == -1) 
				{
				  $passwd = '';
				  $uid    = '';
				}

			      if (! defined('SERIAL_NUMBER')) 
				{
				  if (isset ($ret_value) && ($ret_value != -1) && 
				      preg_match("/^\s*\d+$/", $ret_value)) 
				    {
				      $this->msgbox 
					('Successfully downloaded a ' . trim ($ret_value). '-day trial license!');
				    }
				  else 
				    {
				      $this->msgbox ('Successfully downloaded license file!');
				    }
				}
			      
			      if(defined('SERIAL_NUMBER') && ($ret_value!=-1))
				{
				  if(defined('TRIAL'))
				    {
				      $this->msgbox ('Successfully downloaded a ' . trim($ret_value) .
						     '-day trial license.');
				    } 
				  else 
				    {
				      $this->msgbox 
					("Successfully downloaded license file.\n".
					 "In case you may wish to download your license information in the future,\n".
					 "you may do so by accessing http://www.zend.com/store/pickup.php");
				  }
				  return (0);
				}
			      return ($ret_value);
			      // end of license download
			    }
			    else {
				$this->generate_trial_license();
			    }
			  }
			else
			  {
			    // TODO: I don't know when this supposed to be executed,
			    // bring it in a sync with the fact that only commercial license
			    // could be downloaded from zend.com
			    list ($username, $passwd) = $this->dlg_username_password 
			      ("(If you don't have a Zend username,\nplease register at www.zend.com/add_user.php)\n\n");
			  }
		}
		return 0;
	} // end of get_license()



	// REGISTER_USER
	function register_user(&$args)
	{
		do
		{
			$this->forms->reg_page1($args);
		}
		while(!$this->forms->reg_page2($args));
	}


	// returns: serial number
	//function dlg_ask_serial_number(&$args)
	function dlg_ask_serial_number()
	{
		if(isset($this->conf['serial_number'])) {
			$serial = $this->conf['serial_number'];
		}
		else {
			do
			{
				$serial = $this->inputbox("Please enter your serial number. The serial number can be found at\n".
						"http://www.zend.com/store/pickup.php or in the purchase confirmation E-mail");

				if(strlen($serial) < 8){
					$this->msgbox("Serial number must contain 8 or more characters");
				}
				elseif(!preg_match ("/^SN/i",$serial)){
					$this->msgbox("Serial number should start with SN");
				}
				else break;
			}
			while(true);
		}
		//$args["serial"] = $serial;
		return ($serial);
	}


	// DLG_TRIAL_COMMERCIAL
	function dlg_trial_commercial(&$args)
	{
		if(!$this->conf['interactive']) {
			return "c";
		}

		if(defined('TRIAL')){ /* return immidiately, if the product is for trial usage */
			return "t";
		}
		$m = array(
				"t" => "Get Trial License",
				"c" => "Get Commercial License"
				);
		$r = $this->menubox("Is the product for trial or commercial use?", $m, false, "", "c");
		return $r;
	}


	// DLG_EXISTING_USER_ASK
	function dlg_existing_user_ask()
	{
		if(!isset($this->conf['existing_zend_com_account'])) {
			$this->conf['existing_zend_com_account'] = $this->yesnobox("Do you have a Zend.com account?\n".
				"You will be asked for your Zend.com username\nand password during installation.");
		}
		return $this->conf['existing_zend_com_account'];
	}


	// returns: 2-element array (username, password)
	//function dlg_username_password(&$args, $msg="")
	function dlg_username_password ($msg="")
	{
		$username_def = "";
		if(isset($this->conf['zend_username'])) {
			$username_def = $this->conf['zend_username'];
		}

		$password_def = "";
		if(isset($this->conf['zend_password'])) {
			$password_def = $this->conf['zend_password'];
		}

		$this->conf['zend_username'] = $this->inputbox($msg."Please enter your Zend.com username", $username_def);
		$this->conf['zend_password'] = $this->password_get("Please enter your Zend.com password: ");

		if(empty($this->conf['dialog'])){
			print("\n");
		}

		/*
		$args["uid"] = $this->conf['zend_username'];
		$args["pass"] = $this->conf['zend_password'];
		*/
		return (array ($this->conf['zend_username'], $this->conf['zend_password']));
	}

	
	// FIND_LICENSE_ON_DISK
	function find_license_on_disk($license_file="", $product="", $license_def_dir="")
	{
		if(empty($license_file)){
			$license_file = $this->conf['license_file'];
		}

		// check for license in package
		if($this->my_file_exists("../data/$license_file")){
			return 0;
		}

		if (!$this->conf['interactive']) {
			$this->on_error ("Please put your license file into: ".
				$this->make_path ($this->conf['installer_dir'], "data"));
		}

		// look for a license in current directory
		if($this->my_file_exists("./$license_file")){
			$this->my_move($license_file, "../data/$license_file");
			return 0;
		}

		if(empty($product)){
			$product = $this->conf['product'];
		}

		$license_dir="";

		if(isset($this->conf['accept_licenses'])) {
			while (true) {
				$lf = $this->select_file("Please enter full path to the license file\n".
					"(it's called: ".join(" or ", $this->conf['accept_licenses']).")", $license_def_dir);
				if(in_array(basename($lf), $this->conf['accept_licenses'])) {
					$license_file = basename($lf);
					$license_dir = dirname($lf);
					break;
				}
				$this->msgbox("License file should be called: ".join(" or ", $this->conf['accept_licenses']).
					"\nPlease try again!");	
			}
		}
		else {
			while(true){
				$license_dir = $this->choose_dir(
						"Specify a path to a $product license file\n(it's called: $license_file)", $license_def_dir);
				
				if(basename ($license_dir) == $license_file) {
					if(!$this->my_file_exists($license_dir)) {
						$this->msgbox ("License file: {$license_dir}\ndoes not exist. Please try again.");
					}
					else {
						$license_dir = dirname($license_dir);
						break;
					}
				}
				else if($this->my_dir_exists ($license_dir)) {
					if(!$this->my_file_exists($this->make_path($license_dir, $license_file))) {
						$this->msgbox ("License file: ".
							$this->make_path($license_dir, $license_file)."\ndoes not exist. Please try again.");
					}
					else break;
				}
				else {
					$this->msgbox ("No such file or directory: $license_dir");
				}
			}
		}

		if($this->nice_path($license_dir."/".$license_file) != $this->nice_path("../data/$license_file")) {
			$this->my_copy($license_dir."/".$license_file, "../data/$license_file");
		}

		$this->conf['license_file'] = $license_file;

		return $license_dir;
	}

	// LICENSE_ERRMSG
	function license_errmsg($msg)
	{
		$this->msgbox(
				"There was a problem downloading the license file:\n\n$msg\n" .
				"If you still can't download a license file, you can download it from\n".
				"http://www.zend.com/store/pickup.php, or email the Zend sales team at\n".
				"sales@zend.com."
				);
		return -1;
	}

	// INSTALL_LICENSE
	function install_license($license=null, $prefix=null)
	{
		if(!$prefix){
			$prefix = $this->conf['prefix'];
		}

		if(!$license){
			$license = $this->conf['license_file'];
		}

		if(!$this->my_file_exists("../data/$license")){
			$this->on_error("License file: $license does not exist");
		}
		$this->my_mkdir_long($prefix);

		#$this->infobox("Installing license $license ...");
		

		// don't overrite existing license
		$this->my_copy("../data/$license", $prefix, true, false);

		$this->conf['license_installed'] = true;
	}

	// DETECT_IP_ADDRESS
	function detect_ip_address ($default_ip = null)
	{
		$ext_ip = null;

		# First try to determine IP via external interface
		$def_route_line = shell_exec("netstat -r 2>/dev/null");
		$this->install_log ("\"netstat -r\" returned: $def_route_line");

		if(preg_match("/^\s*default.*\s([^\s]+)\s*$/m", $def_route_line, $match)) {
			$ext_int = $match[1];
		}
		else $this->install_log ("Couldn't detect the default network interface");

		if(isset($ext_int)) {
			$ifconfig_out = shell_exec("/sbin/ifconfig $ext_int 2>/dev/null");
			$this->install_log ("\"/sbin/ifconfig $ext_int\" returned: $ifconfig_out");

			if(preg_match("/inet\s[^\d]*([\d\.]+)/", $ifconfig_out, $match)) {
				$ext_ip = $match[1];
			}
			else $this->install_log ("Couldn't parse the output from the /sbin/ifconfig");
		}
		
		if(!$ext_ip) {
			# Try to reverse hostname:
			$hostname = $this->conf['uname']['nodename'];
			$ext_ip = @gethostbyname ($hostname);
		}
		
		if(!$this->is_ip_valid($ext_ip) || strncmp($ext_ip, "127.", 4) == 0) {
			$ext_ip = null;
		}

		if(!$ext_ip) {
			if ($default_ip) {
				$ext_ip = $default_ip;
			} else {
				$msg = "The installation has failed to detect the external IP address of your machine.";
				while (true) {
					$ext_ip = $this->inputbox ("{$msg}\nPlease enter the external IP address of your machine");
					if (!$this->is_ip_valid ($ext_ip) || strncmp($ext_ip, "127.", 4) == 0) {
						$msg = "The IP address you entered is invalid or irresolvable from outside.";
						continue;
					}
					break;
				}
			}
		}
		
		return $ext_ip;
	}

	// PIDOF
	function pidof($process)
	{
		$ps = $this->search_cmd_in_path ("ps");
		$pids = array();
		
		if($ps) {
			$out = $this->cmd2str("$ps aux 2>/dev/null");
			if(empty($out)) {
				$out = $this->cmd2str("$ps -ef 2>/dev/null");
			}
			$this->install_log ("Trying to find PID of $process...");
			if(preg_match_all("/^[^\d]*(\d+)\s.*".addcslashes($process, "/")."/m", $out, $match)) {
				for($i=0; $i<count($match[0]); $i++) {
					if($match[1][$i] > 10) { # To eliminate any possible error and not to kill the all
						array_push ($pids, $match[1][$i]);
					}
				}
			}
		}

		return $pids;
	}

	// RUNAS2STR
	function runas2str($cmd, $user, $need_logging = true)
	{
		$runas = $this->search_cmd_in_path("runas");

		if(!@file_exists($runas)) {
			$this->on_error ("Cannot find utility: runas");
		}

		$uid = $this->user2uid($user);

		return $this->run_command ("$runas $uid \"$cmd\"", $need_logging);
	}

	// RUNAS
	function runas($cmd, $user)
	{
		$runas = $this->search_cmd_in_path("runas");

		if(!@file_exists($runas)) {
			$this->on_error ("Cannot find utility: runas");
		}

		$uid = $this->user2uid($user);

		return $this->run_command ("$runas $uid \"$cmd\"");
	}

	// DIR_IS_WRITABLE_BY_USER
	function dir_is_writable_by_user($dir, $user, $msg=true)
	{
		if(!$this->my_dir_exists ($dir)) {
			return false;
		}
		$grid = posix_getgrgid(filegroup($dir));
		if($grid["name"] == $user) {
			return true;
		}

		$test_file = "$dir/.test_file_create.".getmypid();
		if($this->runas("touch $test_file", $user) == 0) {
			$this->my_delete($test_file);
			return true;
		}
		if ($msg) {
			$this->msgbox ("Directory: $dir is not writable by user '$user'!\n".
				"Please check, that '$user' can write to that directory,\n".
				"otherwise ".$this->conf['product']." may not work properly!");
		}
		return false;
	}

	// KILL_PROCESS
	function kill_process ($name, $signal="KILL")
	{
		$status = 1;

		# Try to lookup PID's for the process and use simple kill utility
		$kill = $this->search_cmd_in_path ("kill");
		$killall = $this->search_cmd_in_path ("killall");

		$pids = $this->pidof($name);
		if($kill && count($pids) > 0) {
			$status = $this->run_command ("{$kill} -{$signal} ".join(" ", $pids)." 1>/dev/null");
			$pids = $this->pidof($name);
			if(count($pids) > 0) $status = 1;
			else $status = 0;
		}
		else if(PHP_OS != "SunOS" && PHP_OS != "AIX" && $killall) {
			$status = $this->run_command ("{$killall} -{$signal} {$name} 1>/dev/null");
		}

		if($status == 0) $this->install_log ("Killed process: $name with signal: $signal");
		else $this->install_log ("Failed to kill process: $name");

		return $status;
	}

	// WRITE_VHOSTS_FILE
	function write_vhosts_file($vhosts_file)
	{
		if($this->my_file_exists($vhosts_file)) {
			$vhosts_arr = @file($vhosts_file);
		}
		else $vhosts_arr = array();

		//create vhosts.ini for configuration vr_hosts :
		//host:ip;document_root

		foreach ($vhosts_arr as $num => $line) {
			if(strncmp($line, "http://", 7)==0 || strncmp($line,"https://", 8)==0) {
				continue;
			}

			$proto = "http";
			if(preg_match("/:(\d+)/", $line, $match)) {
				if($match[1] == "443"){
					$proto = "https";
				}
			}
			$vhosts_arr[$num] = "$proto://$line";
		}

		if(isset($this->conf['apache_vhosts'])) {
			foreach ($this->conf['apache_vhosts'] as $vhost_key => $vhost_docroot_array) {

				$vhost_ip=$vhost_docroot_array['ip'];
				$vhost_port=$vhost_docroot_array['port'];
				$vhost_servername=$vhost_docroot_array['server'];
				$vhost_docroot = $vhost_docroot_array['docroot'];
				$is_ssl = $vhost_docroot_array['ssl'];

				if($vhost_port == "443" || $is_ssl) {
					$proto = "https";
				}
				else $proto = "http";

				if($vhost_port == "80") {
					$vhost_port = "";
				}
				else $vhost_port = ":$vhost_port";

				if (preg_match("/\d+\.\d+\.\d+\.\d+/",$vhost_ip)) {
					$vhost_servername = $vhost_ip;
				}
				array_push($vhosts_arr, "$proto://$vhost_servername$vhost_port;$vhost_docroot\n");
			}
		}

		$this->my_fwrite($vhosts_file, "w", implode("", array_unique($vhosts_arr)));
	}

	// CURL_GET
	function curl_get($url, $timeout=300, $silent=true)
	{
		if(!extension_loaded("curl")){
			$this->on_error("Your PHP must be loaded with cURL extension.");
		}

		$url_arr = $this->my_parse_url($url);

		$this->install_log ("Trying to access page: $url (timeout: $timeout)");
		
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
		curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
		$proxy = @getenv("HTTP_PROXY").@getenv("http_proxy").@getenv("FTP_PROXY").@getenv("ftp_proxy");
		if(isset($proxy)) {
			curl_setopt($ch, CURLOPT_PROXY, "");
		}
		$buf = curl_exec($ch);

		$curl_errno = curl_errno($ch);
		if((!@is_numeric($buf) && !$buf) || $curl_errno != CURLE_OK) {
			switch($curl_errno) {
				case CURLE_COULDNT_CONNECT:
					$curl_error = "Connection failure";
					break;
				case CURLE_OPERATION_TIMEOUTED:
					$curl_error = "Connection timeout";
					break;
				case CURLE_COULDNT_RESOLVE_HOST:
					$curl_error = "Cannot resolve host";
					break;
				default:
					$curl_error = curl_error($ch);
			}
			$curl_error = "Couldn't access page: $url\nError was: $curl_error";

			$this->install_log ("ERROR: $curl_error");
			if(!$silent) {
				$this->msgbox($curl_error);
			}
			$buf = null;
		}

		curl_close($ch);
		return $buf;
	}

	// EXEC_STR_THROUGH_WEBSERVER
	function exec_str_through_webserver($str, $ext = "php", $docroot=null, $url=null)
	{
		if (!$docroot) {
			$docroot = $this->webserver_docroot_guess();
		}
		if (!$url) {
			$url = $this->webserver_get_virtual_host_url("127.0.0.1", true);
		}

		$test_file = "zend_install_".rand().".".$ext;
		$test_file_full = $this->make_path($docroot, $test_file);

		$this->str2file($str, $test_file_full);

		$this->install_log("Executing: {$str} via URL: {$url}, document root: {$docroot}");

		$cont = $this->curl_get ("{$url}/$test_file");
		$this->my_unlink($test_file_full);

		$this->install_log("Webserver returned: {$cont}");

		return $cont;
	}
	
	// CHECK_URL_CORRESPONDS_TO_DOCROOT
	function check_url_corresponds_to_docroot ($url, $docroot)
	{
		$test_file = "zend_install_".rand().".html";
		$test_file_full = $this->make_path($docroot, $test_file);

		$this->str2file("CORRESPONDS", $test_file_full);

		$cont = $this->curl_get ("{$url}/{$test_file}");
		$this->my_unlink($test_file_full);

		return strstr ($cont, "CORRESPONDS");
	}

	// CHECK_PHP_IS_LOADED
	function check_php_is_loaded($docroot=null, $url=null)
	{
		$test_str = "<? print('PHP_IS_LOADED'); ?>";
		return strstr($this->exec_str_through_webserver($test_str, "php", $docroot, $url), "PHP_IS_LOADED");
	}

	// CHECK_WEBSERVER_IS_LOADED
	function check_webserver_is_loaded($docroot=null, $url=null)
	{
		$test_str = "WEBSERVER_IS_LOADED";
		return strstr($this->exec_str_through_webserver($test_str, "html", $docroot, $url),
			"WEBSERVER_IS_LOADED");
	}

	// CHECK_LOADED_EXTENSIONS
	function check_loaded_extensions($docroot=null, $url=null)
	{
		$test_str = "<? print(join('\n', get_loaded_extensions())); ?>";
		$cont = $this->exec_str_through_webserver($test_str, "php", $docroot, $url);
		return split("\n", strtolower($cont));
	}

	// POSTINSTALL_TEST
	function postinstall_test($test_extensions, $add_info=array(), $docroot=null, $url=null)
	{
		$this->progress_bar_start(100,
			"Running post installation tests\n(this may take a few seconds)...", "Testing ...");

		$res = true;

		if(!$this->check_webserver_is_loaded($docroot, $url)) {
			$this->progress_bar_stop();
			$this->msgbox(
				"Setup was unable to communicate with the Web server. It is possible that\n".
			    	"the Web server failed to start. Try to start the Web server manually."
			);

			$res = false;
		}
		else {
			if(!$this->check_php_is_loaded($docroot, $url)) {
				$this->progress_bar_stop();
				$this->msgbox("Setup was unable to detect working PHP on your Web server.");

				$res = false;
			}
			else {
				if(@count($test_extensions) + @count($add_info) > 0) {
					$extensions = $this->check_loaded_extensions($docroot, $url);
					
					$post_summary = $this->conf['product']." Post Installation Summary:\n\n";
					foreach ($test_extensions as $ext => $ext_name) {
						$success = in_array(strtolower($ext), $extensions);
						$post_summary .= "{$ext_name} (".($success ? "Success" : "Failure").")\n";
						$res = $res && $success;
					}

					foreach ($add_info as $ext_name => $success) {
						$post_summary .= "$ext_name (".($success ? "Success" : "Failure").")\n";
						$res = $res && $success;
					}
				}
				$this->progress_bar_stop();
			}
		}

		if(isset($post_summary)) {
			$this->msgbox($post_summary);
		}

		return $res;
	}

	function substitute_vars_in_script($script, $vars)
	{
		$this->install_log ("Updating $script ...");
		$text = $this->file2str($script);
		foreach ($vars as $name=>$var) {
			$text = str_replace ("%%$name%%", $var, $text);
		}
		$this->str2file ($text, $script);
	}

	function get_php_cmd ($php_path=null, $php_ini_path=null, $php_script=null, $php_args=array())
	{
		$php_cmd = ($php_path ? $php_path : $this->make_path($this->conf['prefix'], 'sbin', 'php'));

		// Unset dangerous PHP.ini entries (if using unknown PHP.ini):
		if ($php_ini_path === null) {
			$php_cmd .= " -d safe_mode=Off -d open_basedir='' -d auto_prepend_file='' -d auto_append_file=''";
		}
		$php_cmd .= ' -c '.($php_ini_path ? $php_ini_path : $this->conf['php_ini_path']);

		if($php_script) {
			$php_cmd .= ' '.$php_script;

			foreach ($php_args as $arg) {
				$php_cmd .= " '".addcslashes($arg, "'")."'";
			}
		}

		return $php_cmd;
	}

	// parms: $filename - filename of shell script to be created
	//        $script   - filename of php script to be wrapped
	//        $php_path - filename of php executable
	//        $php_ini_path - filename of php.ini
	//        $args_arr (optional) - array of additional arguments
	// returns: nothing
	function create_php_script_wrapper($filename, $script, $php_path, $php_ini_path, $args_arr=array())
	{
		$this->install_log ("Installing shell wrapper for $script ...");

		$php_cmd = $this->get_php_cmd ($php_path, $php_ini_path, $script, $args_arr);

		$this->str2file ("#!/bin/sh\n$php_cmd \"\$@\"\n", $filename);
		$this->set_permissions ($filename, "0755");
	}

	function openssl_test()
	{
		$openssl = $this->search_cmd_in_path("openssl");
		if(!@file_exists($openssl)) {
			$this->on_error ("Cannot find utility: openssl");
		}

		if(preg_match("/PRNG is not seeded/", $this->cmd2str("$openssl rand 32 -out /dev/null"))) {
			$this->on_error ("The installation has detected, that \"randomness device\"\n".
				"is not working properly on your system. For more information, refer to:\n".
				"http://www.openssl.org/support/faq.html#USER1");
		}
	}

	function get_binary_strings($binary)
	{
		if (!file_exists ($binary)) {
			return false;
		}
		$strings = $this->search_cmd_in_path ("strings");
		if($strings) {
			$all_strings = $this->cmd2str ("$strings $binary");
		}	else {
			$all_strings = @file_get_contents ($binary);
		}
		return $all_strings;
	}

	function grep_binary_file($pattern, $binary, &$match)
	{
		return preg_match ($pattern, $this->get_binary_strings($binary), $match);
	}

	function zend_module_get_version($filename)
	{
		$zem_path = $this->php_ini_get_entry ("zend_extension", $filename);
		if($this->my_file_exists ($zem_path)) {
			if($this->grep_binary_file ("/VERSION=(\w+)/", $zem_path, $match)) {
				return $match[1];
			}
		}
		return false;
	}


	function ask_keep_existing_component ($comp, $comp_ver, $package_ver)
	{
		return $this->yesnobox ("$comp already installed, and it's version ($comp_ver) is greater,\n"
			."than the one in the package ($package_ver).\n"
			."Do you wish to keep the existing component (recommended) ?");
	}

	function set_known_versions ($known_versions)
	{
		$this->conf['known_versions'] = $known_versions;
	}

	function my_version_compare ($ver1, $ver2)
	{
		$ver1 = preg_replace ("/\s/", "", $ver1);
		$ver2 = preg_replace ("/\s/", "", $ver2);

		if (@isset($this->conf['known_versions'])) {
			foreach ($this->conf['known_versions'] as $main_ver => $arr) {
				if (preg_match ("/".preg_quote($main_ver, '/')."/", $ver1)) {
					foreach ($arr as $ver) {
						if (preg_match ("/".preg_quote($ver, '/')."/", $ver2)) {
							$ver1_is_greater = 1;
							break;
						}
					}
				}
				if (preg_match ("/".preg_quote($main_ver, '/')."/", $ver2)) {
					foreach ($arr as $ver) {
						if (preg_match ("/".preg_quote($ver, '/')."/", $ver1)) {
							$ver1_is_greater = -1;
							break;
						}
					}
				}
			}
		}
		if (!isset($ver1_is_greater)) {
			$ver1_is_greater = version_compare($ver1, $ver2);
		}
		return $ver1_is_greater;
	}

	function check_installed_components ($comps)
	{
		$keep_existing = array();
		foreach ($comps as $comp) {
			unset ($comp_ver);
			if(isset($comp["zemname"])) {
				$comp_ver = $this->php_ini_get_product_version ($comp["zemname"]);
			}	else {
				$comp_ver = $this->zend_module_get_version ($comp["filename"]);
			}
			if($comp_ver !== false) {
				$package_ver = $this->get_component_version($comp["compname"]);
				if ($this->my_version_compare ($comp_ver, $package_ver) > 0) {
					if($this->ask_keep_existing_component ($comp["nicename"], $comp_ver, $package_ver)) {
						$keep_existing[$comp["compname"]] = $comp_ver;
					}
				}
			}
		}
		return $keep_existing;
	}

	function file_encoded($php_file)
	{
		$fp = @fopen($php_file, "r");
		if(!$fp) {
			$this->on_error ("Can't open file for reading: $php_file!");
		}
		$str = fgets ($fp);
		fclose ($fp);
		
		return strstr($str, "Zend");
	}

	function is_evaluation_license($license)
	{
		$license_cont = $this->file2str ($license);
		if (preg_match ('@Expires\s*=\s*Never@', $license_cont)) {
			return false;
		}
		return true;
	}

	function my_parse_url($url)
	{
		if (preg_match('@^[^:]+://.*$@', $url)) {
			return @parse_url($url);
		}
		$url = 'http://'.$url;
		$url_arr = @parse_url($url);
		unset ($url_arr['scheme']);
		return $url_arr;
	}

	// ------------------
	// MYSQL FUNCTIONS
	// ==================

	// MYSQL_INSTALL_CONFIGURE
	function mysql_install_configure ($basedir, $user="mysql")
	{
		if(!$this->add_user ($user, $basedir)) {
			return false;
		}

		# Enter to the MySQL basedir (installation prefix)
		$this->pushd ($basedir);

		$datadir = "{$basedir}/var";
		if (!$this->my_dir_exists ($datadir)) {
			$this->my_mkdir ($datadir);
			$this->set_permissions ($datadir, "0755", $user);
		}

		$mysqld_install_cmd = "{$basedir}/bin/mysqld --defaults-file={$basedir}/etc/my.cnf --language={$basedir}/share/mysql/english --bootstrap --skip-grant-tables --basedir={$basedir} --datadir={$datadir} --skip-innodb --skip-bdb --skip-ndbcluster --max_allowed_packet=8M --net_buffer_length=16K --user={$user}";

		if($this->run_command ("$mysqld_install_cmd < {$basedir}/share/mysql/init_db.sql") != 0) {
			$this->on_error ("Cannot initialize MySQL database!");
		}

		$this->popd();

		$this->link_startup_daemon ("{$basedir}/bin/mysql.sh", "MySQL", "MySQL Server");
	}

	// MYSQL_UPDATE_STARTUP_SCRIPT
	function mysql_update_startup_script ($basedir, $user="mysql")
	{
		$datadir = "{$basedir}/var";

		# Update MySQL startup script:
		$mysqlrc_cont = $this->file2str ("{$basedir}/bin/mysql.sh");
		$mysqlrc_cont = preg_replace ('/^s*BASEDIR\s*=.*$/m', "BASEDIR={$basedir}", $mysqlrc_cont);
		$mysqlrc_cont = preg_replace ('/^s*USER\s*=.*$/m', "USER={$user}", $mysqlrc_cont);
		$mysqlrc_cont = preg_replace ('/^s*TEMP\s*=.*$/m', "TEMP={$datadir}", $mysqlrc_cont);
		$this->str2file ($mysqlrc_cont, "{$basedir}/bin/mysql.sh");
	}

	// MYSQL_START
	function mysql_start ($basedir)
	{
		if ($this->run_command ("{$basedir}/bin/mysql.sh start 1>/dev/null") !== 0) {
			$this->warn ("Failed to start the MySQL server!");
		}
		sleep (3);
	}

	// MYSQL_STOP
	function mysql_stop ($basedir)
	{
		$this->run_command ("{$basedir}/bin/mysql.sh stop 1>/dev/null");
		sleep (3);
	}

	// MYSQL_DELETE_OLD_LOGFILES
	function mysql_delete_old_logfiles ($basedir)
	{
		$this->run_command ("rm -f {$basedir}/var/ib_logfile*");
	}

	// MYSQL_DELETE_DATABASE
	function mysql_delete_database ($basedir, $database, $check_exit_status=false)
	{
		if ($this->run_command ("echo 'DROP DATABASE {$database};' | {$basedir}/bin/mysql --defaults-file={$basedir}/etc/my.cnf --socket={$basedir}/var/mysql.sock") != 0) {

			if ($check_exit_status) {
				$this->on_error ("Cannot delete MySQL database: {$database}!");
			}
		}
	}

	// MYSQL_CREATE_DATABASE
	function mysql_create_database ($basedir, $database)
	{
		if ($this->run_command ("echo 'CREATE DATABASE {$database};' | {$basedir}/bin/mysql --defaults-file={$basedir}/etc/my.cnf --socket={$basedir}/var/mysql.sock") != 0) {
			$this->on_error ("Cannot initialize MySQL database!");
		}
	}

	// MYSQL_RUN_SQL_SCRIPT
	function mysql_run_sql_script ($basedir, $sql_script, $database="mysql")
	{
		if ($this->run_command ("{$basedir}/bin/mysql --defaults-file={$basedir}/etc/my.cnf --socket={$basedir}/var/mysql.sock {$database} < {$sql_script}") != 0) {
			$this->on_error ("Cannot initialize MySQL database!");
		}
	}

	// MYSQL_RUN_SQL_QUERY
	function mysql_run_sql_query ($basedir, $sql_query, $database="mysql")
	{
		$sql_query .= ";";
		$sql_query = escapeshellarg ($sql_query);
		if ($this->run_command ("echo {$sql_query} | {$basedir}/bin/mysql --defaults-file={$basedir}/etc/my.cnf --socket={$basedir}/var/mysql.sock {$database}") != 0) {
			$this->on_error ("Cannot run MySQL query: {$sql_query}!");
		}
	}

	/** FIND_JAVA
	* @param string component, that requires Java
	* @param string the least version of Java
	* @param boolean does component require Sun JRE? (default: false)
	* @param boolean whether we can skip this question (default: true)
	*/
	function find_java ($component, $least_version=null, $sun_java=false, $accept_empty=true)
	{
		$java_path = $this->search_cmd_in_path("java");

		do {
			$msg = "Please ".($java_path ? "submit" : "enter")." the full path to the Java binary, ".
				"that will be used\nfor running the {$component}. You can leave the field empty, or\n".
				"click CANCEL in order to disable the {$component}.";

			if ($sun_java || $least_version !== null) {
				$requires = "requires";
				if ($sun_java) {
					$requires .= " Sun's";
				}
				$requires .= " JRE";
				if ($least_version !== null) {
					$requires .= " {$least_version} or later";
				}
				$msg .= "\n\nPlease note that the {$component} {$requires}!\n";
			}
			$java_path = $this->select_file_nostrip ($msg, $java_path, $accept_empty);

			if ($java_path === null) {
				if ($this->yesnobox ("Do you wish to disable {$component}?")) {
					return null;
				}
				continue;
			}

			if ($this->is_broken_link ($java_path) || !@is_executable($java_path)) {
				$this->msgbox ("Java binary: {$java_path} is not executable or broken link!\n\nPlease try again!");
				$java_path = null;
				continue;
			}

			$verstr = $this->cmd2str ("{$java_path} -version 2>&1");

			// Check whether this Java meets our needs:
			$compatible = true;
			if (!preg_match ('/java\s+version/', $verstr)) {
				// Empty result:
				$compatible = false;
			} else {
				if ($sun_java && preg_match("/gij/", $verstr)) {
					$compatible = false;
				}
				if ($least_version != null && preg_match ("/java\s+version\s+\"([^\"]+)\"/", $verstr, $match)) {
					if (version_compare ($least_version, $match[1]) > 0) {
						$compatible = false;
					}
				}
			}
			if ($compatible || $this->yesnobox (
				"WARNING: An incompatible version of the Java Runtime Environment (JRE) has been detected.\n".
				"The {$component} {$requires}! Do you still wish to use this JRE?", true)) {
					
				return $java_path;
			}
			$java_path = "";
		}
		while (true);
	}

	//=-=-=-=-=-=-=-=-=-=
	// SELinux functions
	//=-=-=-=-=-=-=-=-=-=

	function selinux_enabled()
	{
		$selinuxenabled = $this->search_cmd_in_path ("selinuxenabled", true);
		if ($selinuxenabled) {
			return ($this->run_command ($selinuxenabled) == 0);
		}
	}

	function selinux_disable_httpd_protection()
	{
		$setsebool = $this->search_cmd_in_path ("setsebool", true);
		if ($setsebool) {
			/* You need to restart the Web server after running this command: */
			return ($this->run_command ("$setsebool -P httpd_disable_trans 1") == 0);
		}
		return false;
	}

	function selinux_enabled_for_apache()
	{
		if (!$this->selinux_enabled()) {
			return false;
		}

		$getsebool = $this->search_cmd_in_path ("getsebool", true);
		if ($getsebool) {
			return (preg_match ('/(0|inactive)/',
				$this->cmd2str("$getsebool httpd_disable_trans 2>/dev/null")));
		}
		$getfilecon = $this->search_cmd_in_path ("getfilecon", true);
		if ($getfilecon) {
			$httpd_exec = $this->apache_exec_guess();
			$out = $this->cmd2str ("$getfilecon $httpd_exec 2>/dev/null");
		      	if (preg_match('/^\s*[^ \t]*\s+.*http.*/sx', $out)) {
				true;
			}
		}
		return false;
	}

	function detect_processors_num()
	{
		if (@file_exists ('/proc/cpuinfo')) {
			if (preg_match_all ('/^processor\s*:\s*\d/mi', $this->file2str("/proc/cpuinfo"), $match)) {
				return count($match[0]);
			}
		}
		$psrinfo = $this->search_cmd_in_path ("psrinfo", true);
		if ($psrinfo) {
			if (preg_match_all ('/^\d+\s$/m', $this->cmd2str ($psrinfo), $match)) {
				return count($match[0]);
			}
		}
		$sysctl = $this->search_cmd_in_path ("sysctl", true);
		if ($sysctl) {
			if (preg_match ('/^(\d+)$/', trim($this->cmd2str ("{$sysctl} -n hw.ncpu")), $match)) {
				if ($match[1] > 0) {
					return $match[1];
				}
			}
		}
		return 1; // We surely have at least one processor
	}

	/* Convert IP range to list of IP's and write it to file */
	function expand_ip_range ($ip_range, $file)
	{
		list ($ip, $mask) = explode("/", $ip_range);
		if (!$this->is_ip_valid ($ip) || !is_numeric($mask) || $mask > 31 || $mask < 0) {
			return false;
		}
		$ip_start = ip2long($ip);
		$ip_end = $ip_start + pow (2, 32 - $mask) - 1;

		$fp = fopen ($file, "w");
		if (!is_resource ($fp)) {
			$this->on_error ("Can't write to file: {$file}!");
		}
		while ($ip_start < $ip_end) {
			fwrite ($fp, long2ip ($ip_start++));
			fwrite ($fp, "\n");
		}
		fclose ($fp);
		return true;
	}

	// PARSE_HOST
	function parse_host ($host)
	{
		$arr = split ("\.",$host);

		for ($i=count($arr); $i<4; $i++) {
			$arr[$i] = "*";
		}
		
		if (count($arr) != 4)
			return false;

		$mask=32;
		for ($i=3; $i>=0; $i--) {
			if ($arr[$i]!="*") {
				break;
			}
			$mask-=8;
			$arr[$i]=0;
		}
		for (/*$i is already initialized*/; $i>=0; $i--) {
			$tmp=(integer)($arr[$i]);
			if ($arr[$i]!="$tmp" or $tmp>255 or $tmp<0) {
				// parse error;
				return false;
			}
			$arr[$i]=$tmp;
		}
		return join(".",$arr)."/".$mask;
	}

	// BINARY_GET_PRECISION
	function binary_get_precision ($path)
	{
		$file_cmd = $this->search_cmd_in_path ("file");
		if ($file_cmd) {
			$precision = $this->cmd2str("{$file_cmd} {$path} 2>/dev/null");
			if(preg_match("/(\d+)\-bit/i", $precision, $match)) {
				return $match[1];
			}
		}
		return false;
	}

	
	// install (new) license utility
	// return: path to install utility
	function generate_trial_utility ($destination=null)
	{
	  if (! isset ($this->conf['generated_trial']))  // do that only once
	    {
	      // $destination = $this->conf['prefix'] . '/sbin/trial';
		if (!$destination) {
			$destination = $this->conf['tmp_dir']."/trial";
		}
	      $failure = 0;
	      // the 3rd parm SHOULD be false in final release (no logging), this is for debugging!
	      if ($this->run_command ("../data/trial $destination", '', true) == 0)
		{
		  if ($this->my_file_exists ($destination))
		    {
		      $this->conf['generated_trial'] = $destination;
		    }
		  else
		    {
		      $failure = 1;
		    }
		}
	      else
		{
		  $failure = 1;
		}
	      if ($failure == 1)
		{
		  $this->error_handler->on_error ('Unable to install trial license utility.');
		}
	    }
	  return ($this->conf['generated_trial']);
        }
	


	
	// the following two are for backward compatability, deprecated
	function install_log ($msg)
	{
	  $this->logger->log ($msg);
	}
	function on_error ($msg)
	{
	  $this->error_handler->on_error ($msg);
	}
	  
} // end of class Install

/*
function error_handler($errno, $errstr, $errfile, $errline)
{
	global $INSTALL;
	$INSTALL->error_handler($errno, $errstr, $errfile, $errline);
}
*/



class InstallLogger
{
  var $logfile_name;

  function InstallLogger ($logfile_name)  // ctor
  {
    $this->logfile_name = $logfile_name;
    @unlink ($this->logfile_name);
  }

  function log ($msg)
  {
    $msg = preg_replace("/[\r\n]+/", " ", $msg);
    $msg = preg_replace("/\s+/", " ", $msg);
    error_log (sprintf("%s %s\n", date('[M d H:i:s]'), $msg), 3, $this->logfile_name);
  }

  /*
  function store ($path)
  {
    $this->my_copy ($this->logfile_name, $path);
  }
  */
} // end of class InstallLogger



class InstallErrorHandler
{
  var $install;

  function InstallErrorHandler (&$install)  // ctor
  {
    $this->install = &$install;  // by this we create a bloody circular reference 
                                 // but hopefuly nobody would notice
  }

  function on_error($msg = "") /* die with error message */
  {
    static $in_error;
    
    $this->install->logger->log ("ERROR: $msg");
    
    $msg = "\n{$msg}\n";
    if (
	$this->install->conf['interactive']                                    && 
	@file_exists ($this->install->conf['prefix'] . '/bin/support_tool.sh') && 
	@file_exists ($this->install->conf['prefix'] . '/lib/tools/conf.db')   && 
	(! isset ($in_error))
	) 
      {
      
      $in_error = true;
      
      if ($this->install->yesnobox (
			   <<<EOF
{$this->install->conf['product']} installation was NOT completed successfully.
{$msg}
Do you wish to run the Support Tool for gathering your system configuration
information, that will help to investigate this problem?
EOF
			   )) 
	{
	  $this->install->run_command ($this->install->conf['prefix'] . '/bin/support_tool.sh'
				       . (empty ($this->install->conf['dialog']) ? ' --text-mode' : ''));
	}
      } 
    else 
      {
	$this->install->msgbox (
		    <<<EOF
{$this->install->conf['product']} installation was NOT completed successfully.
{$msg}
For further assistance, please contact Zend Support at
https://www.zend.com/support/
EOF
		    );
    }
    
    $this->install->cleanup();
    die(-1);
  } // end of on_error()
  
} // end of class InstallErrorHandler


?>
