#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include "plantData.h"
#include "declareFunctions.h"
#include "climate.h"

//time constant for tick in main loop
#define TIME_CONSTANT_MILSEC 2000


int main() {
    char kbInput;
    clock_t curTime = 0;
    int sprinklerState, fanState, waterHeatState;
    int ctr = 110;
    int month = 0, swapMonth = 1;
    int plantAmount = 1, selectedProfile = 0;
    struct Plants plants[10];
    struct PlantSensors curPlant;
    
    
    //read data from .txt into struct for easier access during runtime
    read_data(&curPlant);     
    
    init_structs(plants);
    
    edit_mode(plants, &plantAmount, &selectedProfile);
    
    //endless loop
    for(;;) {
        system("cls");
        
        printf("Current plant: %s\nSelected profile: %s\n", curPlant.name, plants[selectedProfile].name);
        
        printf("\nCurrent sensor data:\n pH value: %.2f\n EC value: %.2f\n Current water level in tank: %.1f L\n Current air temperature: %.1f deg C\n Current water temperature: %.1f deg C\n Current humidity level: %.2f%%\n", curPlant.ph, curPlant.ec, curPlant.waterLevel, curPlant.airTemp, curPlant.waterTemp, curPlant.humidity);

        month = (month + 1 * (ctr % 120 == 0)) % 12;
        
        //if its time to swap month, do
        //swapMonth makes sure this only happens once
        if (ctr % 120 == 0 && swapMonth == 1) {
            curPlant.airTemp = temperatureSimulator(month);
            swapMonth *= -1;
        }
        
        if (ctr % 9 == 0 && curPlant.waterLevel > 0) {
            printf("\nSprinklers on..\n", ctr);
            sprinklerState = 1;
        } else {
            printf("\nSprinklers off..\n", ctr);
            sprinklerState = 0;
        }
        
        //turn fans on or off depending on whether or not 
        if (plants[selectedProfile].humidMax < curPlant.humidity) {
            printf("Fans on..\n");
            fanState = 1;
        } else {
            printf("Fans off..\n");
            fanState = 0;
        }
        
        if (plants[selectedProfile].waterTempMax < curPlant.waterTemp) {
            printf("Water cooling on..\n");
            waterHeatState = -1;
        } else if (plants[selectedProfile].waterTempMin > curPlant.waterTemp) {
            printf("Water heating on..\n");
            waterHeatState = 1;
        } else {
            printf("Water temperature control off..\n"),
            waterHeatState = 0;
        }
        
        //edit temperatures. latter argument finds the avg of the range given in plant struct, which should be "ideal"
        climateSystem(&curPlant, (plants[selectedProfile].airTempMax + plants[selectedProfile].airTempMin) / 2); 
        
        
        ph_regulation(&curPlant, plants[selectedProfile]);
        ec_regulation(&curPlant, plants[selectedProfile]);
        edit_data(&curPlant, sprinklerState, fanState, waterHeatState);
        
        printf("\nEnter the number of which action you want to do.\n 1. Enter edit mode\n 2. Enter debugging mode\n 3. Exit program\n");
        
        //if user presses their keyboard, get the char and process which option the user chose
        if(kbhit()) {
            kbInput = getch();
            process_input(kbInput, plants, &plantAmount, &curPlant, &selectedProfile);
        }
       
        //check if current time is more than last 'cycle' (last time this struck true) + the defined time constant
        if (clock() > curTime + TIME_CONSTANT_MILSEC) {
            
            //update curTime to be equal to the current clocktime
            curTime = clock();
            
            //used to turn on or off the sprinklers
            ctr++;
            
            swapMonth *= -1;
        }
        overwrite_data(&curPlant);
        delay(200);
    }

    return 0;
}

void process_input(char a, struct Plants plants[], int *plantAmount, struct PlantSensors *curPlant, int *selection) {
    //clear terminal
    system("cls");

    //switch statement to see which option the user chose
    switch (a) {
        case '1':
        edit_mode(plants, plantAmount, selection);
        break;
        
        case '2':
        debug_mode(curPlant);
        delay(100);
        break;
        
        case '3':
        exit(0);
        break;
        
        default:
        invalid_input();
        break;
    }
}

void delay(int mSeconds) {
    //store start time in variable
    clock_t startTime = clock();
    
    //loop until clock is caught up with variable + specified delay
    while (clock() < startTime + mSeconds);
    //the program exits once while loop is caught up, having delayed a specified amount of seconds
}

void invalid_input() {
    printf("\n\nInvalid input!");
    delay(1500);
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
        if (tempNum == 99999) {
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
    fprintf(file, "%s 99999\n", current->name);
    fprintf(file, "%s %f\n", order[ctr++], current->ph);
    fprintf(file, "%s %f\n", order[ctr++], current->ec);
    fprintf(file, "%s %f\n", order[ctr++], current->waterLevel);
    fprintf(file, "%s %f\n", order[ctr++], current->airTemp);
    fprintf(file, "%s %f\n", order[ctr++], current->waterTemp);
    fprintf(file, "%s %f\n", order[ctr++], current->humidity);
}

void edit_data(struct PlantSensors *current, int sprinklerState, int fanState, int waterHeatState) {
    //initialize random number generator
    time_t t;
    srand((unsigned) time(&t));
    
    //water the plants (subtract amount from waterLevel)
    current->waterLevel -= (((float)rand() / RAND_MAX) * 0.5 + 0.5) * sprinklerState;
    
    //remove humidity if fans are on
    current->humidity -= (((float)rand() / RAND_MAX) * 0.17 + 0.03) * fanState;
    
    //fluctuate in humidity based on whether or not sprinklers are on
    current->humidity += (((float)rand() / RAND_MAX) * 50/current->humidity) * sprinklerState - 0.005;
    
    //give a random but small fluctuation in temperature
    current->airTemp += ((float)rand() / RAND_MAX) * 0.155 - 0.05;
    current->airTemp -= ((float)rand() / RAND_MAX) * 0.125 - 0.025;
    
    //fluctuate water temperature based on whether or not air temperature is hotter
    current->waterTemp += (((float)rand() / RAND_MAX) * 0.05 + 0.025) * (current->waterTemp < current->airTemp);
    current->waterTemp -= (((float)rand() / RAND_MAX) * 0.05 + 0.025) * (current->waterTemp > current->airTemp);
    
    //check if water heating/cooling is on and edit valule of water temperature
    current->waterTemp += (((float)rand() / RAND_MAX) * 0.05 + 0.05) * waterHeatState;

    //if waterlevel is 20 or lower, make these changes. new ph is set to a random number between the current ph and 7
    if (current->waterLevel <= 20) {
        current->ph = (((float)rand() / RAND_MAX) * (7 - current->ph) + current->ph) * (current->waterLevel <= 20) * (current->ph < 7);
        current->ph = (((float)rand() / RAND_MAX) * 7 + current->ph) * (current->waterLevel <= 20) * (current->ph > 7);
        current->ec -= (((float)rand() / RAND_MAX) * (current->ec + 3)  + 3) * (current->waterLevel <= 20);
        current->waterLevel = 900;
    }
}

void ph_regulation(struct PlantSensors *current, struct Plants ideal) {
    //guard clause; if ph is within range, exit function
    if (current->ph > ideal.phMin && current->ph < ideal.phMax) {
        printf("pH regulation: pH within range..\n");
        return;
    }
    
    float phGoal = (ideal.phMin + ideal.phMax) / 2;
    float diffAcid;
    float diffBase;
    float x = 10;
    char str[20];
    
    if (current->ph > ideal.phMax) {
        diffAcid = pow(x, -phGoal) - pow(x, -current->ph);
        printf("pH regulation: solution too basic! Adding HCl (1 mol/L). %.4f mL remaining..\n", diffAcid * current->waterLevel * 1000);
        current->ph -= 0.2567;
        current->waterLevel += 0.3875;
        return;
    }

    diffBase = pow(x, -(14 - phGoal)) - pow(x, -(14 - current->ph));
    printf("pH regulation: solution too acidic! Adding NaOH (1 mol/L). %.4f mL remaining..\n", diffBase * current->waterLevel * 1000);
    current->ph += 0.2567;
    current->waterLevel += 0.3875;
    return;
}

void ec_regulation(struct PlantSensors *current, struct Plants ideal) {
    if (current->ec > ideal.ecMin && current->ec < ideal.ecMax) {
        printf("EC regulation: EC within range..\n");
        return;
    }
    
    if (current->ec > ideal.ecMax) {
        printf("EC regulation: EC too high! Adding water..\n");
        current->ec -= 0.1423001;
        current->waterLevel += 0.3875;
        return;
    }
    
    printf("EC regulation: EC too low! Adding nutrients..\n");
    current->ec += 0.1543;
    return;
}

void edit_mode(struct Plants plants[], int *plantAmount, int *selection) {
    char input;
    int isLoop = 1;
    int num;
    
    while(isLoop) {
        system("cls");
        list_plants(plants, *plantAmount);
        printf("\nType the number of what action you want to take:\n");
        printf(" 1. View and/or edit a plant's limit values\n 2. Add another plant to the list\n 3. Select new plant\n 4. Go to simulation\n");
        input = getch();
        
        switch (input) {
            case '1':
            printf("Type the number of the plant you wish to view/edit:\n");
            scanf("%d", &num);
            if (*plantAmount < num | 0 >= plantAmount) {
                invalid_input();
                break;
            }
            
            print_plant(plants[num - 1]);
            printf("\nDo you want to edit the values of this plant? [y/n]\n");
            input = getch();
            if (input == 'y') {
                edit_plant(&plants[num - 1]);
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
            printf("Type the number of the plant you wish to grow:\n");
            scanf("%d", &input);
            if (input <= 0 | input > *plantAmount) {
                invalid_input();
                break;
            }
            
            *selection = input - 1;
            printf("New plant selected!\n");
            delay(1500);
            break;
            
            case '4':
            isLoop = 0;
            break;
            
            default:
            invalid_input();
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
    printf("Water temperatures for this plant range from %.1f-%.1f deg C\n", current.waterTempMin, current.waterTempMax);
    printf("Humidity percentages for this plant range from %.1f-%.1f%%\n", current.humidMax, current.humidMin);
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
    
    humidity:
    printf("\nEnter the upper limit of the humidity %% for this plant: ");
    scanf(" %f", &a);
    printf("\nEnter the lower limit of the humidity %% for this plant: ");
    scanf(" %f", &b);
    
    if (cmp_nums(a, b)) {
        current->humidMax = a;
        current->humidMin = b;
    } else goto humidity;
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
    plants[0].airTempMin = 25.0;
    plants[0].waterTempMax = 25.0;
    plants[0].waterTempMin = 20.5;
    plants[0].phMax = 8.5;
    plants[0].phMin = 7.3;
    plants[0].ecMax = 12.5;
    plants[0].ecMin = 10;
    plants[0].humidMax = 60.4;
    plants[0].humidMin = 43.5;
}

void debug_mode(struct PlantSensors *current) {
    char input;
    float temp;
    int isLoop = 1;
    
    while (isLoop) {
        system("cls");
        
        printf("Current values:\n");
        printf(" 1. pH: %.2f\n 2. EC: %.2f\n 3. water level in tank: %.1f L\n 4. air temperature: %.1f deg C\n 5. water temperature: %.1f deg C\n 6. humidity level: %.2f%%\n", current->ph, current->ec, current->waterLevel, current->airTemp, current->waterTemp, current->humidity);
        printf("\nEnter the number of what action you want to do.\n 1. Edit value(s)\n 2. Rename current plant\n 3. Exit debug mode\n");
        
        input = getch();
        
        switch (input) {
            case '1':
            printf("\nWhat value do you want to edit? (type number)\n");
            input = getch();
            
            switch (input) {
                case '1':
                printf("\nEnter the new pH value: ");
                scanf("%f", &temp);
                
                
                if (0 > temp | 14 < temp) {
                    invalid_input();
                    break;
                }
                current->ph = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                case '2':
                printf("\nEnter the new EC value: ");
                scanf("%f", &temp);
                current->ec = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                case '3':
                printf("\nEnter the new water level: ");
                scanf("%f", &temp);
                current->waterLevel = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                case '4':
                printf("\nEnter the new air temperature: ");
                scanf("%f", &temp);
                current->airTemp = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                case '5':
                printf("\nEnter the new water temperature: ");
                scanf("%f", &temp);
                current->waterTemp = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                case '6':
                printf("\nEnter the new humidity value: ");
                scanf("%f", &temp);
                current->humidity = temp;
                
                printf("\nValue changed!");
                delay(1000);
                break;
                
                default:
                invalid_input();
                break;
            }
            break;
            
            case '2':
            printf("\nType in the new name for the current plant: ");
            scanf("%s", &current->name);
            printf("\nName changed to %s", current->name);
            delay(1500);
            break;
            
            case '3':
            isLoop = 0;
            break;
            
            default:
            invalid_input();
            break;
        }
            
    }
}