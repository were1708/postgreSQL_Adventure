-- UNIT TEST 1
INSERT INTO Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
    VALUES(101, 'knight', 100, 100, 300); -- no monster ID with ID 100! Should error!

-- UNIT TEST 2
INSERT INTO Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
    VALUES(44, 'rougue', 100, 944, 202); -- no MemberID in Characters table with 44 as the ID

-- UNIT TEST 3
INSERT INTO Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
    VALUES(12, 'sh', 3, 44, 'rougue', 3.24, 2); -- Should error, ownerMemberID not in Character table

-- GENERAL UNIT TESTS!

-- UNIT TEST 4
UPDATE Things
    SET cost = 20.24
    WHERE thingID = 6002; -- This should be a fine update

-- UNIT TEST 5
UPDATE Things
    SET cost = -21.23
    WHERE thingID = 6002; -- This should error! negative cost??

-- UNIT TEST 6
UPDATE Monsters
    SET monsterKind = 'ba'
    WHERE monsterID = 925; -- This should be legal since we're setting a high level monster to a giant

-- UNIT TEST 7
UPDATE Monsters
    SET monsterKind = 'mi'
     WHERE monsterID = 925; -- This is illegal since this high leveled monster won't be a ba or gi!

-- UNIT TEST 8
UPDATE Members
    SET expirationDate = NULL
    WHERE memberID = 111; -- this is okay since isCurrent is NULL so we can set expirationDate to NULL

-- UNIT TEST 9
UPDATE Members
    SET expirationDate = NULL
    WHERE memberID = 120; -- this should error since isCurrent is not NULL but expirationDate is!