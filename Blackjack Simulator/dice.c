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
	int n_times = 24; //rolls the dice n times
	int n_experiment = 10000; //performs the experiment of rolling a die a hundred times,
	                          //for 10,000 experiments
	srand((unsigned int)time(NULL)); //seeding the PRNG
	
//Question 1
printf("Question 1:\n");
	float face1, face2, face3, face4, face5, face6 = 0;
	float Faces[6] = {face1, face2, face3, face4,face5, face6};
	
	
		
		for (j = 0; j < n_times; ++j)
		{
			int result = rollDice1(1, 6);
		
			switch(result)
			{
				case 1:
					Faces[0] += 1;
					printf("%d", 1);
					printf(", ");
					break;
				case 2:
					Faces[1] += 1;
					printf("%d", 2);
					printf(", ");
					break;
				case 3:
					Faces[2] += 1;
					printf("%d", 3);
					printf(", ");
					break;
				case 4:
					Faces[3] += 1;
					printf("%d", 4);
					printf(", ");
					break;
				case 5:
					Faces[4] += 1;
					printf("%d", 5);
					printf(", ");
					break;
				case 6:
					Faces[5] += 1;
					printf("%d", 6);
					printf(", ");
					break;
				default:
					printf("Error! No matching dice face.");
			}
		}
		
	return 0;
}