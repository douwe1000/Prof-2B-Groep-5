/*
 * dft.c
 *
 *  Created on: Dec 21, 2022
 *      Author: Eger
 */
#include  <math.h>
#include "main.h"
#include "settings.h"
	  int k=0;
	  int n=0;
	  uint32_t ReX[buffersize/2];//reele deel van x
	  uint32_t ImX[buffersize/2];// imaginaire deel van x
	  uint32_t AmpX[buffersize/2];// amplitude van x
	  uint32_t buffer[22] = {0,1,2,3,4,5,6,7,8,9,10,10,9,8,7,6,5,4,3,2,1,0};

void dft(uint32_t buffer[])
{
	  for (k=0 ; k<buffersize ; k++)
	      {

	          ReX[k] = 0;//reeel
	          for (n=0 ; n<buffersize ; n++)
	          {
	        	  ReX[k] += buffer[n] * cos(n * k * 6.2832 / buffersize);
	          }

	          ImX[k] = 0;
	          for (n=0 ; n<buffersize ; n++)//imaginair
	          {
	        	  ImX[k] -= buffer[n] * sin(n * k * 6.2832 / buffersize);
	          }

	          AmpX[k] = sqrt(ReX[k]*ReX[k] + ImX[k]*ImX[k]);//sterkte van signal per frequentie van k
	      }
}
