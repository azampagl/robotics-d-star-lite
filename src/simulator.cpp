/**
 * Simulator.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "simulator.h"

using namespace DStarLite;

/**
 * @var  double  cost difference between bitmap and tile;
 */
const double Simulator::COST_DIFFERENCE = 255.0;

/**
 * @var  unsigned char  unwalkable value of bitmap
 */
const unsigned char Simulator::UNWALKABLE_CELL = 0;

/*
 * @var  int  window width padding
 */
const int Simulator::WINDOW_WIDTH_PADDING = 10;

/**
 * @var  int  window height padding
 */
const int Simulator::WINDOW_HEIGHT_PADDING = 10;

/**
 * @var  int  window image padding
 */
const int Simulator::WINDOW_IMG_PADDING = 10;

/**
 * Constructor.
 * 
 * @param  unsigned char*    name of the simulator'
 * @param  Config            config options
*/
Simulator::Simulator(char* name, Config config)
{
	// Set name
	_name = name;

	// Set config
	_config = config;

	// Make two bitmaps
	Fl_BMP_Image real_bitmap(config.real_bitmap);
	Fl_BMP_Image robot_bitmap(config.robot_bitmap);

	// Turn black and white
	real_bitmap.desaturate();
	robot_bitmap.desaturate();

	// Make sure bitmaps were loaded
	//if (real_bitmap.load(config.real_map) < 0 || robot_bitmap.load(config.robot_map) < 0)
	//	throw Exception();

	// Make sure images are same size
	//if (real_bitmap.height() != robot_bitmap.height() || real_bitmap.width() != robot_bitmap.width())
	//	throw Exception();

	// Widths and heights are the same, set a constant
	int img_width = real_bitmap.w();
	int img_height = real_bitmap.h();
	int size = img_width * img_height * real_bitmap.d();

	// Calculate window width and height
	int window_width = img_width * 2 + Simulator::WINDOW_IMG_PADDING + Simulator::WINDOW_WIDTH_PADDING * 2;
	int window_height = img_height + Simulator::WINDOW_HEIGHT_PADDING * 2;

	_window = new Fl_Window(window_width, window_height, name);
	_window->begin();

	_real_widget = new RealWidget(Simulator::WINDOW_WIDTH_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_robot_widget = new RobotWidget(Simulator::WINDOW_WIDTH_PADDING + img_width + Simulator::WINDOW_IMG_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);

	const char* real_buff = real_bitmap.data()[0];
	_real_widget->data = new unsigned char[size];
	for (int i = 0; i < size; i++)
	{
		_real_widget->data[i] = real_buff[i];
	}

	const char* robot_buff = robot_bitmap.data()[0];
	_robot_widget->data = new unsigned char[size];
	for (int i = 0; i < size; i++)
	{
		_robot_widget->data[i] = robot_buff[i];
	}
	
	_window->end();

	//
	_real_widget->robot_radius = _robot_widget->robot_radius = 2;

	//
	_robot_widget->scan_radius = config.scan_radius;

	// Make the map
	_map = new Map(img_height, img_width);
	_real_widget->current = _robot_widget->current = (*_map)(config.start.first, config.start.second);
	_real_widget->goal = _robot_widget->goal = (*_map)(config.goal.first, config.goal.second);

	// Build map
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int k = (i * robot_bitmap.w() * robot_bitmap.d()) + (j * robot_bitmap.d());
			(*_map)(i, j)->cost = Simulator::COST_DIFFERENCE - ((double) robot_buff[k]) + 1.0;
		}
	}

	// Make planner
	_planner = new Planner(_map, _robot_widget->current, _robot_widget->goal);
}

/**
 * Deconstructor.
 */
Simulator::~Simulator()
{
	delete _map;
	delete _planner;
	delete _window;
}

/**
 * Draw.
 *
 * @return  int
 */
int Simulator::draw()
{
	Fl::visual(FL_RGB);
	_window->show();
	return Fl::run();
}

/**
 * Draw.
 *
 * @return  void
 */
void Simulator::redraw()
{
	_real_widget->redraw();
	_robot_widget->redraw();
}

/**
 * Execute.
 *
 * @return  int
 */
int Simulator::execute()
{
	if (_planner->start() == _planner->goal())
		return 1;

	if (update_map())
	{
		if ( ! _planner->replan())
			return -1;

		_robot_widget->path_planned = _planner->path();

		/*if ( ! _robot_widget->path_planned.empty())
		{
			_planned_path.pop_front();
		}*/
	}

	_real_widget->path_traversed.push_back(_robot_widget->path_planned.front());
	_planner->start(_robot_widget->path_planned.front());
	_robot_widget->path_planned.pop_front();

	return 0;
}

/*
 * Scans map for updated tiles.
 *
 * @return  bool  updates found
 */
bool Simulator::update_map()
{
	return true;
	/*bool error = false;

	Tile* position = _traversed_path.back();
	double dov = _config.scan_radius;
	double fov = 2 * Math::PI;
	double orientation = 0;

	int width = _robot_map->cols();
	int height = _robot_map->rows();

	bool border = false;
	bool stop = false;

	double x;
	double y;

	for (double i = orientation - fov / 2; i <= orientation + fov / 2; i += 0.01)
	{
		stop = false;
		border = false;

		for (double j = 0; (j <= dov && ! stop); j += 0.1)
		{
			x =  (j * cos(i)) + position->x;
			y = -(j * sin(i)) + position->y;

			if (x >= width || y >= height || x < 0.0 || y < 0.0)
				border = true;

			if ( ! border && ! stop)
			{
				if ((*_robot_map)((int)y,(int)x)->cost != (*_real_map)((int)y,(int)x)->cost)
				{
					error = true;					
					_dstar->update((*_robot_map)((int)y,(int)x), (*_real_map)((int)y,(int)x)->cost);
				}
				if ((*_real_map)((int)y,(int)x)->cost >= Tile::UNWALKABLE)
				{
					stop = true;
				}
			}
		}
	}

	return error;*/
	return 0;
}