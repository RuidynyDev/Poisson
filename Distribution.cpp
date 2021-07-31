//---------------------------------------------------------------------------

#pragma hdrstop

#include "Distribution.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//--------------------------------------------------плотность распределения
double f(double x, double y, double z)
{
	return 10.0;                        //const
}

//-------------------------------------------------------граничные условия
double mu_x0 (double y, double z)
{
	return 0.0;
}
double mu_xL (double y, double z)
{
	return 5.0;
}
double mu_y0 (double x, double z)
{
	return 0.0;
}
double mu_yL (double x, double z)
{
	return 5.0;
}
double mu_z0 (double x, double y)
{
	return 0.0;
}
double mu_zL (double x, double y)
{
	return 5.0;
}

//----------------------------------------------------------------------------
U::U (int n, double l, double start, double parameter, double d, bool choice)
{
	N = n;
	L = l;
	omega = parameter;
	delta = d;
	h =  1.0 * L / N;
	iteration = 0;
	done = false;

	distribution = new double **[N+1];
	for (int i = 0; i < N+1; i++)
	{
		distribution[i] = new double *[N+1];
		for (int j = 0; j < N+1; j++)
		  distribution[i][j] = new double [N+1];
	}

	for (int i = 0; i < N+1; i++)
		for (int j = 0; j < N+1; j++)
			for (int k = 0; k < N+1; k++)
				distribution[i][j][k] = start; //нулевая итерация

	//-------------------------------------устанавливаем значения на границах
		for (int j = 0; j < N+1; j++)
			for (int k = 0; k < N+1; k++)
			{
				distribution[0][j][k] = mu_x0 (h*j, h*k);
				distribution[N][j][k] = mu_xL (h*j, h*k);
				distribution[j][0][k] = mu_y0 (h*j, h*k);
				distribution[j][N][k] = mu_yL (h*j, h*k);
				distribution[j][k][0] = mu_z0 (h*j, h*k);
				distribution[j][k][N] = mu_zL (h*j, h*k);
			}

	//------------------------------------------ расчет
	while (iteration < 1000)
	{
		double difference = 0.0;
		for (int i = 1; i < N; i++)
			for (int j = 1; j < N; j++)
				for (int k = 1; k < N; k++)
				{
					double u_prev = distribution[i][j][k];
distribution[i][j][k] = (1.0-omega) * distribution[i][j][k] +
omega * (	distribution[i+1][j][k]+distribution[i-1][j][k]+
			distribution[i][j+1][k]+ distribution[i][j-1][k]+
			distribution[i][j][k+1]+distribution[i][j][k-1])/ 6.0;
					if (choice)
					{
						distribution[i][j][k] -= omega *h*h*f(i*h, j*h, k*h) / 6.0;
					}
					difference += (distribution[i][j][k] - u_prev)* (distribution[i][j][k] - u_prev);
				}
		iteration++;
		if (sqrt(difference) < delta)
		{
			done = true;
			break;
		}
	}
 }

U::~U()
{
	 for (int i = 0; i < N+1; i++)
	 {
		for (int j = 0; j < N+1; j++)
			delete [] distribution [i][j];
		delete  [] distribution[i];
	 }
	 delete [] distribution;
}


