PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS Запись;
DROP TABLE IF EXISTS Учащийся;

CREATE TABLE Учащийся (
-- Поля
`Фамилия`			VARCHAR(60)		NOT NULL,
`Имя`				VARCHAR(60)		NOT NULL,
`Отчество`		    VARCHAR(60),
`Тип документа`	  	VARCHAR(60)		NOT NULL,
`Номер документа`	VARCHAR(60)		NOT NULL,
`Пол`			    VARCHAR(3)		NOT NULL,
`Дата рождения`	  	DATE,

`Район школы`		VARCHAR(60),
`Школа`			    VARCHAR(60),
`Класс`			    VARCHAR(5),

`Родители`		    VARCHAR(200),
`Домашний адрес`	VARCHAR(200),
`Телефон`   		VARCHAR(100),
`e-mail`    		VARCHAR(100),

`С ослабленным здоровьем` 	BOOLEAN,
`Сирота`    				BOOLEAN,
`Инвалид`   				BOOLEAN,
`На учёте в полиции`		BOOLEAN,
`Многодетная семья`	      	BOOLEAN,
`Неполная семья`      		BOOLEAN,
`Малообеспеченная семья`  	BOOLEAN,
`Мигранты`    				BOOLEAN,

`Дата заявления`			DATE,

PRIMARY KEY (`Тип документа`, `Номер документа`)
);

CREATE TABLE Запись(
-- Поля
`Тип документа`	  	VARCHAR(60)		NOT NULL,
`Номер документа`	VARCHAR(60)		NOT NULL,
`Объединение`		VARCHAR(60)		NOT NULL,
-- Параметры
PRIMARY KEY (`Тип документа`, `Номер документа`, `Объединение`),
FOREIGN KEY (`Тип документа`, `Номер документа`)  REFERENCES Учащийся(`Тип документа`, `Номер документа`) ON DELETE CASCADE
);


