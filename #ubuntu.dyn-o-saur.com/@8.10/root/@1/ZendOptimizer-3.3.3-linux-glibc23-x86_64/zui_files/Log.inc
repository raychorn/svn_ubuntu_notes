<?php

	/**
	* @author Michael Spector <michael@zend.com>
	* @copyright Zend Technologies Ltd 2005
	* @version $Revision: 1.4.4.2 $
	* @since $Date: 2005/08/24 12:27:56 $
	*/

	/**
	* class Log
	* 
	* This class used for logging entries
	* LOG_FILE constant should be set to the log filename, otherwise system logging
	* instruments will be used.
	*/
	class Log
	{
		/**
		* This function appends entry to the logfile if it was set, otherwise - to the system log
		* @param string entry
		*/
		function append ($entry)
		{
			if(defined ('LOG_FILE')) {
				@error_log (sprintf("%s %s\n", date('[M d H:i:s]'), $entry), 3, LOG_FILE);
			} else {
				@error_log (sprintf("%s %s\n", date('[M d H:i:s]'), $entry));
			}
		}
	}
?>
