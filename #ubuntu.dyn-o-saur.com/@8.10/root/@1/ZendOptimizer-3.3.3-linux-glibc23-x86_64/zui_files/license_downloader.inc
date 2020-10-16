<?php
/*
   +----------------------------------------------------------------------+
   | Zend license downloader class                                        |
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


class LicenseDownloader extends BasicInstall
{
  var $host_id;   // (zend) host id of the current host

  // ctor
  // parms: $logger - class doing logs, supposed to have log() method
  //        $error_handler - class doing error handling, supposed to have on_error() method
  function LicenseDownloader ($logger=null, $error_handler=null)
  {
    $this->conf['license_host'] = getenv("ZEND_LICENSE_HOST");
    if (empty ($this->conf['license_host'])) {
      $this->conf['license_host'] = "www.zend.com";
    }
    $this->conf['license_script'] = "licenses/request.php";
    // TODO: get rid of file, keep it internally
    $this->conf['lic_response_file'] = '/tmp/response.txt';

    parent::BasicInstall(); // this should set default logger and error handler
    if (is_object ($logger))
      {
	$this->logger = $logger;
      }
    if (is_object ($error_handler))
      {
	$this->error_handler = $error_handler;
      }

    if (! extension_loaded ('curl'))
      {
	$this->error_handler->on_error ('curl extension is not loaded');
      }
  }



  function gen_license_url($args)
  {
    $urlstr = "";
    foreach($args as $key => $value){
      $urlstr .= "$key=".urlencode($value)."&";
    }
    $urlstr = rtrim($urlstr, "&");
    
    return "https://".$this->conf['license_host']."/".$this->conf['license_script']."?$urlstr";
  }



  // returns: 2-elements array (error code, string containing contants of license file)
  //          error code - 0 on success, negative eror code on error
  function gen_license ($args)
  {
    error_reporting(0);
    set_error_handler ('_error_handler');
    $err = 0;

    $this->my_unlink ($this->conf['lic_response_file']);
    $url = $this->gen_license_url($args);

    if (method_exists ($this->logger, 'log'))
      {
	$this->logger->log ("Generated URL: $url");
      }

    if(!extension_loaded("curl")){
      $this->error_handler->on_error ('Your PHP must be loaded with cURL extension.');
    }
    /* start cURL session (Secure HTTP) */
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_TIMEOUT, 300);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    
    $buf = curl_exec($ch);
    $curl_errno = curl_errno($ch);
    
    if(!$buf || $curl_errno != CURLE_OK){
      
      $curl_error = "The installation failed to connect to the Zend License Server.\n";
      switch($curl_errno) {
      case CURLE_COULDNT_CONNECT:
      case CURLE_OPERATION_TIMEOUTED:
	$curl_error .= "This may be due to a local or remote connection failure.\n";
	break;
      case CURLE_COULDNT_RESOLVE_HOST:
	$curl_error .= "The host ". $this->conf['license_host'] ." couldn't be resolved.\n";
	break;
      default:
	$curl_error = curl_error($ch)."\n";
      }
      $curl_error .= "Please check that you are connected to the Internet or try again\nin a few minutes.";
      
      if(!$this->my_fwrite($this->conf['lic_response_file'], "a", "$curl_error\n")){
	return (array (-2, null));
      }
      return (array (-1, null));
    }
    curl_close($ch);
    /* end of cURL session */
    
    if(empty($buf)){
      if(!$this->my_fwrite($this->conf['lic_response_file'], "a", "Server returned empty buffer\n")){
	return -2;
      }
      return (array (-1, null));
    }
    
    $lines = preg_split("/\n/", $buf);
    
    /* create response file */
    if(!$this->my_fwrite($this->conf['lic_response_file'], "a", "$lines[0]\n$lines[1]\n")){
      return (array (-2, null));
    }
    
    /* create license file */
    $buf = "";
    for($i=2; !empty($lines[$i]); $i++){
      $buf .= "$lines[$i]\n";
    }
    
    /*
    if(!$this->my_fwrite($file, "w", $buf)){
      return -2;
    }
    */

    restore_error_handler();
    error_reporting(E_ALL);
    return (array (0, $buf));
  } // end of gen_license()



  function _error_handler($errno, $errstr, $errfile, $errline)
  {
    $this->my_fwrite($this->conf['lic_response_file'], "a", "LOCAL ERROR: $errstr in $errfile line: $errline\n");
    restore_error_handler();
  }

  

  function get_response(&$code, &$status)
  {
    if(!($fp = fopen($this->conf['lic_response_file'], "r"))){
      $this->error_handler->on_error 
	('Couldn\'t open ' . $this->conf['lic_response_file'] . 'for reading');
    }
    
    $code = preg_replace("/(^.*)[\r\n]/", "\\1", fgets($fp, 1024));
    $status = preg_replace("/(^.*)[\r\n]/", "\n\\1\n", fgets($fp, 1024));
    $status = str_replace("^", "\n", $status);
    
    if(preg_match("#\s*<HTML>#i", $status)){
      $status = "";
    }
    
    fclose($fp);
  }



  // parms: $user_id - zend.com's user name
  //        $md5_passwd - md5 hash of zend.com's password
  //        $product_id - zend's product id (numeric)
  //        $version    - prpoduct version as understood by license download mechanism
  //        $serial     - serial number (optional)
  // returns: 3-elements array: (return code, string containig license, error message)
  //          return code: (nonnegative?) download status on succes, -1 on failure
  function download ($user_id, $md5_passwd, $product_id, $version, $serial=null)
  {
    $args['uid']     = $user_id;
    $args['pass']    = $md5_passwd;
    $args['pid']     = $product_id;
    $args['version'] = $version;
    $args['serial']  = $serial;
    $args['host']    = $this->get_host_id();
    $args["ZLM"]     = '1';

    list ($r, $license) = $this->gen_license ($args);
    
    if($r != 0){
      switch($r){
      case -1:
	$msg = $this->file2str($this->conf['lic_response_file']);
	break;
	
      case -2:
	$this->error_handler->on_error ('Could not write response file.');
	break;
	
      default:
	$this->error_handler->on_error ("Impossibe return value from gen_license(): $r");
      }
      return (array (-1, null, $msg));
    }
    
    $this->get_response(&$code, &$status);

    if($code != "OK") {
      //$this->my_unlink($license_file);
      return (array (-1, null, $status));
    }
    
    return (array (intval (trim ($status)), $license, null));
  } // end of download()

} // end of class LicenseDownloader

?>
