PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS Учащийся;

CREATE TABLE Учащийся (
-- Поля
"ID"      			INTEGER			PRIMARY KEY			AUTOINCREMENT,
"Фамилия"			VARCHAR(60)		NOT NULL,
"Имя"				VARCHAR(60)		NOT NULL,
"Отчество"		    VARCHAR(60),
"Тип документа"	  	VARCHAR(60)		NOT NULL,
"Номер документа"	VARCHAR(60)		NOT NULL,
"Пол"			    VARCHAR(3)		NOT NULL,
"Год рождения"	  	DATE,

"Район школы"		  VARCHAR(60),
"Школа"			      VARCHAR(60),
"Класс"			      VARCHAR(5),

"Родители"		    VARCHAR(200),
"Домашний адрес"	VARCHAR(200),
"Телефон"   			VARCHAR(100),
"e-mail"     			VARCHAR(100)

);


