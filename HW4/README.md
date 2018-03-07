please find the detailed report on D2L

(Below is the compile information and the corresponding directories for each problem)

### [Problem 2 - 40 pts] (IPC) Multiple Ways to communicate between two processes
- messages_queues (run p2.c first because p2 is used for initializing the message queue)
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
  ./p2.c
  ./p1.c
```

- shared_memory
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
  ./p1.c
  ./p2.c
```
- pipes
```
  gcc pipes.c -o ppipes.o -lrt -Wall
  ./pipes.o
```
- sockets (run p2.c first because p2 is the server to create the socket)
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
  ./p2.c
  ./p1.c
```

### [Problem 3 - 15 Pts] kThread Practice
- kthread_practice
```
  make all
  sudo insmod kthread_practice.ko 
  dmesg
```

### [Problem 4 - 15 pts] kThread API and Queues
- kthread_queues
```
  make all
  sudo insmod kthread_practice.ko 
  dmesg
```
