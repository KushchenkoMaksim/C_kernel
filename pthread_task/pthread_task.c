/*
5. Винни-Пух и пчелы. Заданное количество пчел добывают мед равными порциями,
задерживаясь в пути на случайное время. Винни-Пух потребляет мед порциями
заданной величины за заданное время и столько же времени может прожить без
питания. Работа каждой пчелы реализуется в порожденном процессе.

Сборка:
make all

Запуск:
(Нормальные параметры):
./pthread_task 10 10 100 5
(Экстримальные параметры):
./pthread_task 5 20 100 3
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {
	pthread_mutex_t dataMutex;
	int finish;//Flag to finish bees and bear thread
	int size;//Data
} shared;

typedef struct {
	shared *container;
	int beeEfficiency;
} beeArgs;

typedef struct {
	shared *container;
	int bearEfficiency;
	int bearEatingTime;
} bearArgs;


//Waiting for threads and free id array
void clearThreads(pthread_t *threads, int N) {
	for(int i = 0; i < N; ++i)
		pthread_join(threads[i], NULL);
	free(threads);
}

//Actions that need to be done before exit
void beforeExit(pthread_t *threads, int N, shared *container) {
	//Setting flag to finish bee and bear threads
	pthread_mutex_lock(&(container->dataMutex));
	container->finish = 1;
	pthread_mutex_unlock(&(container->dataMutex));

	clearThreads(threads, N);
}


void bee(void *input) {
	beeArgs * args = (beeArgs *)input;

	printf("Bee with number %lu start working\n", pthread_self());
	while(!args->container->finish) {
		pthread_mutex_lock(&(args->container->dataMutex));
		args->container->size += args->beeEfficiency;
		printf("Bee with number %lu add honey, current amount is %d\n", pthread_self(), args->container->size);
		pthread_mutex_unlock(&(args->container->dataMutex));
		sleep(rand()%10 + 1);
	}
	printf("Bee with number %lu stop working\n", pthread_self());
}

void bear(void *input) {
	bearArgs * args = (bearArgs *)input;

	int hunger = 0;
	int currentPortion = args->bearEfficiency;

	printf("Bear start eating\n");
	while(!args->container->finish) {
		sleep(args->bearEatingTime);

		pthread_mutex_lock(&(args->container->dataMutex));
		//if we have enough honey
		if(args->container->size >= currentPortion) {
			args->container->size -= currentPortion;
			hunger = 0;
		}
		//if we don't have enough honey
		else {
			hunger = currentPortion - args->container->size;
			args->container->size = 0;
			//If hunger is at critical level
			if(hunger >= args->bearEfficiency) {
				args->container->finish = 1;
			}
		}
		printf("Bear ate honey, current amount: %d, current hunger: %d\n", args->container->size, hunger);
		pthread_mutex_unlock(&(args->container->dataMutex));
		currentPortion = args->bearEfficiency + hunger;
	}
	printf("Bear died from hunger\n");
}

int main(int argc, char * argv[]) {
	if(argc < 5) {
		perror("Not enough parametrs\n");
	}
	int beeCount = atoi(argv[1]);
	int beeEfficiency = atoi(argv[2]);
	int bearEfficiency = atoi(argv[3]);
	int bearEatingTime = atoi(argv[4]);
	shared container = {PTHREAD_MUTEX_INITIALIZER, 0, 0};

	srand(time(0));
	//Array of thread ids
	pthread_t * threadIds = malloc(sizeof(pthread_t) * (beeCount + 1));

	//Create bees
	beeArgs locBeeArgs = {&container, beeEfficiency};
	for(int i = 0; i < beeCount; ++i) {
		if(pthread_create(threadIds + i, NULL, (void *)bee, (void *)&(locBeeArgs))){
			perror("Can't create bee\n");
			beforeExit(threadIds, i, &container);
			exit(-1);
		}
	}

	//Create bear
	bearArgs locBearArgs = {&container, bearEfficiency, bearEatingTime};
	if(pthread_create(threadIds + beeCount, NULL, (void *)bear, (void *)&(locBearArgs))) {
		perror("Can't create bear\n");
		beforeExit(threadIds, beeCount, &container);
		exit(-1);
	}

	clearThreads(threadIds, beeCount + 1);
	return 0;
}