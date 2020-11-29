
////////////////////////////////////////////////////////////////////////////////
// Main File:        check_queens.c
// This File:        check_queens.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 SPRING 2019
//
// Author:           Libby Howard
// Email:            ethoward@wisc.edu
// CS Login:         lhoward
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *COMMA = ",";

/* COMPLETED:
 * Retrieves from the first line of the input file,
 * the number of rows and columns for the board.
 *
 * fp: file pointer for input file
 * rows: pointer to number of rows
 * cols: pointer to number of columns
 */
void get_dimensions(FILE *fp, int *rows, int *cols) {
        char *line = NULL;
        size_t len = 0;
        if (getline(&line, &len, fp) == -1) {
                printf("Error in reading the file\n");
                exit(1);
        }

        char *token = NULL;
        token = strtok(line, COMMA);
        *rows = atoi(token);

        token = strtok(NULL, COMMA);
        *cols = atoi(token);
}

/* Finding if there are attacking queens in a minor diagonal
 * returns 1 if a pair exists and 0 if it does not
 * 
 * board: heap allocated 2D board
 * currRow: row where a queen was found 
 * currCol: col where a queen was fount
 * rows: number of rows
 * cols: number of columns
 */
int check_queens_diag_minor_helper(int **board, int currRow, int currCol, int rows, int cols) {
        //var to hold whether or not an attacking queen has been found
        int check = 0;
        //increment to get into next square
        currRow -= 1; 
        currCol += 1; 
        //checking row minor 
        //continues until the row/col is off of the board
        while(currRow >= 0 && currCol < cols){
                check = *(*(board + currRow) + currCol);
                //check if queen is found at that location
                if(1 == check){
                       // printf("found row minor\n");
                        return 1; 
                } else {
                        //increment to get to next square
                        currRow -= 1; 
                        currCol += 1;  
                }   
        }
        return 0;
        
}


/* Finding if there are attacking queens in a major diagonal
 * returns 1 if a pair exists and 0 if it does not
 * 
 * board: heap allocated 2D board
 * currRow: row where a queen was found 
 * currCol: col where a queen was fount
 * rows: number of rows
 * cols: number of columns
 */
int check_queens_diag_major_helper(int **board, int currRow, int currCol, int rows, int cols) {
        //var to hold whether or not an attacking queen has been found
        int check = 0;
//        printf("%d, %d\n", currRow, currCol);
        // increment to get into next square
        currRow += 1; 
        currCol += 1;
        //checking row major 
       // continues until the row/col is off of the board
       while(currRow < rows && currCol < cols){
                check = *(*(board + currRow) + currCol);
                //check if queen is found at that location
                if(1 == check){
                        //printf("found row major\n");
                        return 1; 
                } 
                else {
                        //increment to get to next square
                        currRow += 1; 
                        currCol += 1;  
                }   
       }
        return 0;
        
}



/*
 * Returns 1 if and only if there exists at least one pair
 * of queens that can attack each other.
 * Otherwise returns 0.
 *
 * board: heap allocated 2D board
 * rows: number of rows
 * cols: number of columns
 */
int check_queens(int **board, int rows, int cols) {

        //integer storing whether or not a queen has been found 
        int check = 0; 

        // //checking to see if there are queens sharing the same rows
        for(int i = 0; i < rows; ++i) {
                check = 0; 
                for(int j = 0; j < cols; ++j) {
                        check += *(*(board + i) + j);
                        if(check > 1) {
                               //printf("queens attack in row");
                                return 1;
                        }
                }  
        }

        //checking to see if there are queens sharing the same cols
        for(int i = 0; i < cols; ++i) {
                check = 0; 
                for(int j = 0; j < rows; ++j) {
                        check += *(*(board + j) + i);
                        if(check > 1) {
                                //printf("queens attack in col");
                                return 1;
                        }
                }  
        }

        //checking to see if there are queens sharing the same diagonal 
        for(int i = 0; i < rows; ++i) {
                check = 0; 
                for(int j = 0; j < cols; ++j) {
                        int foundQueen = *(*(board + i) + j);
                        //checking to see if there is a queen at this location on the board
                        if(1 == foundQueen) {
                                //printf("found queen\n");
                                //call helper method to check the major diagonals 
                                check =  check_queens_diag_major_helper(board, i, j, rows, cols);
                                if (1 == check) {
                                        return 1; 
                                }
                                //call helper method to check the minor diagonals 
                                check =  check_queens_diag_minor_helper(board, i, j, rows, cols);
                                if(1 == check) {
                                        return 1; 
                                }
                                
                        }
                }
        }

        //return 0 when there is no pair found 
        return 0;
}


/* This program prints true if the input file has any pair
 * of queens that can attack each other, and false otherwise
 *
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {

        //Check if number of command-line arguments is correct.
        if(argc != 2) {
                fprintf(stderr, "Usage: ./check_queens <input_filename>");
                //printf("%i", argc);
                exit(1);
        }


        //Open the file and check if it opened successfully.
        FILE *fp = fopen(*(argv + 1), "r");
        if (fp == NULL) {
                printf("Cannot open file for reading\n");
                exit(1);
        }


        //Declare local variables.
        int rows = 0;
        int cols = 0;


        //Call get_dimensions to retrieve the board dimensions.
        get_dimensions(fp, &rows, &cols);


        //Dynamically allocate a 2D array of dimensions retrieved above.
        int **brd = malloc(sizeof(int*) * rows);

        if(brd == NULL) {
                fprintf(stderr, "Failed to properly allocate memory.\n");
                exit(1);
        }

        for(int i = 0; i < rows; i++) {
                *(brd+i) = malloc(sizeof(int) * (cols));
                if(*(brd+i) == NULL){
                        fprintf(stderr, "Failed to properly allocate memory.\n");
                        exit(1);
                }
        }


        //Read the file line by line.
        //Tokenize each line wrt comma to store the values in your 2D array.
        char *line = NULL;
        size_t len = 0;
        char *token = NULL;
        for (int i = 0; i < rows; i++) {

                if (getline(&line, &len, fp) == -1) {
                        printf("Error while reading the file\n");
                        exit(1);
                }

                token = strtok(line, COMMA);
                for (int j = 0; j < cols; j++) {
                        //initialize your 2D array
                        *(*(brd + i) + j) = atoi(token);
                        token = strtok(NULL, COMMA);
                }
        }

        //calling check_queens to find the return value
        int checkQueens = check_queens(brd, rows, cols);
        //printf("%d\n", checkQueens);
        //checks output of check_queens and prints appropriate value
        if(1 == checkQueens){
                printf("%s\n", "true");
        } else if(0 == checkQueens) {
                printf("%s\n", "false");
        }

        //Free all dynamically allocated memory.
        for(int i = 0; i < rows; ++i) {
                free(*(brd+i));
        }
        free(brd);


        //Close the file.
        if (fclose(fp) != 0) {
                printf("Error while closing the file\n");
                exit(1);
        }

        return 0;
}
