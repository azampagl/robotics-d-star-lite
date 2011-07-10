/**
 * Real Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "widget_real.h"

/**
 * Constructor.
 *
 * @see  parent
 */
RealWidget::RealWidget(int x, int y, int w, int h) : BaseWidget(x, y, w, h)
{
}

/**
 * Draws basic map and overlaying graphics (path, robot, etc).
 *
 * @see  parent
 */
void RealWidget::draw()
{
	// Keep drawings withing widget
	fl_push_clip(x() ,y() ,w() ,h());

	// Draw map
	fl_draw_image_mono(data, x(), y(), w(), h());

	// Draw traversed path
	fl_color(FL_GREEN);
	for (list<Map::Cell*>::iterator i = path_traversed.begin(); i != path_traversed.end(); i++)
	{
		fl_point(x() + (*i)->x(), y() + (*i)->y());
	}

	// Draw current position
	fl_begin_complex_polygon();
	fl_color(FL_DARK_RED);
	fl_circle(x() + current->x(), y() + current->y(), robot_radius);
	fl_end_complex_polygon();

	fl_pop_clip();
}