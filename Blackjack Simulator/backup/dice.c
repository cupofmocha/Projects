//ECE 108 Lab 3 Part 1 - Dice Roll

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//RNG for rolling first die
int rollDice1(int min, int max)
{
	int rn1 = min + rand() / (RAND_MAX / (max - min + 1) + 1); //generates a random number between 1 and 6 for die
	return rn1;
}

//RNG for rolling second die
int rollDice2(int min, int max)
{
	int rn2 = min + rand() / (RAND_MAX / (max - min + 1) + 1); //generates a random number between 1 and 6 for die
	return rn2;
}

int main() 
{	
	int i, j, k = 0;
	int n_times = 10; //rolls the dice n times
	int n_experiment = 10000; //performs the experiment of rolling a die a hundred times,
	                          //for 10,000 experiments
	srand((unsigned int)time(NULL)); //seeding the PRNG
	
//Question 1
printf("Question 1:\n");
	float face1, face2, face3, face4, face5, face6 = 0;
	float Faces[6] = {face1, face2, face3, face4,face5, face6};
	
	for (i = 0; i < 3; ++i)
	{
		//each new experiment will need to reset the previous face value occurences to 0
		Faces[0] = 0;
		Faces[1] = 0;
		Faces[2] = 0;
		Faces[3] = 0;
		Faces[4] = 0;
		Faces[5] = 0;
		
		for (j = 0; j < n_times; ++j)
		{
			int result = rollDice1(1, 6);
		
			switch(result)
			{
				case 1:
					Faces[0] += 1;
					break;
				case 2:
					Faces[1] += 1;
					break;
				case 3:
					Faces[2] += 1;
					break;
				case 4:
					Faces[3] += 1;
					break;
				case 5:
					Faces[4] += 1;
					break;
				case 6:
					Faces[5] += 1;
					break;
				default:
					printf("Error! No matching dice face.");
			}
		}
		
		printf("%d", n_times);
		printf(" times\n");
		//converting # of occurences of each face to fraction of times (float)
		//and printing the result as type float
		for (k = 0; k < 6; ++k)
		{
			Faces[k] /= n_times;
			printf("fraction of times face %d", k+1);
			printf(" shows up: %.6f\n", Faces[k]);
		}
		
		n_times *= 100; //next experiment will have 100 times # of dice rolls that of previous experiment
	}
	
//Question 2
printf("Question 2:\n");
	int low_numbers = 0;
	int high_numbers = 0;
	int count = 0; //calculates # of times # of low_numbers == # of high_numbers in each experiment

	for (i = 0; i < n_experiment; ++i)
	{
		low_numbers = 0;
		high_numbers = 0;
		for (j = 0; j < 100; ++j)
		{
			int a = rollDice1(1, 6);
		
			switch(a)
			{
				case 1:
					low_numbers += 1;
					break;
				case 2:
					low_numbers += 1;
					break;
				case 3:
					low_numbers += 1;
					break;
				case 4:
					high_numbers += 1;
					break;
				case 5:
					high_numbers += 1;
					break;
				case 6:
					high_numbers += 1;
					break;
				default:
					printf("Error! No matching dice face.");
			}
		}
		if (low_numbers == high_numbers)
			{
				count++;
			}
	}
	
	printf("%d\n ", low_numbers);
	printf("%d\n ", high_numbers);
	float evensplit = (float)count / n_experiment; //fraction of times # of low_numbers == # of high_numbers after experiments are done
	
	printf("fraction of times # of low_numbers equals to # of high_numbers: %.6f\n", evensplit);
	
//Question 3
printf("Question 3:\n");
	float totalvalue_2 = 0;
	float totalvalue_3 = 0;
	float totalvalue_4 = 0;
	float totalvalue_5 = 0;
	float totalvalue_6 = 0;
	float totalvalue_7 = 0;
	float totalvalue_8 = 0;
	float totalvalue_9 = 0;
	float totalvalue_10 = 0;
	float totalvalue_11 = 0;
	float totalvalue_12 = 0;
	float TotalValue[11] = {totalvalue_2, totalvalue_3, totalvalue_4, totalvalue_5, totalvalue_6, 
	                        totalvalue_7, totalvalue_8, totalvalue_9, totalvalue_10, totalvalue_11, totalvalue_12};

	for (i = 0; i < 10000; ++i)
	{
		int twodice_result = rollDice1(1, 6) + rollDice2(1, 6);
		
		switch(twodice_result)
		{
			case 2:
				TotalValue[0] += 1;
				break;
			case 3:
				TotalValue[1] += 1;
				break;
			case 4:
				TotalValue[2] += 1;
				break;
			case 5:
				TotalValue[3] += 1;
				break;
			case 6:
				TotalValue[4] += 1;
				break;
			case 7:
				TotalValue[5] += 1;
				break;
			case 8:
				TotalValue[6] += 1;
				break;
			case 9:
				TotalValue[7] += 1;
				break;
			case 10:
				TotalValue[8] += 1;
				break;
			case 11:
				TotalValue[9] += 1;
				break;
			case 12:
				TotalValue[10] += 1;
				break;			
			default:
				printf("Error! No matching values.");
		}
	}
	
		for (j = 0; j < 11; ++j)
			{
				TotalValue[j] /= 10000;
				printf("fraction of times face %d", j+2);
				printf(" shows up: %.6f\n", TotalValue[j]);
			}
		
	return 0;
}