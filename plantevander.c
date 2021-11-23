#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//Declaring functions ahead of use
int processInput(char a);
int delay(int mSeconds);

int main() {
    int state = 0;
    FILE *fp;
    char kbInput;

    //endless loop
    for(;;) {
    printf("Enter the number of which action you want to do.\n 1. Add magnesium\n 2. Add sodium\n 3. Exit program\n");
     
     //if user presses their keyboard, get the char and process which option the user chose
     if(kbhit()) {
        kbInput = getch();
        processInput(kbInput);
     }
        delay(1000);
        system("cls");
    }


    return 0;
}

int processInput(char a) {
    //switch statement to see which option the user chose
    switch (a) {
    case 1:

    break;
    case 2:

    break;
    case 3:
    exit(0);
    break;

    default:
    printf("Invalid input!\n");
    delay(4000);
    return 0;
    }
}

int delay(int mSeconds) {
    //store start time in variable
    clock_t startTime = clock();
    
    //loop until clock is caught up with variable + specified delay
    while (clock() < startTime + mSeconds)
        ;
    //the program exits once while loop is caught up, having delayed a specified amount of seconds
}
