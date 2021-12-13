#include<stdio.h>

void humidity_regulater(int humid, int ideal_humidity);



void humidity_regulater(int humid, int ideal_humidity){

    if (humid==ideal_humidity) {
        printf("fan turned off");
    }
    else if (humid>ideal_humidity) {
        printf("turning on fans");
        pointer til struct -= 1;
       // scanf("%d", humid); for testing
        /*if (humid == 0) {
            Printf("turning off fans");
        } else return 0;*/
    }
}

