-- Adminer 4.7.4 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

SET NAMES utf8mb4;

DROP DATABASE IF EXISTS `test_prot`;
CREATE DATABASE `test_prot` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `test_prot`;

DROP TABLE IF EXISTS `forms`;
CREATE TABLE `forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `form_json` json NOT NULL,
  `name` varchar(360) NOT NULL,
  `developer` varchar(360) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`),
  KEY `developer` (`developer`),
  CONSTRAINT `forms_ibfk_1` FOREIGN KEY (`developer`) REFERENCES `users` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `schedulers`;
CREATE TABLE `schedulers` (
  `idscheduler` int(11) NOT NULL,
  PRIMARY KEY (`idscheduler`),
  UNIQUE KEY `idscheduler_UNIQUE` (`idscheduler`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `taskers`;
CREATE TABLE `taskers` (
  `idtasker` int(11) NOT NULL,
  `users` varchar(360) NOT NULL,
  PRIMARY KEY (`idtasker`),
  UNIQUE KEY `idtaskers_UNIQUE` (`idtasker`),
  UNIQUE KEY `users_UNIQUE` (`users`),
  CONSTRAINT `user_name` FOREIGN KEY (`users`) REFERENCES `users` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks`;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL,
  `name` varchar(45) DEFAULT NULL,
  `description` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_schedulers`;
CREATE TABLE `tasks_schedulers` (
  `idtask` int(11) NOT NULL,
  `idscheduler` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idscheduler`),
  KEY `idscheduler_idx` (`idscheduler`),
  CONSTRAINT `idscheduler` FOREIGN KEY (`idscheduler`) REFERENCES `schedulers` (`idscheduler`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtask_sche` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_taskers`;
CREATE TABLE `tasks_taskers` (
  `idtask` int(11) NOT NULL,
  `idtasker` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idtasker`),
  KEY `idtasker_idx` (`idtasker`),
  CONSTRAINT `idtask` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtasker` FOREIGN KEY (`idtasker`) REFERENCES `taskers` (`idtasker`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `name` varchar(360) NOT NULL,
  PRIMARY KEY (`name`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `users_forms`;
CREATE TABLE `users_forms` (
  `iduser` varchar(360) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `idform` int(11) NOT NULL,
  KEY `iduser` (`iduser`),
  KEY `idform` (`idform`),
  CONSTRAINT `users_forms_ibfk_1` FOREIGN KEY (`iduser`) REFERENCES `users` (`name`) ON DELETE CASCADE,
  CONSTRAINT `users_forms_ibfk_2` FOREIGN KEY (`idform`) REFERENCES `forms` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


-- 2019-11-19 13:53:34
