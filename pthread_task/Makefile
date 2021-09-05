all: pthread_task.o
	gcc -o pthread_task pthread_task.o -lpthread -Wall -Werror -Wextra
pthread.o:
	gcc -c pthread_task.o pthread_task.c
clean:
	rm pthread_task.o pthread_task
