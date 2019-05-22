//ECE 108 Lab 3 Part 2 - Cards (BlackJack)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//global variables/arrays
int deck_size = 52;
int num_deal = 0; //total number of times dealt
//start with a sorted deck with card values (ace is initialized to 11)
int deck[52] = {2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,
			7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11};

int high_cards, low_cards, middle_cards, bet_count = 0;
			
			
//implementing Fisher-Yates shuffling algorithm
void shuffle(int *A, int num_elements) //takes in an array (deck) with its total # of elements as parameters
{
	int i = 0;
	int temp;
	//starting from the last card of the deck, swap with another card randomly (determined by swapindex)
	//repeat until the element at index 1 has reached, thus finishing all the swapping of the deck
	for (i = num_elements - 1; i > 0; i--)
	{
		int swapindex = (rand() % (i + 1)); //generates a random number (swapindex) in the range [0, i] to swap with i
		temp = A[swapindex];
		A[swapindex] = A[i];
		A[i] = temp;
	}
}

//function for dealing a single card from the last card in the array at a time
//Used for Question 4
int deal_1(int *B)
{
	
	if (deck_size == 0)
	{
		deck_size = 52;
		shuffle(B, 52);
	}
	
	deck_size--;
	num_deal++;
	return B[deck_size];
}

//separate deal function used for bet_count for Question 5
int deal_2(int *C)
{
	if (deck_size == 0)
	{
		deck_size = 52;
		shuffle(C, 52);
		//need to reset these values to 0 every time you shuffle
		bet_count = 0;
		high_cards = 0;
		low_cards = 0;
		middle_cards = 0;
	}
	
	deck_size--;
	int dealt_card = C[deck_size];
	if (dealt_card == 10 || dealt_card == 11)
	{
		high_cards++;
	}
	else if (dealt_card == 2 || dealt_card == 3 || dealt_card == 4 || dealt_card == 5 || dealt_card == 6)
	{
		low_cards++;
	}
	else
	{
		middle_cards++;
	}
	num_deal++;
	return C[deck_size];
}

int remainingCards(num_deal)
{
	return (52 - num_deal); //remainingCards returns the number of cards in the deck array
}

//function for dealing cards for any participant until desired threshold has been met
int participant_hand(int participant, int threshold)
{
	int a = 0;
	int current_card = 0;
	bool flag1 = 1; //flags are used to distinguish between the number of aces drawn for each iteration
	bool flag2 = 1;
	bool flag3 = 1;
	bool flag4 = 1;
	int ace_drawn = 0;
	participant = 0;
	while (participant < threshold)
	{
		current_card = deal_2(deck);
		if (current_card == 11)
		{
			ace_drawn++;
		}
				
		if (ace_drawn == 0)
		{
			participant += current_card;
		}
				
		else if (ace_drawn == 1 && flag1 == 1)
		{
			flag1 = 0;
			if (participant < 10)
			{
				participant += 11;
			}
			else
			{
				participant += 1;
			}
		}
				
		else if (ace_drawn == 2 && flag2 == 1)
		{
			flag2 = 0;
			if (participant > 21)
			{
				participant = participant - 9; //if greater than 21 then need to set all two aces to 1 (-11+2=-9)
			}
			else
			{
				participant += 1; //otherwise just add the drawn ace but must of value 1 in order to not exceed 21
			}
		}
				
		else if (ace_drawn == 3 && flag3 == 1)
		{
			flag3 = 0;
			if (participant > 21)
			{
				participant = participant - 19;
			}
			else
			{
			participant += 1;
			}
		}
				
		else if (ace_drawn == 4 && flag4 == 1)
		{
			flag4 = 0;
			if (participant > 21)
			{
				participant = participant - 29;
			}
			else
			{
				participant += 1;
			}
				  
		}
				
		else
		{
			participant += current_card;
		}
	}
			
		a = participant;
		participant = 0;
		return a;
}

int main()
{
	srand((unsigned int)time(NULL)); //seeding the PRNG
	
	int player = 0; //indicates the sum of the card values in current player's hand
	int count = 0;
	int card = 0;
	int ace_drawn = 0; //counting how many aces are drawn in total
	int i, j = 0;

	//first shuffle the ordered deck once
	shuffle(deck, 52);

//Question 4
printf("Question 4:\n");
	for (i = 10; i < 21; ++i)
	{

		for (j = 0; j < 10000; ++j)
		{
		    bool flag1 = 1;
			bool flag2 = 1;
			bool flag3 = 1;
			bool flag4 = 1;
			ace_drawn = 0;
			player = 0;
			while (player < i)
			{
				card = deal_1(deck);
				if (card == 11)
				{
					ace_drawn++;
				}
				
				if (ace_drawn == 0)
				{
					player += card;
				}
				
				else if (ace_drawn == 1 && flag1 == 1)
				{
					flag1 = 0;
					if (player < 10)
					{
						player += 11;
					}
					else
					{
						player += 1;
					}
				}
				
				else if (ace_drawn == 2 && flag2 == 1)
				{
					flag2 = 0;
					if (player > 21)
					{
						player = player - 9;
					}
					else
					{
					player += 1;
					}
				}
				
				else if (ace_drawn == 3 && flag3 == 1)
				{
					flag3 = 0;
					if (player > 21)
					{
						player = player - 19;
					}
					else
					{
					player += 1;
					}
				}
				
				else if (ace_drawn == 4 && flag4 == 1)
				{
					flag4 = 0;
					if (player > 21)
					{
						player = player - 29;
					}
					else
					{
					player += 1;
					}
				}
				
				else
				{
					player += card;
				}
			}
			
			if (player > 21)
			{
				count++;
			}
			
		}
		
		float bust = (float)count / 10000;
		printf("%d", i);
		printf(": ");
		printf("%.6f\n", bust);
		bust = 0;
		count = 0;
	}
	
//Question 5
printf("Question 5:\n");
	int dealer, player1, player2, player3 = 0;
	player = 0;
	int victory = 0; //sum of total hands won for player
	int lose = 0;    //sum of total hands lost for player
	int tie = 0; //sum of total hands that resulted in a draw
	int player_result = 0;
	int dealer_result = 0;
	int num_hands = 10000;
	int bet_high, bet_low, bet_mid = 0;
	int result_win = 0; //used for detecting current hand's win/lose result
	int result_lost = 0; //used for detecting current hand's win/lose result
	int k, l, m, n = 0;
	int correct = 0;
	float bet_correct = 0;

	for (i = 0; i < num_hands; ++i)
	{
		//very beginning of each hand, bet high or low based on current count value
		if (bet_count >= 2)
		{
			bet_high++;
		}
		else if (bet_count <= -2)
		{
			bet_low++;
		}
		else
		{
			bet_mid++;
		}
		
		//reset bet_count to 0
		bet_count = 0;
		//each iteration represents a single hand(game)...
		//first deal a card to the dealer
		dealer = deal_2(deck);
		//then deal two cards to each of three other players
		for (k = 0; k < 2; ++k)
		{
			player1 += deal_2(deck);
		}
		for (l = 0; l < 2; ++l)
		{
			player2 += deal_2(deck);
		}
		for (m = 0; m < 2; ++m)
		{
			player3 += deal_2(deck);
		}
		//then deal cards to the player, lets say the threshold(stand) is 18 because based on Q4
		//the probability of busting is 40%, which is a decent risk to take
		player_result = participant_hand(player, 18);
		
		//first check if player busts, which automatically results in a loss
		if (player_result > 21)
		{
			lose++;
			result_lost++;
		}
		//then check if player hits blackjack, which automatically results in a win
		else if (player_result == 21)
		{
			victory++;
			result_win++;
		}
		//finally deal cards to the dealer until their hand is at least 17
		else
		{
			dealer_result = participant_hand(dealer, 17);
			//if dealer busts or player's final hand > dealer's hand, player wins
			if (dealer_result > 21 || player_result > dealer_result)
			{
				victory++;
				result_win++;
			}
			//else if dealer's hand > player's final hand, player loses
			else if (dealer_result > player_result)
			{
				lose++;
				result_lost++;
			}
			else
			{
				tie++;
			}
		}
		
		bet_count += (low_cards - high_cards);
		
		if ((bet_high > 0 && result_win > 0) || (bet_low > 0 && result_lost > 0))
		{
			correct++;
		}
		
		//reset values
		result_win = 0;
		result_lost = 0;
		bet_high = 0;
		bet_low = 0;
	}
	
	bet_correct = (float)correct / 10000;
	
	printf("%.6f\n", bet_correct);

	return 0;
}