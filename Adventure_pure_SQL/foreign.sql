ALTER TABLE Battles 
	ADD CONSTRAINT BattleMonsters FOREIGN KEY(monsterID) 
	REFERENCES Monsters
	ON UPDATE CASCADE;

ALTER TABLE Battles
	ADD CONSTRAINT BattleChars FOREIGN KEY(characterMemberID, characterRole)
	REFERENCES Characters(memberID, role)
	ON DELETE CASCADE;

ALTER TABLE Things
	ADD CONSTRAINT CharOwner FOREIGN KEY(ownerMemberID, ownerRole)
	REFERENCES Characters(memberID, role)
	ON DELETE SET NULL
	ON UPDATE CASCADE;