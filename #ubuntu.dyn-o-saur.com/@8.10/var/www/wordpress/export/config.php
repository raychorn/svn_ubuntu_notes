<?php 

//********************************************************//
//               IMPORT SETTINGS
//********************************************************//

# CONFIG SECTION

  # Export Database Information - Your Joomla database that you want to export from.
  $dbh     = 'raychorn_joomla';				//enter db name
  $dbuser = 'root';					//enter db username
  $dbpass = 'peekab00';					//enter db password
  $dbhost = 'localhost';        // localhost is default
  $joomlatblprefix = 'jos_'; 	//jos_ is default

  # Import Database Information - Your WordPress database that you want to import into.
  $dbi     = 'wordpress';				//enter db name
  $dbiuser = 'root';				//enter db username
  $dbipass = 'peekab00';				//enter db password
  $dbihost = 'localhost';       // localhost is default
  $wptblprefix = 'wp_'; 		// wp_ is default
  $authorusername = 'admin'; 	// admin is default

# END CONFIG SECTION  
?>  