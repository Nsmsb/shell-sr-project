/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "readcmd.h"
#include "csapp.h"
#include "util.h"

static void run_cmdline(struct cmdline *cmd)
{
	int next_fd_in, fd_pipe[PIPE_ARRAY];
	unsigned proc;
	pid_t *pids = malloc(sizeof(pid_t) * cmd->seq_len);

	/* Run all commands simultaneous and connect together with a pipe */
	for(proc = 0; proc < cmd->seq_len; proc++)
	{
		if((cmd->seq_len > 1) && (pipe(fd_pipe) < 0))
		{
			print_error_errno("Cannot create pipe, will not start process");
			return;
		}
		pids[proc] = fork();

		if(pids[proc] == 0)
		{
            /* Child process */

			if(proc == 0)
				redirect_file(cmd->in, STDIN_FILENO); // If input is redirected from a file
			else
				redirect_pipe(next_fd_in, STDIN_FILENO);  // This input corresponds to previous process pipe

			if(cmd->seq[proc + 1] != NULL)
				redirect_pipe(fd_pipe[PIPE_OUT], STDOUT_FILENO); // Redirect output in pipe
			else
				redirect_file(cmd->out, STDOUT_FILENO); // If output is redirected into a file
            
			execvp(cmd->seq[proc][0], cmd->seq[proc]);
			print_error_errno("Cannot execute command '%s'", cmd->seq[proc][0]); // Only happen if execvp() fails
			exit(1);
		}
		else if(pids[proc] > 0)
		{
			/* Parent process */
			if(cmd->seq_len > 1)
				close(fd_pipe[PIPE_OUT]);
			next_fd_in = fd_pipe[PIPE_IN];  // update next process input, so it holds the output of the last one
		}
		else
			print_error_errno("Cannot create a child process");
	}

    int status = 0;
	for(proc = 0; proc < cmd->seq_len; proc++) {
        Wait(&status);
    }


}


int main()
{
	while (1) {
		struct cmdline *l;
		// int i, j;

		printf("%s%sshell>%s ", COL_BLD, COL_GRN, COL_RST);
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);

        run_cmdline(l);

		/* Display each command of the pipe */
		// for (i=0; l->seq[i]!=0; i++) {
		// 	char **cmd = l->seq[i];
		// 	printf("seq[%d]: ", i);
		// 	for (j=0; cmd[j]!=0; j++) {
		// 		printf("%s ", cmd[j+1]);
		// 	}
		// 	printf("\n");
		// }
	}
}
