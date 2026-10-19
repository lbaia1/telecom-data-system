/* Handles data migration, CSV parsing, and heap memory allocation for the Telecom Log Analyzer. */

#include "telecom.h"
#include <string.h>

/* Counts the number of non-empty lines in an open CSV file. Rewinds the file to the beginning before returning. */
static int countCSVLines(FILE *fp) {
    char buffer[MAX_BUFFER_LEN];
    int lines = 0;

    rewind(fp);

    while (fgets(buffer, MAX_BUFFER_LEN, fp) != NULL) {
        // Count even if line has trailing newline, skip truly empty lines
        if (buffer[0] != '\n' && buffer[0] != '\0') {
            lines++;
        }
    }

    rewind(fp);
    return lines;
}

/* Loads telecom logs from a CSV file into a dynamically allocated array. Returns NULL on error or empty file. */
LogEntry* loadFromCSV(char *filename, int *count) {
    FILE *fp;
    int n, i;
    LogEntry *logs;
    char line[MAX_BUFFER_LEN];

    if (count == NULL) {
        return NULL;
    }

    *count = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    n = countCSVLines(fp);
    if (n == 0) {
        fclose(fp);
        return NULL;
    }

    logs = malloc(sizeof(LogEntry) * n);
    if (logs == NULL) {
        fclose(fp);
        *count = 0;
        return NULL;
    }

    i = 0;
    while (i < n && fgets(line, MAX_BUFFER_LEN, fp) != NULL) {
        char uid[MAX_ID_LEN];
        int duration, tower, code;
        float usage;

        // Parse: UID-XXXX,duration,dataUsage,towerID,successCode
        if (sscanf(line, " %19[^,],%d,%f,%d,%d", uid, &duration, &usage, &tower, &code) == 5) {
            strncpy(logs[i].userID, uid, MAX_ID_LEN - 1);
            logs[i].userID[MAX_ID_LEN - 1] = '\0';

            logs[i].duration = duration;
            logs[i].dataUsage = usage;
            logs[i].towerID = tower;
            logs[i].successCode = code;

            i++;
        }
        // Skip malformed lines
    }

    fclose(fp);

    *count = i;

    // If everything was malformed, free and return NULL
    if (*count == 0) {
        free(logs);
        return NULL;
    }

    return logs;
}

/* Writes an array of LogEntry records to a binary file using exactly one fwrite call. */
void convertToBinary(char *binFilename, LogEntry *logs, int count) {
    FILE *fp;
    size_t n;
    size_t written;

    if (binFilename == NULL || logs == NULL || count <= 0) {
        return;
    }

    fp = fopen(binFilename, "wb");
    if (fp == NULL) {
        return;
    }

    n = (size_t)count;

    // Write all records in a single batch
    written = fwrite(logs, sizeof(LogEntry), n, fp);

    // Detect partial write
    if (written != n) {
        fclose(fp);
        return;
    }

    fclose(fp);
}

/* Safely frees a dynamically allocated LogEntry array and sets the pointer to NULL. */
void freeArchive(LogEntry **logs) {
    if (logs == NULL) {
        return;
    }

    if (*logs != NULL) {
        free(*logs);
        *logs = NULL;
    }
}