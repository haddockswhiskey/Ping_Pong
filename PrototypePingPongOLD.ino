#include "SPI.h"
#include "Cube.h"
Cube cube;
void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}
void loop(void) {
  int xvec=1, yvec=1, zvec=1;
  int nextx, nexty, nextz;
  int currx=0, curry=0, currz=0;
  rgb_t currCol = WHITE;
  int i;
  
  //turns off all lights to ensure no hangovers from when last run
  cube.all(BLACK);
  //activates first light
  cube.set(currx, curry, currz, currCol);
  
  //finds next light to illuminate
  for (i=0; i<1000; i++)
  {     
      delay(100);
      //sets "next" to the current position
      nextx = currx + xvec;
      nexty = curry + yvec;
      nextz = currz + zvec;
      
      //checks if next# will be outside of cube
      if (nextx > 3 || nextx < 0)
      {
          if (xvec == 0)
              if (random(2)) xvec = -1; else xvec = 1;
          else
              xvec = xvec * -random(2);
          nextx = currx + xvec;
      }
      if (nexty > 3 || nexty < 0)
      {
          if (yvec == 0)
              if (random(2)) yvec = -1; else yvec = 1;
          else
              yvec = yvec * -random(2);
          nexty = curry + yvec;
      }
      if (nextz > 3 || nextz < 0)
      {
          if (zvec == 0)
              if (random(2)) zvec = -1; else zvec = 1;
          else
              zvec = zvec * -random(2);
          nextz = currz + zvec;
      }
      currCol = WHITE;
      //check if null vector doh!
      if (xvec == 0 && yvec == 0 && zvec == 0)
      {
        if (nextx == 3)
            xvec = -1;
          else if (nextx == 0)
            xvec = 1;
            
        if (nexty == 3)
            yvec = -1;
        else if (nexty == 0)
            yvec = 1;        
        
        if (nextz == 3)
            zvec = -1;
        else if (nextz == 0)
            zvec = 1;
        
        nextx = currx + xvec;
        nexty = curry + yvec;
        nextz = currz + zvec;
        currCol = WHITE;
      }
      //disable current light as now ready for next light
      cube.set(currx, curry, currz, BLACK);
      
      //update current# with calculated next#
      currx = nextx;
      curry = nexty;
      currz = nextz;
      cube.set(currx, curry, currz, currCol);
  }
  cube.all(RED);
  delay(500);
}
