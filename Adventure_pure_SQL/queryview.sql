SELECT v.memberID AS theMemberID, v.role AS theRole, v.name AS theName, COUNT(*) AS numLosses
FROM FullBattlePointsView v, Battles b, Characters c
WHERE v.name IS NOT NULL
    AND v.memberID = c.memberID
    AND v.role = c.role
    AND v.memberID = b.characterMemberID
    AND v.role = b.characterRole
    AND b.monsterBattlePoints > b.characterBattlePoints
    AND c.wasDefeated = FALSE
GROUP BY v.memberID, v.role, v.name
HAVING COUNT(*) >= 3;



-- Delete statements!

DELETE FROM Battles
WHERE characterMemberID = 111
    AND characterRole = 'cleric'
    AND monsterID = 925;

DELETE FROM Battles
WHERE characterMemberID = 101
    AND characterRole = 'knight'
    AND monsterID = 944






-- the output of this query is 

--  thememberid | therole | thename  | numlosses 
-- -------------+---------+----------+-----------
--          101 | knight  | Lancelot |         4
--          101 | mage    | Jack     |         5
--          111 | cleric  | Patrick  |         3
-- (3 rows)

--  after the deletes the query results in

--  thememberid | therole | thename  | numlosses 
-- -------------+---------+----------+-----------
--          101 | knight  | Lancelot |         3
--          101 | mage    | Jack     |         5
-- (2 rows)