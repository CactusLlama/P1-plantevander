#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "plantData.h"

//time constant for tick in main loop
#define TIME_CONSTANT_MILSEC 2000

//Declaring functions ahead of use
void process_input(char a);
void delay(int mSeconds);
void invalid_input();
void read_data(struct Plants *current);
void overwrite_data(struct Plants *current);
void edit_data(struct Plants *current, int state);

int main() {
    char kbInput;
    clock_t curTime = 0;
    int state, ctr = 0;
    struct Plants curPlant;
    
    //read data from .txt into struct for easier access during runtime
    read_data(&curPlant);     

    //endless loop
    for(;;) {
        printf("\033[%d;%dH", 1, 1);

        if (ctr % 9 == 0) {
            printf("Sprinklers turned on..\n%d\n", ctr);
            state = 1;
        } else {
            printf("Sprinklers turned off..\n%d\n", ctr);
            state = -1;
        }
        
        printf("Current plant: %s\n", curPlant.name);;
        printf("    Current magnesium levels: %lf\n    Current sodium levels: %lf\n    Current pH-level: %.1lf\n", curPlant.mg, curPlant.sodium, curPlant.ph);
        printf("\nEnter the number of which action you want to do.\n 1. Add magnesium\n 2. Add sodium\n 3. Exit program\n");
     
        //if user presses their keyboard, get the char and process which option the user chose
        if(kbhit()) {
            kbInput = getch();
            process_input(kbInput);
        }
        
        edit_data(&curPlant, state);
       
        //check if current time is more than last 'cycle' (last time this struck true) + the defined time constant
        if (clock() > curTime + TIME_CONSTANT_MILSEC) {
            
            //update curTime to be equal to the current clocktime
            curTime = clock();
            
            //used to turn on or off the sprinklers
            ctr++;
        }
        overwrite_data(&curPlant);
        delay(200);
    }

    return 0;
}

void process_input(char a) {
    //clear terminal
    system("cls");

    //switch statement to see which option the user chose
    switch (a) {
        case '1':
        printf("magnesium added");
        delay(500);
        break;

        case '2':

        break;
        case '3':
        exit(0);

        break;

        default:
        invalid_input();
    }
    return;
}

void delay(int mSeconds) {
    //store start time in variable
    clock_t startTime = clock();
    
    //loop until clock is caught up with variable + specified delay
    while (clock() < startTime + mSeconds);
    //the program exits once while loop is caught up, having delayed a specified amount of seconds
    
    return;
}

void invalid_input() {
    //clear terminal
    system("cls");
    printf("Invalid input!");
    delay(1000);
    return;
}

void read_data(struct Plants *current) {
    char tempStr[100] = {"placeholder"};
    char prevStr[100] = {"asdf"};
    double tempNum = 12.0;
    FILE *file = fopen("plantData.txt", "r+");
    
    //keep scanning and editing struct var values until input is End Of File
    while (strcmp(prevStr, tempStr) != 0) {
        strcpy(prevStr, tempStr);
        fscanf(file, "%s %lf\n", &tempStr, &tempNum);
        //number after the name in pplantData.txt is set to -1
        if (tempNum < 0) {
            strcpy(current->name, tempStr);
        }
        if (strcmp(tempStr, "magnesium") == 0) {
            current->mg = tempNum;
        } else if (strcmp(tempStr, "sodium") == 0) {
            current->sodium = tempNum;
        } else if (strcmp(tempStr, "ph") == 0) {
            current->ph = tempNum;
        }
    }
    return;
}

//do the same as readData() but write instead of read
void overwrite_data(struct Plants *current) {
    FILE *file = fopen("plantData.txt", "w+");
    int ctr = 0;
    //string array for printing in .txt
    char order[12][100] = {
                            "magnesium",
                            "sodium",
                            "ph"
                          };
    
    //behold the wall of fprintf();
    //there might be a better solution, tho idk how. pointers to the struct didnt work
    //needs to be updated along with plantData.h and plantData.txt when more nutriens get added
    fprintf(file, "%s -1\n", current->name);
    fprintf(file, "%s %lf\n", order[ctr++], current->mg);
    fprintf(file, "%s %lf\n", order[ctr++], current->sodium);
    fprintf(file, "%s %lf\n", order[ctr++], current->ph);

    return;
}

void edit_data(struct Plants *current, int state) {
    //initialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    //if/else to determine whether to add or subtract a random number. the random number is between (random statement thing) * (max - min) + (min)
    //this means that mg gets added a random number between 0.5 and 0.15. it gets subtracted between 0.1 and 0.01
    //needs to be updated when nutrient solution gets implemented
    //also with more nutrients
    //ph and ec should be handled another way, since they depend on other factors
    if (state == 1) {
        current->mg += ((double)rand() / RAND_MAX) * 0.35 + 0.15;
    } else {
        current->mg -= ((double)rand() / RAND_MAX) * 0.09 + 0.01;
    }


    return;
}
