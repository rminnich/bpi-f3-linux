CC=riscv64-linux-gnu-gcc
vmthread: vmthread.c Makefile
	$(CC) -g -o vmthread vmthread.c

test: vmthread
	./vmthread

vm: vmthread 
	./vmthread x

clean:
	rm -f vmthread
