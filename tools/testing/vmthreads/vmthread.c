#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

// clone3
#include <linux/sched.h>
#include <signal.h>
#include <string.h>

// wait
#include <sys/types.h>
#include <sys/wait.h>

// open
#include <sys/stat.h>
#include <fcntl.h>

// errx
#include <err.h>

long clone3(struct clone_args *cl_args, size_t size);

#define CLONE_VMTHREAD              (uint64_t)0x100000000
#define __NR_clone3 435

unsigned char stack[4096];

int main(int argc, char *argv[])
{
	struct clone_args args;
	int pid = getpid();
	int fpid = open("/sys/kernel/tracing/set_ftrace_pid", 2);
	if (fpid < 0) {
		errx(1, "can't open set_ftrace_pid");
	}

	char *buf = calloc(512, 1);
	buf[0] = '\n';
	if (write(fpid, buf, 1) < 0){
		errx(1, "can't write newline to fpid\n");
	}
	sprintf(buf, "%d\n", pid);
	if (write(fpid, buf, strlen(buf)) < 0){
		errx(1, "can't write %s to fpid\n", buf);
	}

	memset(&args, 0, sizeof(args));
	
	/* Required: child exit signal */
	args.exit_signal = SIGCHLD;
	if (argc > 1)
		args.flags |= CLONE_VMTHREAD;
	
	long ret = syscall(__NR_clone3,  &args, sizeof(args));
	
	switch (ret) {
	case 0:
		/* Child */
		printf("child: pid=%d ppid=%d\n", getpid(), getppid());
	case -1:
		perror("clone3");
	default:
		wait(NULL);
		/* Parent */
		printf("parent: child pid=%ld\n", ret);
		system("cat /sys/kernel/tracing/trace");
	}

	return 1;
}
