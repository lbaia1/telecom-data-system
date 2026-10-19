/* Linked list construction, traversal, and memory management for LogEntry records. */

#include "telecom.h"
#include <string.h>

/* Recursively reads LogEntry records from an open binary file and builds a linked list. Returns the head node. */
static LogNode* readListRecursive(FILE *fp) {
    LogEntry temp;
    size_t r;
    LogNode *node;

    if (fp == NULL) {
        return NULL;
    }

    r = fread(&temp, sizeof(LogEntry), 1, fp);
    if (r != 1) {
        return NULL;  // EOF or read error
    }

    node = malloc(sizeof(LogNode));
    if (node == NULL) {
        return NULL;  // malloc failure stops list creation
    }

    node->data = temp;
    node->next = readListRecursive(fp);

    return node;
}

/* Builds a complete linked list from a valid binary archive file. Returns the head node or NULL on error. */
LogNode* buildLogListFromBinary(char *binFilename) {
    FILE *fp;
    LogNode *head;

    if (binFilename == NULL) {
        return NULL;
    }

    // Safety: refuse corrupt files
    if (validateBinaryArchive(binFilename) == 0) {
        return NULL;
    }

    fp = fopen(binFilename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    head = readListRecursive(fp);

    fclose(fp);
    return head;
}

/* Recursively counts the number of dropped calls (successCode == 0) for a specific tower ID. */
int countDroppedCalls(LogNode *head, int towerID) {
    int add; 

    if (head == NULL) {
        return 0;
    }

    // Evaluate boolean match as 1 or 0
    add = (head->data.towerID == towerID && head->data.successCode == 0) ? 1 : 0;
    
    return add + countDroppedCalls(head->next, towerID);
}

/* Recursively removes all log nodes belonging to a target user ID and frees their memory. */
void removeUserLogs(LogNode **head, char *targetUID) {
    LogNode *temp;

    if (head == NULL || *head == NULL || targetUID == NULL) {
        return;
    }

    // If current node matches, remove it, free memory, and recurse on the new head
    if (strcmp((*head)->data.userID, targetUID) == 0) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        removeUserLogs(head, targetUID);
    } else {
        // Otherwise recurse on the next pointer
        removeUserLogs(&((*head)->next), targetUID);
    }
}

/* Recursively frees an entire linked list and sets the head pointer to NULL. */
void destroyLogList(LogNode **head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    destroyLogList(&((*head)->next));
    free(*head);
    *head = NULL;
}