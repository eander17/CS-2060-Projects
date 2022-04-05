/*

Evan Anderson
CS 2060-002
Prof Harding
11/5/2021

this program takes four character array pointers with words filled in them, 
creates a file called "randomSentences.txt", 
proceeds to create a sentence from the different words concatenated together with spaces in between them. 
After creating the sentence, it is printed into the text file. 
This sentence process is repeated FORLENGTH times. 
Finally, it closes the file and the program ends. 

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define FORLENGTH 20
#define LENGTH 5
#define SENTENCE_LENGTH 100
#define MAX_WORD_LENGTH 8


int main(void)
{
	srand(time(0)); //random number seed

	
	//File pointer, allows access to files and their manipulation. 
	FILE* filePtr; 


	//character pointer arrays with predetermined values. 
	char* articlePtr[] = { "the", "a", "one", "some", "any" }; 
	char* nounPtr[] = { "baby", "bunny", "dog", "town", "car" }; 
	char* verbPtr[] = { "drove", "jumped", "ran", "walked", "skipped" }; 
	char* prepositionPtr[] = { "to", "from", "over", "under", "on" }; 


	if ((filePtr = fopen("randomSentences.txt", "w")) == NULL) //attempts to open file randomSentences.txt, if it fails (null pointer) prints a message. Otherwise performs rest of program. 
	{
		puts("File could not be opened.");
	}
	else
	{
	
		puts("Printing to file."); 

		//repeat sentence building for a predetermined number of times
		for (size_t i = 0; i < FORLENGTH; i++)
		{
			//character array that holds the entire sentence. 
			char sentence[SENTENCE_LENGTH];
			strcpy(sentence, articlePtr[rand() % LENGTH]); //copies a random index in articlePtr into sentence array. 
			sentence[0] = toupper(sentence[0]); //makes the zeroth index character in sentence uppercase. 
			strcat(sentence, " "); //appends a space to sentence. 
			strcat(sentence, nounPtr[(rand() % LENGTH)]); //concatenates a random word from nounPtr into sentence array. 
			strcat(sentence, " "); //appends a space to sentence. 
			strcat(sentence, verbPtr[(rand() % LENGTH)]); //concatenates a random word from verbPtr onto the end of sentence array. 
			strcat(sentence, " "); //appends a space to sentence. 
			strcat(sentence, prepositionPtr[(rand() % LENGTH)]); //appends a random word from prepositionPtr to the end of sentence array. 
			strcat(sentence, " "); //appends a space to sentence. 
			strcat(sentence, articlePtr[(rand() % LENGTH)]); //appends a random word from articlePtr to the end of the sentence array. 
			strcat(sentence, " "); //appends a space to sentence. 
			strcat(sentence, nounPtr[(rand() % LENGTH)]); //appends a random word from nounPtr to the end of the sentence array. 
			strcat(sentence, "."); 

			fprintf(filePtr, "%s\n", sentence); //prints sentence to file. 
		}

		puts("Printed to randomSentences.txt"); 

		fclose(filePtr); //closes randomSentences.txt file. 

	}


	return 0; 
}