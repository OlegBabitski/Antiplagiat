#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#define SAMPLE_SIZE 3
#define ASCII_CONVERT_CASE 32
#define MAX_LENGTH_WORD 128
#define SHORT_WORDS_LENGTH 2

#define SEPARATORS "!?;:,.~@#$&?%^*(){}[]\\/<>|_-+=\"\'"
#define ENG_LETTERS_TO_REPLACE "AaBCcEeHKMOoPpTXx"
#define RUS_LETTERS_TO_INSERT "АаВСсЕеНКМОоРрТХх"

using namespace std;
using namespace cgicc;

string getDB();//get origin text from db.txt (don't modify tis function)

double antiPlagiarism(string text, string fragment);

void deleteSeparators(string &text);
bool isSeparator(char c);
void deleteNumbers(string &text);
void deleteShortWords(string &text);
void engToRus(string &text);
void toLowercase(string &text);
void deleteRepetitions(string &text);

double textChecking(string text, string fragment);
int getCountOfWords(string text);
void splitIntoWords(string text, char textWords[][MAX_LENGTH_WORD]);
double getPercentOfPlagiat(char textWords[][MAX_LENGTH_WORD], char fragmentWords[][MAX_LENGTH_WORD], int countOfTextWords, int countOfFragmentWords);

int main()
{
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";

    name = form("name");
    if (!name.empty()) {
    	cout << antiPlagiarism(getDB(), name) << "\n";
    }

    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}

double antiPlagiarism(string text, string fragment) {
	deleteSeparators(text);
	deleteSeparators(fragment);
	
	deleteNumbers(text);
	deleteNumbers(fragment);
	
	engToRus(text);
	engToRus(fragment);
	
	toLowercase(text);
	toLowercase(fragment);
	
	deleteShortWords(text);
	deleteShortWords(fragment);
	
	deleteRepetitions(text);
	deleteRepetitions(fragment);

	return textChecking(text, fragment);
}

void deleteSeparators(string &text) {
	for (int i = 0; text[i] != '\0'; i++) 
		if (isSeparator(text[i])) 
			text[i] = ' ';
}

bool isSeparator(char c) {
	for (int i = 0; SEPARATORS[i] != '\0'; i++)
		if (c == SEPARATORS[i])
			return true;

	return false;
}

void deleteNumbers(string &text) {
	for (int i = 0; text[i] != '\0'; i++) 
		if (text[i] >= '0' and text[i] <= '9') 
			text[i] = ' ';
}

void deleteShortWords(string &text) {
	for (int i = 0, counter = 0; text[i] != '\0'; i++) {
		if (text[i] != ' ') {
			counter++;
		} else {
			if ((counter >= 1) and (counter <= SHORT_WORDS_LENGTH)) {
				for (int y = 1; y <= counter; y++) text[i - y] = ' ';
			}
			counter = 0;
		}
	}
}

void engToRus(string &text) {
	for (int i = 0; text[i] != '\0'; i++)
		for (int y = 0; ENG_LETTERS_TO_REPLACE[y] != '\0'; y++)
			if (text[i] == ENG_LETTERS_TO_REPLACE[y]) text[i] = RUS_LETTERS_TO_INSERT[y];
}

void toLowercase(string &text) {
	for (int i = 0; text[i] != '\0'; i++) 
		if (((text[i] >= 'А') and (text[i] <= 'Я')) or ((text[i] >= 'A') and (text[i] <= 'Z')))
			text[i] += ASCII_CONVERT_CASE;
}

void deleteRepetitions(string &text) {
	bool firstSpacePassed = false,
		 isTheSame = true;
	
	int startFirstWord = -1,
		endFirstWord = 0,
		startSecondWord = 0,
		endSecondWord = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
		
		if (text[i] != ' ') {
			firstSpacePassed = true;
			
			if (startFirstWord == -1) startFirstWord = i;
			if ((endFirstWord) and (!startSecondWord)) startSecondWord = i;
			if (text[i + 1] == '\0') endSecondWord = i;
			
		} else if (firstSpacePassed) {
			
			if (!endFirstWord) endFirstWord = i - 1;
			if ((startSecondWord) and (!endSecondWord)) endSecondWord = i - 1;
			
		} else {
		}
		
		if (endSecondWord) {
			
			if ((endFirstWord - startFirstWord) == (endSecondWord - startSecondWord)) {
				
				for(int f = startFirstWord, s = startSecondWord; f <= endFirstWord; f++, s++) {
					if (text[f] != text[s]) {
						isTheSame = false;
						break;
					}
				}
				
				if (isTheSame) {
					for(int f = startFirstWord; f <= endFirstWord; f++) text[f] = ' ';
					i = startSecondWord - 1;
				}
				
			}
			
			firstSpacePassed = false;
			isTheSame = true;
			
			startFirstWord = -1;
			endFirstWord = 0;
			startSecondWord = 0;
			endSecondWord = 0;
		}
	}
}

double textChecking(string text, string fragment) {
	double percentOfPlagiat = 0.0;
	
	int countOfTextWords = getCountOfWords(text),
		countOfFragmentWords = getCountOfWords(fragment);
	
	char textWords[countOfTextWords][MAX_LENGTH_WORD];
	char fragmentWords[countOfFragmentWords][MAX_LENGTH_WORD];
	
	splitIntoWords(text, textWords);
	splitIntoWords(fragment, fragmentWords);
	
	percentOfPlagiat = getPercentOfPlagiat(textWords, fragmentWords, countOfTextWords, countOfFragmentWords);
		
	return percentOfPlagiat;
}

int getCountOfWords(string text) {
	int counter = 0;
	
	for (int i = 0, countOfLetters = 0; text[i] != '\0'; i++) {
		if ((text[i] == ' ') or (text[i + 1] == '\0')) {
			if (countOfLetters) {
				counter++;
				countOfLetters = 0;
			}
		} else {
			countOfLetters++;
		}
	}
	
	return counter;
}

void splitIntoWords(string text, char textWords[][MAX_LENGTH_WORD]) {
	for(int i = 0, y = 0, x = 0, countOfSpaces = 0; text[i] != '\0'; i++) {
		if (text[i] != ' ') {
			textWords[y][x] = text[i];
			x++;
			countOfSpaces++;
		} else if (countOfSpaces != 0) {
			textWords[y][x] = '\0';
			x = 0;
			y++;
			countOfSpaces = 0;
		} else {
		}
	}
}

double getPercentOfPlagiat(char textWords[][MAX_LENGTH_WORD], char fragmentWords[][MAX_LENGTH_WORD], int countOfTextWords, int countOfFragmentWords) {
	bool isMatch = true;
	
	double countOfSamples = 0.0,
		   countOfMatches = 0.0;
	
	for (int i = 0; i < countOfFragmentWords - SAMPLE_SIZE + 1; i++) {
		for (int p = 0; p < countOfTextWords - SAMPLE_SIZE + 1; p++) {
			
			for(int z = 0, y1 = i, y2 = p; z < SAMPLE_SIZE; z++, y1++, y2++)
				for (int x = 0; fragmentWords[y1][x] != '\0'; x++)
					if (fragmentWords[y1][x] != textWords[y2][x])
						isMatch = false;
			
			if (isMatch) {
				countOfMatches++;
				break;
			} else isMatch = true;
			
		}
		
		countOfSamples++;
	}
	
	return (100 - ((countOfMatches / countOfSamples) * 100));
}

string getDB(){
	std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
    	dbText += s1;
  
    in.close();
    
    return dbText;
}
