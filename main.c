//Bartosz Sójka
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

#define OUTPUT 0  //ustawienie na 1 spowoduje przekazywanie mniej więcej co jeden dzień liczby zwierzyny oraz jedzenia na standardowe wyjście

#define NIGTH_LENGTH 10000000

pthread_mutex_t food_queue = PTHREAD_MUTEX_INITIALIZER, meat_queue = PTHREAD_MUTEX_INITIALIZER;

int food, meat;

void *cook_occupation(void *arg);
void *hunter_occupation(void *arg);

int main(int argc, char **argv) {
	int number_of_hunters = atoi(argv[1]);
	int number_of_cooks = atoi(argv[2]);
	#if OUTPUT    
		struct timespec to_check;
		to_check.tv_sec = 0;
		to_check.tv_nsec = NIGTH_LENGTH;
	#endif
	srand(time(NULL));
	meat = atoi(argv[3]);
	food = atoi(argv[4]);
	char cook_name[] = "cooknnnn";
	char hunter_name[] = "hunternnnn";
	char number[3];
	int set = 0;
	int check = 0;
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
	#if OUTPUT
		int i = 0;
		int c_meat, c_food;
		while(i < 367)
		{
			pthread_mutex_lock(&food_queue);
				c_food = food;
			pthread_mutex_unlock(&food_queue);
			pthread_mutex_lock(&meat_queue);
				c_meat = meat;
			pthread_mutex_unlock(&meat_queue);
			printf("%d %d\n",  c_meat, c_food);
			nanosleep(&to_check, NULL);
			i++;
		}	
	#endif	
	return 0;
}


void *hunter_occupation(void *arg)
{
	struct timespec night_length;
	night_length.tv_sec = 0;
	night_length.tv_nsec = NIGTH_LENGTH;

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
			pthread_mutex_lock(&meat_queue);
				meat++;
			pthread_mutex_unlock(&meat_queue);
			hunted_animal = 0;
		}
		pthread_mutex_lock(&food_queue);
		if(food > 0)
		{
			food--;
			pthread_mutex_unlock(&food_queue);
		}
		else
		{
			pthread_mutex_unlock(&food_queue);	
			pthread_exit(NULL);
		}
		nanosleep(&night_length, NULL);
	}	
	pthread_exit(NULL);
}
void *cook_occupation(void *arg)
{
	struct timespec night_length;
	night_length.tv_sec = 0;
	night_length.tv_nsec = NIGTH_LENGTH;

	int meat_in_the_hand = 0;
	int produced_food = 0;
	int d = 0;
	for(d = 0; d < 365; d++)
	{
		pthread_mutex_lock(&meat_queue);
		if(meat > 0)
		{
			meat--;
			meat_in_the_hand = 1;
		}
		pthread_mutex_unlock(&meat_queue);

		if(meat_in_the_hand > 0)
		{
			produced_food = rand()%6 + 1;
			meat_in_the_hand = 0;
		}
		if(produced_food > 0)
		{
			pthread_mutex_lock(&food_queue);
				food += produced_food;
			pthread_mutex_unlock(&food_queue);
			produced_food = 0;
		}
		
		pthread_mutex_lock(&food_queue);
		if(food > 0)
		{
			food--;
			pthread_mutex_unlock(&food_queue);
		}
		else
		{
			pthread_mutex_unlock(&food_queue);
			pthread_exit(NULL);
		}
		nanosleep(&night_length, NULL);
	}	
	pthread_exit(NULL);
}
