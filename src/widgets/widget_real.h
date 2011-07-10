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

using namespace DStarLite;

namespace DStarLite
{
	class RealWidget : public BaseWidget
	{
		public:

			/**
			 * @var  unsigned char*  data
			 */
			list<Map::Cell*> path_traversed;

			/**
			 * Constructor.
			 *
			 * @see  parent
			 */
			RealWidget(int x, int y, int w, int h);

			/**
			 * Draws basic map and overlaying graphics (path, robot, etc).
			 *
			 * @see  parent
			 */
			virtual void draw();
	};
};

#endif // DSTARLITE_WIDGET_REAL_H