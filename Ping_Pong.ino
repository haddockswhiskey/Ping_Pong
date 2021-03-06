/* 
	Written by Des Bylund and Haddocks Whiskey
*/
#include "SPI.h"
#include "Cube.h"
Cube cube;
#define FALSE 0
#define TRUE 1
struct vecAssess
{
    //defined but not declared (initialised)
    int xvec,yvec,zvec;
    int valid;
};

void setup(void)
{
    // Serial port options for control of the Cube using serial commands are:
    // 0: Control via the USB connector (most common).
    // 1: Control via the RXD and TXD pins on the main board.
    // -1: Don't attach any serial port to interact with the Cube.
    cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}
int calcValid(int currx,int curry,int currz,int xvec,int yvec,int zvec)
{
    //this accepts where we are and one of 27 possible next moves and returns if it is valid:
    if (xvec == 0 && yvec == 0 && zvec == 0) //no null vector
        return FALSE;
    if (currx + xvec > 3 || currx + xvec < 0)
        return FALSE;
    if (curry + yvec > 3 || curry + yvec < 0)
        return FALSE;
    if (currz + zvec > 3 || currz + zvec < 0)
        return FALSE;
    return TRUE;
}
void loop(void) 
    {
        int xvec=1, yvec=1, zvec=1;
        int nextx, nexty, nextz, x, y, z;
        int currx=0, curry=0, currz=0;
        rgb_t currCol = WHITE;
        int DELAY = 300;
        int i,j,validCount,ranVal,rowToUse;        
        //array of vecAssess structures
        struct vecAssess vecAssArr[27];
        //turns off all lights to ensure no hangovers from when last run
        cube.all(BLACK);
        //activates first light
        cube.set(currx, curry, currz, currCol);
        //finds next light to illuminate
        
        //populate array with values
        i=0;
        for (x=-1; x<2; x++)
            for (y=-1; y<2; y++)
                for (z=-1; z<2; z++)
                {
                    vecAssArr[i].xvec = x;
                    vecAssArr[i].yvec = y;
                    vecAssArr[i].zvec = z;
                    vecAssArr[i].valid = FALSE;
                    i++;
                }
        
        for (i=0; i<1000; i++)
        {
            delay(DELAY);
            //sets "next" to the current position
            nextx = currx + xvec;
            nexty = curry + yvec;
            nextz = currz + zvec;

            if (nextx > 3 || nextx < 0 || nexty > 3 || nexty < 0 || nextz > 3 || nextz < 0)
            {
                //populate valid assessment "column"
                validCount = 0;
                for (j=0; j<27; j++)
                {
					vecAssArr[j].valid = calcValid(
						//where we are:
						currx,curry,currz,
						//one of 27 possible next moves:
						vecAssArr[j].xvec,vecAssArr[j].yvec,vecAssArr[j].zvec);
					if (vecAssArr[j].valid)
						validCount++;
                }
                ranVal = random(validCount);
                validCount = 0;
                rowToUse = -1;//haven't found row to use yet
                for (j=0; j<27 && rowToUse == -1; j++)
                {
                    if (vecAssArr[j].valid)
                    {
                        if (ranVal == validCount)
                            rowToUse = j;
                        validCount++;
                    }
                }
                if (rowToUse == -1)
                    //this program has failed to find a valid vector 
                    return;
                nextx = currx + vecAssArr[rowToUse].xvec;
                nexty = curry + vecAssArr[rowToUse].yvec;
                nextz = currz + vecAssArr[rowToUse].zvec;
                xvec = vecAssArr[rowToUse].xvec;
                yvec = vecAssArr[rowToUse].yvec;
                zvec = vecAssArr[rowToUse].zvec;
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
