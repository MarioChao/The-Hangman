#include "../include/hangman.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// Extern variables
vector< string > wordList;

// Program-field variables
string word;

int maxWrongCharGuesses, maxGuesses;

int charGuesses, wrongCharGuesses, wordGuesses, allGuesses;
char guessList[26];
string wordGuessList[10], guessProgress;

// Generates a list of words from the words.txt file
void generateWordList() {
    ifstream WordsFile("include/words.txt");
    string fileLine;

    wordList.clear();
    while (getline(WordsFile, fileLine)) {
        wordList.push_back(fileLine);
    }

    WordsFile.close();
}

// Returns a random word from the word list
string getRandomWord() {
    srand(time(0));
    return wordList[rand() % wordList.size()];
}

// Checks if a guess is valid, and if it exists in the word
// Returns:
//  -2: invalid, not lowercase character
//  -1: invalid, repeated guess
//  0: valid, doesn't exist
//  1: valid, exists
int checkGuess(char guess) {
    // Check validation
    if (!(guess >= 'a' && guess <= 'z')) return -2;
    for (int i = 0; i < charGuesses; i++) {
        if (guessList[i] == guess) return -1;
    }

    // Check if exists
    for (int chId = 0; chId < word.length(); chId++) {
        if (word[chId] == guess) return 1;
    }

    // Doesn't exist
    return 0;
}

// Makes a guess by replacing the corresponding characters of the correct guess in the progress string
void makeGuess(char guess) {
    for (int chId = 0; chId < word.length(); chId++) {
        if (word[chId] == guess) {
            guessProgress[chId * 2] = guess;
        }
    }
}

// Returns whether the guessed word is repeated
bool repeatedWordGuess(string guessWord) {
    // Check if word is repeated
    for (int i = 0; i < wordGuesses; i++) {
        if (wordGuessList[i] == guessWord) return 1;
    }
    // Word is unique
    return 0;
}

// Makes a word guess by filling out the progress string
void makeWordGuess(string guessWord) {
    for (int chId = 0; chId < word.length(); chId++) {
        makeGuess(word[chId]);
    }
}

// Returns whether the player won
bool playerWin() {
    for (int chId = 0; chId < guessProgress.length(); chId++) {
        if (guessProgress[chId] == '_') return 0;
    }
    return 1;
}

// Returns whether the game is over
bool isGameOver() {
    if (wrongCharGuesses > maxWrongCharGuesses || wordGuesses > maxGuesses) return 1;
    if (playerWin()) return 1;
    return 0;
}

// Runs around of hangman
void runHangman() {
    // Generate word list if it's empty
    if (wordList.empty()) generateWordList();

    // Get a random word to guess
    word = getRandomWord();
    
    // Initialize guess variables
    maxWrongCharGuesses = 7;
    maxGuesses = 3;

    charGuesses = wrongCharGuesses = wordGuesses = allGuesses = 0;
    for (int i = 0; i < sizeof(guessList) / sizeof(guessList[0]); i++) {
        guessList[i] = '\0';
    }
    for (int i = 0; i < sizeof(wordGuessList) / sizeof(wordGuessList[0]); i++) {
        wordGuessList[i] = "";
    }
    guessProgress = "";
    for (int i = 0; i < word.length(); i++) {
        guessProgress += "_";
        if (i + 1 < word.length()) guessProgress += " ";
    }

    // Input variables
    char guess;
    string guessWord;

    // User-interaction (hangman)
    while (!isGameOver()) {
        // Print out info
        cout << "+ - - - - - - - - - - - - - - - - - - - +\n";
        cout << '\t' << "HANGMAN : Round " << allGuesses + 1 << '\n';
        // Guesses made
        cout << "Guesses: ";
        if (charGuesses == 0) cout << "(empty)";
        else {
            for (int i = 0; i < charGuesses; i++) {
                cout << guessList[i] << ' ';
            }
        }
        cout << '\n';
        cout << "Word guesses: ";
        if (wordGuesses == 0) cout << "(empty)";
        else {
            for (int i = 0; i < wordGuesses; i++) {
                cout << wordGuessList[i] << ' ';
            }
        }
        cout << '\n';
        // Word progress
        cout << "Current word: " << guessProgress << '\n';
        // Guess progress
        cout << "Wrong character guesses left: " << maxWrongCharGuesses - wrongCharGuesses << '\n';
        cout << "Wrong word guesses left: " << maxGuesses - wordGuesses << '\n';

        // User input
        cout << "Make a lowercase character guess (or \"0\" to guess the whole word):\n";
        cout << "> ";
        cin >> guess;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }

        // Valid char
        if (guess == '0') {
            // Word guess
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter the word: ";
            getline(cin, guessWord);
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input!\n";
                continue;
            } else if (repeatedWordGuess(guessWord)) {
                cout << "Repeated guess!\n";
                continue;
            }

            // Valid word
            wordGuessList[wordGuesses++] = guessWord;
            allGuesses++;
            if (guessWord != word) {
                cout << "Wrong guess.\n";
            } else {
                cout << "Correct!\n";
                makeWordGuess(guessWord);
                break;
            }
        } else {
            // Character guess
            int guessState = checkGuess(guess);
            if (guessState == -2) {
                cout << "Invalid guess!\n";
                continue;
            } else if (guessState == -1) {
                cout << "Repeated guess!\n";
                continue;
            }

            // Valid guess
            guessList[charGuesses++] = guess;
            allGuesses++;
            if (guessState == 0) {
                cout << "Wrong guess.\n";
                wrongCharGuesses++;
            } else {
                cout << "Correct!\n";
                makeGuess(guess);
            }
        }
    }

    // Game end
    cout << "+ - - - - - - - - - - - - - - - - - - - +\n";
    if (playerWin()) {
        cout << "Congratulations!\n";
        cout << "You have successfully guessed \"" << word << "\" in " << allGuesses << " rounds!\n";
        cout << "Player has made " << charGuesses << " character guesses, " << wrongCharGuesses << " wrong character guesses, and " << wordGuesses << " word guesses.\n";
        cout << "Thank you for playing!\n";
    } else {
        cout << "Game over!\n";
        cout << "The correct word was \"" << word << "\"!\n";
        cout << "Player has made " << charGuesses << " character guesses, " << wrongCharGuesses << " wrong character guesses, and " << wordGuesses << " word guesses.\n";
        cout << "The total time was " << allGuesses << " rounds.\n";
        cout << "Better luck next time!\n";
    }
}