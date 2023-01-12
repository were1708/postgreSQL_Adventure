-- DROP VIEW FullBattlePointsView; -- USED FOR TESTING!

CREATE VIEW FullBattlePointsView AS
SELECT c.memberID, c.role, c.name, (SELECT SUM(extraBattlePoints)
                                                        FROM Things t 
                                                        WHERE c.memberID = t.ownerMemberID
                                                            AND c.role = t.ownerRole) + r.battlePoints AS fullbattlepoints
FROM Characters c, Roles r
WHERE (SELECT COUNT(*)
            FROM Things t2
            WHERE c.memberID = t2.ownerMemberID
                AND c.role = t2.ownerRole) > 0
            AND c.role = r.role;
    