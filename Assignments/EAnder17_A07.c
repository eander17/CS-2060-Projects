
/*
Evan Anderson
Prof Harding
CS 2060 - 002
Assignment 07: Pets Linked List

Write a program that asks a user to enter Pet information and display them to the user in alphabetical then age order. 

	1. Create a structure for Pets. 
		a. Pet name
		b. Pet age
		c. next pointer
	2. create a linked list for each instance of Pet struct. 
	3. have program ask user how many Pets are to be entered. 
	4. ask user for Pet name and age
	5. sort list by alphabetical order
	6. display names and ages in alphabetical order. 
	7. display names and ages in age order (without changing list)
	8. ask user if they want to remove Pet with a Y/N
		a. loop until user enters no (or if there are no Pets left). 
		b. delete if in list
		c. print remaining Pets in alphabetical order. 

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NAME_LENGTH 25

//Pet struct
typedef struct Pet
{
	char* name;
	unsigned int age; 
	struct Pet* nextPtr; //to point to next Pet. 
}Pet;

//function prototypes
void printList(Pet* listPtr);
void printNumerically(Pet* listPtr, int numPets); 
void insertPet(Pet** headPtr); 
void deletePet(Pet** headPtr, char* petToDelete); 
void petDeleter(Pet** headPtr);
void initializePet(Pet* petPtr); 
void alphabetizePet(Pet** headPtr, Pet* petPtr); 
int decideNumPets(); 
bool validateScanf(int scanValue); 
void sortList(Pet* sourcePet); 
void swapper(Pet* pet1, Pet* pet2); 
Pet* copyList(Pet* head);
bool validator(); 



int main(void)
{
	Pet* headPtr = NULL; 
	unsigned int numPets = 0; 

	numPets = decideNumPets(); //determine number of Pets to be entered. 

	//create number of pets equal to numPets. 
	for (size_t i = 0; i < numPets; i++)
	{
		insertPet(&headPtr); 
	}

	//print alphabetical list
	printList(headPtr); 

	//print numerically ordered list
	printNumerically(headPtr, numPets); 

	//allows user to remove pets from list. 
	petDeleter(&headPtr); 

	//see new list. 
	printList(headPtr); 

	return 0; 
}


//function definitions

/// <summary>
/// obtain input from user for how many Pets they are entering, returns the number of Pets. 
/// </summary>
/// <returns></returns>
int decideNumPets()
{
	unsigned int numToReturn = 0; 	
	 
	do
	{
		printf("%s", "Enter number of Pets: ");

		int scanValue = (scanf("%d", &numToReturn));
		while (getchar() != '\n');
		if (validateScanf(scanValue))
		{
			if (numToReturn < 1)
			{
				puts("Must enter a number greater than zero.\n");
			}
		}
	} while (numToReturn < 1); 

	return numToReturn; 
}

/// <summary>
/// inserts new pet struct in correct alphabetical location in list. 
/// </summary>
/// <param name="headPtr"></param>
void insertPet(Pet** headPtr)
{
	//allocate memory for Pet in the heap
	Pet* petPtr = malloc(sizeof(Pet)); 

	if (petPtr != NULL) //check for nullptr
	{
		initializePet(petPtr); //function call to set values of new pet. 
		petPtr->nextPtr = NULL; 
		alphabetizePet(headPtr, petPtr); //function call to place struct object in correct location in list. 
	}
	else
	{
		puts("No memory to create node."); 
	}
}

/// <summary>
/// sets initial values of new Pet object. 
/// </summary>
/// <param name="petPtr"></param>
void initializePet(Pet* petPtr)
{
	unsigned int petAge = 0; 
	char* petName = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); //allocate some memory for new Pet. 
	bool valid = false; 

	do//getting pet's name. 
	{
		printf("%s", "Enter pet name: ");
		int scanValue = (scanf("%s", petName));
		while (getchar() != '\n');
		valid = validateScanf(scanValue); 
	} while (!valid); 
	valid = false; 
	do//getting pet's age. 
	{
		printf("%s", "Enter pet age: "); 
		int scanValue = (scanf("%u", &petAge));
		while (getchar() != '\n'); 
		if (validateScanf(scanValue))
		{
			if (petAge < 0)
			{
				puts("Invalid age, must be a number equal to or greater than zero.");
			}
			else
				valid = true;
		}		
	} while (!valid); 

	*petName = tolower(*petName); //setting name to all lowercase letters. 
	//putting values into pet object. 
	petPtr->name = petName; 
	petPtr->age = petAge; 
}

/// <summary>
/// places pet object in alphabetically correct location in linked list. 
/// </summary>
/// <param name="headPtr"></param>
/// <param name="petPtr"></param>
void alphabetizePet(Pet** headPtr, Pet* petPtr)
{
	//helper pointers to determine where petPtr should go. 
	Pet* previousPtr = NULL; 
	Pet* currentPtr = *headPtr; 
	int stringCompare = 1; 
	if (currentPtr != NULL)
	{
		//checking to see if petPtr should go at the front of the list. 
		stringCompare = strcmp(petPtr->name, currentPtr->name);
	}

	//iterate through the list comparing names. 
	//will continue to iterate until either end of list is reached, or petPtr is greater alphabetically (closer to z)
	while (currentPtr != NULL && stringCompare >= 1)
	{
		stringCompare = strcmp(petPtr->name, currentPtr->name); //compare petPtr name to name in current location. 
		previousPtr = currentPtr;
		currentPtr = currentPtr->nextPtr; //move to next pointer in list. 
		
	}


	if (previousPtr == NULL)
	{
		*headPtr = petPtr; 
	}
	else
	{
		previousPtr->nextPtr = petPtr; 
	}

	petPtr->nextPtr = currentPtr; 

}

/// <summary>
/// prints entire list of pets alphabetically. 
/// </summary>
/// <param name="listPtr"></param>
/// <param name="numPets"></param>
void printList(Pet* listPtr)
{
	if (listPtr != NULL) //checking for empty list.
	{
		printf("%s\n", "The List in alphabetical order is: "); 
		Pet* currentPtr = listPtr; 
		
		//goes through entire list and prints each one, then moves current pointer to next pointer in list. 
		while (currentPtr != NULL)
		{
			printf("Name: %s\tAge: %u\n", currentPtr->name, currentPtr->age); 
			currentPtr = currentPtr->nextPtr; 
		}
	}
	else
	{
		puts("List is empty"); 
	}
}

/// <summary>
/// creates a copy of the list, 
/// sorts it from lowest to highest age, then prints off that new list in numerical order.
///  then copy's memory is freed. 
/// </summary>
/// <param name="sourcePet"></param>
void printNumerically(Pet* listPtr, int numPets)
{
	

	if (listPtr != NULL ) //checking for empty list. 
	{
		puts("Pets in order of age youngest to eldest: "); 
		Pet* lowestAgePtr = malloc(sizeof(Pet)); //allocating memory for new ptr. 
		lowestAgePtr = copyList(listPtr); //copy masterlist. 
		sortList(lowestAgePtr); //uses bubblesort algorithm to sort new list. 
		while (lowestAgePtr != NULL) //iterate through new list and print off each index. 
		{
			printf("Name: %s\tAge: %u\n", lowestAgePtr->name, lowestAgePtr->age); 
			lowestAgePtr = lowestAgePtr->nextPtr; 
		}
		free(lowestAgePtr); //delete. 
	}
	else
	{
		puts("The list is empty."); 
	}
}

/// <summary>
/// bubble sort algorithm to sort list by age from youngest to eldest. 
/// </summary>
void sortList(Pet* sourcePet)
{
	int swapped = 0;
	//placeholder ptrs used to perform swaps.
	Pet* pet1 = malloc(sizeof(Pet));  
	Pet* pet2 = NULL;

	if (sourcePet != NULL && pet1 != NULL) //checking for empties. 
	{
		do
		{
			swapped = 0;
			pet1 = sourcePet;

			while (pet1->nextPtr != pet2)
			{
				if (pet1->age > pet1->nextPtr->age) //check to see if pet1 age is greater than the next node's age. 
				{
					swapper(pet1, pet1->nextPtr); //perform swap between adjacent nodes. 
 					swapped = 1;
				}
				pet1 = pet1->nextPtr;
			}
			pet2 = pet1;
		} while (swapped);
	}
}

/// <summary>
/// swaps two pet objects. 
/// </summary>
/// <param name="headPtr"></param>
/// <param name="petToDelete"></param>
void swapper(Pet* pet1, Pet* pet2)
{
	char* tempName = pet1->name;
	int tempAge = pet1->age;

	pet1->name = pet2->name;
	pet1->age = pet2->age;

	pet2->name = tempName; 
	pet2->age = tempAge;
}

/// <summary>
/// makes a copy of a pet list. 
/// </summary>
/// <returns></returns>
Pet* copyList(Pet* head)
{
	if (head != NULL) //check for empty list. 
	{
		Pet* newList = (Pet*)malloc(sizeof(Pet)); 

		//copy current node. 
		newList->name = head->name; 
		newList->age = head->age; 

		//recursively copy next node. 
		newList->nextPtr = copyList(head->nextPtr); 

		return newList; 
	}
	else
	{
		puts("could not copy list."); 
		return NULL; 
	}
}

/// <summary>
/// deletes pet if char array petToDelete matches a pet's name in list. 
/// </summary>
/// <param name="headPtr"></param>
void deletePet(Pet** headPtr, char* petToDelete)
{
	Pet* previousPtr = NULL; 
	Pet* currentPtr = *headPtr; 

	if (strcmp((*headPtr)->name, petToDelete) == 0) //compare name of head of list to petToDelete and delete it if they match. 
	{
		*headPtr = (*headPtr)->nextPtr; 
		free(currentPtr); 
		currentPtr = NULL; 
	}
	else //check rest of list. 
	{
		while (currentPtr != NULL && (strcmp(currentPtr->name, petToDelete) != 0)) //iterates through list until a match is found. 
		{
			previousPtr = currentPtr;
			currentPtr = currentPtr->nextPtr; 
		}

		if (currentPtr != NULL)
		{
			previousPtr->nextPtr = currentPtr->nextPtr; //removes currentPtr from list. 

			free(currentPtr); //delete node. 
			currentPtr = NULL; 
		}
		else
		{
			puts("Node to delete not found."); 
		}
	}
}

/// <summary>
/// Ask user if they want to delete a pet from the list, then user inputs name, and runs deletePet function. Repeats until user enters n. 
/// </summary>
/// <param name="headPtr"></param>
void petDeleter(Pet** headPtr)
{
	char* petName = (char*)malloc(MAX_NAME_LENGTH * sizeof(char));
 
	puts("Delete a pet from the list?"); 
	while(validator()) //validator asks user a yes or no and returns true/false respectively. 
	{
		bool validName = false;
		while (!validName) //loops until user inputs a valid name. 
		{
			printf("%s", "Type the name of the pet you wish to remove : ");
			int scanValue = (scanf("%s", petName));
			while (getchar() != '\n');
			validName = validateScanf(scanValue);
		}

		deletePet(headPtr, petName); //calls deletePet function with list and pet's name as arguments. 
		puts("Delete another pet from the list?"); 
	}
	free(petName); 
}

/// <summary>
/// check for valid scanf input.  
/// </summary>
/// <param name="headPtr"></param>
bool validateScanf(int scanValue)
{
	if (scanValue != 1)
	{
		puts("Invalid entry."); 
		return false; 
	}
	else 
	{
		return true;
	}
}

/// <summary>
/// asks user to enter y or n. returns true if Y/y is entered and false if N/n is entered. 
/// Ensures user only selects one of those values, otherwise will tell user what they entered was invalid and to try again. 
/// </summary>
/// <returns></returns>
bool validator()
{
	bool validate = false; //validator boolean variable, initially false, will be set to true if user enters Y/y
	char verify = ' ';

	do
	{
		printf("%s", "Enter (y)es or (n)o: ");
		int scanValue = (scanf(" %c", &verify)); //scanValue is an int variable that stores the return value of scanf
		while ((getchar()) != '\n'); //clear input buffer
		if (scanValue != 1) //checks for invalid input. 
		{
			puts("Invalid entry, try again.");
		}
		else if (tolower(verify) != 'y' && tolower(verify) != 'n') //ensures input is either y or n
		{
			puts("Must enter y or n.");
		}
		else if (tolower(verify) == 'n') //checks for 'n'
		{
			validate = false;
		}
		else //only other value must be 'y'
		{
			validate = true;
		}


	} while (tolower(verify) != 'y' && tolower(verify) != 'n');  //repeats loop until user inputs y or n

	return validate; //returns true if value is y, and false if value is n. 
}


