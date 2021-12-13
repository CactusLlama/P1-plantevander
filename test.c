#include <stdio.h>
#include "fugtighedssensor.h"

int humid = 0;

int main(){
  humid = humidity_regulater(70, 30);
  printf("Luftfugtigheden er %d\n", humid);
}
