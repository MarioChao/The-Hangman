#ifndef __HANGMAN__
#define __HANGMAN__

#include <iostream>
#include <vector>

extern std::vector< std::string > wordList;

void generateWordList();
std::string getRandomWord();
int checkGuess(char);
void makeGuess(char);
bool repeatedWordGuess(std::string);
void makeWordGuess(std::string);
bool playerWin();
bool isGameOver();
void runHangman();

#endif