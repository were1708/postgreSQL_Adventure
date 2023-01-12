CREATE VIEW CharacterLosses AS
SELECT c.memberID, c.role, COUNT(*) AS numLosses
FROM Characters c, Battles b
WHERE b.characterMemberID = c.memberID AND
    b.characterRole = c.role AND
    b.characterBattlePoints < b.monsterBattlePoints -- character lost!
GROUP BY c.memberID, c.role;

CREATE VIEW MonsterLosses AS
SELECT m.monsterID, COUNT(*) AS numLosses
FROM Monsters m, Battles b
WHERE b.monsterID = m.monsterID AND
    b.characterBattlePoints > b.monsterBattlePoints -- monster lost!
GROUP BY m.monsterID;

CREATE VIEW CharacterNotDefeated AS -- going to act as a view for every character that has not lost!
SELECT c.memberID, c.role
FROM Characters c
WHERE NOT EXISTS (SELECT * 
                FROM CharacterLosses l
                WHERE c.memberID = l.memberID AND
                    c.role = l.role);

CREATE VIEW MonsterNotDefeated AS -- going to act as a view for every monster that has not lost!
SELECT m.monsterID
FROM Monsters m
WHERE NOT EXISTS (SELECT *
                FROM MonsterLosses l
                WHERE m.monsterID = l.monsterID);

CREATE VIEW PopularityView AS  -- give me all the thing kinds and their popularities!
SELECT t.thingKind, COUNT(*) AS Popularity
FROM Things t
WHERE t.ownerMemberID IS NOT NULL AND -- popularity is determeined by ownership!
    t.ownerRole IS NOT NULL
GROUP BY t.thingKind
