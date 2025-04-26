#include <stdio.h>
#include <time.h>
#include <math.h>   // For sin() function
#include <signal.h> // For kill() and SIGKILL
#include <unistd.h> // For sleep()

void f(int x) {
    // Example function to be run in the loop (customizable)
    printf("Shutdown condition: %d\n", x);
}

void shutdown_sequence(int a, int b, int force_shutdown) {
    // Step 1: Print shutdown message
    printf("Shutting down...\n");
    fflush(stdout);

    // Step 2: Short delay before forcing shutdown, to allow message to be displayed
    struct timespec req = { 1, 0 };  // 1 second delay
    nanosleep(&req, NULL);

    // Step 3: Force shutdown (kill all processes) if force_shutdown is set
    if (force_shutdown) {
        printf("Forcing shutdown...\n");
        kill(0, SIGKILL);  // Kill all processes in the current process group
    } else {
        // Graceful shutdown: send SIGTERM (this part can be adjusted for specific conditions)
        printf("Gracefully shutting down...\n");
        kill(0, SIGTERM);  // Ask processes to terminate
    }

    // Step 4: Perform any additional shutdown tasks (e.g., running custom function f)
    if (a) {
        do {
            f(b);  // Example task with b as the condition
        } while (1);  // Will never exit, so consider this loop only if you want a blocking action
    } else {
        do {
            f(!b);  // Example task with !b as the condition
        } while (1);  // Will never exit, so consider this loop only if you want a blocking action
    }
}

int main(void) {
    int a = 20;
    int b = 30;

    // Example check for numbers and a message
    if (a > 10 && b > 10)
        printf("Both numbers are greater than 10, beeping\n");

    // Example math operation using sin() (you had Math.Sin which is JavaScript syntax)
    int k = 5;
    double freq = 1.0;
    double A = 2.0;
    double y[k];
    for (int i = 0; i < k; i++) {
        y[i] = (sin(freq * i) >= 0) ? A : -1 * A;  // Using sin() in C
        printf("y[%d] = %f\n", i, y[i]);
    }

    // Initiate the shutdown sequence (with a forced shutdown for testing)
    int force_shutdown = 1;  // Change to 0 for graceful shutdown
    shutdown_sequence(a, b, force_shutdown);

    return 0;
}
