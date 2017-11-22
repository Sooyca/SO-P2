#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>
#include<string.h>
#include<unistd.h>



sem_t food_queue, meat_queue;

int food, meat;

void *cook_occupation(void *arg);
void *hunter_occupation(void *arg);

int main(int argc, char **argv) {
    /*	struct timespec to_check;
	to_check.tv_sec = 0;
	to_check.tv_nsec = 1000000; */
	srand(time(NULL));
	sem_init(&food_queue, 0, 1);
	sem_init(&meat_queue, 0, 1);
	meat = atoi(argv[3]);
	food = atoi(argv[4]);
	char cook_name[] = "cooknnnn";
	char hunter_name[] = "hunternnnn";
	char number[3];
	int set = 0;
	int check = 0;
	int number_of_hunters = atoi(argv[2]);
	int number_of_cooks = atoi(argv[3]);
	pthread_t hunter[number_of_hunters];
	pthread_t cook[number_of_cooks];
	for(set = 0; set < number_of_hunters; set++)
	{
		sprintf(number, "%d", set);
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			hunter[5 + i] = number[i];
		}		
		check = pthread_create(&hunter[set], NULL, hunter_occupation, (void*)&set);
		if(check)
		{
			fprintf(stderr, "Error in pthread_create() with %s return code: %d\n", hunter_name, check);
			exit(EXIT_FAILURE);
		}
	}
	set = 0;
	for(set = 0; set < number_of_cooks; set++)
	{
		sprintf(number, "%d", set);
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			cook_name[3 + i] = number[i];
		}
		check = pthread_create(&cook[set], NULL, cook_occupation, (void*)&set);
		if(check)
		{
			fprintf(stderr, "Error in pthread_create() with %s return code: %d\n", cook_name, check);
			exit(EXIT_FAILURE);
		}

	}
	/*
	int i = 0;
	while(i < 30)
	{
		sem_wait(&food_queue);
		sem_wait(&meat_queue);
		printf("%d, %d\n", food, meat);
		sem_post(&meat_queue);
		sem_post(&food_queue);
		nanosleep(&to_check, NULL);
		i++;
	}
	*/	
	return 0;
}

void *cook_occupation(void *arg)
{
	struct timespec night_length;
	night_length.tv_sec = 0;
	night_length.tv_nsec = 10000000;

	int meat_in_the_hand = 0;
	int produced_food = 0;
	int d = 0;
	for(d = 0; d < 365; d++)
	{
		sem_wait(&meat_queue);
		if(meat > 0)
		{
			meat--;
			meat_in_the_hand = 1;
		}
		sem_post(&meat_queue);

		if(meat_in_the_hand > 0)
		{
			produced_food = rand()%6 + 1;
			meat_in_the_hand = 0;
		}
		if(produced_food > 0)
		{
			sem_wait(&food_queue);
			food += produced_food;
			sem_post(&food_queue);
			produced_food = 0;
		}
		
		sem_wait(&food_queue);
		if(food > 0)
		{
			food--;
			sem_post(&food_queue);
		}
		else
		{
			sem_post(&food_queue);
			pthread_exit(NULL);
		}
		nanosleep(&night_length, NULL);
	}
	pthread_exit(NULL);
}

void *hunter_occupation(void *arg)
{
	struct timespec night_length;
	night_length.tv_sec = 0;
	night_length.tv_nsec = 10000000;

	int hunted_animal = 0;
	int d = 0;
	int animal_score = 0;
	int hunter_score = 0;
	for(d = 0; d < 365; d++)
	{
		animal_score = rand()%6 + 1;
		hunter_score = rand()%6 + 1;
		if(hunter_score > animal_score)
		{
			hunted_animal++;
		}
		if(hunted_animal > 0)
		{
			sem_wait(&meat_queue);
			meat++;
			sem_post(&meat_queue);
			hunted_animal = 0;
		}
		sem_wait(&food_queue);
		if(food > 0)
		{
			food--;
			sem_post(&food_queue);
		}
		else
		{
			sem_post(&food_queue);
			pthread_exit(NULL);
		}
		nanosleep(&night_length, NULL);
	}
	pthread_exit(NULL);
}
