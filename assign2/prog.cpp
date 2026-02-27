
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Devin Cullen
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

bool bg_flag = false;
char history[MAX_LINE];

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    bg_flag = false;
    history = command;

    size_t length = strlen(command);

    if (length > 0 && command[length - 1] == '\n') {
        command[length - 1] = '\0';
	}

    const char* delimiter = " \t";
	char* token = strtok(command, delimiter);
    int argc = 0;

    // iterate through tokens and add them to the args array
    while (token != NULL && argc < MAX_LINE / 2) {
		if strcmp(token, "&") == 0 { bg_flag = true; } // background flag
        else {
            args[argc++] = token;
        }
        token = strtok(NULL, delimiter); // next token
    }

    args[argc] = NULL; // Null-terminate the args array
    return argc;
}

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // hold parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    int status = 0;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        if (fgets(command, MAX_LINE, stdin) == NULL) {
            break;
        }
        // Parse the input command
        int num_args = parse_command(command, args);

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }


        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
        }
        else if (pid == 0) { // child process
            if (execvp(args[0], args) < 0) {
                perror("command not found")
            }
        }
        else { // parent process must wait, unless bg flagged
            if (!bg_flag) {
                waitpid(pid, &status, 0);
            }
        }
    }
    return 0;
}
