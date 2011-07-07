/**
 * Base Widget.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "widget_base.h"

using namespace DStarLite;

BaseWidget::BaseWidget(int x, int y, int w, int h) : Fl_Widget(x, y, w, h)
{
}

BaseWidget::~BaseWidget()
{
	delete data;
}