/**
 * Real Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_WIDGET_REAL_H
#define DSTARLITE_WIDGET_REAL_H

#include "widget_base.h"

namespace DStarLite
{
	class RealWidget : public BaseWidget
	{
		public:

			list<Map::Cell*> path_traversed;

			RealWidget(int x, int y, int w, int h) : BaseWidget(x, y, w, h){}

			/*
			 * @var  unsigned char*  data
			 */
			void draw();
	};
};

#endif // DSTARLITE_WIDGET_REAL_H