<?php
/*
   +----------------------------------------------------------------------+
   | Zend basic installation class                                        |
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


class BasicInstall
{
  var $logger;
  var $error_handler;
  var $conf;

  function BasicInstall () // ctor
  {
    $this->logger        = new BasicInstallLogger();
    $this->error_handler = new BasicInstallErrorHandler();
    $this->conf['slash'] = "/";
  }


  function my_unlink($file, $log=true)
  {
    if ($log)
      {
	$this->logger->log ("Unlinking: $file");
      }
    if($this->my_file_exists($file)){
      $this->run_silently("unlink", $file);
    }
  }



  function my_file_exists($file)
  {
    if(empty($file)){
      return false;
    }
    return  ($this->run_silently("file_exists", $file) && $this->run_silently("is_file", $file)) ||
      ($this->run_silently("is_link", $file) && $this->run_silently("readlink", $file));
  }


  
  function my_rename($from, $to, $log=true)
  {
    if($log) {
      $this->logger->log ("Renaming: $from to: $to");
    }
    if($this->my_file_exists($this->strip_symlink($from))){
      $this->run_silently("rename", $from, $to);
    }
  }



  function strip_symlink($file)
  {
    $file_dir = $this->my_dirname($file);
    
    $new_file=$file;
    $count = 0;
    while($this->my_link_exists($new_file)){
      if ($count++ >= 64){
	$this->on_error ("Too many levels of symbolic links for $file");
      }
      $newer_file = readlink($new_file);
      if(!$newer_file){
	$this->on_error ("Unable to read symbolic link $new_file");
      }
      $new_file = $this->make_full_path($newer_file, $file_dir);
      $file_dir = $this->my_dirname ($new_file);
    }
    
    if(!is_file($new_file) && !is_dir($new_file)){
      if($this->my_link_exists($file)) {
	$this->logger->log ("Broken link: $file");
      }
      return "";
    }
    return $new_file;
  }


  
  function my_link_exists($file)
  {
    return $this->my_file_exists($file) && is_link($file);
  }
  


  function my_dirname($file)
  {
    $dirname = dirname($file);
    if($dirname == "."){
      $dirname = "";
    }
    
    return $dirname;
  }



  function run_silently($function) /* run function without error outputs */
  {
    $args = func_get_args();
    array_shift($args);
    error_reporting (E_ALL & ~E_WARNING & ~E_NOTICE);
    
    $func_cmd = '$r = '.$function.' (\'' . join ('\', \'', $args) . '\');';
    
    eval($func_cmd);
    
    error_reporting (E_ALL);
    return isset($r) ? $r : null;
  }


  function my_fwrite($file, $mode, $str)
  {
    $num_written = -1;
    $fp = $this->my_fopen($file, $mode);
    if(!empty($str)){
      $num_written = fwrite($fp, $str);
    }
    if(!fclose($fp) && $num_written<=0){
      $this->error_handler->on_error ("Cannot write to file: $file");
    }
    return $num_written;
  }


  function my_fopen($file, $mode)
  {
    // define string for mode
    if($mode[0] == "r")      { $modestr = "reading";       }
    else if($mode[0] == "w") { $modestr = "writing";       }
    else if($mode[0] == "a") { $modestr = "appending";     }
    else                     { $modestr = "unknown mode";  }
    
    if(strlen($mode)>1 && $mode[1]=="b"){
      $modestr .= " (Binary)";
    }
    
    $this->logger->log ("Open file: $file for $modestr");
    
    $fp = $this->run_silently("fopen", $file, $mode);
    if(empty($fp)){
      $this->error_handler->on_error ("Cannot open file: \"$file\" for $modestr");
    }
    return $fp;
  }




  function my_popen($cmd, $mode, $need_loging=true)
  {
    if($need_loging){
      $this->logger->log ("Executing: $cmd");
    }
    
    $fp = popen($cmd, $mode);
    if($fp == false){
      $this->error_handler->on_error ("Cannot execute command: $cmd");
    }
    return $fp;
  }


  function safe_pclose($fp)
  {
    $status = pclose($fp);
    if(version_compare(PHP_VERSION, "4.3.0", "<") == 1){
      $status = ($status>>8)&0xFF;
    }
    return $status;
  }



  function file2str($file)
  {
    if($this->my_dir_exists($file)){
      $this->error_handler->on_error ('Can\'t read "${file}": It should not be a directory.');
    }
    return @file_get_contents($file);
  }



  function cmd2str($cmd, $need_logging=true)
  {
    $fp = $this->my_popen($cmd, "r", $need_logging);
    
    $str = "";
    while(!feof($fp)){
      $str .= fread($fp, 4096);
    }
    $str = preg_replace("/^\s*|\s*$/", "", $str);
    
    $status = $this->safe_pclose($fp);
    $this->logger->log  ("Command has returned exit status: $status");
    
    return ($str);
  }



  function my_dir_exists($dir)
  {
    if(empty($dir)){
      return false;
    }
    return $this->run_silently("file_exists", $dir) &&
      $this->run_silently("is_dir", $dir);


  }

  
  function search_cmd_in_path($cmd, $system_dirs=false)
  {
    $cmd_path = null;
    $search_path = getenv ('PATH');
    /*
    if (! empty ($this->conf['installer_path']))
      {
	$search_path = $this->conf['installer_path'] . ":$search_path";
      }
    */
    
    if ($system_dirs) {
      $search_path .= ":/sbin:/usr/sbin";
    }
    $path_arr = explode(":", $search_path);
    if(is_array($path_arr)) {
      foreach ($path_arr as $path) {
	$tmp_path = $this->make_path ($path, $cmd);
	if($this->my_file_exists ($tmp_path)) {
	  $cmd_path = $tmp_path;
	  break;
	}
      }
    }
    return $cmd_path;
  }

  
  function make_path()
  {
    $args = func_get_args();
    $path = join ($this->conf['slash'], $args);
    return $path;
  }


  function make_full_path ($path, $root)
  {
    if(!preg_match("/^\//", $path)){
      $path = $this->make_path($root, $path);
    }
    return $path;
  }


  function get_host_id()
  {
    if (empty ($this->host_id))  // do that only once
      {
	$zendid = $this->search_cmd_in_path ("zendid");
	if (!$zendid) {
	  $this->error_handler->on_error ('Can\'t find zendid!');
	}
	$array = preg_grep("/If multiple IDs are shown/", split("\n", $this->cmd2str("$zendid allid")), PREG_GREP_INVERT);
	$this->host_id = implode (',', $array);
      }
    return ($this->host_id);
  }

} // end of class BasicInstall




// TODO: unify error handler and logger ?
class BasicInstallLogger
{
  function log ($msg) 
  {
    fwrite (STDERR, "${msg}\n");
  }
}

class BasicInstallErrorHandler
{
  function on_error ($msg)
  {
    fwrite (STDERR, "${msg}\n");
    die (-1);
  }
}

?>
