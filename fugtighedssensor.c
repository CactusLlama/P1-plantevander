#include<stdio.h>
#include "random.h"
int humidity_regulater(int humid, int ideal_humidity)
{

  if (humid<=ideal_humidity) {
    printf("turned off fans\n");
  }
  else
  {
    printf("turning on fans\n");

    while (humid>ideal_humidity){
      //printf("Input Humidity: ");
      //scanf("%d", &humid);
      humid = get_random(0, 100);
      printf("Input Humidity: %d\n ", humid);
      if (humid <= ideal_humidity) {
        printf("turning off fans\n");
      }
    }

  }
  return humid;

}
