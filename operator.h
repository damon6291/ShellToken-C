#ifndef OPERATOR_H
#define OPERATOR_H

#include "svec.h"

void semicolon(svec* tokens, int ii);
void and(svec* tokens, int ii);
void or(svec* tokens, int ii);
void redirect_in(char* filename, svec* tokens);
void redirect_out(char* filename, svec* tokens);
void background(svec* tokens, svec* rest);
void pip(svec* tokens, svec* rest);

#endif
