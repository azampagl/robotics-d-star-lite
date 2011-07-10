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
 * @var  double  unwalkable value of bitmap
 */
const double Simulator::UNWALKABLE_CELL = 0.0;

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

void Simulator::callback(Fl_Widget* w, void* p)
{
	Simulator* simulator = (Simulator*) p;

	if (simulator->init())
		return;

	while (simulator->execute() == 0)
	{
		simulator->redraw();
		Fl::check();
		Fl::wait(0.08);
	}
}

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

	_init = false;

	// Make two bitmaps
	Fl_BMP_Image real_bitmap(config.real_bitmap);
	Fl_BMP_Image robot_bitmap(config.robot_bitmap);

	// Widths and heights are the same, set a constant
	int img_width = real_bitmap.w();
	int img_height = real_bitmap.h();
	int img_depth = real_bitmap.d();

	// Make sure the real image and the robot's image are the same dimensions
	if (img_width != robot_bitmap.w() || img_height != robot_bitmap.h() || img_depth != robot_bitmap.d())
		throw;

	// Button width and height
	int button_width = 100;
	int button_height = 50;

	// Calculate window width and height
	int window_width = img_width * 2 + Simulator::WINDOW_IMG_PADDING + Simulator::WINDOW_WIDTH_PADDING * 2;

	if (window_width < button_width)
	{
		window_width = button_width * 2;
	}

	int window_height = img_height + Simulator::WINDOW_HEIGHT_PADDING * 3 + button_height;

	_window = new Fl_Double_Window(window_width, window_height, name);
	_window->box(FL_FLAT_BOX);
	_window->begin();

	_real_widget = new RealWidget(Simulator::WINDOW_WIDTH_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_real_widget->box(FL_NO_BOX);
	_robot_widget = new RobotWidget(Simulator::WINDOW_WIDTH_PADDING + img_width + Simulator::WINDOW_IMG_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_robot_widget->box(FL_NO_BOX);

	_start_button = new Fl_Button((int)((window_width / 2) - (button_width / 2)), img_height + Simulator::WINDOW_HEIGHT_PADDING * 2, button_width, button_height, "Start");
	_start_button->callback(Simulator::callback, (void*) this);

	_real_widget->data = new unsigned char[img_width * img_height];
	_robot_widget->data = new unsigned char[img_width * img_height];

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int k1 = (i * img_width) + j;
			int k2 = (i * img_width * img_depth) + (j * img_depth);

			if (img_depth == 1)
			{
				_real_widget->data[k1] = real_bitmap.array[k2];
				_robot_widget->data[k1] = robot_bitmap.array[k2];
			}
			else if (img_depth == 3)
			{
				_real_widget->data[k1] = (unsigned char) (0.3 * real_bitmap.array[k2] + 0.59 * real_bitmap.array[k2 + 1] + 0.11 * real_bitmap.array[k2 + 2] + 0.5);
				_robot_widget->data[k1] = (unsigned char) (0.3 * robot_bitmap.array[k2] + 0.59 * robot_bitmap.array[k2 + 1] + 0.11 * robot_bitmap.array[k2 + 2] + 0.5);
			}
			else
			{
				throw;
			}
		}
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
			int k = (i * img_width) + j;
			double v = (double) _robot_widget->data[k];

			if (v == Simulator::UNWALKABLE_CELL)
			{
				v = Map::Cell::COST_UNWALKABLE;
			}
			else
			{
				v = Simulator::COST_DIFFERENCE - v + 1.0;
			}

			(*_map)(i, j)->cost = v;
		}
	}

	// Make planner
	_planner = new Planner(_map, _robot_widget->current, _robot_widget->goal);

	_real_widget->path_traversed.push_back(_planner->start());
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
	_window->redraw();
}

/**
 * Execute.
 *
 * @return  int
 */
int Simulator::execute()
{
	if (_planner->start() == _planner->goal())
	{
		_real_widget->current = _robot_widget->current = _planner->goal();
		return 1;
	}

	if (update_map())
	{
		if ( ! _planner->replan())
			return -1;

		_robot_widget->path_planned = _planner->path();

		if ( ! _robot_widget->path_planned.empty())
		{
			_robot_widget->path_planned.pop_front();
		}
	}

	_real_widget->path_traversed.push_back(_robot_widget->path_planned.front());
	_planner->start(_robot_widget->path_planned.front());
	_real_widget->current = _robot_widget->current = _planner->start();
	_robot_widget->path_planned.pop_front();

	return 0;
}

/**
 */
bool Simulator::init()
{
	if (_init)
		return true;

	_init = true;

	_planner->replan();
	_robot_widget->path_planned = _planner->path();

	return false;
}

/*
 * Scans map for updated tiles.
 *
 * @return  bool  updates found
 */
bool Simulator::update_map()
{
	bool error = false;

	Map::Cell* current = _robot_widget->current;

	unsigned int current_x, current_y;
	current_x = current->x();
	current_y = current->y();
/*
	unsigned int radius = _robot_widget->scan_radius;
	unsigned int radius2 = radius * radius;

	unsigned int rows, cols;
	rows = _map->rows();
	cols = _map->cols();

	unsigned int max_x, max_y, min_x, min_y;
	max_x = (x + radius < cols) ? x + radius : cols;
	max_y = (y + radius < rows) ? y + radius : rows;
	min_x = (x - radius > 0) ? x - radius : 0;
	min_y = (y - radius > 0) ? y - radius : 0;

	for (unsigned int i = min_y; i < max_y; i++)
	{
		int dy = y - i;
		unsigned int dy2 = dy * dy;

		for (unsigned int j = min_x; j < max_y; j++)
		{
			int dx = x - j;
			
			if ((dx * dx) + dy2 < radius2)
			{
				unsigned int k = (i * cols) + j;

				if (_robot_widget->data[k] != _real_widget->data[k])
				{
					error = true;

					_robot_widget->data[k] = _real_widget->data[k];
					double v = (double) _robot_widget->data[k];

					if (v == Simulator::UNWALKABLE_CELL)
					{
						v = Map::Cell::COST_UNWALKABLE;
					}
					else
					{
						v = Simulator::COST_DIFFERENCE - v + 1.0;
					}

					_planner->update((*_map)(i, j), v);
				}
			}
		}
	}

	return error;*/

	double dov = _config.scan_radius;
	double fov = 2 * Math::PI;
	double orientation = 0;

	int width = _map->cols();
	int height = _map->rows();

	bool border = false;
	bool stop = false;

	int x;
	int y;

	for (double i = orientation - fov / 2; i <= orientation + fov / 2; i += 0.01)
	{
		stop = false;
		border = false;

		for (double j = 0; (j <= dov && ! stop); j += 0.1)
		{
			x = (int) ((j * cos(i)) + current_x);
			y = (int) (-(j * sin(i)) + current_y);

			if (x >= width || y >= height || x < 0 || y < 0)
			{
				border = true;
			}

			if ( ! border && ! stop)
			{
				unsigned int k = (y * width) + x;

				if (_robot_widget->data[k] != _real_widget->data[k])
				{
					error = true;

					_robot_widget->data[k] = _real_widget->data[k];
					double v = (double) _robot_widget->data[k];

					if (v == Simulator::UNWALKABLE_CELL)
					{
						v = Map::Cell::COST_UNWALKABLE;
						stop = true;
					}
					else
					{
						v = Simulator::COST_DIFFERENCE - v + 1.0;
					}

					_planner->update((*_map)(y, x), v);
				}
			}
		}
	}

	return error;
}