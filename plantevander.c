#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "plantData.h"
#include "SensorData.h"
#include "climateNtemperature.h"

//time constant for tick in main loop
#define TIME_CONSTANT_MILSEC 2000

//Declaring functions ahead of use
void process_input(char a);
void delay(int mSeconds);
void invalid_input();
void read_data(struct Plants *current);
void overwrite_data(struct Plants *current, int state);


int main() {
    char kbInput;
    clock_t curTime = 0;
    int state = 1, ctr = 0;
    srand(time(NULL)); // Provides new seed for rand(), every second, so that it never returns the same sequence of numbers.
    Plant curPlant;
    Data sensor_current;
    read_data(&curPlant);     

    //endless loop
    for(;;) {

        //print cls in terminal to clear
        system("cls");

        // Measure the outdoor temperature ca. every 5 minutes.
        if (ctr % 150 == 0) { // (150 * (one iteration >= 2sek))/60 = 5
            sensor_current.air_temp = temperatureSimulator();
        }
        printf("The outdoor temperature is now: %.2f degrees C\n\n", sensor_current.air_temp);

        // Regulates temperature by approaching the ideal temperature.
        climateSystem(&sensor_current, 20);


        if (ctr % 9 == 0) {
            printf("Sprinklers turned on..\n\n");
            sensor_current.water_level -= 3000; // How much water is removed from the tank, excluding the amount not used by plants, if measured in mL
        } else {
            printf("Sprinklers turned off..\n\n");
        }
        
        //overwrite_data(fp, &curPlant, state);
        printf("Current species: %s\n", curPlant.name);
        printf("\tCurrent magnesium levels: %lf\n\tCurrent sodium levels: %lf\n\tCurrent pH-level: %.1lf\n", curPlant.mg, curPlant.sodium, curPlant.ph);
        printf("\nEnter the number of which action you want to do: \n\t 1. Add magnesium\n\t 2. Add sodium\n\t 3. Exit program\n");
     
        //if user presses their keyboard, get the char and process which option the user chose
        if(kbhit()) {
            kbInput = getch();
            process_input(kbInput);
        }
       
       //check if current time is more than last 'cycle' (last time this struck true) + the defined time constant
        if (clock() > curTime + TIME_CONSTANT_MILSEC) {
            printf("checked");
            //update curTime to be equal to the current clocktime
            curTime = clock();
            //used in overwrite_data to define whether or not the function should add or subtract from current amount of nutrients
            state *= -1;
            //used to turn on or off the sprinklers
            ctr++;
        }
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
    delay(1500);
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
        printf("%s %s\n", tempStr, prevStr);
        delay(1000);
    }
    return;
}

//do the same as readData() but write instead of read
void overwrite_data(struct Plants *current, int state) {

}
