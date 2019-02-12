/*
 * stdhup - runtime stdio switching library.
 *
 * Author: Edvinas Stunzenas <edvinas.stunzenas@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>

static int oldsa_saved;
static struct sigaction oldsa;

static char stdio_last[32];
static int stdio_null;

static void
patch_stdio(const char *device)
{
        struct stat s;
        int fd;

        if (stat(device, &s)) {
                fprintf(stderr, "stat(%s) failed: %s (%i)\n", device, strerror(errno), errno);
                return;
        }

        fd = open(device, O_RDWR | O_NOCTTY);
        if (fd < 0) {
                fprintf(stderr, "open(%s) failed: %s (%i)\n", device, strerror(errno), errno);
                return;
        }

        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if (fd != STDIN_FILENO &&
            fd != STDOUT_FILENO &&
            fd != STDERR_FILENO)
                close(fd);
}

static void
handle_sighup(int sig, siginfo_t *si, void *uc)
{
	char fdp[128];
	char tty[128] = {0};

	sprintf(fdp, "/proc/%i/fd/2", si->si_pid);
	if (readlink(fdp, tty, sizeof(tty)) == -1) {
		printf("readlink() failed: %s (%i)\n");
		strcpy(tty, "/dev/console");;
	}

	/* Process previous signal handlers. */
	if (!(oldsa.sa_flags & SA_SIGINFO) && oldsa.sa_handler)
		oldsa.sa_handler(sig);
	else if ((oldsa.sa_flags & SA_SIGINFO) && oldsa.sa_sigaction)
		oldsa.sa_sigaction(sig, si, uc);

	if (si->si_code == SI_USER) {
		if (strcmp(stdio_last, tty)) {
			stdio_null = 0;
			strncpy(stdio_last, tty, sizeof(stdio_last));
			patch_stdio(tty);
		} else {
			if (stdio_null) {
				stdio_null = 0;
				patch_stdio(stdio_last);
			} else {
				stdio_null = 1;
				patch_stdio("/dev/null");
			}
		}
	} else {
		if (stdio_null) {
			return;
		}
		stdio_null = 1;
		patch_stdio("/dev/null");
	}
}

int
stdhup_init(void)
{
	struct sigaction sa;

	if (!oldsa_saved) {
		sigaction(SIGHUP, NULL, &oldsa);
		oldsa_saved = 1;
	}

	sa.sa_flags = oldsa.sa_flags | SA_SIGINFO;
	sa.sa_mask = oldsa.sa_mask;
	sa.sa_sigaction = handle_sighup;
	return sigaction(SIGHUP, &sa, NULL);
}
