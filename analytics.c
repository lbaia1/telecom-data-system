/* Recursive data aggregation and analysis functions for LogEntry records. */

#include "telecom.h"

/* Recursively calculates the total data usage across an array of LogEntry records. Returns 0.0 if the array is empty or NULL. */
float recursiveTotalUsage(LogEntry *logs, int recordCount) {
    if (logs == NULL || recordCount <= 0) {
        return 0.0f;
    }

    // Add current element and recurse on the remaining array
    return logs[recordCount - 1].dataUsage + recursiveTotalUsage(logs, recordCount - 1);
}

/* Recursively finds the maximum data usage value within an array of LogEntry records. Returns 0.0 if the array is empty or NULL. */
float recursiveMaxUsage(LogEntry *logs, int recordCount) {
    float maxSoFar;
    float currentUsage;

    if (logs == NULL || recordCount <= 0) {
        return 0.0f;
    }

    // Base case: single element
    if (recordCount == 1) {
        return logs[0].dataUsage;
    }

    maxSoFar = recursiveMaxUsage(logs, recordCount - 1);
    currentUsage = logs[recordCount - 1].dataUsage;

    // Return the maximum of the two
    return (currentUsage > maxSoFar) ? currentUsage : maxSoFar;
}