// ECE 108 Lab 2 -- Sat Solver
//
// Read logical expressions in DIMACS format from files specified on the
// command line, and for each one print its name and one of "satisfiable", "unsatisfiable" or "tautology"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dimacs_reader.h"

//# variables = # of columns
//# clauses = # of rows, each row has certain number of literals (collection of variables)

//global variables/arrays
int expression[MAXCLAUSES][MAXVARS*2]; //initializing 2D array with fixed size of MAXCLAUSES and MAXVAR*2
                                      //due to each variable having complemented or uncomplemented expression
int numLiterals[MAXCLAUSES]; //numLiterals is an array that stores each clause's #literals
int totalVar; //total number of variables
int totalClause; //total number of clauses
int targetClause; //the target clause which is to be added with literals

//initialization function
void init(int numvars, int numclauses) {

	totalVar = numvars; //initialize totalVar, which equals to the total # of variables in a file
	totalClause = numclauses; //initialize totalClause
	targetClause = 0; //initialize current clause index to 0

	//read_dimacs(test1.cnf);
	// Callback function that gets passed to read_dimacs().
	// Indicates start of a new expression with up to numvars variables
	// and numclauses clauses.
}

void add_clause(int numlits, int literals[]) {
	
	//{[1,2] [4,3] [3,2] ...} --- example of a clause{} containing literals[] which contains variables 1,2,3 etc.
	int i = 0;
	while (i < numlits)
	{
		expression[targetClause][i] = literals[i]; //copying literals[] to each targetClause(row)
		++i;
	}
	numLiterals[targetClause] = numlits; //assigning the index (numlits) of each literal to corresponding targetClause
	targetClause++; //moves to the next clause to copy literals
	
	// Callback function that gets passed to read_dimacs().
	// The literals argument is an array of ints, numlits is its length.
	// Each literal is the index (starting from 1) of a variable.
	// Negative integers represent logical negations of variables.
}

//power function
int power(int base, int exponent) 
{
	int i = 1;
	int result = base;
	while (i < exponent)
	{
		result *= base;
		++i;
	}
	return result;
}

char *classify() {
	// Evaluate the current ***expression*** and return one of
	// "satisfiable", "unsatisfiable" or "tautology".
	//satisfiable means for at least 1 combination, the expression evaluates to true
	//tautology means for every combination, the expression must evaluate to true
	
	int totalCombination = power(2, totalVar); //total number of combinations of each file, which equals to 2^totalVar

	//for each clause, convert every possible combination in binary (2^32) to their binary representation (truth table) for the end result comparison
	int i = 0;
	int j = 0;
	int k = 0;
	bool satisfiable = false;
	bool tautology = true;
	
	for (i = 0; i < totalCombination; ++i) //loops for every possible combination of each file depending on the number of variables in that file, because for tautolog
	{
		bool combination = true;
		
		//loops for every clause (# loops = totalClause-1), and reads each file from bottom to top to reduce time
        //reason for Descending order is to read each file from bottom to top; in this particular case reduces a lot of time because many of them have one true clause at the END of the file,
		//therefore by going bottom to top you can avoid checking for the rest of the clauses as satisfiability has already been evaluated to true
        for (j = totalClause-1; j >= 0; --j)		
		{
			int clauseResult = 0; //the truth value of the clause after performing OR on every literal's combinations in each clause, initialized to 0
			
			for (k = 0; k < numLiterals[j]; ++k) //loops for every literal within each clause
			{
				//if the variable (represented by expression[j][k])is positive
				if (expression[j][k] > 0)
				{
					clauseResult += (i >> (expression[j][k] - 1)) & 1; //***bitshift each combination (i) right to the least significant bit, then perform bitwise AND with 1 to obtain each variable's value: 0 or 1
				}                                                      //then clauseResult performs OR (effectively adding up the variables' values) to see if it is greater than 0 (true) or equal to 0 (false) 
				
				//if the variable is negative
				if (expression[j][k] < 0)
				{
					int temp = (i >> (expression[j][k]*-1 - 1)) & 1;
					
					//for negative variable, first make it positive and store in temp, then need to invert the result of temp: if 1 then invert to 0 and vice-versa 
					if (temp == 1)
					{
						clauseResult += 0;
					}
					
					else
					{
						clauseResult += 1;
					}
				}
				
				//if any particular clauseResult (result of performing OR on n # of combinations) is positive, then no need to continue evaluating the remaining literals for that clause 
				if (clauseResult > 0)
				{
					break; //moves on to the next clause to save time...
				}
				
			}
			
			   //if any particular clauseResult is 0, then no need to continue checking the remaining clauses because this implies the so far evaluated combination is false and thus tautology is false,
			   //but need to move on to a different combination to check for satisfiability
			if (clauseResult == 0)
	        {
			  combination = false;
		      tautology = false;
			  break;
	        }
		}
		
		//finally, if the final evaluated combination is true through looping for certain number of combinations, if combination is true then it is satisfiable
		if (combination == true)
		{
			satisfiable = true;
		}
		
		//However, if satisfiable is deemed true but tautology is deemed false, then immediately break out of the loop because our goal of evaluating satisfiability and tautology has already been completed!
		if (tautology == false && satisfiable == true)
		{
			break;
		}
	}
	
	//output results based on evaluation results, order matters, tautology needs to be before satisfiable for printing results
	if (tautology == true)
	{
		return "tautology";
	}
	
	if (satisfiable == true)
	{
		return "satisfiable";
	}
	
	return "unsatisfiable";

}

//argc contains all the files that read_dimacs will read, while argv are the data contained within each file to be passed to init function
int main(int argc, char *argv[]) {
	int i;
	for (i = 1; i < argc; ++i) {
		read_dimacs(argv[i], init, add_clause);
		printf("%s: %s\n", argv[i], classify());
	}
	return 0;
}

