//
// Created by marct on 08/12/2021.
//
#ifndef PLANTEVANDERP1_PLANTS_H
#define PLANTEVANDERP1_PLANTS_H

typedef struct Plants {
    char name[20];
    double air_temp_max;
    double air_temp_min;
    double water_temp_max;
    double water_temp_min;
    double ec_max;
    double ec_min;
    double ph_max;
    double ph_min;

} curPlant;

#endif //PLANTEVANDERP1_PLANTS_H
