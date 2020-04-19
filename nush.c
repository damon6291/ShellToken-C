#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "svec.h"
#include "token.h"
#include "operator.h"

/*
//case of semi colon
void
semicolon(svec* tokens, int ii)
{
//	svec* tokens = tokenize(cmd);
	char* args1 = "";
	for (int jj = 0; jj < ii; ++jj) {
		char* c = svec_get(tokens, jj);
		strcat(args1, " ");
		strcat(args1, c);
	}

	char* args2 = "";
	int max2 = tokens->size - ii - 1;
	for (int kk = 0; kk < max2; ++kk) {
		char* c = svec_get(tokens, ii + 1 + kk);
		strcat(args2, " ");
		strcat(args2, c);
	}
	evaluate(args1);
	evaluate(args2);
}
*/

//check cd or exit
void
check(svec* tokens)
{
	  // svec* tokens = tokenize(cmd);
   	 char* args[tokens->size];
   	 args[tokens->size] = 0;

   	 for (int ii = 0; ii < tokens->size; ++ii) {
	    char* c = svec_get(tokens, ii);
	    args[ii] = c;
   	 } 

	if (strcmp(args[0], "cd") == 0) {
		chdir(args[1]);
	}
	else if (strcmp(args[0], "exit") == 0) {
		_exit(0);
	}
}
		
int
execute(svec* tokens)
{

	char* args[tokens->size];
   	 args[tokens->size] = 0;

   	 for (int ii = 0; ii < tokens->size; ++ii) {
	    char* c = svec_get(tokens, ii);
	 //check redirector
		if (strcmp(c, "<") == 0) {
			svec* tok = make_svec();
			for (int kk = 0; kk < ii; ++kk) {
				char* c = svec_get(tokens, kk);
				svec_push_back(tok, c);
			}
			char* filename = svec_get(tokens, ii + 1);
			redirect_in(filename, tok);
			free_svec(tok);
			break;
		}
		else if (strcmp(c, ">") == 0) {
			svec* tok = make_svec();
			for (int kk = 0; kk < ii; ++kk) {
				char* c = svec_get(tokens, kk);
				svec_push_back(tok, c);
			}
			char* filename = svec_get(tokens, ii + 1);
			redirect_out(filename, tok);
			free_svec(tok);
			args[0] = "z";
			break;	
		}
		else if (strcmp(c, "|") == 0) {
			svec* tok = make_svec();
			for (int kk = 0; kk < ii; ++kk) {
				char* c = svec_get(tokens, kk);
				svec_push_back(tok, c);
			}
			svec* rest = make_svec();
			for (int ll = ii + 1; ll < tokens->size; ++ll) {
				char* c = svec_get(tokens, ll);
				svec_push_back(rest, c);
			}
			pip(tok, rest);
			free_svec(rest);
			free_svec(tok);	
			args[0] = "z";
			break;	
		}
		else {
			args[ii] = c;
		}
	 }

    int cpid;

    if ((cpid = fork())) {
        // parent process

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

//	printf("%d\n", status);

	if (status == 0) {
		return 0;
	} else {
		return 1;
	}

	_exit(0);

    //    printf("== executed program complete ==\n");

  /*      printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", 
			    WEXITSTATUS(status));
        }*/
    }
    else {
	    if (strcmp(args[0], "z") != 0) {
        // child process

     //   printf("== executed program's output: ==\n");

      		  execvp(args[0], args);
	    }
	_exit(0);
      
    }
}

void 
evaluate(svec* tokens) 
{
	int ii = 0;
	int nn = tokens->size;
	
	//check cd and exit
	check(tokens);

	while (ii < nn) {
		char* c = svec_get(tokens, ii);
		if (strcmp(c, ";") == 0) {
			semicolon(tokens, ii);
			break;
		} 
		if (strcmp(c, "&&") == 0) {
			and(tokens, ii);
			break;
		}
		if (strcmp(c, "||") == 0) {
			or(tokens, ii);
			break;
		}
		if (strcmp(c, "&") == 0) {
			svec* tok = make_svec();
			for (int kk = 0; kk < ii; ++kk) {
				char* c = svec_get(tokens, kk);
				svec_push_back(tok, c);
			}
			svec* rest = make_svec();
			for (int ll = ii + 1; ll < tokens->size; ++ll) {
				char* c = svec_get(tokens, ll);
				svec_push_back(rest, c);
			}
			background(tok, rest);
			free_svec(rest);
			free_svec(tok);
			ii -= 1;
			break;
		}
	
			++ii;	
	}
	if (ii == nn) {
		execute(tokens);
	}
}

int
main(int argc, char* argv[])
{
    char cmd[256];

    if (argc == 1) {
	    while (1) {
		    printf("nush$ ");
		    fflush(stdout);
		    char* line = fgets(cmd, 256, stdin);
		    if (!line) {
			    exit(0);
		    }
		    svec* tokens = tokenize(cmd);
		    evaluate(tokens);
		    free_svec(tokens);
	    }
    }
    else {
//	    int fn = open(argv[1], O_RDONLY);
//	    close(0);
//	    dup(fn);
	    FILE *fp;
	    fp = fopen(argv[1], "r");
	    while(1) {
		    fflush(stdout);
		    char* line = fgets(cmd, 256, fp);
		    if (!line) {
			    exit(0);
		    }
		    svec* tokens = tokenize(cmd);
		    evaluate(tokens);
		    free_svec(tokens);
	    }
    }
    return 0;
}
