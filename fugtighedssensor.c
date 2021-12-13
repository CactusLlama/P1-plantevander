#include<stdio.h>

int main() {
    int humid=0;

    if (humid==0) {
        printf("");
        return 0;
    }
        else if (humid>0) {
        printf("turning on fans");
        scanf("%d", humid);
        if (humid == 0) {
            Printf("turning off fans");
        } else return 0;
    }
}

