-- Adminer 4.7.5 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

SET NAMES utf8mb4;

DROP DATABASE IF EXISTS `prot`;
CREATE DATABASE `prot` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `prot`;

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


DROP TABLE IF EXISTS `forms`;
CREATE TABLE `forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `json` json NOT NULL,
  `name` varchar(360) NOT NULL,
  `developer` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `developer` (`developer`),
  CONSTRAINT `forms_ibfk_1` FOREIGN KEY (`developer`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `schedulers`;
CREATE TABLE `schedulers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`id`,`user`),
  KEY `user` (`user`),
  KEY `id` (`id`),
  CONSTRAINT `schedulers_ibfk_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `taskers`;
CREATE TABLE `taskers` (
  `idtasker` int(11) NOT NULL AUTO_INCREMENT,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`idtasker`),
  UNIQUE KEY `idtaskers_UNIQUE` (`idtasker`),
  KEY `fk_taskers_1_idx` (`user`),
  CONSTRAINT `taskers_ibfk_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks`;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(720) NOT NULL,
  `group` varchar(360) NOT NULL,
  `json` json NOT NULL,
  `start` timestamp NULL DEFAULT NULL,
  `end` timestamp NULL DEFAULT NULL,
  `confirmed_done` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `group` (`group`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_schedulers`;
CREATE TABLE `tasks_schedulers` (
  `idtask` int(11) NOT NULL,
  `idscheduler` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idscheduler`),
  KEY `idtask_idx` (`idtask`),
  KEY `tasks_schedulers_ibfk_2` (`idscheduler`),
  CONSTRAINT `tasks_schedulers_ibfk_3` FOREIGN KEY (`idscheduler`) REFERENCES `schedulers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tasks_schedulers_ibfk_4` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_taskers`;
CREATE TABLE `tasks_taskers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idtask` int(11) NOT NULL,
  `idtasker` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index2` (`id`),
  KEY `fk_tasks_taskers_1_idx` (`idtasker`),
  KEY `fk_tasks_taskers_2_idx` (`idtask`),
  CONSTRAINT `fk_tasks_taskers_1` FOREIGN KEY (`idtasker`) REFERENCES `taskers` (`idtasker`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tasks_taskers_ibfk_1` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(360) NOT NULL,
  `json` json DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


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
  CONSTRAINT `users_forms_ibfk_3` FOREIGN KEY (`iduser`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `users_forms_ibfk_4` FOREIGN KEY (`idform`) REFERENCES `forms` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


-- 2019-12-06 09:12:00
