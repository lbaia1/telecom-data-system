#ifndef TELECOM_H
#define TELECOM_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_ID_LEN 20
#define MAX_BUFFER_LEN 100

/* LogEntry struct to hold raw telecom data */
typedef struct {
    char userID[MAX_ID_LEN];  /* Format: "UID-XXXX" */
    int duration;             /* In seconds */
    float dataUsage;          /* In Megabytes */
    int towerID;              /* Integer identifier */
    int successCode;          /* 1 = Success, 0 = Failed */
} LogEntry;

/* Singly Linked List Node */
typedef struct LogNode {
    LogEntry data;
    struct LogNode *next;
} LogNode;

/* Returns a dynamically allocated array of LogEntry from the CSV data in filename. */
LogEntry* loadFromCSV(char *filename, int *count);

/* Writes the entire dynamic array in logs to a binary file using a single fwrite call. */
void convertToBinary(char *binFilename, LogEntry *logs, int count);

/* Safely releases all dynamically allocated memory on the heap and sets pointer to NULL. */
void freeArchive(LogEntry **logs);

/* Validates if the file is a valid binary archive (returns 1 if valid, 0 if invalid). */
int validateBinaryArchive(char *binFilename);

/* Loads data from the binary file into a dynamically allocated array of LogEntry. */
LogEntry* restoreFromBinary(char *binFilename, int *count);

/* Recursively sums the dataUsage of all LogEntry elements in logs. */
float recursiveTotalUsage(LogEntry *logs, int recordCount);

/* Recursively returns the largest dataUsage value in non-empty logs. */
float recursiveMaxUsage(LogEntry *logs, int recordCount);

/* Reads all records in the binary file into a dynamically allocated linked list. */
LogNode* buildLogListFromBinary(char *binFilename);

/* Counts the number of dropped calls on the specified tower (successCode == 0). */
int countDroppedCalls(LogNode *head, int towerID);

/* Removes all LogNodes belonging to the target user ID and frees their memory. */
void removeUserLogs(LogNode **head, char *targetUID);

/* Frees all dynamically allocated memory associated with the linked list. */
void destroyLogList(LogNode **head);

#endif