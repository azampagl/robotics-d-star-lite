/**
 * Base Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_WIDGET_BASE_H
#define DSTARLITE_WIDGET_BASE_H

#include <list>

#include <FL/Fl.H>
#include <FL/FL_Widget.H>
#include <FL/FL_BMP_Image.H>
#include <FL/FL_Widget.H>
#include <FL/fl_draw.H>

#include "../map.h"

namespace DStarLite
{
	class BaseWidget : public Fl_Widget
	{
		public:

			/*
			 * @var  unsigned char*  map data
			 */
			unsigned char* data;

			/*
			 * @var  int  radius of the robot (in pixels)
			 */
			int robot_radius;

			/*
			 * @var  Map::Cell*  current position
			 */
			Map::Cell* current;

			/*
			 * @var  Map::Cell*  goal position
			 */
			Map::Cell* goal;

			BaseWidget(int x, int y, int w, int h);

			~BaseWidget();
	};
};

#endif // DSTARLITE_WIDGET_BASE_H