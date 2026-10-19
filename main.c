/* Main driver for the Telecom Log Analyzer. Handles CSV parsing, binary serialization, and memory aggregation. */

#include "telecom.h"

int main(void) {
    LogEntry *logs = NULL;  // Holds CSV data after loading
    LogEntry *restored = NULL;  // Holds data restored from binary

    int count = 0;
    int restoredCount = 0;

    const char *csvFile = "network_logs.csv";
    const char *binFile = "archive.bin";

    // --- Phase 1: CSV Parsing & Binary Serialization ---
    printf("Loading from CSV: %s\n", csvFile);

    logs = loadFromCSV((char *)csvFile, &count);
    if (logs == NULL || count == 0) {
        printf("Failed to load CSV or CSV was empty.\n");
        return 1;
    }

    printf("Loaded %d records.\n", count);
    printf("Converting to binary: %s\n", binFile);
    convertToBinary((char *)binFile, logs, count);

    // --- Phase 2: Binary Restoration & Validation ---
    printf("Validating binary archive...\n");

    if (validateBinaryArchive((char *)binFile) == 0) {
        printf("Binary archive invalid.\n");
        freeArchive(&logs);
        return 1;
    }

    printf("Binary archive valid.\n");
    printf("Restoring from binary...\n");
    
    restored = restoreFromBinary((char *)binFile, &restoredCount);
    if (restored == NULL || restoredCount == 0) {
        printf("Failed to restore from binary.\n");
        freeArchive(&logs);
        return 1;
    }

    printf("Restored %d records.\n", restoredCount);

    // --- Phase 3: Memory Aggregation & Analysis ---
    printf("Total data usage (MB): %.2f\n", recursiveTotalUsage(restored, restoredCount));
    printf("Max data usage (MB): %.2f\n", recursiveMaxUsage(restored, restoredCount));

    // --- Phase 4: Linked List Construction & Filtering ---
    printf("\nBuilding linked list from binary...\n");

    LogNode *head = buildLogListFromBinary((char *)binFile);

    if (head == NULL) {
        printf("Failed to build linked list.\n");
    } else {
        int tower = 10;
        int drops = countDroppedCalls(head, tower);
        printf("Dropped calls on tower %d: %d\n", tower, drops);

        printf("Removing logs for UID-2001...\n");
        removeUserLogs(&head, "UID-2001");

        printf("Dropped calls on tower %d after removal: %d\n", tower, countDroppedCalls(head, tower));

        destroyLogList(&head);

        if (head == NULL) {
            printf("Linked list successfully destroyed (head is NULL).\n");
        } else {
            printf("Warning: head is not NULL after destroyLogList.\n");
        }
    }

    // --- Final Cleanup ---
    printf("\nFreeing dynamic arrays...\n");
    freeArchive(&logs);
    freeArchive(&restored);

    printf("Done.\n\nProgram finished\n");

    return 0;
}