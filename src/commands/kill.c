#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    char user_input;
    pid_t i = 12345;  // Replace with the actual process ID you want to kill

    // Display confirmation message
    printf("Are you sure? This will kill all tasks, especially system files! (y/n): ");
    scanf(" %c", &user_input);  // Note the space before %c to ignore any leading whitespace

    switch(user_input) {
        case 'y':
        case 'Y':
            // Send SIGKILL to the process
            if (kill(i, SIGKILL) == -1) {
                perror("Failed to kill process");
                return 1;
            }
            printf("Process %d killed.\n", i);
            break;
        case 'n':
        case 'N':
            printf("Operation cancelled.\n");
            break;
        default:
            printf("Invalid input. Operation cancelled.\n");
            break;
    }

    return 0;
}
