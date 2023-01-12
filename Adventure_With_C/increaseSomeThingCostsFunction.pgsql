CREATE OR REPLACE FUNCTION
increaseSomeThingCostsFunction(maxTotalIncrease INTEGER)
RETURNS INTEGER AS $$


    DECLARE
        currentIncrease     INTEGER;
        currentThingID      INTEGER;
        currentPopularity   INTEGER;
    
    DECLARE increaseCursor CURSOR FOR 
        SELECT t.thingID, v.popularity
        FROM Things t, PopularityView v
        WHERE t.thingKind = v.thingKind
        ORDER BY v.popularity DESC;

    BEGIN
    -- Input Validation like in example
    IF maxTotalIncrease <= 0 THEN 
        RETURN -1;
        END IF;

    currentIncrease := 0; 

    OPEN increaseCursor;

    LOOP

        FETCH increaseCursor INTO currentThingID, currentPopularity;

        EXIT WHEN NOT FOUND OR currentIncrease >= maxTotalIncrease;

        IF currentPopularity >= 5 THEN 
            IF currentIncrease + 5 <= maxTotalIncrease THEN
                UPDATE Things SET cost = cost + 5 WHERE thingID = currentThingID;
                currentIncrease := currentIncrease + 5;
            END IF;

        ELSEIF currentPopularity = 4 THEN
            IF currentIncrease + 4 <= maxTotalIncrease THEN
                UPDATE Things SET cost = cost + 4 WHERE thingID = currentThingID;
                currentIncrease := currentIncrease + 4;
            END IF;
        ELSEIF currentPopularity = 3 THEN
            IF currentIncrease + 2 <= maxTotalIncrease THEN
                UPDATE Things SET cost = cost + 2 WHERE thingID = currentThingID;
                currentIncrease := currentIncrease + 2;
            END IF;
        ELSE
        END IF;

    END LOOP;
    CLOSE increaseCursor;

    RETURN currentIncrease;
    END

$$ LANGUAGE plpgsql;