#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#if 0
int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
                 /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
#endif
#define CLONE_VMTHREAD              0x100000000

int done(void*arg)
{
	printf("kid\n");
	return 0;
}

unsigned char stack[4096];

int main(int argc, char *argv[])
{
	int flags = CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|0x11;
	int ret;

	if (argc > 1)
		flags |= CLONE_VMTHREAD;

	ret = clone(done, &stack[4095], flags, 0);
	printf("clone returns %d, errno %d\n", ret, errno);
}

