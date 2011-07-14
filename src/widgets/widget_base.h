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
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

#include "../map.h"

using namespace DStarLite;

namespace DStarLite
{
	class BaseWidget : public Fl_Widget
	{
		public:

			/**
			 * @var  Map::Cell*  current position
			 */
			Map::Cell* current;

			/**
			 * @var  unsigned char*  map data
			 */
			unsigned char* data;

			/*
			 * @var  Map::Cell*  goal position
			 */
			Map::Cell* goal;

			/**
			 * @var  int  radius of the robot (in pixels)
			 */
			int robot_radius;

			/**
			 * Constructor.
			 *
			 * @see  parent
			 */
			BaseWidget(int x, int y, int w, int h);

			/**
			 * Deconstructor.
			 *
			 * @see  parent
			 */
			~BaseWidget();
	};
};

#endif // DSTARLITE_WIDGET_BASE_H
