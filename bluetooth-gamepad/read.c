#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h> // open()
#include "../gpio/gpio.h"

void writeOut(int a, int b, int c);
int connectController();
void lightData();

struct pollfd pfd;
int fd;
char buf[16];
int i;
int status;

void writeOut(int a, int b, int c) {
  GpioSetValue(33, a);
  GpioSetValue(34, b);
  GpioSetValue(35, c);
}

int connectController() {

  fd = open("/dev/input/js0", O_RDONLY);
  if (fd >= 0) {
    return 0; // opened
  }
  
  int status = system("/home/linaro/Dragonboard-C-Modules/bluetooth-gamepad/connect.sh");
  printf("Status: %d\n", status);

  for (i = 0; i < 3; i++) {
    fd = open("/dev/input/js0", O_RDONLY);
    if (fd >= 0) {
      return 0; // opened
    }    
    sleep(1);
  }

  // 3 times and try again
  return -1;

}

void lightData() {

  /*fd = open("/dev/input/js0", O_RDONLY);
  if (fd < 0) {
    puts("ERROR: Could not open file:\n");
    return -1;
    }*/
  
  pfd.fd = fd;
  pfd.events = POLLIN;
  
  // consume any prior interrupt
  // what ever reason need to clear 13 lines in js0 each time opened
  for ( i = 0; i < 13; i++) {
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, sizeof(buf));
  }

  while(1) {
    poll(&pfd, 1, -1);
    
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, sizeof(buf));

    for (i = 0; i < 16; i++){
      printf("0%02X ", buf[i]);
    }
    puts("\n");
    
    if (buf[6] == 2) {

      if (buf[7] == 4 && buf[4] == 0) {
	//	puts("Left/Right turn off");
	writeOut(0,0,0);
      } else if (buf[7] == 4 && buf[4] == 0x01) {
	//	puts("Move Left");
	writeOut(1,0,0);
      } else if (buf[7] == 4 && buf[4] == 0xff) {
	//	puts("Move right");
	writeOut(0,1,0);
      } else if (buf[7] == 5 && buf[4] == 0) {
	//	puts("Up/Down turn off");
	writeOut(0,0,0);
      } else if (buf[7] == 5 && buf[4] == 0x01) {
	//	puts("Move Up");
	writeOut(0,0,1);
      } else if (buf[7] == 5 && buf[4] == 0xff) {
	//	puts("Move Down");
	writeOut(0,1,1);
      } else {
	continue;
      }

    } else if (buf[6] == 1) {

      if (buf[4] == 0) {
	//	puts("Release\n");
	writeOut(0,0,0);
      } else {

	if (buf[7] == 8) {
	  //	  puts("Left Turn");
	  writeOut(1,0,1);
	} else if (buf[7] == 9) {
	  //	  puts("Right Turn");
	  writeOut(1,1,0);
	}
      }

    } else {
      continue;
    }
  }

  return;
  
}


int main() {

  GpioInput(28);
  GpioOutput(33, 0);
  GpioOutput(34, 0);
  GpioOutput(35, 0);

  
  while(1) {

    status = connectController();

    printf("init while status: %d\n", status);
    
    if (status != 0) {
      // wait for button
      while(1) {
	if (0 == GpioGetValue(28)) {
	  status = connectController();
	  printf("button while status: %d\n", status);
	  if (status == 0) {
	    break;
	  }
	}
	sleep(1);
      } // button while 1
    } else {
      
      lightData();
      puts("light data eneded\n");
    }
  } // main while 1
}
	  
