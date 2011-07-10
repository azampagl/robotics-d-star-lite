/**
 * Robot Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "widget_robot.h"

/**
 * Constructor.
 *
 * @see  parent
 */
RobotWidget::RobotWidget(int x, int y, int w, int h) : BaseWidget(x, y, w, h)
{
}

/**
 * Draws basic map and overlaying graphics (path, robot, etc).
 *
 * @see  parent
 */
void RobotWidget::draw()
{
	// Keep drawings within the widget
	fl_push_clip(x() ,y() ,w() ,h());

	// Draw map
	fl_draw_image_mono(data, x(), y(), w(), h());

	// Draw planned path
	fl_color(FL_BLUE);
	for (list<Map::Cell*>::iterator i = path_planned.begin(); i != path_planned.end(); i++)
	{
		fl_point(x() + (*i)->x(), y() + (*i)->y());
	}

	// Draw scanner radius
	fl_begin_complex_polygon();
	fl_color(FL_RED);
	fl_arc(x() + current->x(), y() + current->y(), scan_radius, -360, 360);
	fl_end_complex_polygon();

	// Draw current position
	fl_begin_complex_polygon();
	fl_color(FL_DARK_RED);
	fl_circle(x() + current->x(), y() + current->y(), robot_radius);
	fl_end_complex_polygon();
	
	fl_pop_clip();
}
