#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "svec.h"

char*
read_alphadigit(char* line, int ii) 
{
	int nn = 0;
	while (isalpha(line[ii + nn]) || isdigit(line[ii + nn])) {
		++nn;
	}

	char* alp = malloc(nn + 1);
	memcpy(alp, line + ii, nn);
	alp[nn] = 0;
	return alp;
}

char*
read_special(char* line, int ii)
{
	int nn = 0;
	while (ispunct(line[ii + nn])) {
		++nn;
	}

	while (isalpha(line[ii + nn])) {
		++nn;
	}

	char* spe = malloc(nn + 1);
	memcpy(spe, line + ii, nn);
	spe[nn] = 0;
	return spe;
}

char*
read_dotslash(svec* sv, char* line, int ii, char c)
{
	int nn = sv->size - 1;
	char* prev = sv->data[nn];
	char dot = c;
	strncat(prev, &dot, 1); 

	if(isalpha(line[ii + 1])) {
		char* alp = read_alphadigit(line, ii+1);
		strncat(prev, alp, strlen(alp));
		free(alp);
	}

	return prev;
}

char*
read_or(char* line, int ii) {
	int nn = 0;
	while (line[ii + nn] == '|') {
		++nn;
	}

	char* or = malloc(nn + 1);
	memcpy(or, line + ii, nn);
	or[nn] = 0;
	return or;
}


svec*
tokenize(char* line)
{
	svec* sv = make_svec();
	int len = strlen(line);
	int ii = 0;
	while (ii<len) {
		if (isspace(line[ii])) {
			++ii;
			continue;
		}

		if (isalpha(line[ii]) || isdigit(line[ii])) {
			char* alp = read_alphadigit(line, ii);
			svec_push_back(sv, alp);
			ii += strlen(alp);
			free(alp);
			continue;
		}
		//case of . add front and back
		if (line[ii] == 46 && !isspace(line[ii - 1])) {
			int prev = strlen(sv->data[sv->size - 1]);
			char* dot = read_dotslash(sv, line, ii, '.');
			svec_put(sv, sv->size - 1, dot);
			int after = strlen(sv->data[sv->size - 1]);
			ii = ii + after - prev;
			free(dot);
			continue;
		}

		//case of / add front and back
		if (line[ii] == 47 && !isspace(line[ii - 1])) {
			int prev = strlen(sv->data[sv->size - 1]);
			char* slash = read_dotslash(sv, line, ii, '/');
			svec_put(sv, sv->size - 1, slash);
			int after = strlen(sv->data[sv->size - 1]);
			ii = ii + after - prev;
			free(slash);
			continue;
		}

		//case of _ add front and back
		if (line[ii] == '_' && !isspace(line[ii - 1])) {
			int prev = strlen(sv->data[sv->size - 1]);
			char* under = read_dotslash(sv, line, ii, '_');
			svec_put(sv, sv->size - 1, under);
			int after = strlen(sv->data[sv->size - 1]);
			ii = ii + after - prev;
			free(under);
			continue;
		}
		
		//case of |
		if (line[ii] == '|') {
			char* or = read_or(line, ii);
			svec_push_back(sv, or);
			ii += strlen(or);
			continue;
		}
		
		char* spe = read_special(line, ii);
		svec_push_back(sv, spe);
		ii += strlen(spe);
		free(spe);
		continue;
	}

	return sv;


}
