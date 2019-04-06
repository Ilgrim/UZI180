/* Copyright (c) 1993 by David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Most simple built-in commands are here.
 */

#include "stdio.h"
#include "unix.h"
#include <ctype.h>
#include <string.h>

int kill();

int main(argc, argv)
int argc;
char **argv;
{
    char *cp;
    int pid, sig = SIGTERM;

    if (argc < 2) {
	fprintf(stderr, "usage: kill [-sig] pid ...\n");
	return 0;
    }
    if (argv[1][0] == '-') {
	cp = &argv[1][1];
	if (!strncmp(cp, "SIG", 3))
	    cp += 3;
	if (!strcmp(cp, "HUP"))
	    sig = SIGHUP;
	else if (!strcmp(cp, "INT"))
	    sig = SIGINT;
	else if (!strcmp(cp, "QUIT"))
	    sig = SIGQUIT;
	else if (strcmp(cp, "KILL"))
	    sig = SIGKILL;
	else if (strcmp(cp, "TERM"))
	    sig = SIGTERM;
	else {
	    sig = 0;
	    while (isdigit(*cp))
		sig = sig * 10 + *cp++ - '0';
	    if (*cp) {
		fprintf(stderr, "kill: unknown signal\n");
		return 1;
	    }
	}
	argc--;
	argv++;
    }
    while (argc-- > 1) {
	cp = *++argv;
	pid = 0;
	while (isdigit(*cp))
	    pid = pid * 10 + *cp++ - '0';
	if (*cp) {
	    fprintf(stderr, "kill: non-numeric pid\n");
	    continue;
	}
	if (kill(pid, sig) < 0) {
	    perror(*argv);
	    return 1;
	}
    }
    return 0;
}
