/*
 * rogprocess.c
 * Receipt Of Goods (ROG) Processes for unxus kernel
 *
 * This file implements the core logic for handling the receipt of goods operations.
 * ROG = Receipt Of Goods
 *
 * Author: electric-otter
 *
 * This version treats running processes as the "goods" being received.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>

#define PROC_DIR "/proc"
#define MAX_COMM_LEN 256

// Structure to represent a received process (treated as a good)
typedef struct {
    pid_t pid;
    char comm[MAX_COMM_LEN];
    time_t received_time;
} ROG_Process;

// Structure to represent an ROG transaction for processes
typedef struct {
    int transaction_id;
    ROG_Process* processes;
    int num_procs;
    time_t transaction_time;
} ROG_Transaction;

// Log a received process
void rog_log_process(const ROG_Process* proc) {
    printf("ROG PROCESS RECEIVED:\n");
    printf("  PID: %d\n", proc->pid);
    printf("  Command: %s\n", proc->comm);
    printf("  Received: %s", ctime(&(proc->received_time)));
}

// Count the number of running processes (by counting numeric directories in /proc)
int rog_count_processes() {
    DIR* d = opendir(PROC_DIR);
    if (!d) return 0;
    int count = 0;
    struct dirent* entry;
    while ((entry = readdir(d)) != NULL) {
        if (isdigit(entry->d_name[0]))
            count++;
    }
    closedir(d);
    return count;
}

// Fill the ROG_Process array with details of running processes
int rog_collect_processes(ROG_Process* array, int max_procs) {
    DIR* d = opendir(PROC_DIR);
    if (!d) return 0;
    struct dirent* entry;
    int idx = 0;
    while ((entry = readdir(d)) != NULL && idx < max_procs) {
        if (!isdigit(entry->d_name[0]))
            continue;
        pid_t pid = atoi(entry->d_name);
        char comm_path[64];
        snprintf(comm_path, sizeof(comm_path), PROC_DIR "/%d/comm", pid);
        FILE* f = fopen(comm_path, "r");
        if (!f)
            continue;
        array[idx].pid = pid;
        if (fgets(array[idx].comm, MAX_COMM_LEN, f) == NULL)
            snprintf(array[idx].comm, MAX_COMM_LEN, "unknown");
        // Remove trailing newline
        size_t len = strlen(array[idx].comm);
        if (len > 0 && array[idx].comm[len-1] == '\n')
            array[idx].comm[len-1] = '\0';
        fclose(f);
        array[idx].received_time = time(NULL);
        idx++;
    }
    closedir(d);
    return idx;
}

// Initialize a new ROG transaction for processes
void rog_init_transaction(ROG_Transaction* txn, int txn_id, int num_procs) {
    txn->transaction_id = txn_id;
    txn->num_procs = num_procs;
    txn->transaction_time = time(NULL);
    txn->processes = (ROG_Process*)malloc(sizeof(ROG_Process) * num_procs);
    if (!txn->processes) {
        fprintf(stderr, "ROG: Failed to allocate memory for processes\n");
        exit(1);
    }
}

// Finalize a transaction and log each process
void rog_finalize_transaction(const ROG_Transaction* txn) {
    printf("=== ROG PROCESS TRANSACTION %d ===\n", txn->transaction_id);
    printf("Transaction time: %s", ctime(&(txn->transaction_time)));
    for (int i = 0; i < txn->num_procs; ++i) {
        rog_log_process(&txn->processes[i]);
    }
}

// Free resources for a transaction
void rog_free_transaction(ROG_Transaction* txn) {
    if (txn->processes) free(txn->processes);
    txn->processes = NULL;
    txn->num_procs = 0;
}

// Example usage (for testing/demo)
#ifdef ROGPROCESS_MAIN
int main() {
    int num_procs = rog_count_processes();
    if (num_procs == 0) {
        printf("No processes found or cannot access /proc.\n");
        return 1;
    }
    ROG_Transaction txn;
    rog_init_transaction(&txn, 3001, num_procs);
    int collected = rog_collect_processes(txn.processes, num_procs);
    txn.num_procs = collected;
    rog_finalize_transaction(&txn);
    rog_free_transaction(&txn);
    return 0;
}
#endif
