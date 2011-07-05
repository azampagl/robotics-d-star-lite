/**
 * Robot Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "widget_robot.h"

using namespace DStarLite;

void RobotWidget::draw()
{
	fl_push_clip(x() ,y() ,w() ,h());

	// Draw map
	fl_draw_image_mono(data, x(), y(), w(), h());

	// Draw planned path
	fl_color(FL_YELLOW);
	for (list<Map::Cell*>::iterator i = path_planned.begin(); i != path_planned.end(); i++)
	{
		fl_point((*i)->x(), (*i)->y());
	}

	// Draw scanner radius
	fl_color(FL_RED);
	fl_arc(current->x(), current->y(), scan_radius, -360, 360);

	// Draw current position
	fl_color(FL_DARK_RED);
	fl_circle(current->x(), current->y(), robot_radius);
	
	fl_pop_clip();
}