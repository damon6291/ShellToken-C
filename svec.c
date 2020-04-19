/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->data = malloc(2 * sizeof(char*));
    sv->size = 0;
    // TODO: correctly allocate and initialize data structure
    sv->capacity = 2;
    return sv;
}

void
free_svec(svec* sv)
{
    // TODO: free all allocated data
        for (int ii = 0; ii < sv->size; ++ii) {
		free(sv->data[ii]);
	}
	free(sv->data);
	free(sv);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);
    // TODO: insert item into slot ii
    // Consider ownership of string in collection
    char* str = (char*)malloc(sizeof(char) * strlen(item) + 1);
    strcpy(str, item);

    sv->data[ii] = str;

}

void
svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;
    int jj = sv->capacity;


    if (ii == jj) {
   	 char** data = realloc(sv->data, jj * 2 * sizeof(char*));
   		 if (data) {
			 sv->data = data;
			 sv->capacity = jj * 2;
		 }
    }
    sv->size += 1;
    svec_put(sv, ii, item);


    // TODO: expand vector if backing erray
    // is not big enough

}

void
svec_swap(svec* sv, int ii, int jj)
{
    // TODO: Swap the items in slots ii and jj
    char* temp = sv->data[ii];
    sv->data[ii] = sv->data[jj];
    sv->data[jj] = temp;
   
	
}

svec*
svec_reverse(svec* sv)
{
	svec* rsv = make_svec();
	for (int ii = sv->size - 1; ii >= 0; --ii) {
		char* curr = svec_get(sv, ii);
		svec_push_back(rsv, curr);
	}

	free_svec(sv);
	return rsv;
}

char*
to_string(svec* svec, int ii, int jj)
{
	char* arg = malloc(255);
	for (int kk = ii; kk < jj + 1; ++kk)
	{
		char* c = svec_get(svec, kk);
		strcat(arg, " ");
		strcat(arg, c);
	}
	return arg;
}


/*
int
main(int argc, char* argv[])
{

	svec* xs = make_svec();

	char* line = "banana";
	svec_push_back(xs, line);

	char* line2 = "letter";
	svec_push_back(xs, line2);

	char* line3 = "omlet";
	svec_push_back(xs, line3);

	for (int ii = 0; ii < xs->size; ++ii) {

		char* lin = svec_get(xs, ii);
		printf("%s\n", lin);
	}

	free_svec(xs);

	return 0;

}*/
