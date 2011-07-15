/**
 * Robot Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_WIDGET_ROBOT_H
#define DSTARLITE_WIDGET_ROBOT_H

#include "widget_base.h"

using namespace DStarLite;

namespace DStarLite
{
	class RobotWidget : public BaseWidget
	{
		public:

			/**
			 * @var  int  scan radius of the robot
			 */
			int scan_radius;

			/**
			 * @var  list<Map::Cell*>  planned path
			 */
			list<Map::Cell*> path_planned;

			/**
			 * Constructor.
			 *
			 * @see  parent
			 */
			RobotWidget(int x, int y, int w, int h);

			/**
			 * Draws basic map and overlaying graphics (path, robot, etc).
			 *
			 * @see  parent
			 */
			virtual void draw();
	};
};

#endif // DSTARLITE_WIDGET_BASE_H