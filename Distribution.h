//---------------------------------------------------------------------------

#ifndef DistributionH
#define DistributionH

#include <math.h>
class U
{
public:
	double ***distribution;
	int iteration;
	bool done;

	U (int n, double l, double start, double parameter, double d, bool choice);
	~U();

private:
	int N;
	double L, h, omega, delta;
};





//---------------------------------------------------------------------------
#endif
