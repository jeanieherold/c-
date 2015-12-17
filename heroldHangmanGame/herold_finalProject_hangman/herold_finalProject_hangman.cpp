/*author: Jeanie Herold
c++ programming class
final project - Hangman Game. Phrases are loaded into the program from a file. 
A user enters in a playing level they would like to play. The program has 3 playing levels sorted according to how many guesses are required
to get a correct answer and win the game. If the user guesses all letter in the phrase before guessing 5 wrong times. they win the game. 
If the player guesses 5 wrong times - they lose the game. 
Each wrong guess outputs a body part on the hangman gallows.
April 21, 2015
*/

#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h> //header file to use isalpha function
#include <iomanip>
#include <cstdlib> //header file for random number generator
#include <ctime> //gives access to time function


using namespace std;

// struct to be used throughout the program containing phrase, #of unique letters,
//and if the phrase has been used
struct Phrase
{
	string text;
	string::size_type guessesRequired;
	bool isUnused;
};

//global variables
int const ARRAY_SIZE = 256;

//prototypes
void loadPhrasesFromFile(string fileName, Phrase phrases[], int& arrayLength); // loads phrases from a file and creates the array length
void sortPhrases(Phrase phrases[], int arrayLength); // prints the phrases sorted according to difficulty
void printPhrases(Phrase phrases[], int arrayLength); //print the phrases unsorted
int uniqueLetterCount(const string& text); //determines how many unique letters in the phrase
string phraseWithBlanks(const string& text, const string& correctLoLetters); //creates the phrase with blanks for the game set up
int getDifficultyLevel(int level); // request a desired difficulty level from user
int randomIndex(int min, int max); //generates a random number between given range
void drawGallows(int missCount); // draws the gallows
void runGame(string& Phrase, int guessesRequired, int& noOfPhrasesGuessed); //loops through as user plays game guessing letters until phrase guessed or they lose
string toLowerCase(const string& Phrase); // function to reference a string and return string in lower case to be called in runGame function

int main()
{
	int arrayLength = ARRAY_SIZE; //create a variable = to global ARRAYSIZE to be able to use size of array as a variable throughout program
	string fileName = "phrases.txt"; // file of phrases to be read in to program
	Phrase phrases[ARRAY_SIZE]; //array of phrases of struct type Phrase
	int level = 0; //int to store desired user level
	int randPhraseIndex= 0; // int to store random generator index number
	int guessesRequired; //int to store the number of guesses required for phrase selected
	string gamePhrase = ""; //string to store the phrase selected
	int noOfPhrasesGuessed = 0;
	int level1Count = 0; // count for phrases used in level 1
	int level2Count = 0; // count for phrases used in level 2
	int level3Count = 0; // count for phrases used in level 3
	char playAgain; // char to input y/n answer to user wanting to play again

	
	unsigned int seed = static_cast<unsigned int>(time(NULL)); //setting seed of starting value for random generator

	srand(seed);  //starts random number according to time


	// call to function to load the phrases and set array length according to the number of phrases loaded
	loadPhrasesFromFile(fileName, phrases, arrayLength);


	// setting variable to use in random Index = to 1/3, 2/3 and 3/3for getting random phrase at chosen level
	int level1 = (arrayLength / 3);
	int level2 = (arrayLength * 2 / 3);
	int level3 = (arrayLength);

	//call to function to sort phrases according to guess required
	sortPhrases(phrases, arrayLength);

	do
	{
		//call function to get the desired difficulty level from the user

		level = getDifficultyLevel(level);

		// switch conditions to end the game if there are no more phrases to guess at the users desired play level.
		switch (level)
		{
		case 1:

			if (level1Count > level1) // condition that accounts for all phrases at level 1
			{
				cout << "There are no more phrases at level 1." << endl << endl;
				goto stop;
			}
			level1Count += 1;
			break;
		case 2:

			if (level2Count >= (level2 - level1)) // condition that accounts for all phrases at level 2
			{
				cout << "There are no more phrases at level 2." << endl << endl;
				goto stop;
			}
			level2Count += 1;
			break;
		case 3:

			if (level3Count >= ((level3-1)-level2)) // condition that accounts for all phrases at level 3
			{
				cout << "There are no more phrases at level 3." << endl << endl;
				goto stop;
			}
			level3Count += 1;
			break;
		}
		cout << endl << endl;

		//switch to obtain a phrase according to the user desired difficulty level
		do
		{
			switch (level)
			{
			case 1:
				randPhraseIndex = randomIndex(0, level1);  //generate random number from 1st third of array
				gamePhrase = phrases[randPhraseIndex].text; //set gamePhrase = to the phrase chosen from random generator index
				break;
			case 2:
				randPhraseIndex = randomIndex(level1 + 1, level2);  //generate random number from 2nd third of array
				gamePhrase = phrases[randPhraseIndex].text;  //set gamePhrase = to the phrase chosen from random generator index
				break;
			case 3:
				randPhraseIndex = randomIndex(level2 + 1, level3 - 1); //generate random number from last third of array
				gamePhrase = phrases[randPhraseIndex].text;  //set gamePhrase = to the phrase chosen from random generator index
				break;
			}
		} while (phrases[randPhraseIndex].isUnused == true);
		
		guessesRequired = phrases[randPhraseIndex].guessesRequired; //set the guesses required to guess the phrase according to the phrase from the random generator index

		// call function to begin the game
		runGame(gamePhrase, guessesRequired, noOfPhrasesGuessed);
		phrases[randPhraseIndex].isUnused = true;

	
	
		
		//loop to ask user if they want to play again
		
		do
		{
				cout << "Would you like to play again? (y/n): ";
				cin >> playAgain;
		} while ((playAgain != 'y') && (playAgain != 'Y') && (playAgain != 'n') && (playAgain != 'N'));
		
		//conditional to continue or break out of the game
		if ((playAgain == 'y') || (playAgain == 'Y'))
		{
			continue;
		}
		else
			break;


		
	} while(noOfPhrasesGuessed < arrayLength); //game loop condition to end
	stop:
	cout << endl << "Thank you for playing HangMan!" << endl << endl;
	
	return 0;
}

// void function to load phrases in from a file and initialize them into an array.
//each element of the array is a struct set to the members above in the struct Phrase
void loadPhrasesFromFile(string fileName, Phrase phrases[], int& arrayLength)
{
	ifstream inFile;
	inFile.open(fileName);
	string line;
	int count = 0;

	//loop to input the phile of phrases
	while (!inFile.eof() && (count <= arrayLength))

	{
		getline(inFile, line);
		phrases[count].text = line;
		phrases[count].guessesRequired = uniqueLetterCount(line);
		phrases[count].isUnused = false;

		count++;
	}

	inFile.close();
	arrayLength = count; //array length variable to use throughout the program 

}
// function to return the number of unique letters in a particular phrase

int uniqueLetterCount(const string& text)
{

	string::size_type index = 0;
	string unique;
	int position;
	string textLower = text;
	string::size_type guessesRequired;


	for (index = 0; index < textLower.size(); index++)
	{
		textLower[index] = tolower(textLower[index]); //convert phrase to lower case
		position = unique.find(textLower[index]);
		if (position == string::npos)
		{
			if (isalpha(textLower[index])) //determine if the char is a letter
			{
				unique = unique.append(1, (textLower[index])); // a string of the unique letters in the phrase
			}
		}

	}

	guessesRequired = unique.size(); // the guesses required to guess the phrase
	return guessesRequired;


}

//void function to sort the phrases according to how many unique guesses will be required per phrase
void sortPhrases(Phrase phrases[], int arrayLength)
{
	int minGuessReq;
	Phrase temp;
	int location;

	for (int index = 0; index < arrayLength; index++)
	{
		minGuessReq = index;
		for (location = index + 1; location < arrayLength + 1; location++)
		{
			if (phrases[location].guessesRequired < phrases[minGuessReq].guessesRequired)
				minGuessReq = location;
		}


		temp = phrases[minGuessReq];
		phrases[minGuessReq] = phrases[index];
		phrases[index] = temp;
	}

}
//void function to print to the screen an array of phrases. one phrase per line
void printPhrases(Phrase phrases[], int arrayLength)
{
	//printing header file
	cout << left << setw(10) << "Guesses" << endl;
	cout << setw(15) << left << "Required";
	cout << setw(47) << "Phrase";
	cout << left << "Is Used";
	cout << endl;
	for (int i = 1; i < 76; i++)
	{
		cout << "_";
	}
	cout << endl;


	//printing phrases array
	for (int row = 0; row < arrayLength; row++)
	{
		cout << setw(9) << left << phrases[row].guessesRequired << "    "; //setting the columns for print display
		cout << setw(50) << left << phrases[row].text;
		if (phrases[row].isUnused == false)
		{
			cout << "Unused";
		}
		else
		{
			cout << "Used";
		}
		cout << endl;
	}
}
// function to create the phrase with blanks in place of the alpha characters separated by a space between each character
string phraseWithBlanks(const string& text, const string& correctLoLetters)
{
	string blanksPhrase;
	string::size_type index = 0;
	string textLower = text;
	int i = 0;



	for (index = 0; index < textLower.size(); index++)
	{
		if (isalpha(textLower[index])) //determining if charactere is a letter using isalpha function
		{
			textLower[index] = tolower(textLower[index]); //converting letters to lower case
			if (correctLoLetters.find(textLower[index]) != string::npos)
			{
				blanksPhrase.append(1, text[index]);
			}
			else
			{
				blanksPhrase.append(1, '_');
			}
		}
		else
		{
			blanksPhrase.append(1, text[index]);
		}

		blanksPhrase.append(1, ' '); //adding spaces between characters


	}
	

	return blanksPhrase;
}
//function to request the user for a difficulty level to play at.
int getDifficultyLevel(int level)
{

	//requesting the input from the user
	cout << "Enter a number between 1 and 3 for the difficulty level you would like to play: ";
	cin >> level;
	cout << endl;

	while (!(level >= 1 && level <= 3))

	{
		cout << "You must enter the numeral 1, 2 or 3. Pleaase re-enter the difficulty level you would like: ";
		cin >> level;
	}

	return level;
}
//function to get a random number between a minimum and maximum value
int randomIndex(int min, int max)
{
	int range = max - min + 1;

	return rand() % range + min;
}
//function to loop through the game as user guesses letters. 
void runGame(string& Phrase, int guessesRequired, int& noOfPhrasesGuessed)
{
	int missCount = 0;
	int correctGuesses = 0;
	string correctLoLetters = "";
	char guess;
	string alreadyGuessed = "";
	string missedLetters = "";
	string lowerPhrase = toLowerCase(Phrase);

	do
	{
		drawGallows(missCount); //draw the gallows and update and user guesses letters
		cout << "Previous incorrect guesses: " << missedLetters << endl << endl; //print a string of the missed guesses by the user
		cout << phraseWithBlanks(Phrase, correctLoLetters) << endl << endl << endl;
		cout << "Enter a guess: ";
		cin >> guess;
		cout << endl;

		if (!isalpha(guess)) //if not a letter - ask user to input letter again
		{
			cout << guess << " is not a valid guess. Please enter a letter: ";
			cin >> guess;
			cout << endl;
		}
		else if (alreadyGuessed.find(guess) != string::npos) //if letter guessed is already guessed - ask user to guess another letter
		{
			cout << "Error: You have already guessed an " << guess;
			cout << endl;
		}
		else if (lowerPhrase.find(guess) != string::npos) // if letter is a part of the phrase - fill in blank with correct letter
		{
			cout << "Good guess!" << endl;
			correctLoLetters.append(1, guess);
			correctGuesses += 1;
		}
		else
		{
			cout << "Sorry, bad guess." << endl; // if letter is not a part of the phrase increment missed count integer and add letter to missed letters string
			missedLetters.append(1, guess);
			missCount += 1;
		}
		alreadyGuessed.append(1, guess);
		
	} while (missCount < 5 && correctGuesses < guessesRequired); //condition to end game

	drawGallows(missCount);

	if (missCount >= 5) //conditional to tell user if they won or lost and what the phrase was
	{
		cout << "Your Dead! The phrase was \"" << Phrase << "\"." << endl << endl;
	}
	else
	{
		cout << "You Win! The phrase was \"" << Phrase << "\"." << endl << endl;
	}
	noOfPhrasesGuessed += 1;

}

void drawGallows(int missCount)
{
	// Output the top of the gallows
	cout
		<< "   +----+     \n"
		<< "   |    |     \n";

	// Output the stand and body
	switch (missCount)
	{
	case 0:
		cout
			<< "   |       \n"
			<< "   |       \n"
			<< "   |       \n"
			<< "   |       \n";
		break;
	case 1:
		cout
			<< "   |    O  \n"
			<< "   |    |  \n"
			<< "   |       \n"
			<< "   |       \n";
		break;
	case 2:
		cout
			<< "   |    O  \n"
			<< "   |   /|  \n"
			<< "   |       \n"
			<< "   |       \n";
		break;
	case 3:
		// The \\ will translate as '\', because it is a special char
		cout
			<< "   |    O   \n"
			<< "   |   /|\\ \n"
			<< "   |        \n"
			<< "   |        \n";
		break;
	case 4:
		cout
			<< "   |    O   \n"
			<< "   |   /|\\ \n"
			<< "   |     \\ \n"
			<< "   |        \n";
		break;
	default:
		cout
			<< "   |    O    \n"
			<< "   |   /|\\  \n"
			<< "   |   / \\  \n"
			<< "   |You're Dead\n";
	}

	// Output the base
	cout << "  =============\n" << endl;
}
//function to convert phrase in runGame function to lower letters
string toLowerCase(const string& Phrase)
{
	string::size_type index = 0;
	string unique;
	int position;
	string textLower = Phrase;


	for (index = 0; index < textLower.size(); index++)
	{
		textLower[index] = tolower(textLower[index]); //converting to lower letters
		position = unique.find(textLower[index]);
		if (position == string::npos)
		{
			if (isalpha(textLower[index]))
			{
				unique = unique.append(1, (textLower[index])); //creating a string of the letters
			}
		}
	}
	return textLower;
}
