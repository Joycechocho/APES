please find the detailed report on D2L

(Below is the compile information and the corresponding directories for each prolem)

### [Problem 2 - 40 pts] (IPC) Multiple Ways to communicate between two processes
- messages_queues
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
```

- shared_memory
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
```
- pipes
```
  gcc pipes.c -o ppipes.o -lrt -Wall
```
- sockets
```
  gcc p1.c -o p1.o -lrt -Wall
  gcc p2.c -o p2.o -lrt -Wall
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
