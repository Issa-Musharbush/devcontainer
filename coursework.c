// A code to run simulations on a 9 by 9  map, to find the chances of success, mean steps to success, and the standard deviation 
//of the steps, by running 10 steps from each cell a thousand times.


#include <stdio.h>
// Enter any other #includes that you deem necessary below
#include <stdlib.h>
#include <math.h>


#define NUMROWS	 9    // map size
#define NUMCOLS  9    // map size
#define NUMWALKS 1000 // number of random walks
#define NUMSTEP  10   // number of steps in each walk
// Enter any other #defines as you deem necessary below

//prototypes
void simulate_cell(char map[NUMCOLS][NUMROWS], int inrow, int incol, int *result, int *steps);

void Find_suc(int results[NUMCOLS][NUMWALKS], double *Chances);

void Mean_path(int results[NUMCOLS][NUMWALKS], int steps[NUMCOLS][NUMWALKS], double *meanpath);

void Find_standDev(int results[NUMCOLS][NUMWALKS], int steps[NUMCOLS][NUMWALKS], double meanstep[NUMCOLS],double *StandDev);



int main(void) {
  //seed rand()
  srand(123456);

    // Load the map
    //deifne map as a 2D array
    char map[NUMROWS] [NUMCOLS];

    //open file, and check if file is not found
    FILE *fptr = NULL;
    fptr = fopen("island_map.txt", "r");

    if (fptr == NULL){

        printf("Error!"); 
        exit(EXIT_FAILURE);     //return exit status 1 and display error if file not found
    }


//READING MAP SECTION
//Define test variable
char Test; //extra variable to test for extra letters, just to give scanf a place to store it
int pos = 0;
char ch;
int row_read = 0;
int column_read = 0;

    // read the map and store it into the array, display error if incorrect letter is in the map

    while ((ch = getc(fptr)) != EOF){

          
          
          if (ch != 'B' && ch != 'W' && ch != 'L' && ch != 'D' && ch != 'V'){  //check for invalid symbols, or missing symbols, and display error if detected
            printf("Error!"); 
            fclose(fptr);
            exit(EXIT_FAILURE);        //close file and return exit status 1
          }

            if (ferror(fptr)) { //Check if Scanf returns an error when reading
            printf("Error!");
            fclose(fptr); 
            exit(EXIT_FAILURE);      //close file and return exit status 1 and display error
            }

            map[row_read][column_read] = ch;

            pos++;
            ch = getc(fptr);

            if (ch == ' '){
                column_read++;

                if (column_read > 8){
                    printf("Error!");
                    fclose(fptr); 
                    exit(15);
                }
                pos++;
            }  

            if (ch == '\n'){
                if (column_read < 8){
                    printf("Error!");
                    fclose(fptr); 
                    exit(17);
                }
                column_read = 0;
                row_read++;
                if (row_read > 8){
                    printf("Error!");
                    fclose(fptr); 
                    exit(20);
                }
                pos++;
            } 

            if (column_read == 8 && row_read == 8 && ch == EOF){
                break;

            }


            if  (ch != '\n' && ch != ' '){

                printf("Error!");
                fclose(fptr); 
                exit(25);
            }
                
        
        } //end of row

    

    fclose(fptr); //close file

    //print the map

    printf("Map:\n");

    for (int i = 0; i < NUMROWS; i++){
        for (int q = 0; q < NUMCOLS; q++){

         
          if(q == NUMCOLS - 1){  //special case for end of line so no Space
            printf("%c", map[i][q]);

          }else{

           printf("%c ", map[i][q]); //print every cell in a row            
          }
          
        }
        printf("\n");
    }


  // DATA PROCESSING SECTION



    // Perform random walks and calculate results

    int row_results[NUMROWS][NUMCOLS][NUMWALKS];  //create array to store all the results of success or failure for every run, for every cell
    int row_steps[NUMROWS][NUMCOLS][NUMWALKS];   //create array to store the number of steps for every run
    double chances[NUMROWS][NUMCOLS];    //array containing the chances of success for every cell
    double meanstep[NUMROWS][NUMCOLS];   //array containing mean path length for every cell
    double standDev[NUMROWS][NUMCOLS];    //array for standard deviation for ever cell 



//get data for each cell

for (int Row = 0; Row < NUMROWS; Row++){ //set row

    for (int col = 0; col < NUMCOLS; col++){ //set cell to run simulation

        simulate_cell(map, Row, col, row_results[Row][col], row_steps[Row][col]); //run the simulation 1000 times per cell


    }
}




    for (int Row = 0; Row < NUMROWS; Row++){ // Run the simulation Row by Row

    Find_suc(row_results[Row], chances[Row]);  // calculate the Chances of escape for each cell in a row

    Mean_path(row_results[Row], row_steps[Row], meanstep[Row]);   // calculate the mean path length  for each cell in a row
    Find_standDev(row_results[Row], row_steps[Row], meanstep[Row], standDev[Row]);    // calculate the standard deviation of path length for each cell in a row

    }



    // Print results


    printf("\nProbability of escape:\n");  //Print header for chances of escape
    for (int i =0; i < NUMROWS; i++){
        for (int q = 0; q < NUMCOLS; q++){

          printf("%6.2f ", chances[i][q]);  //print every cell in a row

        
 
        }
        printf("\n");  //enter to next line for column
    }
    



    printf("\nMean path length:\n");  //Header for mean path length
    for (int i =0; i < NUMROWS; i++){   
        for (int q = 0; q < NUMCOLS; q++){

          if(q == NUMCOLS - 1){      //special case for end of line so no Space
            printf("%5.2f", meanstep[i][q]);

          }else{

          printf("%5.2f ", meanstep[i][q]);   //print every cell in a row, 5.2 to allow for 2 DP, and 5 spaces for each cell, to ensure alignment even if a number is 10.00

          } 
        }
        printf("\n");   //enter to next line for column
    }

    printf("\nStandard deviation of path length:\n");  //Header for path standard devation
    for (int i =0; i < NUMROWS; i++){

        for (int q = 0; q < NUMCOLS; q++){

          if(q == NUMCOLS - 1){  //special case for end of line so no Space
            printf("%5.2f", standDev[i][q]);

          }else{

          printf("%5.2f ", standDev[i][q]);  //print every cell in a row            
          }
          
        }
        printf("\n");   //enter to next line for column
    }
    
    



    return 0;
}

void simulate_cell(char map[NUMCOLS][NUMROWS], int inrow, int incol, int *result, int *steps){



// 1000 repeats from starting position
        for (int q = 0; q < NUMWALKS; q++){
          int row = inrow;  //reset row for next walk
          int col = incol;  //reset column for next walk
          char position = map[row][col]; //set position

          int step = 0;

// 10 steps per repeat

          for (step; step < NUMSTEP; step++){

            //ensure default result is 0 (if nothing happens after 10 steps)
            *result = 0;

            //check position before motion, incase of death or success
            if (position == 'W' || position == 'D' || position == 'V'){
                *result = 2;
                break;
            }

            if (position == 'B'){
                *result = 1;
                break;
            }


          //run random movement
          int direction = rand() % 8;


          //make the movement

          if (direction == 0){   //N
            row -= 1;
          }

          if (direction == 1){  //NE
            row -= 1;
            col += 1;

          }

          if (direction == 2){  //E
            col += 1;
          }

          if (direction == 3){  //SE
            row += 1;
            col += 1;
          }

          if (direction == 4){  //S
            row += 1;
          }

          if (direction == 5){  //SW
            row += 1;
            col -= 1;
          }

          if (direction == 6){  //W
            col -= 1;
          }

          if (direction == 7){  //NW
            row -= 1;
            col -= 1;
          }

          if (col > 8){   //reset column if gone off map
            col = 8;
          } else if (col < 0)
          {
            col = 0;
          }
          

          if (row > 8){   //reset row if gone off map
            row = 8;
          } else if (row < 0)
          {
            row = 0;
          }


          //update location
          position = map[row][col];   
          } //end of step loop


          //check position incase if 10th step was a success or a fail, if L remain at default.
            if (position == 'W' || position == 'D' || position == 'V'){
                *result = 2;
            }
            if (position == 'B'){
                *result = 1;
            }

          // store number of steps and increment result and steps:
          *steps = step;
          steps++;
          result++;
        
        } //end of 1000 walks loop


    } //end of all columns loop and function





void Find_suc(int results[NUMCOLS][NUMWALKS], double *Chances){
//define variables
int successes = 0; //variable to store total successful steps

for(int col = 0; col < NUMCOLS; col++){  //calculate chance of success for ever column in a given row

  successes = 0; //reset success


  for (int i = 0; i < NUMWALKS; i++){ //Check if success

    if (results[col][i] == 1){
      successes += 1;   //sum total number of successful steps
      }
    }

    *Chances = ( (double) successes/NUMWALKS) * 100; //find % chance
    Chances++;
}


}



void Mean_path(int results[NUMCOLS][NUMWALKS], int steps[NUMCOLS][NUMWALKS], double *meanpath){

  for(int col = 0; col < NUMCOLS; col++){  //calculate total steps for ever column in a given row

  int step = 0; //reset steps
  int total = 0;  //reset total successful walks


    for (int i = 0; i < NUMWALKS; i++){ //check each cell 

      if (results[col][i] == 1){    //check if walk is successful
      step = step + steps[col][i];    //add all steps
      total++;
      }
  }
    if (total == 0){  //for cells without successful walks, mean steps = 0
      *meanpath = 0.00;
      meanpath++;

    }else{
    *meanpath = ( (double) step/ (double) total); //divide by number of successful walks to find average steps
    meanpath++;

    }

}

}



void Find_standDev(int results[NUMCOLS][NUMWALKS], int steps[NUMCOLS][NUMWALKS], double meanstep[NUMCOLS],double *StandDev){

  for (int i = 0; i < NUMCOLS; i++){  //repeat for each comlumn in given row

    double sum = 0; //create variable for part of the equation with sums
    int total = 0;  //variable to store successful steps

    //calculate sum of (x-u) ^2 for each value of steps
    for (int walk = 0; walk < NUMWALKS; walk++){
      if (results[i][walk] == 1){    //check if walk is successful
        sum += pow(steps[i][walk] - meanstep[i], 2); //add square of difference of each term and the mean
        total++;  //find total successful steps
      }
    }

    if (total == 0){    //for cells without successful walks StandDev = 0
      *StandDev = 0.00;
      StandDev++;

    }else{

      *StandDev = pow(sum/ (double) total, 0.5);  //divide sum by total steps and radical answer
      StandDev++;   //increment pointer
    }
  }

}