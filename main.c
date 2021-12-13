#include <stdio.h>
#include "string.h"
#include "math.h"
#include "Plants.h"
#include "SensorData.h"


void Set_up(curPlant *current_plant);
void print_plant(curPlant current);
void PH_regulator(Data *sensor_current, curPlant *current);





int main() {
    //Creates a struct of sensor data
    int num_plants=0;
    printf("Type in the amount of plants that u have: ");
    scanf("%d", &num_plants);

    curPlant current_plant[num_plants];
    //Data sensor_current;


    for (int i = 0; i < num_plants; ++i) {
       Set_up(&current_plant[i]);
    }


    for (int i = 0; i < num_plants; ++i) {
        print_plant(current_plant[i]);
    }

   // PH_regulator(&sensor_current, &current_plant);


}

void Set_up(curPlant *current_plant) {

    printf("type in name of plant: ");
    scanf("%s", current_plant->name);

    printf("type in max pH values for the plant: ");
    scanf(" %lf", &current_plant->ph_max);
    printf("type in min pH values for the plant: ");
    scanf(" %lf", &current_plant->ph_min);

    printf("type in max EC values for the plant: ");
    scanf(" %lf", &current_plant->ec_max);
    printf("type in min EC values for the plant: ");
    scanf(" %lf", &current_plant->ec_min);

    printf("type in max air temperature in celsius for the plant: ");
    scanf(" %lf, %lf", &current_plant->air_temp_max);
    printf("type in min air temperature in celsius for the plant: ");
    scanf(" %lf", &current_plant->air_temp_min);

    printf("type in max water temperature in celsius: ");
    scanf(" %lf", &current_plant->water_temp_max);
    printf("type in min water temperature in celsius: ");
    scanf(" %lf", &current_plant->water_temp_min);

}

void print_plant(curPlant current) {
    printf("plant name: %s \n", current.name);
    printf("plant max pH level: %.2lf. plant min pH level: %.2lf \n", current.ph_max, current.ph_min);
    printf("Plant max EC level: %.2lf. Plant min EC level: %.2lf\n", current.ec_max, current.ec_min);
    printf("Plant max air temp: %.2lf celsius. Plant min air temp: %.2lf celsius.\n", current.air_temp_max, current.air_temp_min);
    printf("Plant max water temp: %.2lf celsius. Plant min water temp: %.2lf celsius.\n", current.water_temp_max, current.water_temp_min);

}

void PH_regulator(Data *sensor_current, curPlant *current) {
    float water level=100;

    double acid_nutrients=4;
    double neutral_nutrients=7;
    double basic_nutrients=10;
    if(&current->ph_min < &sensor_current->ph && &sensor_current->ph < &current->ph_max) {
        return;
    } else if(&sensor_current->ph > &current->ph_max) {


    }else if(&sensor_current->ph < &current->ph_min) {

    }

}


