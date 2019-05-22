# Blackjack Simulator (cards.c file)

<h2> Purpose: </h2>
<p>
  This is a simple blackjack game simulator featuring the player (you), 3 other players, as well as the dealer. The program has the ability of calculating the probability of busting the player's hand when reaching certain thresholds (e.g. a hand of 18 points), and from that the player can make a sound judgment whether he/she should take the risk of calling another card, or choose to stand and conclude the current round.
</p>

<h2> How it works </h2>
<p>
  Card dealing: implements the Fisher-Yates shuffling algorithm using rand() function to ensure fairness of card dealing.
  Calculating probability: utilizes the hi-lo card counting algorithm, which tells the player whether he/she should bet high or low based on his/her current hand value. This is also influenced by the threshold value that we choose (e.g. max 18 points). Then this experiment is run say, 1000 times, and we can see how accurate the player is at betting high or low respectively.
</p>

<h2> Interesting results </h2>
<p>
  From the results, it appears that reaching 18 points or less is a reasonable threshold to call stand on the current round. The reasoning is because it is
pretty close to the blackjack value of 21, with a relatively low busting probability (≈40%).
I would not want to consider thresholds 19 or above since their busting probability are all
greater than 50%, which means that the player is more likely to bust than to be safe. (More interesting results can be found in the report.txt file)
</p>
