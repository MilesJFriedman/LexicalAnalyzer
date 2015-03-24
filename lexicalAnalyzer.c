// Miles Friedman
// HW2: Lexical Analyzer
// COP3402 Systems Software

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Function Prototypes:
void fillCleanInput ();
void fillLexemeOutput ();

int main (int argc, char* argv[]) {
	
	fillCleanInput ();
	
	fillLexemeOutput ();
	
	return 0;
}

//Function Definitions:
void fillCleanInput () {
	FILE* input;
	FILE* cleanInput;
	FILE* lexemeTable;
	FILE* lexemeList;
	input = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\Friedman_Miles-HW2.zip\\input.txt", "r");
	cleanInput = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\Friedman_Miles-HW2.zip\\cleaninput.txt", "w");
	
	//check to make sure the file exists.
	if (input == NULL)
		printf("Error, unable to read the file...");
	else {
		int commentFlag1 = 0; //a flag which will determine whether or not the cursor is in the middle of reading a comment 
		int commentFlag2 = 0; //a flag which will determine whether or not the cursor is in the middle of reading a comment 
		int inComment = 0; //tells whether or not the cursor is in the middle of a comment 
		int c = 0; //holds the current character returned by getC
		int previousC = 0; //holds the previous character returned by getC
		int markPrevious = 0; //if this flag is set to 1, the lexical analyzer should print the previously scanned char and current char to cleaninput.txt
		
		c = getc(input);
				
		//if the character read is not the end of file marker: 
		while (c != EOF) {
			
			//test for the end of a comment:
			if (c == 42 && commentFlag1 == 1 && commentFlag2 == 1) {//If in the middle of a comment and a '*' is found...
				commentFlag1 = 0;
				previousC = c;
				c = getc(input);
				if (c == 47 && commentFlag1 == 0 && commentFlag2 == 1) {//test if the '/' is found...
					commentFlag2 = 0; //if so, reset commentFlag2 to signify that the cursor is no longer in the middle of a comment
				} else if (c != 47 && c != 42 && commentFlag1 == 0 && commentFlag2 == 1) //the required '/' has not been so the end of comment is a false alarm
				commentFlag1 = 1; //set commentFlag1 back to 1 to signify that the cursor is still in the middle of a comment
				//c = previousC;
			}
		
			//test for the beggining of a comment:
			else if (c == 47 && commentFlag1 == 0 && commentFlag2 == 0) {//is the character a '/'
				commentFlag1 = 1;
				previousC = c;
				c = getc(input);
				if (c == 42 && commentFlag1 == 1 && commentFlag2 == 0) {//is the character a '*'
				commentFlag2 = 1;
				inComment = 1;
				} else if (c != 42 && c != 47 && commentFlag1 == 1) {//If a '*' is not found immedeatly after a '/' symbol, then the possible comment was a flase alarm
				commentFlag1 = 0; //reset commentFlag1 back to 0 as the possible comment was a false alarm
				markPrevious = 1;
				}
			}
			
			if (markPrevious == 1) {
				fprintf(cleanInput, "%c", previousC);
				markPrevious = 0;
			}
			if (inComment != 1) //if the character read is part of the actual PL/0 code and not a comment, output the character to the cleanInput file
				fprintf(cleanInput, "%c", c);
			
			if (commentFlag1 == 0 && commentFlag2 == 0) //use this to notify the program that the cursor is no longer in the middle of a comment
				inComment = 0;
				
			c = getc(input); //scan the next character
		}
		
	}	
	
	fclose(input);
	fclose(cleanInput);
}

void fillLexemeOutput () {
	int errorFlag = 0; //will be used to determine whether or not an error has been found and the program should terminate
	int completeTokenFlag = 0; //used to determine whether or not the token is complete and no error has been found
	int skipFlag = 0; //will be used to determine whether or not the next character has already been read by the cursor
	int identMax = 11;
	int numMax = 5;
	int identCount = 0;
	int numCount = 0;
	int isIdent = 0; //boolean to test whether or not a scanned character is an identifier
	int isDigit = 0; //boolean to test whether or not a scanned character is a digit
	char integer[5] = {'x', 'x', 'x', 'x', 'x'}; //create an array to hold the integer
	char identifier[11]; //create an array to hold the identifier

	FILE* input;
	FILE* lexemeTable;
	FILE* lexemeList;
	input = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\Friedman_Miles-HW2.zip\\cleaninput.txt", "r");
	lexemeTable = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\Friedman_Miles-HW2.zip\\lexemetable.txt", "w");
	lexemeList = fopen ("C:\\Users\\Miles\\Desktop\\P-MACHINE\\Friedman_Miles-HW2.zip\\lexemelist.txt", "w");
	
	//check to make sure the file exists.
	if (input == NULL)
		printf("Error, unable to read the file...");
	else {
		int c = 0; //handles the current character the cursor is on
		
		fprintf(lexemeTable, "lexeme		token type\n");
		
		c = getc(input);
		
		while (c != EOF) {
			
			//determine whether or not the token is meant to be a digit:
			if (c >= 48 && c <= 57) { //if the first character read is a digit,
				integer[numCount] = c; //start to fill the integer array
				numCount++; //incriment the digit count incase the character is part of a number token
				
				while (numCount < numMax) { //while the number is under 5 digits total and valid
				
					c = getc(input); //scan the next character
					
					//the next scanned char is a space or valid symbol or letter, the integer token must be complete so print the lexeme and token type to lexemetable.txt
					if (c == 32 || c >= 65 && c <= 90 || c >= 97 && c <= 122 || c >= 42 && c <= 47 || c >= 58 && c <= 62) {
						//loop through and print the integer in the array
						int i = 0;
						fprintf(lexemeList, "3 ");
						while (integer[i] != 'x') {
							fprintf(lexemeTable, "%c", integer[i]);
							fprintf(lexemeList, "%c", integer[i]);
							i++;
						}
						
						fprintf(lexemeList, " ");
						
						//print the lexeme token for int which is 3
						fprintf(lexemeTable, "		3\n");
						
						//reset the integer array and numCount
						integer[0] = 'x';
						integer[1] = 'x';
						integer[2] = 'x';
						integer[3] = 'x';
						integer[4] = 'x';
						numCount = 0;
						completeTokenFlag = 1;
						skipFlag = 1;
						break;
						
					} else if (c >= 48 && c <= 57) { //if the next char scanned is a digit
						integer[numCount] = c;
						numCount++; //incriment numCount
						
					} else { //an invalid symbol has been encountered
						printf("Error: Invalid symbol encountered, terminating...");
						errorFlag = 1;
						break;
					}
				}
				
				//this if else tests whether or not a valid integer has been found and printed in lexemetable.txt,
				//if so, the program will continue. Otherwise, the number must be too long and therefore an error.
				if (completeTokenFlag != 1) {
					printf("Error: Number too long, terminating...");
					errorFlag = 1;
				} else
					completeTokenFlag = 0;
			
			
			} //handles cases where the token is one of the standard valid symbols:	
			else if (c == 43) {
				fprintf(lexemeTable, "+		4\n");
				fprintf(lexemeList, "4 ");
			} else if (c == 45) {
				fprintf(lexemeTable, "-		5\n");
				fprintf(lexemeList, "5 ");
			} else if (c == 42) {
				fprintf(lexemeTable, "*		6\n");
				fprintf(lexemeList, "6 ");
			} else if (c == 47) {
				fprintf(lexemeTable, "/		7\n");
				fprintf(lexemeList, "7 ");
			} else if (c == 61) { 
				fprintf(lexemeTable, "=		9\n");
				fprintf(lexemeList, "9 ");
			} else if (c == 60) {
				c = getc(input);
				skipFlag = 1;
				if (c == 62) {
					fprintf(lexemeTable, "<>		10\n");
					fprintf(lexemeList, "10 ");
				} else if (c == 61) {
					fprintf(lexemeTable, "<=		12\n");
					fprintf(lexemeList, "12 ");
				} else {
					fprintf(lexemeTable, "<		11\n");
					fprintf(lexemeList, "11 ");
				}
			} else if (c == 62) {
				c = getc(input);
				skipFlag = 1; //because we have scanned the next char already, we don't need to do so later
				if (c == 61) {
					fprintf(lexemeTable, ">=		14\n");
					fprintf(lexemeList, "14 ");
				} else {
					fprintf(lexemeTable, ">		13\n");
					fprintf(lexemeList, "13 ");
				}
			} else if (c == 40) {
				fprintf(lexemeTable, "(		15\n");
				fprintf(lexemeList, "15 ");
			} else if (c == 41) {
				fprintf(lexemeTable, ")		16\n");
				fprintf(lexemeList, "16 ");
			} else if (c == 44) {
				fprintf(lexemeTable, ",		17\n");
				fprintf(lexemeList, "17 ");
			} else if (c == 59) {
				fprintf(lexemeTable, ";		18\n");
				fprintf(lexemeList, "18 ");
			} else if (c == 46) {
				fprintf(lexemeTable, ".		19\n");
				fprintf(lexemeList, "19 ");
			} else if (c == 58) {
				c = getc(input);
				if (c == 61) {
					fprintf(lexemeTable, ":=		20\n");
					fprintf(lexemeList, "20 ");
				} else {
					printf("Error: Invalid symbol encountered, terminating...");
					errorFlag = 1;
					break;
				}
			}
			
			
			
			//handles cases for identifiers or key words:
			//if the first char scanned is a letter...
			else if (c >= 65 && c <= 90 || c >= 97 && c <= 122) {
				
				completeTokenFlag = 0;
				identifier[identCount] = c;
				identCount++;
				
				while (identCount < identMax) {
					
					c = getc(input);
					
					//if the next char in the string of chars is a digit or letter, add that char to the identifier in the array
					if (c >= 65 && c <= 90 || c >= 97 && c <= 122 || c >= 48 && c <= 57) {
						identifier[identCount] = c;
						identCount++;
					} else {//otherwise break out of the while loop
						completeTokenFlag = 1;
						skipFlag = 1;
						break;
					}
					
				}
				
				if (completeTokenFlag != 1) {
					printf("Error: The variable is too long, terminating...");
					break;
				} else
					completeTokenFlag = 0;
				
				if (strncmp("odd", identifier, identCount) == 0) {
					fprintf(lexemeTable, "odd		8\n");
					fprintf(lexemeList, "8 ");
				} else if (strncmp("begin", identifier, identCount) == 0) {
					fprintf(lexemeTable, "begin		21\n");
					fprintf(lexemeList, "21 ");
				} else if (strncmp("end", identifier, identCount) == 0) {
					fprintf(lexemeTable, "end		22\n");
					fprintf(lexemeList, "22 ");
				} else if (strncmp("if", identifier, identCount) == 0) {
					fprintf(lexemeTable, "if		23\n");
					fprintf(lexemeList, "23 ");
				} else if (strncmp("then", identifier, identCount) == 0) {
					fprintf(lexemeTable, "then		24\n");
					fprintf(lexemeList, "24 ");
				} else if (strncmp("while", identifier, identCount) == 0) {
					fprintf(lexemeTable, "while		25\n");
					fprintf(lexemeList, "25 ");
				} else if (strncmp("do", identifier, identCount) == 0) {
					fprintf(lexemeTable, "do		26\n");
					fprintf(lexemeList, "26 ");
				} else if (strncmp("call", identifier, identCount) == 0) {
					fprintf(lexemeTable, "call		27\n");
					fprintf(lexemeList, "27 ");
				} else if (strncmp("const", identifier, identCount) == 0) {
					fprintf(lexemeTable, "const		28\n");
					fprintf(lexemeList, "28 ");
				} else if (strncmp("var", identifier, identCount) == 0) {
					fprintf(lexemeTable, "var		29\n");
					fprintf(lexemeList, "29 ");
				} else if (strncmp("procedure", identifier, identCount) == 0) {
					fprintf(lexemeTable, "procedure		30\n");
					fprintf(lexemeList, "30 ");
				} else if (strncmp("write", identifier, identCount) == 0) {
					fprintf(lexemeTable, "write		31\n");
					fprintf(lexemeList, "31 ");
				} else if (strncmp("read", identifier, identCount) == 0) { 
					fprintf(lexemeTable, "read		32\n");
					fprintf(lexemeList, "32 ");
				} else if (strncmp("else", identifier, identCount) == 0) {
					fprintf(lexemeTable, "else		33\n");
					fprintf(lexemeList, "33 ");
				} else {
					fprintf(lexemeTable, "%s		2\n", identifier);
					fprintf(lexemeList, "2 ");
					fprintf(lexemeList, "%s ", identifier);
				}
			}
			
			
					
			identCount = 0;	
			memset(identifier, '\0', 11);	
					
			if (errorFlag == 1)
				break;
			
			if (skipFlag == 0)	
				c = getc(input);
				
			skipFlag = 0; //reset skipFlag
		}
	}
	

fclose(input);
fclose(lexemeTable);
fclose(lexemeList);
}
