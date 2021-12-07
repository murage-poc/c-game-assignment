#include<stdio.h>

// Define the MAXIMUM Size of the board, consider this a constant value you can use anywhere.
// No other global variables can be used.
#define BOARD_SIZE 25

// Prototype for the Bonus question see below for details..
void modifyBoard(const int *row, const int *col, char gameBoard[][BOARD_SIZE]);

/* Function: readBoard
 * Parameters: FILE - a pointer to a file. Assume the file has been opened and ready to read from.
 * 		int* boardSize - will be read from the file to actual board size.
 * 		char gameBoard[][BOARD_SIZE] - is a square grid of char characters. It is a multi-dimensional array.
 * Return type: void
 * Description: This function should read a valid game board from a file into the parameter gameBoard.
 * 		You will first need to read the size of the board then the game board itself.
 * 		boardSize and gameBoard should contain the contents of the file.
 */
void readBoard(FILE *filePtr, int *boardSize, char gameBoard[][BOARD_SIZE]);

/* Function: printBoard
 * Parameters:
 * 		int boardSize -which is the size of the square gameboard gride.
 * 		char gameBoard[][BOARD_SIZE] - is a square grid of char characters. It is a multi-dimensional array.
 * 		const int useQuestionMarks = 0 indicates show the characters in the board 1 indicates show ? for all squares.
 * Return type: void
 * Description: This function prints to the screen the gameboard or character array. It contains one option 'useQuestionMark'
 * 		which allows the board to be shown as all ? or shows the actual square values.
 */
void printBoard(const int boardSize, const char gameBoard[][BOARD_SIZE], const int useQuestionMark);

/* Function: isValidSquare
 * Parameters:
 * 		int row - the row value you want to confirm is valid.
 * 		int col - the column value you want to confirm is valid
 * 		int boardSize = the row and column length of the grid board.
 * Return type: int which is boolean for 1 == true, 0 == false
 * Description: Returns whether the row and column values are valid in the board size.
 */
int isValidSquare(int row, int col, int boardSize);

/* Function: menu
 * Parameters: void
 * Return type:void
	* Description: This prints the menu options for the game.
 */
void menu() {
    printf("\nFor this game you will will pick three squares.\n");
    printf("If the square contains an 'S' it is safe.\n");
    printf("If the square contains an 'B' you lose.\n");
    printf("If the square contains an 'R' you must pick again.\n");
    printf("If the square contains an 'C' it safely clears the area (Bonus).\n");
    printf("All row columns will be 0 based so 3 columns will be referred to as 0,1, and 2\n\n");
}


/* Function: playGame
 * Parameters:
 * 		int boardSize = the row and column length of the grid board.
 * 		const char gameBoard[][] - is the gameboard that has already been read.
 * Return type: int which is true(1) if they won otherwise it returns 0
 * Description: Runs the core mechanics of the game and returns if it was won or not.
 * 		It does not print if the game was won or not.
 * 		It needs to identify when the game is over and return the win/lose result.
 * 		The game board should be all at and ready to be played.
 */
int playGame(int boardSize, char gameBoard[][BOARD_SIZE]) {


    // Declare variables to help play the game
    int won = -1; //will hold the results. 1=lost, 0= win
    int endGame = 1; //will be used to indicate if to end game. 1=no, 0=yes
    int correctChoicesCount = 0; //number of correct choices
    int row, col; //will hold user row and column choice.

    // Print the menu
    menu();

    // print the game boards with question marks
    printBoard(boardSize, gameBoard, 1);

    // Play the game loop
    while (endGame != 0) {
        printf("\n");
        // Collect the row and column for their guess.
        printf("Please input the row and column you want to select\n");

        scanf("%d %d", &row, &col);

        //validate if their choice is valid based on the board
        if (isValidSquare(row, col, boardSize) == 1) {
            printf("You've input an invalid square position, please try again.\n");
            continue; //skip executing all the logic below this
        }


        // print what the square contained.
        printf("The game board tile was %c\n", gameBoard[row][col]);

        // Check if the game is over or not
        if (gameBoard[row][col] == 'S') {
            printf("You are safe for now.\n");
            ++correctChoicesCount; //increment the choices

            if (correctChoicesCount == 3) {
                endGame = 0; //stop the game
                won = 0; //set to won
            }
        } else if (gameBoard[row][col] == 'R') {
            printf("You must pick again.\n");
        } else if (gameBoard[row][col] == 'B') {

            printf("You have hit a bad square.\n");
            won = 1; //set to lost
            endGame = 0;//stop the game
        } else if (gameBoard[row][col] == 'C') {
            modifyBoard(&row, &col, gameBoard);
        }

    }

    return won;
}


int main(int argc, char **argv) {
    // Your main function should declare the variables you need for the game.
    // It should allow the user to input a filename.
    // It should open that filePtr or indicate an error ( this might be a separate function).
    // It should read the game board
    // 	make the function call to play the game, and print the result of the game.

    // You may add your own functions.
    // Declare variables
    char filename[255]; //will hold the file name
    FILE *filePtr;
    int boardSize;
    // Request your game board filePtr here.
    printf("Please input the filename you wish to open: \n");
    scanf("%s", filename);

    //open board game file
    filePtr = fopen(filename, "r");

    if (filePtr == NULL) {

        printf("Could not open the file %s", filename);
    } else {

        //get the board size
        fscanf(filePtr, "%d", &boardSize);


        //board size should be between 1 & 25
        if (boardSize > 1 && boardSize < BOARD_SIZE) {


            char gameBoard[boardSize][boardSize];

            // Read the board
            readBoard(filePtr, &boardSize, gameBoard);

            //play the game
            if (playGame(boardSize, gameBoard) == 1) {
                printf("You've lost the game.\n");
            } else {
                printf("You survived!\n");
            }

            //print the board
            printf("The board game was \n");
            printBoard(boardSize, gameBoard, 0);
        }
    }
    //free the allocated memory
    //free(filePtr);

    return 0;
}

void readBoard(FILE *filePtr, int *boardSize, char gameBoard[][BOARD_SIZE]) {
    //skip the first line. Assumption: each line is separated by end of line character (\n)
    char skipChar = '\0';
    while (skipChar != '\n') { //read up to the end of first line
        skipChar = fgetc(filePtr);
    }

    int row;
    int col;
    for (row = 0; row < *boardSize; ++row) {
        for (col = 0; col < *boardSize; ++col) {
            //read the character at that position
            gameBoard[row][col] = fgetc(filePtr);
        }
        //we skip the line separator by reading it but not assigning it to anything
        fgetc(filePtr);

    }
}

void printBoard(const int boardSize, const char gameBoard[][BOARD_SIZE], const int useQuestionMark) {
    //for each row
    for (int row = 0; row < boardSize; ++row) {
        //for each column on the row
        for (int col = 0; col < boardSize; ++col) {
            if (useQuestionMark == 1) {
                printf("?");
            } else {
                printf("%c", gameBoard[row][col]);
            }
        }
        printf("\n"); //print new line to separate rows
    }
}

int isValidSquare(int row, int col, int boardSize) {
    //if the row and column are less than zero, or, they are greater than board size, return invalid
    if (row < 0 || col < 0 || row >= boardSize || col >= boardSize) {
        return 1;
    }
    return 0;
}


/* Bonus Part (5 Marks)
 * Write a function that is performed after the user picks a square with a new letter 'C'
 * This function will covert all adjacent neighbour squares (distance of 1) to 'S'
 * This makes all those squares safe.
 *
 * Ex) with the board
 *     BBB
 *     BCB
 *     BBB
 *
 *     If the user selects 'C' at 1,1  the 'B' squares are tranformed to 'S' since they are adjacent neighbours.
 *     The C square should not be transformed into a 'S'.
 *
 *     You must not modify the function prototype.
 *     You will need to modify your playGame function to achieve this.
 *     You should at least one other function in this function to complete the task.
 */
void modifyBoard(const int *row, const int *col, char gameBoard[][BOARD_SIZE]) {
    int boardSize = sizeof(*gameBoard) / sizeof(gameBoard[0][0]) / 2;


    //if there are available neighbours at immediate top row
    if (*row > 0) {
        //immediate top box
        gameBoard[*row - 1][*col] = 'S';

        //if there is a bottom left neighbour
        if (*col > 0) {
            gameBoard[*row - 1][*col - 1] = 'S';
        }

        //if there is a bottom right neighbour
        if (*col < boardSize - 1) {
            gameBoard[*row - 1][*col + 1] = 'S';
        }

    }

    //if there is a left neighbour on same row
    if (*col > 0) {
        gameBoard[*row][*col - 1] = 'S';
    }

    //if there is a right neighbour on same row
    if (*col < boardSize - 1) {
        gameBoard[*row][*col + 1] = 'S';
    }


    //if there are available neighbours at immediate bottom row
    if (*row < boardSize) {

        //immediate bottom box
        gameBoard[*row + 1][*col] = 'S';

        //Check neighbouring diagonal boxes

        //if there is a bottom left neighbour
        if (*col > 0) {
            gameBoard[*row + 1][*col - 1] = 'S';
        }

        //if there is a bottom right neighbour
        if (*col < boardSize - 1) {
            gameBoard[*row + 1][*col + 1] = 'S';
        }

    }

}