-- --------------------------------------------------------
-- Host:                         192.168.0.104
-- Server version:               8.0.32-0ubuntu0.22.04.2 - (Ubuntu)
-- Server OS:                    Linux
-- HeidiSQL Version:             12.3.0.6589
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Dumping database structure for football
DROP DATABASE IF EXISTS `football`;
CREATE DATABASE IF NOT EXISTS `football` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `football`;

-- Dumping structure for table football.championships
DROP TABLE IF EXISTS `championships`;
CREATE TABLE IF NOT EXISTS `championships` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `country` char(2) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `f_country` (`country`),
  CONSTRAINT `f_country` FOREIGN KEY (`country`) REFERENCES `countries` (`country_code`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB AUTO_INCREMENT=556 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table football.championships: ~0 rows (approximately)
DELETE FROM `championships`;

-- Dumping structure for table football.clubs
DROP TABLE IF EXISTS `clubs`;
CREATE TABLE IF NOT EXISTS `clubs` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '',
  `country` char(2) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `country` (`country`)
) ENGINE=InnoDB AUTO_INCREMENT=2854 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table football.clubs: ~0 rows (approximately)
DELETE FROM `clubs`;

-- Dumping structure for table football.clubs_seasons
DROP TABLE IF EXISTS `clubs_seasons`;
CREATE TABLE IF NOT EXISTS `clubs_seasons` (
  `season` int unsigned NOT NULL,
  `club` int unsigned NOT NULL,
  UNIQUE KEY `season` (`season`,`club`),
  KEY `f_club` (`club`),
  CONSTRAINT `f_club` FOREIGN KEY (`club`) REFERENCES `clubs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `f_season` FOREIGN KEY (`season`) REFERENCES `seasons` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table football.clubs_seasons: ~0 rows (approximately)
DELETE FROM `clubs_seasons`;

-- Dumping structure for table football.countries
DROP TABLE IF EXISTS `countries`;
CREATE TABLE IF NOT EXISTS `countries` (
  `id` int NOT NULL AUTO_INCREMENT,
  `phone_code` int NOT NULL,
  `country_code` char(2) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `country_name` varchar(80) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `country_code` (`country_code`)
) ENGINE=InnoDB AUTO_INCREMENT=254 DEFAULT CHARSET=utf8mb3;

-- Dumping data for table football.countries: ~252 rows (approximately)
DELETE FROM `countries`;
INSERT INTO `countries` (`id`, `phone_code`, `country_code`, `country_name`) VALUES
	(1, 93, 'AF', 'Afghanistan'),
	(2, 358, 'AX', 'Aland Islands'),
	(3, 355, 'AL', 'Albania'),
	(4, 213, 'DZ', 'Algeria'),
	(5, 1684, 'AS', 'American Samoa'),
	(6, 376, 'AD', 'Andorra'),
	(7, 244, 'AO', 'Angola'),
	(8, 1264, 'AI', 'Anguilla'),
	(9, 672, 'AQ', 'Antarctica'),
	(10, 1268, 'AG', 'Antigua and Barbuda'),
	(11, 54, 'AR', 'Argentina'),
	(12, 374, 'AM', 'Armenia'),
	(13, 297, 'AW', 'Aruba'),
	(14, 61, 'AU', 'Australia'),
	(15, 43, 'AT', 'Austria'),
	(16, 994, 'AZ', 'Azerbaijan'),
	(17, 1242, 'BS', 'Bahamas'),
	(18, 973, 'BH', 'Bahrain'),
	(19, 880, 'BD', 'Bangladesh'),
	(20, 1246, 'BB', 'Barbados'),
	(21, 375, 'BY', 'Belarus'),
	(22, 32, 'BE', 'Belgium'),
	(23, 501, 'BZ', 'Belize'),
	(24, 229, 'BJ', 'Benin'),
	(25, 1441, 'BM', 'Bermuda'),
	(26, 975, 'BT', 'Bhutan'),
	(27, 591, 'BO', 'Bolivia'),
	(28, 599, 'BQ', 'Bonaire, Sint Eustatius and Saba'),
	(29, 387, 'BA', 'Bosnia and Herzegovina'),
	(30, 267, 'BW', 'Botswana'),
	(31, 55, 'BV', 'Bouvet Island'),
	(32, 55, 'BR', 'Brazil'),
	(33, 246, 'IO', 'British Indian Ocean Territory'),
	(34, 673, 'BN', 'Brunei Darussalam'),
	(35, 359, 'BG', 'Bulgaria'),
	(36, 226, 'BF', 'Burkina Faso'),
	(37, 257, 'BI', 'Burundi'),
	(38, 855, 'KH', 'Cambodia'),
	(39, 237, 'CM', 'Cameroon'),
	(40, 1, 'CA', 'Canada'),
	(41, 238, 'CV', 'Cape Verde'),
	(42, 1345, 'KY', 'Cayman Islands'),
	(43, 236, 'CF', 'Central African Republic'),
	(44, 235, 'TD', 'Chad'),
	(45, 56, 'CL', 'Chile'),
	(46, 86, 'CN', 'China'),
	(47, 61, 'CX', 'Christmas Island'),
	(48, 672, 'CC', 'Cocos (Keeling) Islands'),
	(49, 57, 'CO', 'Colombia'),
	(50, 269, 'KM', 'Comoros'),
	(51, 242, 'CG', 'Congo'),
	(52, 242, 'CD', 'Congo, Democratic Republic of the Congo'),
	(53, 682, 'CK', 'Cook Islands'),
	(54, 506, 'CR', 'Costa Rica'),
	(55, 225, 'CI', 'Cote D\'Ivoire'),
	(56, 385, 'HR', 'Croatia'),
	(57, 53, 'CU', 'Cuba'),
	(58, 599, 'CW', 'Curacao'),
	(59, 357, 'CY', 'Cyprus'),
	(60, 420, 'CZ', 'Czech Republic'),
	(61, 45, 'DK', 'Denmark'),
	(62, 253, 'DJ', 'Djibouti'),
	(63, 1767, 'DM', 'Dominica'),
	(64, 1809, 'DO', 'Dominican Republic'),
	(65, 593, 'EC', 'Ecuador'),
	(66, 20, 'EG', 'Egypt'),
	(67, 503, 'SV', 'El Salvador'),
	(68, 240, 'GQ', 'Equatorial Guinea'),
	(69, 291, 'ER', 'Eritrea'),
	(70, 372, 'EE', 'Estonia'),
	(71, 251, 'ET', 'Ethiopia'),
	(72, 500, 'FK', 'Falkland Islands (Malvinas)'),
	(73, 298, 'FO', 'Faroe Islands'),
	(74, 679, 'FJ', 'Fiji'),
	(75, 358, 'FI', 'Finland'),
	(76, 33, 'FR', 'France'),
	(77, 594, 'GF', 'French Guiana'),
	(78, 689, 'PF', 'French Polynesia'),
	(79, 262, 'TF', 'French Southern Territories'),
	(80, 241, 'GA', 'Gabon'),
	(81, 220, 'GM', 'Gambia'),
	(82, 995, 'GE', 'Georgia'),
	(83, 49, 'DE', 'Germany'),
	(84, 233, 'GH', 'Ghana'),
	(85, 350, 'GI', 'Gibraltar'),
	(86, 30, 'GR', 'Greece'),
	(87, 299, 'GL', 'Greenland'),
	(88, 1473, 'GD', 'Grenada'),
	(89, 590, 'GP', 'Guadeloupe'),
	(90, 1671, 'GU', 'Guam'),
	(91, 502, 'GT', 'Guatemala'),
	(92, 44, 'GG', 'Guernsey'),
	(93, 224, 'GN', 'Guinea'),
	(94, 245, 'GW', 'Guinea-Bissau'),
	(95, 592, 'GY', 'Guyana'),
	(96, 509, 'HT', 'Haiti'),
	(97, 0, 'HM', 'Heard Island and Mcdonald Islands'),
	(98, 39, 'VA', 'Holy See (Vatican City State)'),
	(99, 504, 'HN', 'Honduras'),
	(100, 852, 'HK', 'Hong Kong'),
	(101, 36, 'HU', 'Hungary'),
	(102, 354, 'IS', 'Iceland'),
	(103, 91, 'IN', 'India'),
	(104, 62, 'ID', 'Indonesia'),
	(105, 98, 'IR', 'Iran, Islamic Republic of'),
	(106, 964, 'IQ', 'Iraq'),
	(107, 353, 'IE', 'Ireland'),
	(108, 44, 'IM', 'Isle of Man'),
	(109, 972, 'IL', 'Israel'),
	(110, 39, 'IT', 'Italy'),
	(111, 1876, 'JM', 'Jamaica'),
	(112, 81, 'JP', 'Japan'),
	(113, 44, 'JE', 'Jersey'),
	(114, 962, 'JO', 'Jordan'),
	(115, 7, 'KZ', 'Kazakhstan'),
	(116, 254, 'KE', 'Kenya'),
	(117, 686, 'KI', 'Kiribati'),
	(118, 850, 'KP', 'Korea, Democratic People\'s Republic of'),
	(119, 82, 'KR', 'Korea, Republic of'),
	(120, 381, 'XK', 'Kosovo'),
	(121, 965, 'KW', 'Kuwait'),
	(122, 996, 'KG', 'Kyrgyzstan'),
	(123, 856, 'LA', 'Lao People\'s Democratic Republic'),
	(124, 371, 'LV', 'Latvia'),
	(125, 961, 'LB', 'Lebanon'),
	(126, 266, 'LS', 'Lesotho'),
	(127, 231, 'LR', 'Liberia'),
	(128, 218, 'LY', 'Libyan Arab Jamahiriya'),
	(129, 423, 'LI', 'Liechtenstein'),
	(130, 370, 'LT', 'Lithuania'),
	(131, 352, 'LU', 'Luxembourg'),
	(132, 853, 'MO', 'Macao'),
	(133, 389, 'MK', 'Macedonia, the Former Yugoslav Republic of'),
	(134, 261, 'MG', 'Madagascar'),
	(135, 265, 'MW', 'Malawi'),
	(136, 60, 'MY', 'Malaysia'),
	(137, 960, 'MV', 'Maldives'),
	(138, 223, 'ML', 'Mali'),
	(139, 356, 'MT', 'Malta'),
	(140, 692, 'MH', 'Marshall Islands'),
	(141, 596, 'MQ', 'Martinique'),
	(142, 222, 'MR', 'Mauritania'),
	(143, 230, 'MU', 'Mauritius'),
	(144, 269, 'YT', 'Mayotte'),
	(145, 52, 'MX', 'Mexico'),
	(146, 691, 'FM', 'Micronesia, Federated States of'),
	(147, 373, 'MD', 'Moldova, Republic of'),
	(148, 377, 'MC', 'Monaco'),
	(149, 976, 'MN', 'Mongolia'),
	(150, 382, 'ME', 'Montenegro'),
	(151, 1664, 'MS', 'Montserrat'),
	(152, 212, 'MA', 'Morocco'),
	(153, 258, 'MZ', 'Mozambique'),
	(154, 95, 'MM', 'Myanmar'),
	(155, 264, 'NA', 'Namibia'),
	(156, 674, 'NR', 'Nauru'),
	(157, 977, 'NP', 'Nepal'),
	(158, 31, 'NL', 'Netherlands'),
	(159, 599, 'AN', 'Netherlands Antilles'),
	(160, 687, 'NC', 'New Caledonia'),
	(161, 64, 'NZ', 'New Zealand'),
	(162, 505, 'NI', 'Nicaragua'),
	(163, 227, 'NE', 'Niger'),
	(164, 234, 'NG', 'Nigeria'),
	(165, 683, 'NU', 'Niue'),
	(166, 672, 'NF', 'Norfolk Island'),
	(167, 1670, 'MP', 'Northern Mariana Islands'),
	(168, 47, 'NO', 'Norway'),
	(169, 968, 'OM', 'Oman'),
	(170, 92, 'PK', 'Pakistan'),
	(171, 680, 'PW', 'Palau'),
	(172, 970, 'PS', 'Palestinian Territory, Occupied'),
	(173, 507, 'PA', 'Panama'),
	(174, 675, 'PG', 'Papua New Guinea'),
	(175, 595, 'PY', 'Paraguay'),
	(176, 51, 'PE', 'Peru'),
	(177, 63, 'PH', 'Philippines'),
	(178, 64, 'PN', 'Pitcairn'),
	(179, 48, 'PL', 'Poland'),
	(180, 351, 'PT', 'Portugal'),
	(181, 1787, 'PR', 'Puerto Rico'),
	(182, 974, 'QA', 'Qatar'),
	(183, 262, 'RE', 'Reunion'),
	(184, 40, 'RO', 'Romania'),
	(185, 70, 'RU', 'Russian Federation'),
	(186, 250, 'RW', 'Rwanda'),
	(187, 590, 'BL', 'Saint Barthelemy'),
	(188, 290, 'SH', 'Saint Helena'),
	(189, 1869, 'KN', 'Saint Kitts and Nevis'),
	(190, 1758, 'LC', 'Saint Lucia'),
	(191, 590, 'MF', 'Saint Martin'),
	(192, 508, 'PM', 'Saint Pierre and Miquelon'),
	(193, 1784, 'VC', 'Saint Vincent and the Grenadines'),
	(194, 684, 'WS', 'Samoa'),
	(195, 378, 'SM', 'San Marino'),
	(196, 239, 'ST', 'Sao Tome and Principe'),
	(197, 966, 'SA', 'Saudi Arabia'),
	(198, 221, 'SN', 'Senegal'),
	(199, 381, 'RS', 'Serbia'),
	(200, 381, 'CS', 'Serbia and Montenegro'),
	(201, 248, 'SC', 'Seychelles'),
	(202, 232, 'SL', 'Sierra Leone'),
	(203, 65, 'SG', 'Singapore'),
	(204, 1, 'SX', 'Sint Maarten'),
	(205, 421, 'SK', 'Slovakia'),
	(206, 386, 'SI', 'Slovenia'),
	(207, 677, 'SB', 'Solomon Islands'),
	(208, 252, 'SO', 'Somalia'),
	(209, 27, 'ZA', 'South Africa'),
	(210, 500, 'GS', 'South Georgia and the South Sandwich Islands'),
	(211, 211, 'SS', 'South Sudan'),
	(212, 34, 'ES', 'Spain'),
	(213, 94, 'LK', 'Sri Lanka'),
	(214, 249, 'SD', 'Sudan'),
	(215, 597, 'SR', 'Suriname'),
	(216, 47, 'SJ', 'Svalbard and Jan Mayen'),
	(217, 268, 'SZ', 'Swaziland'),
	(218, 46, 'SE', 'Sweden'),
	(219, 41, 'CH', 'Switzerland'),
	(220, 963, 'SY', 'Syrian Arab Republic'),
	(221, 886, 'TW', 'Taiwan, Province of China'),
	(222, 992, 'TJ', 'Tajikistan'),
	(223, 255, 'TZ', 'Tanzania, United Republic of'),
	(224, 66, 'TH', 'Thailand'),
	(225, 670, 'TL', 'Timor-Leste'),
	(226, 228, 'TG', 'Togo'),
	(227, 690, 'TK', 'Tokelau'),
	(228, 676, 'TO', 'Tonga'),
	(229, 1868, 'TT', 'Trinidad and Tobago'),
	(230, 216, 'TN', 'Tunisia'),
	(231, 90, 'TR', 'Turkey'),
	(232, 7370, 'TM', 'Turkmenistan'),
	(233, 1649, 'TC', 'Turks and Caicos Islands'),
	(234, 688, 'TV', 'Tuvalu'),
	(235, 256, 'UG', 'Uganda'),
	(236, 380, 'UA', 'Ukraine'),
	(237, 971, 'AE', 'United Arab Emirates'),
	(238, 44, 'GB', 'United Kingdom'),
	(239, 1, 'US', 'United States'),
	(240, 1, 'UM', 'United States Minor Outlying Islands'),
	(241, 598, 'UY', 'Uruguay'),
	(242, 998, 'UZ', 'Uzbekistan'),
	(243, 678, 'VU', 'Vanuatu'),
	(244, 58, 'VE', 'Venezuela'),
	(245, 84, 'VN', 'Viet Nam'),
	(246, 1284, 'VG', 'Virgin Islands, British'),
	(247, 1340, 'VI', 'Virgin Islands, U.s.'),
	(248, 681, 'WF', 'Wallis and Futuna'),
	(249, 212, 'EH', 'Western Sahara'),
	(250, 967, 'YE', 'Yemen'),
	(251, 260, 'ZM', 'Zambia'),
	(252, 263, 'ZW', 'Zimbabwe'),
	(253, 44, 'EN', 'England');

-- Dumping structure for table football.seasons
DROP TABLE IF EXISTS `seasons`;
CREATE TABLE IF NOT EXISTS `seasons` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `championship` int unsigned NOT NULL,
  `year` char(10) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `championship` (`championship`,`year`),
  CONSTRAINT `k_championship` FOREIGN KEY (`championship`) REFERENCES `championships` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB AUTO_INCREMENT=384 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Dumping data for table football.seasons: ~0 rows (approximately)
DELETE FROM `seasons`;

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
