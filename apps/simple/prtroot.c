/* printroot - print root device on stdout	Author: Bruce Evans */

/* This program figures out what the root device is by doing a stat on it, and
 * then searching /dev until it finds an entry with the same device number.
 * A typical use (probably the only use) is in /etc/rc for initializing
 * /etc/mtab, as follows:
 *
 *	/usr/bin/printroot >/etc/mtab
 *
 *  9 Dec 1989	- clean up for 1.5 - full prototypes (BDE)
 * 15 Oct 1989	- avoid ACK cc bugs (BDE):
 *		- sizeof "foo" is 2 (from wrong type char *) instead of 4
 *		- char foo[10] = "bar"; allocates 4 bytes instead of 10
 *  1 Oct 1989	- Minor changes by Andy Tanenbaum
 *  5 Oct 1992	- Use readdir (kjb)
 * 26 Nov 1994	- Flag -r: print just the root device (kjb)
 * 19 May 2001  - Ported to UZI180
 */

/*#include <unix.h>*/
#include <dirent.h>

#define DEV_PATH	"/dev/"
#define MESSAGE		" / uzi rw\n"
#define UNKNOWN_DEV	"/dev/unknown"
#define ROOT		"/"

int rflag;

int strcmp(char *, char *);
int stat();

int  main(int argc, char **argv);
void done(char *name, int status);

int main(argc, argv)
int argc;
char **argv;
{
    DIR *dp;
    struct direct *entry;
    struct stat filestat, rootstat;
    static char namebuf[sizeof(DEV_PATH) + MAXNAMLEN + 1];

    rflag = (argc > 1 && strcmp(argv[1], "-r") == 0);

    if (stat(ROOT, &rootstat) == 0
	&& (dp = opendir(DEV_PATH)) != (DIR *) NULL) {
	while ((entry = readdir(dp)) != (struct direct *) NULL) {
	    strcpy(namebuf, DEV_PATH);
	    strcat(namebuf, entry->d_name);
	    if (stat(namebuf, &filestat) != 0)
		continue;
	    if ((filestat.st_mode & F_MASK) != F_BDEV)
		continue;
	    if (filestat.st_rdev != rootstat.st_dev)
		continue;
	    done(namebuf, 0);
	}
    }
    done(UNKNOWN_DEV, 1);
    return (0);			/* not reached */
}

void done(name, status)
char *name;
int status;
{
    write(1, name, strlen(name));
    if (rflag) {
	write(1, "\n", 1);
	exit(status);
    }
    write(1, MESSAGE, sizeof(MESSAGE));

    exit(status);
}
