/**
 * Real Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "widget_real.h"

using namespace DStarLite;

void RealWidget::draw()
{
	fl_push_clip(x() ,y() ,w() ,h());

	// Draw map
	fl_draw_image_mono(data, x(), y(), w(), h());

	// Draw traversed path
	fl_color(FL_GREEN);
	for (list<Map::Cell*>::iterator i = path_traversed.begin(); i != path_traversed.end(); i++)
	{
		fl_point((*i)->x(), (*i)->y());
	}

	// Draw current position
	fl_color(FL_DARK_RED);
	fl_circle(current->x(), current->y(), robot_radius);

	fl_pop_clip();
}