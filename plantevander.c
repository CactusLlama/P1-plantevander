#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "plantData.h"
#include "declareFunctions.h"

//time constant for tick in main loop
#define TIME_CONSTANT_MILSEC 2000


int main() {
    char kbInput;
    clock_t curTime = 0;
    int state, ctr = 0;
    int plantAmount = 1, selectedProfile = 0;
    struct Plants plants[10];
    struct PlantSensors curPlant;
    
    
    //read data from .txt into struct for easier access during runtime
    read_data(&curPlant);     
    
    init_structs(plants);
    
    edit_mode(plants, &plantAmount);
    
    //endless loop
    for(;;) {
        system("cls");
         
        if (ctr % 9 == 0) {
            printf("Sprinklers turned on..\n%d\n", ctr);
            state = 1;
        } else {
            printf("Sprinklers turned off..\n%d\n", ctr);
            state = -1;
        }
        
        printf("Current plant: %s\nSelected profile: %s\n", curPlant.name, plants[selectedProfile].name);
        printf("Current sensor data:\n pH value: %.2f\n EC value: %.2f\n Current water level in tank: %.1f L\n Current air temperature: %.1f deg C\n Current water temperature: %.1f deg C\n Current humidity level: %.2f%%\n", curPlant.ph, curPlant.ec, curPlant.waterLevel, curPlant.airTemp, curPlant.waterTemp, curPlant.humidity);
        printf("\nEnter the number of which action you want to do.\n 1. Enter edit mode\n 2. Enter debugging mode\n 3. Exit program\n");
        
        //if user presses their keyboard, get the char and process which option the user chose
        if(kbhit()) {
            kbInput = getch();
            process_input(kbInput, plants, &plantAmount, &curPlant);
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

void process_input(char a, struct Plants plants[], int *plantAmount, struct PlantSensors *curPlant) {
    //clear terminal
    system("cls");

    //switch statement to see which option the user chose
    switch (a) {
        case '1':
        edit_mode(plants, plantAmount);
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

void read_data(struct PlantSensors *current) {
    char tempStr[100] = {"placeholder"};
    char prevStr[100] = {"asdf"};
    float tempNum = 12.0;
    FILE *file = fopen("plantData.txt", "r+");
    
    //keep scanning and editing struct var values until input is End Of File
    while (strcmp(prevStr, tempStr) != 0) {
        strcpy(prevStr, tempStr);
        fscanf(file, "%s %f\n", &tempStr, &tempNum);
        //number after the name in pplantData.txt is set to -1
        if (tempNum < 0) {
            strcpy(current->name, tempStr);
        }
        if (strcmp(tempStr, "ph") == 0) {
            current->ph = tempNum;
        } else if (strcmp(tempStr, "ec") == 0) {
            current->ec = tempNum;
        } else if (strcmp(tempStr, "waterLevel") == 0) {
            current->waterLevel = tempNum;
        } else if (strcmp(tempStr, "airTemp") == 0) {
            current->airTemp = tempNum;
        } else if (strcmp(tempStr, "waterTemp") == 0) {
            current->waterTemp = tempNum;
        } else if (strcmp(tempStr, "humidity") == 0) {
            current->humidity = tempNum;
        }
    }
    return;
}

//do the same as readData() but write instead of read
void overwrite_data(struct PlantSensors *current) {
    FILE *file = fopen("plantData.txt", "w+");
    int ctr = 0;
    //string array for printing in .txt
    char order[6][100] = {
                            "ph",
                            "ec",
                            "waterLevel",
                            "airTemp",
                            "waterTemp",
                            "humidity"
                          };
    
    //prints this data to .txt file from struct
    fprintf(file, "%s -1\n", current->name);
    fprintf(file, "%s %f\n", order[ctr++], current->ph);
    fprintf(file, "%s %f\n", order[ctr++], current->ec);
    fprintf(file, "%s %f\n", order[ctr++], current->waterLevel);
    fprintf(file, "%s %f\n", order[ctr++], current->airTemp);
    fprintf(file, "%s %f\n", order[ctr++], current->waterTemp);
    fprintf(file, "%s %f\n", order[ctr++], current->humidity);

    return;
}

void edit_data(struct PlantSensors *current, int state) {
    //initialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    //if/else to determine whether to add or subtract a random number. the random number is between (random statement thing) * (max - min) + (min)
    //this means that mg gets added a random number between 0.5 and 0.15. it gets subtracted between 0.1 and 0.01
    //needs to be updated when nutrient solution gets implemented
    //also with more nutrients
    //ph and ec should be handled another way, since they depend on other factors
    /*
    if (state == 1) {
        current->mg += ((double)rand() / RAND_MAX) * 0.35 + 0.15;
    } else {
        current->mg -= ((double)rand() / RAND_MAX) * 0.09 + 0.01;
    }
*/

    return;
}

void edit_mode(struct Plants plants[], int *plantAmount) {
    char input;
    int isLoop = 1;
    int num;
    
    while(isLoop) {
        system("cls");
        list_plants(plants, *plantAmount);
        printf("\nType the number of what action you want to take:\n");
        printf(" 1. View and/or edit a plant's limit values\n 2. Add another plant to the list\n 3. Enter simulation\n");
        input = getch();
        
        switch (input) {
            case '1':
            printf("Type the number of the plant you wish to view/edit:\n");
            input = getch();
            num = input - '0' - 1;
            if (*plantAmount > num | 0 >= plantAmount) {
                printf("Invalid input!");
                break;
            }
            print_plant(plants[num]);
            printf("\nDo you want to edit the values of this plant? [y/n]\n");
            input = getch();
            if (input == 'y') {
                edit_plant(&plants[num]);
                printf("Plant edited!");
                delay(1500);
            }
            break;
            
            case '2':
            edit_plant(&plants[*plantAmount]);
            *plantAmount += 1;
            printf("Plant added!");
            delay(1500);
            break;
            
            case '3':
            isLoop = 0;
            break;
            
            default:
            printf("\nInvalid input!");
            break;
        }
    }
}

//print the ranges of selected plant
void print_plant(struct Plants current) {
    printf("\nName of this plant: %s\n", current.name);
    printf("pH values for this plant range from %.2f-%.2f\n", current.phMin, current.phMax);
    printf("EC levels for this plant range from %.2f-%.2f\n", current.ecMin, current.ecMax);
    printf("Air temperatures for this plant range from %.1f-%1.f deg C\n", current.airTempMin, current.airTempMax);
    printf("Water temperatures for this plant range from %.2f-%.2f deg C\n", current.waterTempMin, current.waterTempMax);
}

//print the names of the current plants stored in array of structs in a list
void list_plants(struct Plants plants[], int plantAmount) {
    system("cls");
    
    printf("Current plants in system:\n");
    for (int i = 0; i < plantAmount; i++) {
        printf(" %d. %s\n", i + 1, plants[i].name);
    }
}

//edit the ranges of selected plant
void edit_plant(struct Plants *current) {  
    float a, b;
    
    system("cls");
    
    printf("Enter the name of the plant: ");
    scanf(" %s", current->name);
    
    ph:
    printf("\nEnter the upper limit of the pH value for this plant: ");
    scanf(" %f", &a);
    printf("\nEnter the lower limit of the pH value for this plant: ");
    scanf(" %f", &b);
    
    //check if phMax and phMin are within 0 and 14 and and if phMax is the biggest of the two before assigning
    if (a > b && (14 - a) >= 0 && (b) >= 0) {
        current->phMax = a;
        current->phMin = b;
    } else {
        printf("\nInvalid input! Upper limit must be greater than lower limit and they must be between 0 and 14 both inclusive!");
        delay(1500);
        system("cls");
        goto ph;
    }
    
    ec:
    printf("\nEnter the upper limit of the EC value for this plant: ");
    scanf(" %f", &a);
    printf("\nEnter the lower limit of the EC value for this plant: ");
    scanf(" %f", &b);
    
    if (cmp_nums(a, b)) {
        current->ecMax = a;
        current->ecMin = b;
    } else goto ec;
    
    airtemp:
    printf("\nEnter the upper limit of the air temperature for this plant: ");
    scanf(" %f", &a);
    printf("\nEnter the lower limit of the air temperature for this plant: ");
    scanf(" %f", &b);
    
    if (cmp_nums(a, b)) {
        current->airTempMax = a;
        current->airTempMin = b;
    } else goto airtemp;
    
    watertemp:
    printf("\nEnter the upper limit of the water temperature for this plant: ");
    scanf(" %f", &a);
    printf("\nEnter the lower limit of the water temperature for this plant: ");
    scanf(" %f", &b);
    
    if (cmp_nums(a, b)) {
        current->waterTempMax = a;
        current->waterTempMin = b;
    } else goto watertemp;
}

//compare floats. ph does not call this since it has to be within a defined range and this only finds the biggest number of two floats
int cmp_nums(float a, float b) {
    if (a > b) {
        return 1;
    } else {
        printf("\nInvalid input! Upper limit must be greater than lower limit!");
        delay(1500);
        system("cls");
    }
}

//predefine the first plant in the array of structs
void init_structs(struct Plants plants[]) {
    
    strcpy(plants[0].name, "Kartoffel");
    plants[0].airTempMax = 30.0;
    plants[0].airTempMin = 5.0;
    plants[0].waterTempMax = 25.0;
    plants[0].waterTempMin = 12.5;
    plants[0].phMax = 8.5;
    plants[0].phMin = 7.3;
    plants[0].ecMax = 12.5;
    plants[0].ecMin = 10;
    
}