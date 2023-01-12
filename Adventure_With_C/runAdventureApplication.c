/**
 * runAdventureApplication skeleton, to be modified by students
 */

#include "libpq-fe.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value
 * this big */
#define MAXNUMBERSTRINGSIZE 20

/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn) {
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn) {
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 */

/* Function: printNumberOfThingsInRoom:
 * -------------------------------------
 * Parameters:  connection, and theRoomID, which should be the roomID of a room.
 * Prints theRoomID, and number of things in that room.
 * Return 0 if normal execution, -1 if no such room.
 * bad_exit if SQL statement execution fails.
 */

int printNumberOfThingsInRoom(PGconn *conn, int theRoomID) {
    PGresult *start = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    if (PQresultStatus(start) != PGRES_COMMAND_OK) {printf("Start failed\n");}
    char roomID[MAXNUMBERSTRINGSIZE];
        // number long value
    snprintf(roomID, MAXNUMBERSTRINGSIZE, "%d",theRoomID); // theRoomID now as a string!
    char initialQuery[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT * FROM Rooms WHERE RoomID = ";
    strcat(initialQuery, roomID);
    PGresult *res = PQexec(conn, initialQuery);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {printf("first room query failed\n");}
    int roomValid = PQntuples(res);
    PQclear(res);
    if (roomValid <= 0) {
        PGresult *finish = PQexec(conn, "COMMIT TRANSACTION");
        PQclear(start);
        PQclear(finish);
        return -1; // no room exists with RoomID = theRoomID
    } else { // there is a room!
        
        int thingsInRoom = 0; // to hold how many things are in the room!
        char roomCheckQuery[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT * FROM Things WHERE ownerMemberID IS NULL AND initialRoomID = ";
        strcat(roomCheckQuery, roomID);
        PGresult *checkRoom = PQexec(conn, roomCheckQuery); // execute ROOM query
        thingsInRoom += PQntuples(checkRoom); // add things that are in RoomID and not owned
        if (PQresultStatus(checkRoom) != PGRES_TUPLES_OK) {printf("%d\n", PQresultStatus(checkRoom));}
        PQclear(checkRoom);
        char characterCheckQuery[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT * FROM Characters c, Things t WHERE c.memberID = t.ownerMemberID AND c.role = t.ownerRole AND c.roomID = ";
        strcat(characterCheckQuery, roomID);
        PGresult *checkCharacter = PQexec(conn, characterCheckQuery); // execute CHARACTER query
        thingsInRoom += PQntuples(checkCharacter);
        if (PQresultStatus(checkCharacter) != PGRES_TUPLES_OK) {printf("character failed\n");}
        PQclear(checkCharacter);
        char thingsInRoomString[MAXNUMBERSTRINGSIZE];
        snprintf(thingsInRoomString, MAXNUMBERSTRINGSIZE, "%d", thingsInRoom);
        char descriptionQuery[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT roomDescription FROM Rooms WHERE roomID = ";
        strcat(descriptionQuery, roomID);
        PGresult *getDescription = PQexec(conn, descriptionQuery);
        if (PQresultStatus(getDescription) != PGRES_TUPLES_OK) {printf("description failed\n");}
        char printString[MAXSQLSTATEMENTSTRINGSIZE] = "Room ";
        strcat(printString, roomID);
        strcat(printString, ", ");
        strcat(printString, PQgetvalue(getDescription, 0, 0)); // gets the room description
        strcat(printString, ", ");
        strcat(printString, "has ");
        strcat(printString, thingsInRoomString);
        strcat(printString, " in it.\n");
        printf(printString); // prints formatted string output
        PGresult *finish = PQexec(conn, "COMMIT TRANSACTION");
        if (PQresultStatus(finish) != PGRES_COMMAND_OK) {printf("finish failed\n");}
        PQclear(getDescription);
        PQclear(start);
        PQclear(finish);
        return 0;
    }
}

/* Function: updateWasDefeated:
 * ----------------------------
 * Parameters:  connection, and a string, doCharactersOrMonsters, which should
 * be 'C' or 'M'. Updates the wasDefeated value of either characters or monsters
 * (depending on value of doCharactersOrMonsters) if that value is not correct.
 * Returns number of characters or monsters whose wasDefeated value was updated,
 * or -1 if doCharactersOrMonsters value is invalid.
 */

int updateWasDefeated(PGconn *conn, char *doCharactersOrMonsters) {
    int updated = 0;
    if (doCharactersOrMonsters[0] != 'M' && doCharactersOrMonsters[0] != 'C') {
        return -1;
    } else if (doCharactersOrMonsters[0] == 'M') { // Monsters!
        PGresult *start = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
        if (PQresultStatus(start) != PGRES_COMMAND_OK) {printf("Start failed\n");}
        char case_one_update[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters m SET wasDefeated = true WHERE EXISTS ( SELECT * FROM MonsterLosses l WHERE l.monsterID = m.monsterID ) AND (m.wasDefeated IS false OR m.wasDefeated IS NULL)";
        char case_two_update[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters m SET wasDefeated = false WHERE EXISTS ( SELECT * FROM MonsterNotDefeated w WHERE w.monsterID = m.monsterID ) AND (m.wasDefeated IS true OR m.wasDefeated IS NULL)";
        PGresult *case_one_res = PQexec(conn, case_one_update);
        PGresult *case_two_res = PQexec(conn, case_two_update);
        int updates_one = atoi(PQcmdTuples(case_one_res));
        int updates_two = atoi(PQcmdTuples(case_two_res));
        if (PQresultStatus(case_one_res) != PGRES_COMMAND_OK) {printf("Update 1 failed\n");}
        if (PQresultStatus(case_two_res) != PGRES_COMMAND_OK) {printf("Update 2 failed\n");}
        PQclear(case_one_res);
        PQclear(case_two_res);
        PGresult *finish = PQexec(conn, "COMMIT TRANSACTION");
        if (PQresultStatus(finish) != PGRES_COMMAND_OK) {printf("finish failed\n");}
        PQclear(start);
        PQclear(finish);
        return (updates_one + updates_two);

    } else { // Characters
        PGresult *start = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
        char case_one_update[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters c SET wasDefeated = true WHERE EXISTS ( SELECT * FROM CharacterLosses l WHERE l.memberID = c.memberID AND l.role = c.role ) AND (c.wasDefeated IS false OR c.wasDefeated IS NULL)";
        char case_two_update[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters c SET wasDefeated = false WHERE EXISTS ( SELECT * FROM CharacterNotDefeated w WHERE w.memberID = c.memberID AND w.role = c.role ) AND (c.wasDefeated IS true OR c.wasDefeated IS NULL)";
        PGresult *case_one_res = PQexec(conn, case_one_update);
        PGresult *case_two_res = PQexec(conn, case_two_update);
        int updates_one = atoi(PQcmdTuples(case_one_res));
        int updates_two = atoi(PQcmdTuples(case_two_res));
        if (PQresultStatus(case_one_res) != PGRES_COMMAND_OK) {printf("Update 1 failed\n");}
        if (PQresultStatus(case_two_res) != PGRES_COMMAND_OK) {printf("Update 2 failed\n");}
        PQclear(case_one_res);
        PQclear(case_two_res);
        PGresult *finish = PQexec(conn, "COMMIT TRANSACTION");
        if (PQresultStatus(finish) != PGRES_COMMAND_OK) {printf("finish failed\n");}
        PQclear(start);
        PQclear(finish);
        return (updates_one + updates_two);
    }
}

/* Function: increaseSomeThingCosts:
 * -------------------------------
 * Parameters:  connection, and an integer maxTotalIncrease, the maximum total
 * increase that it should make in the cost of some things. Executes by invoking
 * a Stored Function, increaseSomeThingCostsFunction, which returns a negative
 * value if there is an error, and otherwise returns the total cost increase
 * that was performed by the Stored Function.
 */

int increaseSomeThingCosts(PGconn *conn, int maxTotalIncrease) {
    char funcCall[MAXSQLSTATEMENTSTRINGSIZE];
    snprintf(funcCall, MAXSQLSTATEMENTSTRINGSIZE, "SELECT increaseSomeThingCostsFunction(%d)", maxTotalIncrease);
    PGresult *result = PQexec(conn, funcCall);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {printf("increaseSomeThingCosts failed\n");}
    int totalIncreased = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return totalIncreased;
}

int main(int argc, char **argv) {
    PGconn *conn;
    int theResult;

    if (argc != 3) {
        fprintf(stderr, "Usage: ./runAdventureApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        bad_exit(conn);
    }

    int result;

    /* Perform the calls to printNumberOfThingsInRoom listed in Section 6 of Lab4,
   * printing error message if there's an error.
   */
    int test1 = printNumberOfThingsInRoom(conn, 1);
    if (test1 == -1) {
        fprintf(stderr, "no room exists whose id is %d\n", 1);
    }
    else if (test1 != -1 && test1 != 0) {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    int test2 = printNumberOfThingsInRoom(conn, 2);
    if (test2 == -1) {
        fprintf(stderr, "no room exists whose id is %d\n", 2);
    }
    else if (test2 != -1 && test2 != 0) {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    int test3 = printNumberOfThingsInRoom(conn, 3);
    if (test3 == -1) {
        fprintf(stderr, "no room exists whose id is %d\n", 3);
    }
    else if (test3 != -1 && test3 != 0) {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }
    
    int test4 = printNumberOfThingsInRoom(conn, 7);
    if (test4 == -1) {
        fprintf(stderr, "no room exists whose id is %d\n", 7);
    }
    else if (test4 != -1 && test4 != 0) {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    /* Extra newline for readability */
    printf("\n");

    /* Perform the calls to updateWasDefeated listed in Section 6 of Lab4,
   * and print messages as described.
   */
    char *c = "C";
    char *m = "M";

    int test5 = updateWasDefeated(conn, c);
    if (test5 == -1) {
        fprintf(stderr, "Illegal value for doCharactersOrMonsters %s\n", c);
    }
    else if (test5 >= 0) {
        printf("%d wasDefeated values were fixed for %s\n", test5, c);
    }
    else {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    int test6 = updateWasDefeated(conn, m);
    if (test6 == -1) {
        fprintf(stderr, "Illegal value for doCharactersOrMonsters %s\n", m);
    }
    else if (test6 >= 0) {
        printf("%d wasDefeated values were fixed for %s\n", test6, m);
    }
    else {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    int test7 = updateWasDefeated(conn, c);
    if (test7 == -1) {
        fprintf(stderr, "Illegal value for doCharactersOrMonsters %s\n", c);
    }
    else if (test7 >= 0) {
        printf("%d wasDefeated values were fixed for %s\n", test7, c);
    }
    else {
        fprintf(stderr, "Something stranged happened, closing connection\n");
        bad_exit(conn);
    }

    /* Extra newline for readability */
    printf("\n");

    /* Perform the calls to increaseSomeThingCosts listed in Section 6 of Lab4,
   * and print messages as described.
   */

    int test8 = increaseSomeThingCosts(conn, 12);
    if (test8 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 12, test8);
    }

    int test9 = increaseSomeThingCosts(conn, 500);
    if (test9 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 500, test9);
    }

    int test10 = increaseSomeThingCosts(conn, 39);
    if (test10 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 39, test10);
    }

    int test11 = increaseSomeThingCosts(conn, 1);
    if (test11 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 1, test11);
    }

    int test12 = increaseSomeThingCosts(conn, 2);
    if (test12 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 2, test12);
    }

    int test13 = increaseSomeThingCosts(conn, 3);
    if (test13 < 0) {
        fprintf(stderr, "Illegal value for increaseSomeThingsCosts\n");
        bad_exit(conn);
    }
    else {
        printf("Total increase for maxTotalIncrease %d is %d\n", 3, test13);
    }

    good_exit(conn);
    return 0;
}
