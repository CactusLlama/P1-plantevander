#include <stdio.h>
#include "string.h"
#include "math.h"
#include "Plants.h"
#include "SensorData.h"


void Set_up(curPlant *current_plant);
void print_plant(curPlant current);
void PH_regulator(Data sensor_current, curPlant current);
void EC_regulator(Data *sensor_current, curPlant current);





int main() {
    Data sensor_current;
    sensor_current.ph=4.0;
    sensor_current.air_temp=0;
    sensor_current.water_temp=0;
    sensor_current.ec=0;
    sensor_current.humidity=0;
    sensor_current.water_level=1000.0;

    //Creates a struct of sensor data
    int num_plants=1;
    //printf("Type in the amount of plants that u have: ");
    //scanf("%d", &num_plants);

    curPlant current_plant[10];
    current_plant[0].ph_min=6.0;
    current_plant[0].ph_max=7.5;
    current_plant[0].ec_min=0;
    current_plant[0].ec_max=1;
    current_plant[0].water_temp_min=5;
    current_plant[0].water_temp_max=5;
    current_plant[0].air_temp_min=10;
    current_plant[0].air_temp_max=20;



   /* for (int i = 1; i < num_plants; ++i) {
       Set_up(&current_plant[i]);
    }*/


    for (int i = 0; i < num_plants; ++i) {
        print_plant(current_plant[i]);
    }

   PH_regulator(sensor_current, current_plant[0]);

   EC_regulator(&sensor_current, current_plant[0]);


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

void PH_regulator(Data sensor_current, curPlant current) {


    double ph_goal = (current.ph_max+current.ph_min)/2;
    double poh_goal;

    double dif_acid;
    double add_acid;

    double add_base;

    double x=10;

    if(current.ph_min < sensor_current.ph && sensor_current.ph < current.ph_max) {
        return;
    } else if(sensor_current.ph > current.ph_max) {

       dif_acid=pow(x, -ph_goal) - pow(x, -sensor_current.ph);

       add_acid = dif_acid*sensor_current.water_level;
       add_acid = add_acid*1000;
        printf("Add %.4lf mL of HCL (1 mol/liter)\n", add_acid);


    }else if(sensor_current.ph < current.ph_min) {

        poh_goal = 14-ph_goal;
        double amount_goal = pow(x, -poh_goal);
        //printf("%.10lf\n", amount_goal);

        double poh_current = 14-sensor_current.ph;
        double amount_current = pow(x, -poh_current);
        //printf("%.10lf\n", amount_current);

        add_base =(amount_goal-amount_current)*sensor_current.water_level;
        add_base = add_base*1000;
        printf("Add %.4lf mL of NaOH (1 mol/liter)", add_base);
    }

}

void EC_regulator(Data *sensor_current, curPlant current) {

    double ec_avr=(current.ec_max+current.ec_min)/2;

    if(current.ec_min < sensor_current->ec && sensor_current->ec < current.ec_max) {
        return;
    } else if(sensor_current->ec < current.ec_min) {
        printf("Add nutrients");
        sensor_current->ec = ec_avr;
    } else if(sensor_current->ec > current.ec_max) {
        printf("Add water");
        sensor_current->ec = ec_avr;
    }

}


