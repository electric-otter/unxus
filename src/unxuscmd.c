#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

// Define a function pointer type for command functions
typedef void (*command_func)();

// Function to load and execute commands dynamically from shared objects
void load_and_execute_command(const char *command_name) {
    char path[256];
    snprintf(path, sizeof(path), "./src/commands/%s.so", command_name);

    // Open the shared object
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return;
    }

    // Get the function pointer for the command
    command_func cmd = (command_func) dlsym(handle, "execute");
    if (!cmd) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return;
    }

    // Execute the command
    cmd();

    // Close the shared object
    dlclose(handle);
}

int main() {
    char input[256];

    printf("UnxusCMD$: \n");

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  // Exit on EOF
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;  // Exit the shell
        }

        load_and_execute_command(input);
    }

    return 0;
}
