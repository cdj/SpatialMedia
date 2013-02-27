SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;


CREATE TABLE IF NOT EXISTS `Players` (
  `Num` int(11) NOT NULL AUTO_INCREMENT,
  `TeamNum` int(11) NOT NULL,
  `Entries` int(11) NOT NULL,
  PRIMARY KEY (`Num`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;


CREATE TABLE IF NOT EXISTS `Settings` (
  `Num` int(11) NOT NULL AUTO_INCREMENT,
  `RoundLength` int(11) NOT NULL,
  `WaitingTime` int(11) NOT NULL,
  `RoundStartTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`Num`),
  UNIQUE KEY `RoundLength` (`RoundLength`),
  UNIQUE KEY `WaitingTime` (`WaitingTime`),
  UNIQUE KEY `RoundStartTime` (`RoundStartTime`),
  UNIQUE KEY `RoundLength_2` (`RoundLength`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

INSERT INTO `Settings` (`Num`, `RoundLength`, `WaitingTime`, `RoundStartTime`) VALUES
(1, 30, 120, '2013-02-19 23:01:49');

CREATE TABLE IF NOT EXISTS `Teams` (
  `Num` int(11) NOT NULL AUTO_INCREMENT,
  `Color` varchar(20) NOT NULL,
  `Wins` int(11) NOT NULL,
  `IsEnabled` tinyint(1) NOT NULL,
  PRIMARY KEY (`Num`),
  UNIQUE KEY `Color` (`Color`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

INSERT INTO `Teams` (`Num`, `Color`, `Wins`, `IsEnabled`) VALUES
(1, 'blue', 0, 1),
(2, 'yellow', 0, 1),
(3, 'green', 0, 1),
(4, 'red', 0, 1),
(5, 'purple', 0, 1),
(6, 'orange', 0, 1);
