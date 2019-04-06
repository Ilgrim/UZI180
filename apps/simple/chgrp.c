/*
 * Copyright (c) 1993 by David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 */

#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "unix.h"

int main(argc, argv)
char **argv;
{
    char *cp;
    int gid;
    struct group *grp;
    struct stat statbuf;
    int stat(), chown();

    cp = argv[1];
    if (!cp) {
	fprintf(stderr, "chgrp: too few arguments\n");
	return 1;
    }
    if (isdigit(*cp)) {
	gid = 0;
	while (isdigit(*cp))
	    gid = gid * 10 + (*cp++ - '0');

	if (*cp) {
	    fprintf(stderr, "chgrp: bad gid value\n");
	    return 1;
	}
    } else {
	grp = getgrnam(cp);
	if (grp == NULL) {
	    fprintf(stderr, "chgrp: unknown group name\n");
	    return 1;
	}
	gid = grp->gr_gid;
    }

    argc--;
    argv++;

    while (argc-- > 1) {
	argv++;
	if ((stat(*argv, &statbuf) < 0) ||
	    (chown(*argv, statbuf.st_uid, gid) < 0)) {
	    perror(*argv);
	    return 1;
	}
    }

    return 0;
}
