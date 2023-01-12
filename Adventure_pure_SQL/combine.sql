BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    
    INSERT INTO Members(memberID, name, address, expirationDate, joinDate)
    SELECT mm.memberID, mm.name, mm.address, mm.expirationDate, CURRENT_DATE
    FROM ModifyMembers mm
    WHERE mm.memberID NOT IN 
    (SELECT m.memberID
        FROM Members m); 


    UPDATE Members m
    SET name = mm.name, address = mm.address, expirationDate = mm.expirationDate, isCurrent = TRUE
    FROM ModifyMembers mm
    WHERE m.memberID = mm.memberID;
    

COMMIT;