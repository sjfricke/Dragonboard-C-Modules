#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "../gpio/gpio.h"

// pins on board used
#define RotateAPin 34 // Define as CLK
#define RotateBPin 32 // Define as DT
#define SwitchPin  30 // Define as Push Button Switch

static volatile int globalCounter = 0 ;

unsigned char flag;
unsigned char last_rob;
unsigned char current_rob;

int rotate_a, rotate_b, push_button;

void btnISR(void)
{
  globalCounter = 0;
}

void rotaryDeal(void)
{
  last_rob = GpioGetValue(rotate_b);

  while(0 == GpioGetValue(rotate_a)){
    current_rob = GpioGetValue(rotate_b);
    flag = 1;
  }

  if(flag == 1){
    flag = 0;
    if((last_rob == 0)&&(current_rob == 1)){
      globalCounter ++;
    }
    if((last_rob == 1)&&(current_rob == 0)){
      globalCounter --;
    }
  }
}

int main(void)
{
  rotate_a = GpioInputPin(RotateAPin);
  rotate_b = GpioInputPin(RotateBPin);
  push_button = GpioInputPin(SwitchPin);

  int tmp = 0;

  while(1){
    rotaryDeal();
    if (tmp != globalCounter){
      printf("%d\n", globalCounter);
      tmp = globalCounter;
    }
  }

  return 0;
}

