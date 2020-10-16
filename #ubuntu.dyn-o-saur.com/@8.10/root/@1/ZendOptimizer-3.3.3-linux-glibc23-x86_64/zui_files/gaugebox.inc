<?php
/*
   +----------------------------------------------------------------------+
   | Gauge box (progress bar)                                             |
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
   | Tips:                                                                |
   | 1) If you want to find some function - search it with uppercase      |
   |                                                                      |
   +----------------------------------------------------------------------+
*/

class Gaugebox
{
	var $fp;      /* file descriptor to dialog */
	var $shtu4ka = array(
			0 => '\\ ',
			1 => '| ',
			2 => '/ ',
			3 => '--'
			);
	var $percent;

	function Gaugebox($dialog, $max_x, $max_y, $msg, $percent=0, $title="")
	{
		error_reporting(0);

		$this->dialog = $dialog;
		$max_y = empty($max_y)? 24 : $max_y;
		$max_x = empty($max_x)? 78 : $max_x;
		$msg = strlen($msg) ? $msg : " ";

		if(!empty($this->dialog)){
			$dialog_cmd = sprintf ("%s --gauge %s %s %s 0",
				$this->dialog,
				escapeshellarg ($msg),
				escapeshellarg ($max_y),
				escapeshellarg ($max_x)
			);
			$this->fp = popen($dialog_cmd, "w");
		}

		if(empty($this->dialog)){
			print("{$msg}\n\n");
		}
	}

	// UPDATE
	function update($msg, $percent)
	{
		if(!empty($this->dialog)){ /* GUI */
			fwrite($this->fp, "XXX\n"); fflush($this->fp);
			fwrite($this->fp, "{$percent}\n"); fflush($this->fp); /* update percent */
			fwrite($this->fp, "{$msg}\n"); fflush($this->fp); /* update message */
			fwrite($this->fp, "XXX\n"); fflush($this->fp);
		}
		else{ /* TEXT */
			printf("%-50s  %2s [%d%%]             \r", $msg, $this->shtu4ka[$percent % 4], $percent);
		}
	}

	// CLOSE
	function close()
	{
		if(!empty($this->dialog)){
			pclose($this->fp);
		}
		else{
			print("\n");
		}
	}
}

?>
