ALTER TABLE Things
	ADD CONSTRAINT positiveCost 
	CHECK(cost > 0);

ALTER TABLE Monsters
	ADD CONSTRAINT majorMonsters
		CHECK((battlePoints >= 40 AND (monsterKind = 'gi' OR monsterKind = 'ba')) OR battlePoints < 40);

	
ALTER TABLE Members
	ADD CONSTRAINT expirationCurrent
		CHECK((expirationDate IS NULL AND isCurrent IS NULL) OR expirationDate IS NOT NULL);