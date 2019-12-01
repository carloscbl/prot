CREATE DATABASE  IF NOT EXISTS `test_prot` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `test_prot`;
-- MySQL dump 10.13  Distrib 8.0.18, for Linux (x86_64)
--
-- Host: localhost    Database: test_prot
-- ------------------------------------------------------
-- Server version	8.0.18

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `forms`
--

DROP TABLE IF EXISTS `forms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `forms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `json` json NOT NULL,
  `name` varchar(360) NOT NULL,
  `developer` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `developer` (`developer`),
  CONSTRAINT `fk_forms_1` FOREIGN KEY (`developer`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `forms`
--

LOCK TABLES `forms` WRITE;
/*!40000 ALTER TABLE `forms` DISABLE KEYS */;
INSERT INTO `forms` VALUES (1,'{\"form\": {\"version\": 0, \"form.name\": \"Washer easer\", \"author.name\": \"carlos bernal\"}, \"diss_test\": 1, \"questions\": [{\"id\": 1, \"mode\": [\"sequential\", \"once\"], \"question\": \"Do you have washer?\", \"answer_tag\": \"washer\", \"type_user_input\": \"STRING\", \"answers_branches\": {\"predefined_boolean_yes_no_affirmative_yes\": {\"else\": -5, \"true\": 72, \"taskstory_id\": \"NONE\"}}}, {\"id\": 72, \"mode\": [\"sequential\", \"if_error_repeat\", \"ignorecase\"], \"question\": \"How many Kilograms have your washer capacity?\", \"answer_tag\": \"kg_capacity\", \"taskstories\": {\"normal_user\": [{\"tag\": \"base_task\", \"name\": \"step 1\", \"comment\": \"The user is implicit\", \"duration\": {\"hours\": 1, \"minutes\": 5, \"seconds\": 3}, \"frequency\": \"weekly\", \"description\": \"Gather Clothes around home\", \"restrictions\": [\"early\", \"soon\", \"launch\"]}, {\"tag\": \"washer_start\", \"name\": \"step 2\", \"duration\": {\"minutes\": 2}, \"frequency\": \"weekly\", \"description\": \"Put them inside and start the machine\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_intermediate\", \"name\": \"step 3\", \"duration\": {\"hours\": 1, \"minutes\": 59, \"seconds\": 59}, \"frequency\": \"weekly\", \"description\": \"Put them inside and start the machine\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_end\", \"name\": \"step 4\", \"when\": {\"after\": \"washer_start\", \"maximum_delay\": {\"hours\": 2}, \"minimum_delay\": {\"hours\": 1, \"minutes\": 15}}, \"duration\": {\"minutes\": 15}, \"frequency\": \"weekly\", \"description\": \"Collect and hang out the washing\", \"restrictions\": [\"night\"]}, {\"tag\": \"washer_clean_up\", \"name\": \"step 5\", \"when\": {\"after\": \"washer_end\", \"maximum_delay\": {\"days\": 1, \"hours\": 22}, \"minimum_delay\": {\"days\": 1, \"hours\": 1, \"minutes\": 15}}, \"duration\": {\"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"Collect and hang out the washing\", \"restrictions\": [\"night\"]}], \"industrial_user\": [{\"tag\": \"industrial_start\", \"name\": \"step 1\", \"duration\": {\"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"start the machinery\", \"restrictions\": [\"night\"]}, {\"tag\": \"industrial_middle\", \"name\": \"step 2\", \"duration\": {\"hours\": 7, \"minutes\": 32}, \"frequency\": \"some-days\", \"description\": \"start the machinery\", \"restrictions\": [\"soon\", \"early\", \"launch\", \"day\"]}]}, \"type_user_input\": \"INTEGER\", \"answers_branches\": {\"ranges\": [{\"range\": {\"values\": {\"<\": 1000, \">\": 30}, \"if_branch\": 3, \"taskstory_id\": \"industrial_user\"}}, {\"range\": {\"values\": {\"<\": 30, \">\": 0}, \"if_branch\": -1, \"taskstory_id\": \"normal_user\"}}, {\"range\": {\"values\": {\"<\": 0, \">\": 1000}, \"if_branch\": -4, \"taskstory_id\": 3}}]}}, {\"id\": 3, \"mode\": [\"sequential\", \"once\"], \"question\": \"Your washer is a industrial one?\", \"answer_tag\": \"industrial\", \"type_user_input\": \"STRING\", \"answers_branches\": {\"custom\": {\"no\": {\"if_branch\": 0}, \"yes\": {\"if_branch\": -3}, \"else\": {\"if_branch\": -1}}}}]}','Washer easer',1);
/*!40000 ALTER TABLE `forms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `schedulers`
--

DROP TABLE IF EXISTS `schedulers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `schedulers` (
  `idscheduler` int(11) NOT NULL,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`idscheduler`),
  UNIQUE KEY `idscheduler_UNIQUE` (`idscheduler`),
  KEY `fk_schedulers_1_idx` (`user`),
  CONSTRAINT `fk_schedulers_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `schedulers`
--

LOCK TABLES `schedulers` WRITE;
/*!40000 ALTER TABLE `schedulers` DISABLE KEYS */;
/*!40000 ALTER TABLE `schedulers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taskers`
--

DROP TABLE IF EXISTS `taskers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `taskers` (
  `idtasker` int(11) NOT NULL,
  `user` int(11) NOT NULL,
  PRIMARY KEY (`idtasker`),
  UNIQUE KEY `idtaskers_UNIQUE` (`idtasker`),
  KEY `fk_taskers_1_idx` (`user`),
  CONSTRAINT `fk_taskers_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taskers`
--

LOCK TABLES `taskers` WRITE;
/*!40000 ALTER TABLE `taskers` DISABLE KEYS */;
/*!40000 ALTER TABLE `taskers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tasks`
--

DROP TABLE IF EXISTS `tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL,
  `json` json NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks`
--

LOCK TABLES `tasks` WRITE;
/*!40000 ALTER TABLE `tasks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tasks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tasks_schedulers`
--

DROP TABLE IF EXISTS `tasks_schedulers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tasks_schedulers` (
  `idtask` int(11) NOT NULL,
  `idscheduler` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idscheduler`),
  KEY `idscheduler_idx` (`idscheduler`),
  CONSTRAINT `idscheduler` FOREIGN KEY (`idscheduler`) REFERENCES `schedulers` (`idscheduler`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtask_sche` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks_schedulers`
--

LOCK TABLES `tasks_schedulers` WRITE;
/*!40000 ALTER TABLE `tasks_schedulers` DISABLE KEYS */;
/*!40000 ALTER TABLE `tasks_schedulers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tasks_taskers`
--

DROP TABLE IF EXISTS `tasks_taskers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tasks_taskers` (
  `idtask` int(11) NOT NULL,
  `idtasker` int(11) NOT NULL,
  PRIMARY KEY (`idtask`,`idtasker`),
  KEY `idtasker_idx` (`idtasker`),
  CONSTRAINT `idtask` FOREIGN KEY (`idtask`) REFERENCES `tasks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `idtasker` FOREIGN KEY (`idtasker`) REFERENCES `taskers` (`idtasker`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks_taskers`
--

LOCK TABLES `tasks_taskers` WRITE;
/*!40000 ALTER TABLE `tasks_taskers` DISABLE KEYS */;
/*!40000 ALTER TABLE `tasks_taskers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(360) NOT NULL,
  `json` json DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'carloscbl','{\"username\": \"carloscbl\"}'),(2,'pepe','{}');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users_forms`
--

DROP TABLE IF EXISTS `users_forms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users_forms` (
  `iduser` int(11) NOT NULL,
  `idform` int(11) NOT NULL,
  PRIMARY KEY (`iduser`,`idform`),
  KEY `iduser` (`iduser`),
  KEY `idform` (`idform`),
  CONSTRAINT `fk_users_forms_1` FOREIGN KEY (`iduser`) REFERENCES `users` (`id`),
  CONSTRAINT `users_forms_ibfk_2` FOREIGN KEY (`idform`) REFERENCES `forms` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users_forms`
--

LOCK TABLES `users_forms` WRITE;
/*!40000 ALTER TABLE `users_forms` DISABLE KEYS */;
INSERT INTO `users_forms` VALUES (1,1),(2,1);
/*!40000 ALTER TABLE `users_forms` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-12-01 16:03:51
