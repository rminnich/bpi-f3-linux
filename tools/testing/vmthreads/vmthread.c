#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <linux/sched.h>

long clone3(struct clone_args *cl_args, size_t size);

#define CLONE_VMTHREAD              (uint64_t)0x100000000

int done(void*arg)
{
	printf("kid\n");
	return 0;
}

unsigned char stack[4096];

int main(int argc, char *argv[])
{
	uint64_t flags = CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|0x11;
	int ret;

	printf("iszeof int %ld\n", sizeof(int));
	if (argc > 1)
		flags |= CLONE_VMTHREAD;

	printf("flags %#lx\n", flags);

	struct clone_args *args = calloc(sizeof(*args), 1);
	args->flags = flags;

	ret = syscall(435/*__NR_clone3*/, args, sizeof(args));
	printf("clone returns %d, errno %d\n", ret, errno);
}

