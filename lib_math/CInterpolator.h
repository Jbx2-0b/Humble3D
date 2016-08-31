#ifndef CINTERPOLATOR_H
#define CINTERPOLATOR_H

class Interpolator
{
	double cubicInterpolate (double p[4], double x)
	{
		return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
	}

	double bicubicInterpolate (double p[4][4], double x, double y)
	{
		double arr[4];
		arr[0] = cubicInterpolate(p[0], y);
		arr[1] = cubicInterpolate(p[1], y);
		arr[2] = cubicInterpolate(p[2], y);
		arr[3] = cubicInterpolate(p[3], y);
		return cubicInterpolate(arr, x);
	}

	double tricubicInterpolate (double p[4][4][4], double x, double y, double z)
	{
		double arr[4];
		arr[0] = bicubicInterpolate(p[0], y, z);
		arr[1] = bicubicInterpolate(p[1], y, z);
		arr[2] = bicubicInterpolate(p[2], y, z);
		arr[3] = bicubicInterpolate(p[3], y, z);
		return cubicInterpolate(arr, x);
	}

	double nCubicInterpolate (int n, double* p, double coordinates[])
	{
		assert(n > 0);
		if (n == 1)
		{
			return cubicInterpolate(p, *coordinates);
		}
		else
		{
			double arr[4];
			int skip = 1 << (n - 1) * 2;
			arr[0] = nCubicInterpolate(n - 1, p, coordinates + 1);
			arr[1] = nCubicInterpolate(n - 1, p + skip, coordinates + 1);
			arr[2] = nCubicInterpolate(n - 1, p + 2*skip, coordinates + 1);
			arr[3] = nCubicInterpolate(n - 1, p + 3*skip, coordinates + 1);
			return cubicInterpolate(arr, *coordinates);
		}
	}
};

#define CINTERPOLATOR_H