#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10
#define BOMBS 15


void placeBombs(char board[SIZE][SIZE]) {
    int bombsPlaced = 0;

    //Initialize the library for random numbers
    srand(time(NULL)); 

    //Until 15 bombs are placed, randomly select a row and column and place a bomb (ensuring that there isn't already one there)
    while (bombsPlaced < BOMBS) {
        int rowselected = rand() % SIZE;
        int columnselected = rand() % SIZE;

        if (board[rowselected][columnselected] != 'X'){
            board[rowselected][columnselected] = 'X';
            bombsPlaced++;
        }
    }
}

int countAdjacentBombs(char board[SIZE][SIZE], int x, int y){
    int bombsAdjacent = 0;

    //Initialize the differences in x and y needed to reach the adjacent points (8 in total)
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Check if the selected cell itself has a bomb
    if (board[x][y] == 'X'){
        return -1;  // Return this value if a bomb is hit, it will be used later
    }

    //For 8 iterations, add the difference in x and y to reach the new point and check for a bomb
    for (int i = 0; i < 8; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        // Just to make sure, confirm that newX and newY are valid indices (within the board)
        if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE) {
            if (board[newX][newY] == 'X') {
                bombsAdjacent++;
            }
        }
    }

    // Set the value of the cell the user picked based on the adjacent bombs present
    if (bombsAdjacent == 0) {
        board[x][y] = '0';  // No adjacent bombs
    } else {
        board[x][y] = '0' + bombsAdjacent;  // Convert integer to character representation
    }

    return bombsAdjacent;
}

int main() {
    int gameOver = 0;
    int remainingCells = SIZE * SIZE - BOMBS;
    char board[SIZE][SIZE];
    char displayBoard[SIZE][SIZE];
    int x, y;
    int result;

    // Initialize the library for random numbers
    srand(time(NULL));

    // Initialize both of the boards with dashes
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '-';
            displayBoard[i][j] = '-';
        }
    }

    // Place the bombs on the game board only, NOT the display board
    placeBombs(board);

    printf("Welcome to Minesweeper!\n");

    while (remainingCells > 0 && !gameOver) {
        // Display column numbers on top
        for (int i = 0; i < SIZE; i++) {
            if (i == 0) printf("  ");
            printf("%d ", i);
        }
        printf("\n");

        // Print the display board for the user
        for (int i = 0; i < SIZE; i++) {
            // Display row numbers along the left side
            printf("%d ", i);
            for (int j = 0; j < SIZE; j++) {
                printf("%c ", displayBoard[i][j]);
            }
            printf("\n");
        }
        // Ensure valid coordinates are picked (Checks for two cases, one if they're out of range, and one for the spot already being revealed)
        printf("Enter the coordinates (x y) to reveal a cell: ");
        result = scanf("%d %d", &x, &y);

        if (result == 2) {
        printf("Both values were successfully scanned: x = %d, y = %d\n", x, y);
        goto continued;
        } 

        else if (result != 2) {
        printf("Invalid input.\n");
        continue;
        }

        continued:
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            printf("Coordinates are out of range or invalid. Please enter numbers between 0-9.\n");
            continue;
        } else if (displayBoard[x][y] != '-') {
            printf("This coordinate has already been revealed. Try again!\n");
            continue;
        }
        // Use the countAdjacentBombs function from earlier to check if a bomb was hit. If so, modify the gameOver value
        int adjacentBombs = countAdjacentBombs(board, x, y);
        if (adjacentBombs == -1) {
            printf("You've hit a bomb! Game Over.\n");
            displayBoard[x][y] = 'X';  // Mark the hit bomb
            gameOver = 1;
        } else {
            displayBoard[x][y] = board[x][y];
            remainingCells--;
        }
    }

    // If the game is over (regarless of how), display the actual board with bomb locations
    if (gameOver) {
        printf("\n");
        printf("Here's the rest of the board:\n");
        // Display column numbers on top
        for (int i = 0; i < SIZE; i++) {
            if (i == 0) printf("  ");
            printf("%d ", i);
        }
        printf("\n");

        for (int i = 0; i < SIZE; i++) {
            // Display row numbers along the left side
            printf("%d ", i);
            for (int j = 0; j < SIZE; j++) {
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }
    }
    //In the case that you've won the game, there must be zero remaining cells
    if (remainingCells == 0) {
        printf("Congratulations! You've cleared the board without hitting any bombs!\n");
    }
}
