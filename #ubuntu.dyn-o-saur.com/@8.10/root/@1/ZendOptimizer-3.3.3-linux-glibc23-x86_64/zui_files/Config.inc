<?php

	/**
	* @author Michael Spector <michael@zend.com>
	* @copyright Zend Technologies Ltd 2005
	* @version $Revision: 1.10.2.1.2.4 $
	* @since $Date: 2005/08/24 12:27:55 $
	*/

	include_once ('Util.inc');

	define ('CONFIG_SQLITE_DB', 1);
	define ('CONFIG_INI', 2);

	/**
	* class Config
	* 
	* This class hides interface for accessing configuration
	* of different types: SQLite database, INI file, etc...
	*/
	class Config
	{
		/**
		* Constructor
		* Lock the php.ini
		* @param string path to the configuration file
		* @param int type of the config
		*/
		function Config($path, $config_type=CONFIG_SQLITE_DB)
		{
			if($config_type == CONFIG_SQLITE_DB && !extension_loaded('sqlite')) {
				die ("SQLite extension must be loaded!\n");
			}
			$this->path = $path;
			$this->config_type = $config_type;
			$this->read();
		}

		/**
		* Destructor
		* Unlock the php.ini
		*/
		function __destruct()
		{
		}

		/**
		* This function reads configuration database
		* @return bool exit status of the operation
		*/
		function read()
		{
			if(isset($this->cont)) {
				return true;
			}
			switch ($this->config_type)
			{
				case CONFIG_INI:
					Log::append ("Opening INI configuration file: {$this->path}");

					/* Read file, split it to sections */
					$cont = @file($this->path);
					if (!$cont) $cont = array();

					$section = $this->last_section = $this->first_section = '__section'.getmypid();
					$this->cont = array();
					foreach ($cont as $i => $line) {
						if (preg_match ("#^\s*\[([^\]]+)\]\s*$#", $line, $match)) {
							$section = $match[1];
							$this->last_section = $section;
							continue;
						}
						$this->cont[$section][$i] = rtrim($line);
					}
					$this->md5 = md5(serialize($this->cont));
					return true;

				case CONFIG_SQLITE_DB:
					Log::append ("Opening Sqlite DB configuration file: {$this->path}");

					$this->cont = sqlite_open($this->path);
					if (!is_resource($this->cont)) {
						return false;
					}
					return true;
			}
			return false;
		}

		/**
		* This function sets path to configuration database
		*/
		function setPath($path)
		{
			if (isset($this->path) && realpath ($path) == realpath ($this->path)) {
				return;
			}
			unset ($this->cont);
			$this->path = $path;
		}

		/**
		* This function saves changes into configuration database
		* @return bool exit status of the operation
		*/
		function save()
		{
			switch ($this->config_type)
			{
				case CONFIG_INI:
					if(!isset($this->cont)) {
						return false;
					}
					if(md5(serialize($this->cont)) != $this->md5) {
						/* First backup the old copy */
						$backup_suffix = defined('BACKUP_SUFFIX') ? BACKUP_SUFFIX : '.bak';
						if(file_exists($this->path) && !file_exists($this->path.$backup_suffix)) {
							Log::append ("Saving INI configuration file backup: {$this->path}{$backup_suffix}");
							copy($this->path, $this->path.$backup_suffix);
						}
						$cont = "";
						foreach ($this->cont as $section => $section_cont) {
							if ($section != $this->first_section) {
								$cont .= "[$section]\n";
							}
							foreach ($section_cont as $line) {
								$cont .= "$line\n";
							}
						}
						Util::strToFile($cont, $this->path);
						$this->md5 = md5(serialize($this->cont));
					}
					return true;

				case CONFIG_SQLITE_DB:
					if(!isset($this->cont)) {
						return false;
					}
					/* Need to do nothing */
					return true;
			}
			return true;
		}

		/**
		* This function retrieves config value
		* @param string name of configuration entry
		* @param string section (optional)
		* @param string pattern (optional)
		* @return string value that was retrieved
		*/
		function get($name, $section=null, $pattern="")
		{
			if ($this->read()) {
				switch ($this->config_type)
				{
					case CONFIG_INI:
						foreach ($this->cont as $sect => $sect_cont) {
							if ($section && $sect != $section) continue;
							foreach ($sect_cont as $line) {
								if(preg_match("/^\s*$name\s*=(.*$pattern.*)$/", $line, $match)) {
									 return trim($match[1], "\t \"\'");
								}
							}
						}
						return null;

					case CONFIG_SQLITE_DB:
						if(!$section) $section = "main";
						$res = @sqlite_query($this->cont, sprintf("SELECT value FROM %s WHERE name='%s';", sqlite_escape_string($section), sqlite_escape_string($name)));
						if(@sqlite_has_more($res)) {
							return sqlite_fetch_single($res);
						}
						return null;
				}
			}
			return false;
		}

		/**
		* This function sets configuration entry
		* @param string name of the entry
		* @param string value of the entry
		* @param string section where to insert new entry (optional)
		* @param string pattern of old entry (optional)
		* @return string old value
		*/
		function set($name, $value, $section=null, $pattern="")
		{
			if ($this->read()) {
				switch ($this->config_type)
				{
					case CONFIG_INI:
						foreach ($this->cont as $sect => $sect_cont) {
							if ($section && $sect != $section) continue;
							foreach ($sect_cont as $i => $line) {
								if(preg_match("/^\s*$name\s*=(.*$pattern.*)$/", $line, $match)) {
									if (strcmp ($this->cont[$sect][$i], "{$name}={$value}")) {
										Log::append ("Replacing INI configuration entry: {$this->cont[$sect][$i]} with: {$value} in: {$this->path}");
										$this->cont[$sect][$i] = "{$name}={$value}";
									}
									return trim($match[1], "\t \"\'");
								}
							}
						}
						Log::append ("Adding INI configuration entry: {$name}={$value} into: {$this->path}");
						$this->cont[($section ? $section : $this->last_section)][] = "$name=$value";
						return null;

					case CONFIG_SQLITE_DB:
						if(!$section) $section = "main";
						$this->addSection ($section);

						if (($old_value = $this->get($name, $section)) !== null) {
							Log::append ("Replacing Sqlite DB configuration entry: {$name} with: {$value} in: {$this->path}:{$section}");
							sqlite_query($this->cont, sprintf("UPDATE %s SET value='%s' WHERE name='%s';", sqlite_escape_string($section), sqlite_escape_string($value), sqlite_escape_string($name)));
							return $old_value;
						} else {
							Log::append ("Adding Sqlite DB configuration entry: {$name}={$value} into: {$this->path}:{$section}");
							sqlite_query($this->cont, sprintf("INSERT INTO %s values('%s', '%s');", sqlite_escape_string($section), sqlite_escape_string($name), sqlite_escape_string($value)));
						}
						return null;
				}
			}
			return false;
		}

		/**
		* This function removes configuration entry
		* @param string name of the entry
		* @param string value pattern  (optional)
		* @param string section where to remove the entry (optional)
		* @return string entry value that was removed
		*/
		function del($name, $pattern="", $section=null)
		{
			if ($this->read()) {
				switch ($this->config_type)
				{
					case CONFIG_INI:
						foreach ($this->cont as $sect => $sect_cont) {
							if ($section && $sect != $section) continue;
							foreach ($sect_cont as $i => $line) {
								if(preg_match("/^\s*$name\s*=(.*$pattern.*)$/", $line, $match)) {
									Log::append ("Removing INI configuration entry: {$name}={$pattern} from: {$this->path}");
									unset($this->cont[$sect][$i]);
									return trim($match[1], "\t \"\'");
								}
							}
						}
						return null;

					case CONFIG_SQLITE_DB:
						if(!$section) $section = "main";
						if (($old_value = $this->get($name, $section)) !== null) {
							Log::append ("Removing Sqlite DB configuration entry: {$name} from: {$this->path}:{$section}");
							sqlite_query($this->cont, sprintf("REMOVE FROM %s WJERE name='%s';", sqlite_escape_string($section), sqlite_escape_string($name)));
							return $old_value;
						}
						return null;
				}
			}
			return false;
		}

		/**
		* This function adds new section into configuration
		* @param string section name
		*/
		function addSection($section)
		{
			if ($this->read()) {
				switch ($this->config_type)
				{
					case CONFIG_INI:
						if (!isset($this->cont[$section])) $this->cont[$section] = array();
						$this->last_section = $section;
						break;

					case CONFIG_SQLITE_DB:
						if(!$section) $section = "main";
						@sqlite_query($this->cont, sprintf("CREATE TABLE %s(name TEXT, value TEXT);", sqlite_escape_string($section)));
						break;
				}
			}
		}
	}
?>
