#ifndef _COMMON_H_
#define _COMMON_H_


#include "Defs.h"

void kill_child_handler(int sig)
{
    int status;
    pid_t done = waitpid(-1, // Any child
                         &status,
                         0); // Blocked mode.
    if (done == -1)
    {
        printf("No more child processes.\n");
    }
    else
    {
        short isNormalTermination = WIFEXITED(status);
        if (!isNormalTermination ||
            // WEXITSTATUS should be used only if normal termination = true.
            (isNormalTermination && WEXITSTATUS(status) != 0))
        {
            printf("Zombie for PID -- %d failed.\n", done);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Zombie for PID -- %d successfully removed.\n", done);
        }
    }
}

void handle_child_finishing()
{
    // Handle child process killing.
    struct sigaction kill_child_signal;
    kill_child_signal.sa_handler = kill_child_handler;
    sigemptyset(&kill_child_signal.sa_mask);
    kill_child_signal.sa_flags = SA_RESTART; // Mark handler as Permanent.
    
    if (sigaction(SIGCHLD, &kill_child_signal, 0) == -1)
    {
        perror("Error of calling sigaction");
        exit(EXIT_FAILURE);
    }
}

#endif /* _COMMON_H_ */