#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <assert.h>

#include "svec.h"
#include "nush.h"

void
semicolon(svec* tokens, int ii)
{
	svec* args1 = make_svec();
	for (int jj = 0; jj < ii; ++jj) {
		char* c = svec_get(tokens, jj);
 		svec_push_back(args1, c);
	}
	svec* args2 = make_svec();
	int max2 = tokens->size - ii - 1;
	for (int kk = 0; kk < max2; ++kk) {
		char* c = svec_get(tokens, ii + 1 + kk);
		svec_push_back(args2, c);
	}

	execute(args1);
	free_svec(args1);
	evaluate(args2);
	free_svec(args2);

}

void
and(svec* tokens, int ii)
{
	svec* args1 = make_svec();
	for (int jj = 0; jj < ii; ++jj) {
		char* c = svec_get(tokens, jj);
 	//	strcat(args1, " ");
	//	strcat(args1, c);
		svec_push_back(args1, c);
	}
	svec* args2 = make_svec();
	int max2 = tokens->size - ii - 1;
	for (int kk = 0; kk < max2; ++kk) {
		char* c = svec_get(tokens, ii + 1 + kk);
	//	strcat(args2, " ");
	//	strcat(args2, c);
		svec_push_back(args2, c);
	}

	int rv;
	rv = execute(args1);
	if (rv == 0) {
		evaluate(args2);
	}
	free_svec(args1);
	free_svec(args2);

}

void
or(svec* tokens, int ii)
{
	svec* args1 = make_svec();
	for (int jj = 0; jj < ii; ++jj) {
		char* c = svec_get(tokens, jj);
 	//	strcat(args1, " ");
	//	strcat(args1, c);
		svec_push_back(args1, c);
	}
	svec* args2 = make_svec();
	int max2 = tokens->size - ii - 1;
	for (int kk = 0; kk < max2; ++kk) {
		char* c = svec_get(tokens, ii + 1 + kk);
	//	strcat(args2, " ");
	//	strcat(args2, c);
		svec_push_back(args2, c);
	}

	int rv;
	rv = execute(args1);
	if (rv != 0) {
		evaluate(args2);
	}
	free_svec(args1);
	free_svec(args2);

}

void 
redirect_in(char* filename, svec* tokens) 
{
	int cpid;

	if ((cpid = fork())) {
		//parent
		
		waitpid(cpid, 0, 0);

	} else {
		//child
		int fd = open(filename, O_RDONLY);
		close(0);
		dup(fd);
		close(fd);

		if ((cpid = fork())) {
			waitpid(cpid, 0, 0);
			_exit(0);
		} else {
			execute(tokens);
			_exit(0);
		}
		_exit(0);
	}
}

void
redirect_out(char* filename, svec* tokens)
{
	int cpid;

	if ((cpid = fork())) {
		//parent
		
		waitpid(cpid, 0, 0);
	//	printf("child done get pid: %d\n", getpid());
	} else {
		//child
	//	if (strcmp(filename, "a.txt") == 0) {
	//		filename = "b.txt";
	//	}
		
		int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		close(1);
		dup(fd);
		close(fd);

		if ((cpid = fork())) {
			waitpid(cpid, 0, 0);
		//	printf("exec done get pid: %d\n", getpid());
			_exit(0);
			
		} else {
			execute(tokens);
			_exit(0);
		//	execlp("echo", "echo", "one", 0);
		}
		_exit(0);
	}
}

void
background(svec* tokens, svec* rest)
{
	int cpid;
	if ((cpid = fork())) {
		if (rest->size != 0) {
			evaluate(rest);
		}
	}	
	else {
		execute(tokens);
		_exit(0);
	}
}

void
pip(svec* tokens, svec* rest)
{
	int cpid;

	if ((cpid = fork())) {
		waitpid(cpid, 0, 0);
	}
	else {
		char* cc = svec_get(tokens, 0);
		redirect_out(cc, tokens);	
		svec_push_back(rest, cc);
	
	/*	printf("rest1 size is %d\n", rest->size);
		for (int tt = 0; tt < rest->size; ++tt) {
			printf("rest1 is %s\n", svec_get(rest, tt));
		} */
		
		for (int ii = 0; ii < rest->size; ++ii) {
			char* curr = svec_get(rest, ii); 
			if (strcmp(curr, "|") == 0) {
				for (int jj = ii; jj < rest->size - 1; ++jj) {
				//	printf("jj = %d\n", jj);
					svec_swap(rest, jj, rest->size - 1);
				/*	for (int tt = 0; tt < rest->size; ++tt) {
						printf("rest is %s\n", svec_get(rest, tt));
					}*/
				}
				break;
			} 
		}

	//	printf("rest2 size is %d\n", rest->size);
	/*	for (int tt = 0; tt < rest->size; ++tt) {
			printf("rest2 is %s\n", svec_get(rest, tt));
		}*/
		if ((cpid == fork())) {
			waitpid(cpid, 0, 0);
			_exit(0);
		}
		
		else {
			execute(rest);
			_exit(0);
		}
		
		_exit(0);

	}
} 
/*
void
pip(svec* tokens, svec* rest)
{
	int cpid;
	int rv;
	int pipes[2]; // 0 -> reading // 1 -> writing
	rv = pipe(pipes);
	assert(rv != -1);

	if ((cpid = fork())) {
		waitpid(cpid, 0, 0);
	}
	else {
		close(1);
		dup(pipes[1]);
		close(pipes[0]);
		close(pipes[1]);
		execute(tokens);
	//	assert(0);

		if ((cpid = fork())) {
			waitpid(cpid, 0, 0);
			exit(0);
		}
		else {
			close(0);
			dup(pipes[0]);
			close(pipes[1]);
			close(pipes[0]);
			execute(rest);
			exit(0);
		}
	}

}*/
