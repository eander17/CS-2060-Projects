
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

*/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Constants
const int PIN = 81405; 
const int MIN_PRICE = 10; 
const int MAX_PRICE = 50; 
const int MIN_PCT = 0; 
const int MAX_PCT = 100; 
static const char SHIRT_COLORS[2] = {'b', 'w'};
const char SHIRT_COLORS_STRING[] = "color, (w)hite or (b)lack"; 

static const char SHIRT_SIZE[5] = {'s', 'm', 'l', 'x', 'q'}; 
const char SHIRT_SIZE_STRING[] = "size: (s)mall, (m)edium, (l)arge, (x)-large"; 


//Function Prototypes
#pragma region startDay funcs

bool startDay(double *price, double *percent);
bool enterPin(); 
void setValue(double *value, int min, int max, const char valName[]); 

#pragma endregion

#pragma region Purchasing func prototypes

int purchasing(int amtShirts, double percent, double cost); 

char shirtSelection(const char arr[], const char shirtString[]);  
int scanCard(); 
void displayReceipt(char size, char color, int zip, double percent, double cost, int amtShirts); 

#pragma endregion

#pragma region EndDay func prototypes

bool endDay(int amtSold, double cost, double percent); 

void displayTotals(double sales, double donations); 

#pragma endregion

#pragma region Utility Func prototypes

bool validator(); 

int numDigits(unsigned int num); 

int createRandNum(int lower, int upper); 
#pragma endregion



int main()
{
	srand(time(0)); //seed for random numbers. seeds via current time. 
	double donatePercent = 0; //the percentage value for donations. 
	double shirtPrice = 0;  //cost of the shirt. 
	unsigned int amtSold = 0;  //total number of shirts sold. 


	//program only runs if startDay returns true. 
	if (startDay(&shirtPrice, &donatePercent))
	{
		do
		{
			//amtSold is set to the return value of purchasing. 
			//purchasing function will continue to sell shirts until the condition for endDay function is met. 
			amtSold = purchasing(amtSold, (donatePercent * 100), shirtPrice); 
		} while (!endDay(amtSold, shirtPrice, donatePercent)); //continues to loop until endDay returns true. 
	}

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
bool startDay(double *price, double *percent)
{
	bool validate = false; //boolean variable to determine truth value. 

	if (enterPin()) //if statement only runs if enterPin returns true. 
	{
		setValue(price, MIN_PRICE, MAX_PRICE, "price"); //set price of shirt. 
		setValue(percent, MIN_PCT, MAX_PCT, "donation percent"); //sets value of donation percentage. 
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
		else if (counter >= 3) //user input incorrect value too many times. validate is set to false and function ends returning false. 
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

	} while (!validate && counter <= 3); //loop continues while validate is false and counter <= 3. if either of these no longer qualify, loop ends. 

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
int purchasing(int amtShirts, double percent, double cost)
{
	bool endPurchasing = false;  //boolean value to determine if purchasing is to stop. 
	do
	{
		//determine size of user's shirt. entering q will end function loop. 
		char size = shirtSelection(SHIRT_SIZE, SHIRT_SIZE_STRING); 

		if (tolower(size) != 'q') //only runs if user does NOT enter Q/q during size selection. 
		{
			char color = shirtSelection(SHIRT_COLORS, SHIRT_COLORS_STRING);  //allow user to select shirt color. 
			int zipCode = scanCard(); //set zipCode equal to the return value of scanCard() function. 
			amtShirts++;  //adds 1 to total number of shirts sold. 
			displayReceipt(size, color, zipCode, percent, cost, amtShirts); //calls displayReceipt function with the user's choices. 
		}
		else
		{
			endPurchasing = true; //is triggered when size is set to Q/q, ends loop. 
		}

	} while (!endPurchasing);  //continues to run while endPurchasing is false; 


	return amtShirts; //returns total amount of shirts sold. 
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
/// <returns>returns value of zip </returns>
int scanCard()
{
	int zip = 0; //variable to hold user's zipcode. 
	bool valid = false; //truth value, loop will continue until valid is true. 

	do {
		printf("%s", "Enter your Zip Code: "); 
		int scanValue = (scanf("%d", &zip)); //scanValue holds return value of scanf function. user is setting value of zip variable.  
		while ((getchar()) != '\n');  //clear input buffer. 
		if (scanValue != 1) //essentially, if scanValue returns an invalid value
		{
			puts("Invalid input."); 
		} 
		else if (numDigits(zip) != 5) //if the number of digits entered is not equal to 5. 
		{
			puts("Zipcode must be 5 digits."); 
		}
		else //at this point, must be a valid input that is 5 digits in length. 
		{
			printf("Your zip code is: %d\n", zip); 
			valid = true; //correctly entered zip code, valid is set to true. 
		}
	} while (!valid); //loops while valid is false. 

	return zip; //return zipcode. 
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
void displayReceipt(char size, char color, int zip, double percent, double cost, int amtShirts)
{
	
	printf("%s", "Would you like a receipt?\n"); 
	if (validator()) //executes following steps if validator returns true. 
	{
		int rando = createRandNum(10000, 99999); //creates a random 5 digit number. 
		printf("Reciept #%d\n\n", rando);  //prints customer's receipt number (random 5 digit number). 
		printf("t-shirt\tsize: %c\t color: %c \t cost: $%.2lf\n\n", size, color, cost); //prints t-shirt size, color, and cost. 
		printf("%.2lf%% of  your cost will be donated to charity. \nThank you for your support.\n\n", percent); //tells customer percentage of sale going to charity. 
		printf("Current amount raised for charity is $%.2lf\n\n", ((amtShirts * cost) * (percent / 100))); //tells user the current total of donation money raised. 
	}
	else
	{
		puts("Thank you, have a nice day!"); 
	}

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
bool endDay(int amtSold, double cost, double percent)
{
	bool validate = false; //validator variable to return true or false at end of function. 
	if (enterPin()) //only runs if enterPin returns true. 
	{
		double totalSales = amtSold*cost; //variable that stores the total amount of money made in sales. 
		double totalDonations = totalSales*percent; //variable that stores the total donation money made. 
		displayTotals(totalSales, totalDonations); //function that will print the totals. 
		validate = true; 
	}
	return validate;
}

/// <summary>
/// function that will display the total sales and total donation amount. 
/// </summary>
/// <param name="sales"></param>
/// <param name="donations"></param>
void displayTotals(double sales, double donations)
{
	printf("Total sales: %.2lf \nTotal Donations: %.2lf", sales, donations); 
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
#pragma endregion

