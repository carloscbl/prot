-- Adminer 4.7.6 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

USE `prot`;

SET NAMES utf8mb4;

DROP TABLE IF EXISTS `form_sessions`;
CREATE TABLE `form_sessions` (
  `id` int NOT NULL AUTO_INCREMENT,
  `unq_name` varchar(720) NOT NULL,
  `json` json NOT NULL,
  `user_forms` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `unq_name` (`unq_name`),
  KEY `user_forms` (`user_forms`),
  CONSTRAINT `form_sessions_ibfk_2` FOREIGN KEY (`user_forms`) REFERENCES `users_forms` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `forms`;
CREATE TABLE `forms` (
  `id` int NOT NULL AUTO_INCREMENT,
  `json` json NOT NULL,
  `name` varchar(360) NOT NULL,
  `developer` int NOT NULL,
  `id_public` tinyint(1) NOT NULL DEFAULT '1',
  `disabled` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `developer` (`developer`),
  CONSTRAINT `forms_ibfk_1` FOREIGN KEY (`developer`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `schedulers`;
CREATE TABLE `schedulers` (
  `id` int NOT NULL AUTO_INCREMENT,
  `user` int NOT NULL,
  PRIMARY KEY (`id`,`user`),
  KEY `user` (`user`),
  KEY `id` (`id`),
  CONSTRAINT `schedulers_ibfk_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `taskers`;
CREATE TABLE `taskers` (
  `idtasker` int NOT NULL AUTO_INCREMENT,
  `user` int NOT NULL,
  PRIMARY KEY (`idtasker`),
  UNIQUE KEY `idtaskers_UNIQUE` (`idtasker`),
  KEY `fk_taskers_1_idx` (`user`),
  CONSTRAINT `taskers_ibfk_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks`;
CREATE TABLE `tasks` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(720) NOT NULL,
  `group` varchar(360) NOT NULL,
  `json` json NOT NULL,
  `start` timestamp NULL DEFAULT NULL,
  `end` timestamp NULL DEFAULT NULL,
  `confirmed_done` tinyint(1) DEFAULT NULL,
  `from_user_forms_id` int DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`),
  KEY `group` (`group`),
  KEY `id` (`id`),
  KEY `from_user_forms_id` (`from_user_forms_id`),
  CONSTRAINT `tasks_ibfk_2` FOREIGN KEY (`from_user_forms_id`) REFERENCES `users_forms` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_schedulers`;
CREATE TABLE `tasks_schedulers` (
  `idtask` int NOT NULL,
  `idscheduler` int NOT NULL,
  PRIMARY KEY (`idtask`,`idscheduler`),
  KEY `idtask_idx` (`idtask`),
  KEY `tasks_schedulers_ibfk_2` (`idscheduler`),
  CONSTRAINT `tasks_schedulers_ibfk_3` FOREIGN KEY (`idscheduler`) REFERENCES `schedulers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tasks_schedulers_ibfk_4` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `tasks_taskers`;
CREATE TABLE `tasks_taskers` (
  `id` int NOT NULL AUTO_INCREMENT,
  `idtask` int NOT NULL,
  `idtasker` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index2` (`id`),
  KEY `fk_tasks_taskers_1_idx` (`idtasker`),
  KEY `fk_tasks_taskers_2_idx` (`idtask`),
  CONSTRAINT `fk_tasks_taskers_1` FOREIGN KEY (`idtasker`) REFERENCES `taskers` (`idtasker`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tasks_taskers_ibfk_1` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(360) NOT NULL,
  `json` json DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


DROP TABLE IF EXISTS `users_forms`;
CREATE TABLE `users_forms` (
  `id` int NOT NULL AUTO_INCREMENT,
  `iduser` int NOT NULL,
  `idform` int NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `user_form` (`iduser`,`idform`),
  KEY `iduser` (`iduser`),
  KEY `idform` (`idform`),
  KEY `id_index` (`id`),
  CONSTRAINT `users_forms_ibfk_3` FOREIGN KEY (`iduser`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `users_forms_ibfk_5` FOREIGN KEY (`idform`) REFERENCES `forms` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


-- 2020-02-16 16:41:15
