<?php
/*
   +----------------------------------------------------------------------+
   | Progress bar                                                         |
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
   | Hints:                                                               | 
   | 1) Must be run as different process.                                 |
   | 2) Environment viriables must be set:                                |
   |    DIALOG - to the path of dialog utility                            |
   |    TIME - to the time (in seconds) for progress                      |
   |    PID_FILE - file where the process ID will be stored               |
   |    FATAL_FILE - flag-file whether to kill this progress bar          |
   |    MESSAGE - message to be shown in the dialog                       |
   |    MESSAGE_TTY - message to be shown in the text mode                |
   +----------------------------------------------------------------------+
*/

$DIALOG = getenv("DIALOG");           // path to dialog

if($DIALOG == "DBUILDER"){
  define('DBUILDER', 1);
  include_once("dbuilder.inc");
}

include_once("gaugebox.inc");


//-----------------------
// Variables:
//=======================

$TIME = getenv("TIME");                // time in seconds to progress
$PID_FILE = getenv("PID_FILE");       // PID file to store my process ID
$FATAL_FILE = getenv("FATAL_FILE");
$MSG = getenv("MESSAGE");
$MSG_TTY = getenv("MESSAGE_TTY");
$TITLE = getenv("TITLE");

//----------------------
// Begin work:
//======================

error_reporting(0);

// Write my PID to the PID file
$fp = fopen($PID_FILE, "w");
if(empty($fp)){
  die("Cannot open PID file: $PID_FILE\n");
}
fwrite($fp, getmypid());
fclose($fp);

if(defined('DBUILDER')){
  $dialog =& new Dbuilder();
  $gb = new Gaugebox($dialog, 48, 12, $MSG, $TITLE);
}
else{
  $gb = new Gaugebox($DIALOG, 48, 12, $MSG);
}

if($DIALOG == FALSE){
  $MSG = $MSG_TTY;
}

for($percent=1; $percent<=100; $percent++){
  $gb->update($MSG, $percent);
  usleep($TIME * 10000);

	if(@file_exists($FATAL_FILE)) {
		unlink($FATAL_FILE);
		$percent = 100;
	}
}
$gb->update($MSG, "100");
$gb->close();

?>
