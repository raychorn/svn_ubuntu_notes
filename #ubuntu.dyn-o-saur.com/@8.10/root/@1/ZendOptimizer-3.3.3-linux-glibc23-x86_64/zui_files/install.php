<?php
/*
   +----------------------------------------------------------------------+
   | ZendOptimizer installation script                                    |
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

include_once("install.inc");


class OptimizerInstall extends Install {

        function OptimizerInstall() {

              # parent::Install ('Zend Optimizer', '3.0.0 Beta2');
		parent::Install ('Zend Optimizer');
        }
}

$INSTALL =& new OptimizerInstall();

#################### Components List ################

$INSTALL->set_components(array(
			"doc"                     =>  array ("%PREFIX%/doc", false),
			"zendid"                  =>  array ("%PREFIX%/bin", false),
			"ZendExtensionManager.so" =>  array ("%PREFIX%/lib", false),
			"poweredbyoptimizer.gif"  =>  array ("%PREFIX%/etc", false),
			"README-ZendOptimizer"    =>  array ("%PREFIX%/doc", false),
			"EULA-ZendOptimizer"      =>  array ("%PREFIX%/doc", false)
			));

$INSTALL->conf['supported_systems'] = array(
		"Linux" => array(
			"glibc" => array("2.1", "2.2", "2.3")
			),
		"SunOS" => array(
			"release" => array("5.x")
			),
		"FreeBSD" => array(
			"release" => array("3.4", "4.x", "5.x"),
			),
		"AIX" => array(
			"release" => array("4.x", "5.x"),
			)
		);

//$INSTALL->check_system_supported();

$INSTALL->welcome_box();
$INSTALL->license_agmnt_box();

$INSTALL->choose_install_prefix("/usr/local/Zend");
$INSTALL->php_ini_location_guess();

# Check for installed components:
$keep_existing = $INSTALL->check_installed_components(array( 
	array (
		"filename" => "ZendExtensionManager.so",
		"compname" => "extension_manager",
		"nicename" => "ZendExtensionManger"
	),
	array (
		"zemname" => "optimizer",
		"compname" => "optimizer",
		"nicename" => "ZendOptimizer"
	)
));

if(isset($keep_existing["extension_manager"])) {
	$INSTALL->remove_component ("ZendExtensionManager.so");
}


$using_apache = $INSTALL->is_using_apache();
if($using_apache){
	$apache_ver = $INSTALL->apache_get_version();
}

if($INSTALL->conf['uname']['sysname'] == "AIX") {
	$INSTALL->php_version_remove("4.0.6");
}

if($INSTALL->conf['uname']['sysname'] == "Darwin" )
{
	$INSTALL->php_version_remove("4.0.6");
	$INSTALL->php_version_remove("4.1.x");
	$INSTALL->php_version_remove("4.2.0");
	$INSTALL->php_version_remove("4.2.x");
	$INSTALL->php_version_remove("4.3.x");
	$INSTALL->php_version_add("PHP 4.3.x ", "4.3.x ", 2);
}

$INSTALL->php_version_detect(true, true);

$php_versions = $INSTALL->php_versions_get_array();
foreach ($php_versions as $php_ver) {

	$PHP_VER = preg_replace("/\./", "_", $php_ver);

	if(!isset($keep_existing["optimizer"])) {
		$INSTALL->add_component($PHP_VER."_comp/ZendOptimizer.so",
				$INSTALL->make_path($INSTALL->conf['prefix'], "lib",
					"Optimizer-".$INSTALL->get_component_version("optimizer"), "php-$php_ver"),
				false);
	}

	/* install Optimizer thread safety for PHP version >= 4.2.1 */
	if(version_compare($php_ver, "4.2.0")>0 && $INSTALL->conf['uname']['sysname'] != "AIX" && $INSTALL->conf['uname']['sysname'] != "Darwin" ){

		if(!isset($keep_existing["optimizer"])) {
			$INSTALL->add_component($PHP_VER."_comp/TS/ZendOptimizer.so",
					$INSTALL->make_path($INSTALL->conf['prefix'], "lib",
						"Optimizer_TS-".$INSTALL->get_component_version("optimizer-ts"), "php-$php_ver"),
					false);
		}

		if(!isset($keep_existing["extension_manager"])) {
			$INSTALL->add_component("ZendExtensionManager_TS.so",
								$INSTALL->make_path($INSTALL->conf['prefix'], "lib"), false);
		}
	}
}


$INSTALL->set_var_component("%PREFIX%", $INSTALL->conf['prefix']);

$INSTALL->start_install();

/* do php.ini modifications */
$INSTALL->php_ini_open();

$INSTALL->php_ini_add_zend_section();

# 2.6.2- no need for this directive any more.
#$INSTALL->php_ini_add_entry("zend_optimizer.optimization_level", 15);
$INSTALL->php_ini_add_zend_extension($INSTALL->conf['prefix']."/lib/ZendExtensionManager.so");

# Remove ZendOptimizer.so zend_extension entry before using ZendExtensionManager
$INSTALL->php_ini_remove_entry("ZendOptimizer.so");

if(!isset($keep_existing["optimizer"])) {
	$INSTALL->php_ini_add_entry("zend_extension_manager.optimizer",
			$INSTALL->make_path($INSTALL->conf['prefix'], "lib",
				"Optimizer-".$INSTALL->get_component_version("optimizer")));
}

if($INSTALL->conf['uname']['sysname'] != "AIX" && $INSTALL->conf['uname']['sysname'] != "Darwin"){
	if(!isset($keep_existing["optimizer"])) {
		$INSTALL->php_ini_add_entry("zend_extension_manager.optimizer_ts",
				$INSTALL->make_path($INSTALL->conf['prefix'], "lib",
					"Optimizer_TS-".$INSTALL->get_component_version("optimizer-ts")));
	}

	if(!isset($keep_existing["extension_manager"])) {
		$INSTALL->php_ini_add_zend_extension($INSTALL->make_path($INSTALL->conf['prefix'], "lib",
					"ZendExtensionManager_TS".$INSTALL->conf['so_ext']), "zend_extension_ts");
	}
}

$INSTALL->add_package_info();

$INSTALL->php_ini_reorder();
$INSTALL->php_ini_fix();
$INSTALL->php_ini_close();

$INSTALL->php_ini_relocate();

/* There may be a case when PHP type is not detected, since php_type_guess() call is commented */
if(isset($INSTALL->conf['php_type']) && $INSTALL->conf['php_type'] == "executable"){
	$INSTALL->msgbox("The installation has completed successfully.");
}
else{
	$INSTALL->msgbox("The installation has completed successfully.\n".
			$INSTALL->conf['product']." is now ready for use.\n".
			"You must restart your Web server for the modifications to take effect.");

	if($INSTALL->conf['webserver'] == "Apache" && $INSTALL->yesnobox("Restart the Web server now?")){
		$INSTALL->webserver_restart();
	}
}

$INSTALL->cleanup();

?>
