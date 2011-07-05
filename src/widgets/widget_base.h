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

#include <FL/FL_Widget.H>

#include "../map.h"

namespace DStarLite
{
	class BaseWidget : public Fl_Widget
	{
		public:

			/*
			 * @var  unsigned char*  data
			 */
			unsigned char* data;

			/*
			 * @var  Map::Cell  current position
			 */
			Map::Cell* current;

			/*
			 * @var  Map::Cell  goal position
			 */
			Map::Cell* goal;
	};
};

#endif // DSTARLITE_WIDGET_BASE_H