CREATE TABLE `devices` (
  `device_id` int(11) NOT NULL AUTO_INCREMENT,
  `resource_id` varchar(45) NOT NULL DEFAULT '1',
  `device_type` int(11) NOT NULL,
  `mac_address` char(12) NOT NULL,
  `voltage` float NOT NULL DEFAULT '0',
  `orientation` int(11) NOT NULL DEFAULT '0',
  `last_checked_in` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `batteries_replaced_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `firmware_version` varchar(45) DEFAULT NULL,
  `scheduling_system` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`device_id`),
  UNIQUE KEY `mac_address_UNIQUE` (`mac_address`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=latin1
