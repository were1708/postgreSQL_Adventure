DROP SCHEMA lab3 CASCADE;
CREATE SCHEMA lab3;
 

-- Members(memberID, name, address, joinDate, expirationDate, isCurrent)  
CREATE TABLE Members (
	memberID INT, 
	name VARCHAR(50), 
	address VARCHAR(50), 
	joinDate DATE, 
	expirationDate DATE, 
	isCurrent boolean,
	PRIMARY KEY (memberID)
); 


-- Rooms(roomID, roomDescription, northNext, eastNext, southNext, westNext)
CREATE TABLE Rooms (
	roomID INT, 
	roomDescription VARCHAR(30), 
	northNext INT, 
	eastNext INT, 
	southNext INT, 
	westNext INT,
	PRIMARY KEY(roomID),
	FOREIGN KEY (northNext) REFERENCES Rooms(roomID),
	FOREIGN KEY (eastNext) REFERENCES Rooms(roomID),
	FOREIGN KEY (southNext) REFERENCES Rooms(roomID),
	FOREIGN KEY (westNext) REFERENCES Rooms(roomID)
); 


-- Roles(role, battlePoints, InitialMoney)
CREATE TABLE Roles (
	role VARCHAR(6), 
	battlePoints INT, 
	initialMoney NUMERIC(5,2),
	PRIMARY KEY (role)
); 


-- Characters(memberID, role, name, roomID, currentMoney, wasDefeated)
CREATE TABLE Characters (
	memberID INT, 
	role VARCHAR(6), 
	name VARCHAR(50), 
	roomID INT, 
	currentMoney NUMERIC(5,2), 
	wasDefeated boolean,
	PRIMARY KEY (memberID, role),
	FOREIGN KEY (memberID) REFERENCES Members,
	FOREIGN KEY (role) REFERENCES Roles,
	FOREIGN KEY (roomID) REFERENCES Rooms
); 


-- Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
CREATE TABLE Things (
	thingID INT, 
	thingKind CHAR(2), 
	initialRoomID INT, 
	ownerMemberID INT, 
	ownerRole VARCHAR(6), 
	cost NUMERIC(4,2), 
	extraBattlePoints INT,
	PRIMARY KEY (thingID),
	FOREIGN KEY (initialRoomID) REFERENCES Rooms(roomID)
); 


-- Monsters(monsterID, monsterKind, name, battlePoints, roomID, wasDefeated)
CREATE TABLE Monsters (
	monsterID INT, 
	monsterKind CHAR(2), 
	name VARCHAR(50), 
	battlePoints INT, 
	roomID INT, 
	wasDefeated boolean,
	PRIMARY KEY (monsterID),
	FOREIGN KEY (roomID) REFERENCES Rooms
); 


-- Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
CREATE TABLE Battles (
	characterMemberID INT, 
	characterRole VARCHAR(6), 
	characterBattlePoints INT, 
         monsterID INT, 
	monsterBattlePoints INT,
	PRIMARY KEY (characterMemberID, characterRole, monsterID)
); 


-- ModifyMembers(memberID, name, address, expirationDate)  
CREATE TABLE ModifyMembers (
	memberID INT, 
	name VARCHAR(50), 
	address VARCHAR(50), 
	expirationDate DATE, 
	PRIMARY KEY (memberID)
); 
