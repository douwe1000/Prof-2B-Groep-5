/*
 * dft.c
 *
 *  Created on: Dec 21, 2022
 *      Author: Eger
 */
#include  <math.h>
#include "main.h"
#include "settings.h"
extern float buffer[buffersize];

	 // int k=0;
	  int n=0;
	  int j=0;
	  float ReX[buffersize/2];//reele deel van x
	  float ImX[buffersize/2];// imaginaire deel van x
	  float AmpX[buffersize/2];// amplitude van x


float dft(int k)
{
	for (j=0; j<(buffersize/2);j++)
	{
		ReX[j]=0;
		ImX[j]=0;
	}

		  for (n=0 ; n<(buffersize-1) ; n++)
		  {
			  ReX[k] = ReX[k] + buffer[n] * cos(n * k * M_PI * 2 / buffersize);
			  ImX[k] = ImX[k] - buffer[n] * sin(n * k * M_PI * 2 / buffersize);
		  }

		  ReX[k] /= buffersize/2;
		  ImX[k] /= buffersize/2;

		  AmpX[k] = sqrt(ReX[k]*ReX[k] + ImX[k]*ImX[k]);//sterkte van signal per frequentie van k
		  return AmpX[k];
}


