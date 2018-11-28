#!/bin/sh

# Include settings from settings.cfg
. web/config/settings.cfg

# Create device database table with one example entry for Nagios monitoring script
mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername -p$deviceDatabasePassword -D $deviceDatabaseName -e "\
CREATE TABLE IF NOT EXISTS \`devices\` (\
  \`device_id\` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,\
  \`resource_id\` varchar(64) NOT NULL DEFAULT '1',\
  \`device_type\` int(11) UNSIGNED NOT NULL,\
  \`mac_address\` char(12) NOT NULL,\
  \`voltage\` float NOT NULL DEFAULT '0',\
  \`orientation\` int(11) UNSIGNED NOT NULL DEFAULT '0',\
  \`last_checked_in\` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,\
  \`batteries_replaced_date\` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,\
  \`firmware_version\` varchar(45) DEFAULT NULL,\
  \`scheduling_system\` int(11) UNSIGNED NOT NULL DEFAULT '0',\
  \`notes\` varchar(1000) NOT NULL DEFAULT '',\
  \`height\` int(11) UNSIGNED NOT NULL DEFAULT '384',\
  \`width\` int(11) UNSIGNED NOT NULL DEFAULT '640',\
  \`is_production\` boolean NOT NULL DEFAULT 1,\
  PRIMARY KEY (\`device_id\`),\
  UNIQUE KEY \`mac_address_UNIQUE\` (\`mac_address\`)\
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=latin1;\
INSERT INTO devices VALUES ('4','58','7','AAAABBBBCCCC','4','1','2018-10-10 08:52:49','2018-10-05 15:53:09','nagios','3','This is a fake device for monitoring the server with Nagios. Change the plugin type to monitor a particular plugin.','384','640','1');"

# Create empty monitoring table for Nagios monitoring script
mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername -p$deviceDatabasePassword -D $deviceDatabaseName -e "\
CREATE TABLE IF NOT EXISTS \`monitoring\` (\
	\`mac_address\` char(12) NOT NULL,\
	\`next_check_in\` datetime NOT NULL,\
	PRIMARY KEY (\`mac_address\`)\
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=latin1;"
