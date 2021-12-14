#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef PLANTEVANDERP1_CLIMATENTEMPERATURE_H
#define PLANTEVANDERP1_CLIMATENTEMPERATURE_H

float temperatureSimulator(int month); 
void climateSystem(curPlant *degrees, float ideal);

//returns random float for temperature based on average temperatures in burkina faso during specific months of the year
float temperatureSimulator(int month) { // Returns semi-random float, based on the time of the day and the year.
    
    /*
    struct tm* curtime;
    time_t s;
    s = time(NULL);
    curtime = localtime(&s);
    */
    
    //note:
    //jeg kan godt lide ideen om at bruge computeren til at checke tiden, men det tager for lang tid at scrolle gennem måneder, hvis vi kigger på real time

    float lower, upper; // Boundary variables.

    // Temperature boundaries pr. month for Burkina Faso
    switch (month) { // months go from 0 to 11
        case 0: // January
            upper = 30;
            lower = 17;
            break;
        case 1: // February
            upper = 34;
            lower = 19.5;
            break;
        case 2: // March
            upper = 37;
            lower = 24;
            break;
        case 3: // April
            upper = 39.8;
            lower = 27.9;
            break;
        case 4: // May
            upper = 40;
            lower = 30;
            break;
        case 5: // June
            upper = 38.1;
            lower = 26;
            break;
        case 6: // July
            upper = 36;
            lower = 23.8;
            break;
        case 7: // August
            upper = 34.5;
            lower = 23;
            break;
        case 8: // September
            upper = 35.2;
            lower = 24.5;
            break;
        case 9: // October
            upper = 35;
            lower = 26.9;
            break;
        case 10: // November
            upper = 33;
            lower = 22.2;
            break;
        case 11: // December
            upper = 30.3;
            lower = 18.5;
            break;
    }
    
    // Selects a random number within boundaries: lower & upper.
    float rnum = (((float)rand() / RAND_MAX) * (upper - lower)) + lower; 
    return rnum;
}


void climateSystem(curPlant *degrees, float ideal) {
    if (degrees->airTemp > ideal+1) {
        printf("Climate system cooling..\n");
        degrees->airTemp -= 0.1;
    }
    else if (degrees->airTemp < ideal-1) {
        printf("Climate system heating..\n");
        degrees->airTemp += 0.1;
    } else {
        printf("Climate system off..\n");
    }
}


#endif //PLANTEVANDERP1_CLIMATENTEMPERATURE_H