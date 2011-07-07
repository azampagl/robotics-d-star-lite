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

namespace DStarLite
{
	class RobotWidget : public BaseWidget
	{
		public:

			/*
			 * @var  int
			 */
			int scan_radius;

			/*
			 * @var  list<Map::Cell*>
			 */
			list<Map::Cell*> path_planned;

			RobotWidget(int x, int y, int w, int h);
			//RobotWidget(int X,int Y,int W,int H) : BaseWidget(X,Y,W,H) {}

			/*
			 * @see  parent
			 */
			virtual void draw();
	};
};

#endif // DSTARLITE_WIDGET_BASE_H