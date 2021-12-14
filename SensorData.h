//
// Created by marct on 08/12/2021.
//
#ifndef PLANTEVANDERP1_SENSORDATA_H
#define PLANTEVANDERP1_SENSORDATA_H

typedef struct SensorData{
    float ph;
    float ec;
    int water_level;
    float air_temp;
    float water_temp;
    short int humidity;
} Data;

#endif //PLANTEVANDERP1_SENSORDATA_H