
/*
Evan Anderson
Prof. Deb Harding
CS 2060-002

T-Shirt Fundraiser: 
	The goal of this program is to create an interface that allows for 
	an admin to log in, set price/donation percent of shirts, and start a work day. 
	If the admin does not enter the pin correctly within a certain number of attempts, the program is designed to shut down. 
	
	The program will then allow for customers to purchase shirts 
	where they will choose the shirt's size and color, 
	and it will proceed to ask for payment information and display a receipt. 

	There will be an end of day portion to the program too where: 
		the admin will have to enter the Pin correctly again
		then review the sales made and amount raised for charity. 


	UPDATE: 
		1. Pin can now be entered 4 times. 
		2. max and min values of percent and price have been changed. 
		3. there is now a constant named FILE_PATH that creates a path to a folder called tshirtapp
		4. Customers now get a printed receipt that holds information including shirt price, color, size, and card number. 
		5. When the application is shut down, a sales information is saved to a file named tshirtfund.txt. 
		6. previous sales are read from a file named tshirtfund.txt. 

	UPDATE 2: 
		1. Admin can now set up multiple fundraisers which will be stored in a struct. 
			a. structs contain org name, shirt price and percentage.  
			b. structs are stored in a linked list in alphabetical order
		2. Program now asks user which Org they wish to purchase shirt from. 
			a. Information (cost and percent) will be presented to user. 
			b. Assume same shirt colors and sizes are used between Orgs. 
		3. A file will be created that stores the total sales and funds raised for each organization separately. 
			b. Program will no longer read store info in variables at startup. 
*/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Constants
const int PIN = 81405; 
const int MAX_NAME_LENGTH = 50; 
const int MIN_PRICE = 20; //updated max and min price
const int MAX_PRICE = 50; 
const int MIN_PCT = 5;   //updated max and min percent
const int MAX_PCT = 20; 
static const char SHIRT_COLORS[6] = {'b', 'w', 'r', 'o', 'b', 'p'}; //changed array to include new colors
const char SHIRT_COLORS_STRING[] = "color, (w)hite, (b)lack, (r)ed, (o)range, (b)lue, or (p)urple";  //updated string with new colors

static const char SHIRT_SIZE[5] = {'s', 'm', 'l', 'x', 'q'}; 
const char SHIRT_SIZE_STRING[] = "size: (s)mall, (m)edium, (l)arge, (x)-large"; 

const char FILE_PATH[] = ".\\tshirtapp\\"; 


//Fundraiser struct
typedef struct Organization
{
	char* orgName;
	double shirtPrice;
	double donatePercent;
	unsigned int amtSold;
	struct Organization* next;

}Organization;

//Function Prototypes
#pragma region startDay funcs

bool startDay(Organization** head);
bool enterPin(); 
void setValue(double *value, int min, int max, const char valName[]); 
void readSalesFile(double* sales, double* funds); 

//Iteration 03 startday Funcs: 
void setAllValues(Organization** head); 
int decideNumOrgs(); 
void insertOrg(Organization** head);
void initializeOrg(Organization* orgPtr);
void alphabetizeOrg(Organization** head, Organization* orgPtr);

#pragma endregion

#pragma region Purchasing func prototypes

char purchasing(Organization* orgPtr); 

char shirtSelection(const char arr[], const char shirtString[]);  
void scanCard(char *cardnum); 
void displayReceipt(char size, char color, char* cardNum, double percent, double cost, int amtShirts); 

//Iteration 03 Purchasing Funcs: 
Organization* selectOrg(Organization** head); 

#pragma endregion

#pragma region EndDay func prototypes

bool endDay(Organization** head);

void displayTotals(Organization* head, double sales, double donations); 

//Iteration 03 EndDay Funcs:
double determineTotalSales(Organization* head); 
double determineTotalDonations(Organization* head);

#pragma endregion

#pragma region Utility Func prototypes

bool validator(); 
int numDigits(unsigned int num); 
int createRandNum(int lower, int upper); 
bool validateScanf(int scanValue);

//general Linked List Functions
void printList(Organization* listPtr);
void printListNames(Organization* listPtr);
void printNode(Organization* orgPtr);
Organization* findOrg(Organization** head, char* orgName);
#pragma endregion



int main()
{

	srand(time(0)); //seed for random numbers. seeds via current time. 
	Organization* headPtr = NULL; 

	//program only runs if startDay returns true. 
	if (startDay(&headPtr))
	{
		do
		{
			bool endPurchasing = false; 
			do
			{
				Organization* customerOrg = NULL; 
				//if selectOrg func fails or user opts to quit. 
				if ((customerOrg = selectOrg(&headPtr)) == NULL)
					endPurchasing = true; 
				else if (purchasing(customerOrg) == 'q') //if user opts to quit when they select shirt size. 
					endPurchasing = true; 
			} while (!endPurchasing);

		} while (!endDay(&headPtr)); //continues to loop until endDay returns true. 
	}

	free(headPtr); //free some memory. 
	return 0; 
}



//Function Definitions


#pragma region StartDay func definitions

/// <summary>
/// function that controls all of the admin tasks at the start of day. 
/// runs enterPin, sets values for price and percent. 
/// returns true if enterPin returns true, and will return after both values are set. 
/// </summary>
/// <param name="price"> represents the price of the shirts to be sold. </param>
/// <param name="percent"> represents the percent of the cost of a shirt. </param>
/// <returns></returns>
bool startDay(Organization** head)
{
	bool validate = false; //boolean variable to determine truth value. 

	if (enterPin()) //if statement only runs if enterPin returns true. 
	{
		setAllValues(head); 
		validate = true; //sets validate to true, returning the function with true. 
	}
	else //enterPin returned false, therefore the application shuts down. 
	{
		puts("System shutting down"); 
	}

	return validate; //returns truth value of variable validate. 
}

/// <summary>
/// function to control pin entry. 
/// Function will ask user to enter a pin, and checks to see if the entered value is equal to the constant PIN. 
/// User gets three tries, after which the program shuts down. 
/// If user enters correct pin within three tries, the program will continue to the next steps. 
/// </summary>
/// <returns></returns>
bool enterPin()
{
	bool validate = false; //truth value to be returned at end of function. 
	int key = 0;  //int variable that stores user input and is compared to PIN constant. 
	int counter = 1;  //counter variable that increments each time user enters input incorrectly. as soon as counter is greater than 3, function quits. 

	printf("%s", "Enter Pin: ");
	do
	{
		int scanValue = (scanf("%d", &key)); //scanValue stores the return value of scanf function. key is set to user's input. 

		if (scanValue != 1) //if user put invalid value into key. 
		{
			printf("%s", "Invalid Entry. \nEnter Pin: ");
			counter++; //counter increases. 
		}
		else if (key == PIN) //user inputs correct PIN value, validate is set to true and function ends returning true. 
		{
			puts("Access granted.");
			validate = true;
		}
		//updated number of times user can input PIN. 
		else if (counter >= 4) //user input incorrect value too many times. validate is set to false and function ends returning false. 
		{
			puts("Too many attempts.");
			counter++;
		}
		else
		{
			printf("%s", "Incorrect Pin. Try again. \nEnter Pin: "); //user input incorrect value, but has not exceeded max num of attempts. 
			counter++;
		}

		while ((getchar()) != '\n'); //clear input field. 

	} while (!validate && counter <= 4); //loop continues while validate is false and counter <= 3. if either of these no longer qualify, loop ends. 

	return validate; //returns truth value of validate variable. 
}

/// <summary>
/// function that allows user to set a value for a specific variable. 
/// </summary>
/// <param name="value"> the value that is to be set. </param>
/// <param name="min"> minimum number that value can be set to. </param>
/// <param name="max"> maximum number that value can be set to. </param>
/// <param name="valName"> what value is to be referred as. </param>
/// <returns></returns>
void setValue(double *value, int min, int max, const char valName[])
{
	bool validate = false; //validator boolean variable, initially set to false. 
 
	do
	{
		printf("Enter an amount for %s: ", valName);

		int scanValue = (scanf("%lf", value)); //scanValue stores the return type of scanf function. variable value is set to user's input. 
		while ((getchar()) != '\n');

		if (scanValue != 1) //if user input is invalid. 
		{
			puts("Invalid entry. Try again. "); 
		}
		else if (*value < (double)min || *value > (double)max) //if value is outside of acceptable min/max range. 
		{
			printf("Invalid entry. value must be between %d and %d.\n", min, max); 
		}
		else //at this point, value must be a valid input as well as within the acceptable range. therefore it must be correct. 
		{
			printf("Is the %s %.2lf correct?\n", valName, *value); 
			validate = validator(); //confirmation for value. 
		}

 
	} while (!validate); //loop continues until validate is set to true. 

	if (valName[0] == 'd') //checks the valName array to see if the first letter is a d. this signifies that it is for donations and divides it by 100, setting it to correct math number for percentage. 
	{
		(*value) /= 100; 
	}
}

/// <summary>
/// reads the file tshirtfund.txt if there is one, and sets the values within to sales and funds respectively. 
/// </summary>
/// <param name="sales"></param>
/// <param name="funds"></param>
void readSalesFile(double* sales, double* funds)
{
	FILE* filePtr; //file pointer

	char filename[64]; //character array that will hold the full filepath. 
	sprintf(filename, "%stshirtfund.txt", FILE_PATH);

	if ((filePtr = fopen(filename, "r")) == NULL) //verify that the file can be opened. 
	{
		puts("tshirtfund.txt could not be opened.");
	}
	else
	{
		double numbers[2] = { 0,0 }; //number array will hold the values in the file. 
		int count = 0; //incrementer variable

		while (fscanf(filePtr, "%lf", &numbers[count]) != EOF) //continues reading lines from file until it reaches the end of the file (EOF)
		{
			count++;
		}

		*sales = numbers[0]; //sets sales equal to zeroth index of numbers array. 
		*funds = numbers[1]; //sets funds equal to oneth index of numbers array. 
		fclose(filePtr); //gotta close that file. 
	}


}

//***********************************************************************************
//Iteration 03 startday Funcs: 

/// <summary>
/// Sets values of all organizations in linked list. 
/// </summary>
/// <param name="head"></param>
void setAllValues(Organization** head)
{
	int numOrgs = decideNumOrgs(); //number of organizations to be included. 

	for (size_t i = 0; i < numOrgs; i++)
	{
		insertOrg(head); 
	}
}

/// <summary>
/// function that controls initializing structs and placing them in a linked list in correct order. 
/// </summary>
/// <param name="head"></param>
void insertOrg(Organization** head)
{
	Organization* orgPtr = malloc(sizeof(Organization)); //allocate some memories. 

	if (orgPtr != NULL) //verifying memory has been allocated. 
	{
		//set values in struct. 
		initializeOrg(orgPtr); 
		orgPtr->next = NULL; 
		//place struct in correct order. 
		alphabetizeOrg(head, orgPtr); 
	}
	else
	{
		puts("No memory to create Organization"); 
	}
}

/// <summary>
/// sets values of struct elements. 
/// </summary>
/// <param name="orgPtr"></param>
void initializeOrg(Organization* orgPtr)
{
	int scanValue = -1; 
	orgPtr->orgName = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); //allocate some memory for orgName. 
	do
	{
		printf("%s", "Enter name of Organization: ");
		scanValue = (scanf("%s", orgPtr->orgName)); //user inputs name of organization. 
		while (getchar() != '\n'); 
	} while (!validateScanf(scanValue)); 

	setValue(&(orgPtr->shirtPrice), MIN_PRICE, MAX_PRICE, "price"); //set price of shirt. 
	setValue(&(orgPtr->donatePercent), MIN_PCT, MAX_PCT, "donation percent"); //sets value of donation percentage. 
	orgPtr->amtSold = 0; 
}

/// <summary>
/// inserts node into list in alphabetical order. 
/// </summary>
/// <param name="head"></param>
/// <param name="fundPtr"></param>
void alphabetizeOrg(Organization** head, Organization* orgPtr)
{
	//helper struct pointers. 
	Organization* prevPtr = NULL; 
	Organization* currentPtr = *head; 
	int stringCompare = 1; 

	if (currentPtr != NULL) 
	{
		stringCompare = strcmp(orgPtr->orgName, currentPtr->orgName); 
	}
	
	
	while (currentPtr != NULL && stringCompare >= 1)
	{
		stringCompare = strcmp(orgPtr->orgName, currentPtr->orgName); //compare current pointer orgName to target orgName. 
		prevPtr = currentPtr; 
		currentPtr = currentPtr->next; 
	}

	if (prevPtr == NULL)
	{
		*head = orgPtr;
	}
	else
	{
		prevPtr->next = orgPtr; 
	}

	orgPtr->next = currentPtr; 

}

/// <summary>
/// allows user to enter number of organizations participating in fundraiser. 
/// </summary>
/// <returns></returns>
int decideNumOrgs()
{
	unsigned int numToReturn = 0;

	do
	{
		printf("%s", "Enter number of Organizations: ");

		int scanValue = (scanf("%d", &numToReturn));
		while (getchar() != '\n');
		if (validateScanf(scanValue))
		{
			if (numToReturn < 1) //must have at least one organization. 
			{
				puts("Must enter a number greater than zero.\n");
			}
		}
	} while (numToReturn < 1);

	return numToReturn; //returns the number of organizations admin wants to include. 
}

#pragma endregion

#pragma region Purchasing Func Definitions

/// <summary>
/// function that houses the overview logic for the purchasing part of day. 
/// allows customers to purchase shirts in a specific size and color, then asks them for a receipt. 
/// continues to loop until user inputs a 'q' during size selection, which then sends program to next part of function. 
/// </summary>
/// <param name="amtShirts"> total number of shirts sold throughout the day. </param>
/// <param name="percent"> percent of a given sale that will be donated. </param>
/// <param name="cost"> cost of shirt. </param>
/// <returns> returns amount of shirts sold. </returns>
char purchasing(Organization* orgPtr)
{

	//determine size of user's shirt. entering q will end function loop. 
	char size = tolower(shirtSelection(SHIRT_SIZE, SHIRT_SIZE_STRING)); 

	if (size != 'q') //only runs if user does NOT enter Q/q during size selection. 
	{
		char color = shirtSelection(SHIRT_COLORS, SHIRT_COLORS_STRING);  //allow user to select shirt color. 
		char* cardNum = (char*)malloc(20*sizeof(char)); //creating character pointer and allocating enough memory for 20 characters which is the max length of string (including null). 
		scanCard(cardNum); //calls method to set cardNum equal to user's credit card number 
		orgPtr->amtSold++;  //adds 1 to total number of shirts sold. 
		displayReceipt(size, color, cardNum, orgPtr->donatePercent, orgPtr->shirtPrice, orgPtr->amtSold); //calls displayReceipt function with the user's choices. 
	}

	return size; //returns total amount of shirts sold. 
}

/// <summary>
/// function that allows user to select properties of their shirt. 
/// properties are determined in the parameter list of the function. 
/// </summary>
/// <param name="arr"> array property that will be searched. </param>
/// <param name="shirtString"> a string that will contain message to user designating shirt property. </param>
/// <returns></returns>
char shirtSelection(const char arr[], const char shirtString[])
{
	char shirtSelection = ' '; 
	bool valid = false; 
	size_t arrLength = strlen(arr); //gets length of array. 

	do
	{

		printf("Enter shirt %s: ", shirtString); 
		int scanValue = (scanf(" %c", &shirtSelection)); //scanValue holds return value of scanf function. scanf is setting shirtSelection to user's entry. 
		while ((getchar()) != '\n');  //clear input buffer. 
		if (scanValue != 1) //check for invalid scanf input. 
		{
			puts("Invalid input. Try again"); 
		}
		else //if not invalid input. 
		{
			//loop to check to see if user input is a character in the array. 
			for (size_t i = 0; i < arrLength; i++)
			{
				if (tolower(shirtSelection) == (tolower(arr[i]))) //checks equality between shirtSelection and array index. toLower function ensures variables are lowercase.
				{
					valid = true; 
				}
			}
			if (!valid)
			{
				puts("invalid selection. Try again."); 
			}
		}

	} while (!valid); //loops until a valid input is entered. 

	return shirtSelection; //returns the selection of the shirt. 
}

/// <summary>
/// function that "scans" customers card. currently only asks user for their zip code and returns that value. 
/// </summary>
void scanCard(char* cardNum)
{
	 
	bool valid = false; //truth value, loop will continue until valid is true. 

	do {
		printf("%s", "Enter your Credit Card Number (####-####-####-#### format): "); 
		int scanValue = (scanf("%s", cardNum)); //scanValue holds return value of scanf function. user is setting value of cardNum variable.  
		while ((getchar()) != '\n');  //clear input buffer. 
		if (scanValue != 1) //essentially, if scanValue returns an invalid value
		{
			puts("Invalid input."); 
		} 
		else if (strlen(cardNum) != 19 ) //if the number of digits entered is not equal to 19. 
		{
			puts("card number must be 19 digits."); 
		}
		else //correct input and length, now must check for numbers and dashes in correct spaces.  
		{ 
			valid = true;  //set valid to true
			for (size_t i = 0; i < 19; i++) //iterate through entire cardNum array. 
			{
				if ((i == 4 || i == 9 || i == 14)) //checks for dashes at 4th, 9th, and 14th indeces
				{
					if (cardNum[i] != '-' && valid == true) //if index is not equal to a dash and this hasn't already been triggered. 
					{
						valid = false;
						puts("card must have a dash - after every fourth number.");
					}
				}
				else if (!isdigit(cardNum[i]) && valid == true) //check to ensure every other index is in fact a number. 
				{
					valid = false; 
					puts("must input numbers only (other than a dash - after every fourth number).");
				}
			}
		}
	} while (!valid); //loops while valid is false. 
}

/// <summary>
/// Function that prints a receipt for customer. 
/// </summary>
/// <param name="size"> variable that holds the value of the shirt's size. </param>
/// <param name="color"> variable that holds the value of the shirt's color. </param>
/// <param name="zip"> variable that holds the value of the user's zip code. </param>
/// <param name="percent"> holds the value of the percent of sales going to donations. </param>
/// <param name="cost"> holds the value of the cost of a shirt. </param>
/// <param name="amtShirts"> holds value of total number of shirts sold. </param>
void displayReceipt(char size, char color, char* cardNum, double percent, double cost, int amtShirts)
{
	
	printf("%s", "Would you like a receipt?\n"); 
	if (validator()) //executes following steps if validator returns true. 
	{
		FILE* filePtr; 

		char filename[64]; //creating a character array, this will hold the entire filepath. 
		sprintf(filename, "%sreceipt.txt", FILE_PATH);  //setting file name equal to the filepath with filename. 

		if ((filePtr = fopen(filename, "a")) == NULL) //ensure file opens. 
		{
			puts("file could not be opened."); 
		}
		else //triggered if file opens successfully.  
		{
			int rando = createRandNum(10000, 99999); //creates a random 5 digit number. 
			fprintf(filePtr, "\nReciept #%d\n", rando);  //prints customer's receipt number (random 5 digit number). 
			fprintf(filePtr, "Card Number: %s\n\n", cardNum); //prints customer's card number. 
			fprintf(filePtr, "t-shirt\tsize: %c\t color: %c \t cost: $%.2lf\n\n", size, color, cost); //prints t-shirt size, color, and cost. 
			fprintf(filePtr, "%.2lf%% of  your cost will be donated to charity. \nThank you for your support.\n\n", percent); //tells customer percentage of sale going to charity. 
			fprintf(filePtr, "Current amount raised for charity is $%.2lf\n\n", ((amtShirts * cost) * (percent / 100))); //tells user the current total of donation money raised. 
			fputs("\n\n", filePtr); //put some space between receipts. 

			fclose(filePtr); //gotta close that file pointer. 
		}
	}
	else
	{
		puts("Thank you, have a nice day!"); //have a good day!
	}

}

//----------------------------------------------------------------------------------------------------------------------
//Iteration 03 Function Definitions

/// <summary>
/// function that determines which organization customer wants to purchase a shirt from
/// </summary>
/// <param name="head"></param>
/// <returns></returns>
Organization* selectOrg(Organization** head)
{
	char* userSelection = (char*)malloc(MAX_NAME_LENGTH * sizeof(char)); 
	Organization* selectedOrg = NULL; 
	bool selected = false; 

	puts("which organization you wish to purchase from?"); 
	printListNames(*head); 
	do
	{
		bool validName = false; 
		while (!validName) //loops until user inputs a valid organization name or the letter q
		{
			printf("%s", "Type the name of the Organization (type q to quit): "); 
			int scanValue = scanf("%s", userSelection); 
			while (getchar() != '\n'); 
			validName = validateScanf(scanValue); 
		}
		if ((strcmp(userSelection, "q") == 0) || strcmp(userSelection, "q") == 0) //check for Q/q
		{
			selectedOrg = NULL; //will return null
			selected = true; 
		}
		else if ((selectedOrg = findOrg(head, userSelection)) != NULL) //triggered if there's a match. 
		{
			printNode(selectedOrg); //prints the organization the user selected. 
			printf("%s\n", "Is this selection correct?");
			selected = validator(); //verify this is the organization user wants to purchase from. 
		}
	} while (!selected); 

	return selectedOrg; 
}
#pragma endregion

#pragma region End Day Funcs Definitions

/// <summary>
/// function that is triggered at the end of the day
/// user must correctly enter the Admin Pin, otherwise returns false and continues selling shirts. 
/// function then displays the total sales of the day through the displayTotals() function. 
/// </summary>
/// <param name="amtSold"> number of shirts sold. </param>
/// <param name="cost"> variable that stores the cost of the shirts. </param>
/// <param name="percent"> variable that stores the percent of a sale that goes to donation. </param>
/// <returns></returns>
bool endDay(Organization** head)
{
	bool validate = false; //validator variable to return true or false at end of function. 
	if (enterPin()) //only runs if enterPin returns true. 
	{
		double totalSales = determineTotalSales(*head); //variable that stores the total amount of money made in sales. 
		double totalDonations = determineTotalDonations(*head); //variable that stores the total donation money made.
		displayTotals(*head, totalSales, totalDonations); //function that will print the totals. 
		validate = true; 
	}
	return validate;
}

/// <summary>
/// function that will display the total sales and total donation amount. 
/// </summary>
/// <param name="sales"></param>
/// <param name="donations"></param>
void displayTotals(Organization* head, double sales, double donations)
{
	if (head != NULL)
	{
		FILE* filePtr;

		char filename[128];
		sprintf(filename, "%stshirtfund.txt", FILE_PATH); //setting a character array equal to the filepath + the filename. 

		if ((filePtr = fopen(filename, "a")) == NULL) //checking to make sure file is actually opened/created. 
		{
			puts("file could not be read.");
		}
		else //if file is opened properly. 
		{
			Organization* currentPtr = head;

			while (currentPtr != NULL)
			{
				printf("Organization: %s \nsales: $%.2lf \ndonations: $%.2lf \n", currentPtr->orgName, currentPtr->shirtPrice * currentPtr->amtSold, currentPtr->shirtPrice * currentPtr->amtSold * currentPtr->donatePercent);
				fprintf(filePtr, "\nOrganization: %s \nsales: %.2lf \ndonations: %.2lf \n", currentPtr->orgName, currentPtr->shirtPrice * currentPtr->amtSold, currentPtr->shirtPrice * currentPtr->amtSold * currentPtr->donatePercent); 
				currentPtr = currentPtr->next; 
			}
			fclose(filePtr);
		}

		printf("Total sales: $%.2lf \nTotal Donations: $%.2lf", sales, donations);

	}
	else
	{
		puts("List is empty. NO SALES BUMMER!"); 
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//Iteration 03 End Day Func Definitions: 

/// <summary>
/// determines the total sales across all organizations
/// </summary>
/// <param name="head"></param>
/// <returns></returns>
double determineTotalSales(Organization* head)
{
	Organization* currentPtr = head; 

	if (currentPtr == NULL) //checks to see if pointer is null, end of list or lack of list. 
		return 0; 
	else
	{
		//returns sales from current pointer + self function call for next pointer. 
		return (currentPtr->shirtPrice * currentPtr->amtSold) + determineTotalSales(currentPtr->next); 
	}	
}

/// <summary>
/// Determines total donation funds raised across all organizations. 
/// </summary>
/// <param name="head"></param>
/// <returns></returns>
double determineTotalDonations(Organization* head)
{
	Organization* currentPtr = head; 

	if (currentPtr == NULL)
		return 0; 
	else
	{
		//returns donation funds from current pointer + self func call for next pointer .
		return (currentPtr->shirtPrice * currentPtr->amtSold * currentPtr->donatePercent) + determineTotalDonations(currentPtr->next); 
	}
}

#pragma endregion

#pragma region Utility Funcs

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
		}else if (tolower(verify) != 'y' && tolower(verify) != 'n') //ensures input is either y or n
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

/// <summary>
/// recursive function that determines the number of digits in a number (the length). 
/// will continue to re-call the function until num is broken down to one's place. 
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
int numDigits(unsigned int num)
{
	//if number is a one's place number, returns length of 1. 
	if (num / 10 == 0)
	{
		return 1; 
	}
	//calls this function with num/10 (reduces place once and calls again). 
	return 1 + numDigits(num / 10); 
}

/// <summary>
/// creates a random number between a lower and upper bound. 
/// </summary>
/// <param name="lower"></param>
/// <param name="upper"></param>
/// <returns></returns>
int createRandNum(int lower, int upper)
{
	return ((rand() % (upper - lower + 1)) + lower); 
}


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
/// prints entire Linked List of Organizations in alphabetical order. 
/// </summary>
/// <param name="listPtr"></param>
void printList(Organization* listPtr)
{
	if (listPtr != NULL) //checking for empty list.
	{
		printf("%s\n", "The List in alphabetical order is: ");
		Organization* currentPtr = listPtr;

		//goes through entire list and prints each one, then moves current pointer to next pointer in list. 
		while (currentPtr != NULL)
		{
			printf("Name: %s\tShirt Price: %.2lf\tDonation Percentage: %.2lf\tAmount sold: %u\n", currentPtr->orgName, currentPtr->shirtPrice, currentPtr->donatePercent*100, currentPtr->amtSold);
			currentPtr = currentPtr->next;
		}

		free(currentPtr); 
	}
	else
	{
		puts("List is empty");
	}
}

/// <summary>
/// prints only the names of the entire linked list of organizations in alphabetical order. 
/// </summary>
/// <param name="listPtr"></param>
void printListNames(Organization* listPtr)
{
	if (listPtr != NULL) //checking for empty list.
	{
		printf("%s\n", "The List in alphabetical order is: ");
		Organization* currentPtr = listPtr;

		//goes through entire list and prints each one, then moves current pointer to next pointer in list. 
		while (currentPtr != NULL)
		{
			printf("Name: %s\n", currentPtr->orgName);
			currentPtr = currentPtr->next;
		}

		free(currentPtr); 
	}
	else
	{
		puts("List is empty");
	}
}

/// <summary>
/// prints a single node from Organizations linked list. 
/// </summary>
/// <param name="orgPtr"></param>
void printNode(Organization* orgPtr)
{
	if (orgPtr != NULL)
	{
		printf("Name: %s\tShirt Price: %.2lf\tDonation Percentage: %.2lf%%\tAmount sold: %u\n", orgPtr->orgName, orgPtr->shirtPrice, orgPtr->donatePercent*100, orgPtr->amtSold);
	}
}


/// <summary>
/// searches a linked list of Organizations to find a specific organization by name. 
/// </summary>
/// <param name="head"></param>
/// <param name="orgName"></param>
/// <returns></returns>
Organization* findOrg(Organization** head, char* orgName)
{
	if (*head != NULL) //checking for empty pointer. 
	{
		Organization* currentPtr = *head;

		if (strcmp(currentPtr->orgName, orgName) == 0) //triggered if currentPtr orgName is equivalent to target name. 
		{
			return currentPtr;
		}

		if (currentPtr->next == NULL) //if name still hasn't been found and next pointer is null, organization doesn't exist. 
		{
			puts("Organization not found.");
			return NULL;
		}
		else
		{
			//recursive call for next pointer. 
			findOrg(&currentPtr->next, orgName);
		}
	}
	else
	{
		puts("list not found"); 
		return NULL; 
	}
}
#pragma endregion

