obj-m += kthread_queues.o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
