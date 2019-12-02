-- Adminer 4.7.4 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

SET NAMES utf8mb4;

DROP DATABASE IF EXISTS `test_prot`;
CREATE DATABASE `test_prot` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `test_prot`;

DROP TABLE IF EXISTS `form_sessions`;
CREATE TABLE `form_sessions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `unq_name` varchar(720) NOT NULL,
  `json` json NOT NULL,
  `user_forms` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `unq_name` (`unq_name`),
  KEY `user_forms` (`user_forms`),
  CONSTRAINT `form_sessions_ibfk_2` FOREIGN KEY (`user_forms`) REFERENCES `users_forms` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `form_sessions`;

DROP TABLE IF EXISTS `forms`;
CREATE TABLE `forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `json` json NOT NULL,
  `name` varchar(360) NOT NULL,
  `developer` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `developer` (`developer`),
  CONSTRAINT `fk_forms_1` FOREIGN KEY (`developer`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `forms`;
INSERT INTO `forms` (`id`, `json`, `name`, `developer`) VALUES
(1,	'{\"form\": {\"version\": 0, \"form.name\": \"Washer easer\", \"author.name\": \"carlos bernal\"}, \"diss_test\": 1, \"questions\": [{\"id\": 1, \"mode\": [\"sequential\", \"once\"], \"question\": \"Do you have washer?\", \"answer_tag\": \"washer\", \"type_user_input\": \"STRING\", \"answers_branches\": {\"predefined_boolean_yes_no_affirmative_yes\": {\"else\": -5, \"true\": 72, \"taskstory_id\": \"NONE\"}}}, {\"id\": 72, \"mode\": [\"sequential\", \"if_error_repeat\", \"ignorecase\"], \"question\": \"How many Kilograms have your washer capacity?\", \"answer_tag\": \"kg_capacity\", \"taskstories\": {\"normal_user\": [{\"tag\": \"base_task\", \"name\": \"step 1\", \"comment\": \"The user is implicit\", \"duration\": {\"hours\": 1, \"minutes\": 5, \"seconds\": 3}, \"frequency\": \"weekly\", \"description\": \"Gather Clothes around home\", \"restrictions\": [\"early\", \"soon\", \"launch\"]}, {\"tag\": \"washer_start\", \"name\": \"step 2\", \"duration\": {\"minutes\": 2}, \"frequency\": \"weekly\", \"description\": \"Put them inside and start the machine\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_intermediate\", \"name\": \"step 3\", \"duration\": {\"hours\": 1, \"minutes\": 59, \"seconds\": 59}, \"frequency\": \"weekly\", \"description\": \"Put them inside and start the machine\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_end\", \"name\": \"step 4\", \"when\": {\"after\": \"washer_start\", \"maximum_delay\": {\"hours\": 2}, \"minimum_delay\": {\"hours\": 1, \"minutes\": 15}}, \"duration\": {\"minutes\": 15}, \"frequency\": \"weekly\", \"description\": \"Collect and hang out the washing\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_clean_up\", \"name\": \"step 5\", \"when\": {\"after\": \"washer_end\", \"maximum_delay\": {\"days\": 1, \"hours\": 22}, \"minimum_delay\": {\"days\": 1, \"hours\": 1, \"minutes\": 15}}, \"duration\": {\"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"Collect and hang out the washing\", \"restrictions\": [\"night\"]}], \"industrial_user\": [{\"tag\": \"industrial_start\", \"name\": \"step 1\", \"duration\": {\"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"start the machinery\", \"restrictions\": [\"night\"]}, {\"tag\": \"industrial_middle\", \"name\": \"step 2\", \"duration\": {\"hours\": 7, \"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"start the machinery\", \"restrictions\": [\"soon\", \"early\", \"launch\", \"day\"]}]}, \"type_user_input\": \"INTEGER\", \"answers_branches\": {\"ranges\": [{\"range\": {\"values\": {\"<\": 1000, \">\": 30}, \"if_branch\": 3, \"taskstory_id\": \"industrial_user\"}}, {\"range\": {\"values\": {\"<\": 30, \">\": 0}, \"if_branch\": -1, \"taskstory_id\": \"normal_user\"}}, {\"range\": {\"values\": {\"<\": 0, \">\": 1000}, \"if_branch\": -4, \"taskstory_id\": 3}}]}}, {\"id\": 3, \"mode\": [\"sequential\", \"once\"], \"question\": \"Your washer is a industrial one?\", \"answer_tag\": \"industrial\", \"type_user_input\": \"STRING\", \"answers_branches\": {\"custom\": {\"no\": {\"if_branch\": 0}, \"yes\": {\"if_branch\": -3}, \"else\": {\"if_branch\": -1}}}}]}',	'Washer easer',	1);

DROP TABLE IF EXISTS `schedulers`;
CREATE TABLE `schedulers` (
  `idscheduler` int(11) NOT NULL,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`idscheduler`),
  UNIQUE KEY `idscheduler_UNIQUE` (`idscheduler`),
  KEY `fk_schedulers_1_idx` (`user`),
  CONSTRAINT `fk_schedulers_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `schedulers`;

DROP TABLE IF EXISTS `taskers`;
CREATE TABLE `taskers` (
  `idtasker` int(11) NOT NULL,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`idtasker`),
  UNIQUE KEY `idtaskers_UNIQUE` (`idtasker`),
  KEY `fk_taskers_1_idx` (`user`),
  CONSTRAINT `fk_taskers_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `taskers`;

DROP TABLE IF EXISTS `tasks`;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL,
  `json` json NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `tasks`;

DROP TABLE IF EXISTS `tasks_schedulers`;
CREATE TABLE `tasks_schedulers` (
  `idtask` int(11) NOT NULL,
  `idscheduler` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idscheduler`),
  KEY `idscheduler_idx` (`idscheduler`),
  CONSTRAINT `idscheduler` FOREIGN KEY (`idscheduler`) REFERENCES `schedulers` (`idscheduler`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtask_sche` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `tasks_schedulers`;

DROP TABLE IF EXISTS `tasks_taskers`;
CREATE TABLE `tasks_taskers` (
  `idtask` int(11) NOT NULL,
  `idtasker` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idtasker`),
  KEY `idtasker_idx` (`idtasker`),
  CONSTRAINT `idtask` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtasker` FOREIGN KEY (`idtasker`) REFERENCES `taskers` (`idtasker`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `tasks_taskers`;

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(360) NOT NULL,
  `json` json DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `users`;
INSERT INTO `users` (`id`, `username`, `json`) VALUES
(1,	'carloscbl',	'{\"username\": \"carloscbl\"}'),
(2,	'pepe',	'{}');

DROP TABLE IF EXISTS `users_forms`;
CREATE TABLE `users_forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `iduser` int(11) NOT NULL,
  `idform` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `user_form` (`iduser`,`idform`),
  KEY `iduser` (`iduser`),
  KEY `idform` (`idform`),
  KEY `id_index` (`id`),
  CONSTRAINT `fk_users_forms_1` FOREIGN KEY (`iduser`) REFERENCES `users` (`id`),
  CONSTRAINT `users_forms_ibfk_2` FOREIGN KEY (`idform`) REFERENCES `forms` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

TRUNCATE `users_forms`;
INSERT INTO `users_forms` (`id`, `iduser`, `idform`) VALUES
(1,	1,	1),
(2,	2,	1);

-- 2019-12-02 14:17:57
