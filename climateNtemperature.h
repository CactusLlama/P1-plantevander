//
// Created by johan on 09/12/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // contains sleep()
#include <time.h>
#include "SensorData.h"
#ifndef PLANTEVANDERP1_CLIMATENTEMPERATURE_H
#define PLANTEVANDERP1_CLIMATENTEMPERATURE_H

float temperatureSimulator(void); // Simulates the outdoor temperature of Burkina Faso
void climateSystem(struct SensorData *degrees, float ideal);


float temperatureSimulator(void) { // Returns semi-random float, based on the time of the day and the year.

    struct tm* curtime;
    time_t s;
    s = time(NULL);
    curtime = localtime(&s);


    float lower, upper; // Boundary variables.

    // Temperature boundaries pr. month for Burkina Faso
    switch (curtime->tm_mon) { // months go from 0 to 11
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

    float rnum = (((float)rand() / RAND_MAX) * (upper - lower)) + lower; // Selects a random number within boundaries: lower & upper.
    return rnum;

   // if (curtime->tm_hour <= 6 && rnum > normal morning temperature) {
   //     return rnum % /*A maximum morning temperature*/;
  //  }
  //  else if (curtime->tm_hour <= 18 && rnum > normal evening temperature) {
  //      return rnum % /*A maximum evening temperature*/;
  //  } else {
   //     return rnum;
  //  }

}


void climateSystem(struct SensorData *degrees, float ideal) {
    if (degrees->air_temp > ideal) {
        printf("Cooling system: ON");
        degrees->air_temp -= 0.01;
    }
    else if (degrees->air_temp < ideal) {
        printf("Heating system: ON");
        degrees->air_temp += 0.01;
    } else {
        printf("Climate system: OFF");
    }
}

#endif //PLANTEVANDERP1_CLIMATENTEMPERATURE_H
