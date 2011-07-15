/**
 * Math.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_MATH_H
#define DSTARLITE_MATH_H

#include <cmath>
#include <cfloat>

namespace DStarLite
{
	class Math
	{
		public:

			/**
			 * @var  double  INFINITY
			 */
			static const double INF;

			/**
			 * @var  double  PI
			 */
			static const double PI;

			/**
			 * @var  double  SQRT2
			 */
			static const double SQRT2;

			/**
			 * Converts degrees to radians.
			 *
			 * @param   double   degrees
			 * @return  double   radians
			 */
			static double deg2rad(double degrees);

			/**
			 * Converts degrees to a signed degree (-180 < degrees <= 180)
			 *
			 * @param   double   degrees
			 * @return  double   degrees
			 */
			static double deg2signed(double degrees);

			/**
			 * Determines if two doubles are equal based on a precision.
			 *
			 * @param   double              first double
			 * @param   double              second duble
			 * @param   double [optional]   precision
			 * @return  bool
			 */
			static bool equals(double a, double b, double precision = 0.000000000000001);

			/**
			 * Determines if a double is greater than compared to another double
			 * based on a precision.
			 *
			 * @param   double              first double
			 * @param   double              second duble
			 * @param   double [optional]   precision
			 * @return  bool
			 */
			static bool greater(double a, double b, double precision = 0.000000000000001);

			/**
			 * Determines if a double is less than compared to another double
			 * based on a precision.
			 *
			 * @param   double              first double
			 * @param   double              second duble
			 * @param   double [optional]   precision
			 * @return  bool
			 */
			static bool less(double a, double b, double precision = 0.000000000000001);

			/**
			 * Converts radians to degrees.
			 *
			 * @param   double   radians
			 * @return  double   degrees
			 */
			static double rad2deg(double radians);

			/**
			 * Converts radians to a signed radians value (-PI < degrees <= PI)
			 *
			 * @param   double   radians
			 * @return  double   radians
			 */
			static double rad2signed(double radians);
	};
};


#endif // DSTARLITE_MATH_H
