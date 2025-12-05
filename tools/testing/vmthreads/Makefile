CC?=gcc
vmthread: vmthread.c
	$(CC) -o vmthread vmthread.c

test: vmthread
	./vmthread

vm: vmthread 
	./vmthread x

clean:
	rm -f vmthread
