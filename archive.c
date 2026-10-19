/* Handles binary archive validation and restoration of LogEntry records. */

#include "telecom.h"

/* Checks whether a binary archive file exists and contains a valid multiple of LogEntry structs. Returns 1 if valid, 0 otherwise. */
int validateBinaryArchive(char *binFilename) {
    FILE *fp;
    long size;

    if (binFilename == NULL) {
        return 0;
    }

    fp = fopen(binFilename, "rb");
    if (fp == NULL) {
        return 0;
    }

    // Determine file size
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return 0;
    }

    size = ftell(fp);
    fclose(fp);

    if (size <= 0) {
        return 0;
    }

    if (size % (long)sizeof(LogEntry) != 0) {
        return 0;
    }

    return 1;
}

/* Restores all LogEntry records from a binary archive into a dynamically allocated array. Returns NULL on error. */
LogEntry* restoreFromBinary(char *binFilename, int *count) {
    FILE *fp;
    long size;
    int n;
    LogEntry *logs;
    size_t readCount;

    if (count == NULL) {
        return NULL;
    }
    *count = 0;

    if (binFilename == NULL) {
        return NULL;
    }

    fp = fopen(binFilename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // Determine file size to calculate record count
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    size = ftell(fp);
    if (size <= 0 || size % (long)sizeof(LogEntry) != 0) {
        fclose(fp);
        return NULL;
    }

    n = (int)(size / (long)sizeof(LogEntry));

    // Reset to beginning of file for reading
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return NULL;
    }

    logs = malloc(sizeof(LogEntry) * n);
    if (logs == NULL) {
        fclose(fp);
        return NULL;
    }

    readCount = fread(logs, sizeof(LogEntry), (size_t)n, fp);
    fclose(fp);

    if ((int)readCount != n) {
        free(logs);
        *count = 0;
        return NULL;
    }

    *count = n;
    return logs;
}