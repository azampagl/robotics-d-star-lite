/**
 * Simulator.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "simulator.h"

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

/**
 * Executes the simulator when the start button is clicked.
 *
 * @param   Fl_Widget*   widget
 * @param   void*        simulator
 * @return  void
 */
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

	// Not initialized yet (after start is clicked)
	_init = false;

	// Make two bitmaps images
	Fl_BMP_Image real_bitmap(config.real_bitmap);
	Fl_BMP_Image robot_bitmap(config.robot_bitmap);

	// Widths and heights are the same, set a constant
	int img_width = real_bitmap.w();
	int img_height = real_bitmap.h();
	int img_depth = real_bitmap.d();

	// Make sure the real image and the robot's image are the same dimensions
	if (img_width != robot_bitmap.w() || img_height != robot_bitmap.h() || img_depth != robot_bitmap.d())
	{
		fl_alert("Invalid Files or Bitmaps Are Different Sizes!");
		throw;
	}

	// Button width and height
	int button_width = 100;
	int button_height = 50;

	// Calculate window width and height
	int window_width = img_width * 2 + Simulator::WINDOW_IMG_PADDING + Simulator::WINDOW_WIDTH_PADDING * 2;

	// Make sure the button isn't larger than the window
	if (window_width < button_width)
	{
		window_width = button_width * 2;
	}

	int window_height = img_height + Simulator::WINDOW_HEIGHT_PADDING * 3 + button_height;

	// Make main window
	_window = new Fl_Double_Window(window_width, window_height, name);
	_window->box(FL_FLAT_BOX);
	_window->begin();

	// Make real and robot widgets
	_real_widget = new RealWidget(Simulator::WINDOW_WIDTH_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_real_widget->box(FL_NO_BOX);
	_robot_widget = new RobotWidget(Simulator::WINDOW_WIDTH_PADDING + img_width + Simulator::WINDOW_IMG_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_robot_widget->box(FL_NO_BOX);

	// Make start button
	_start_button = new Fl_Button((int)((window_width / 2) - (button_width / 2)), img_height + Simulator::WINDOW_HEIGHT_PADDING * 2, button_width, button_height, "Start");
	_start_button->callback(Simulator::callback, (void*) this);

	// Prepare real and robot image buffers
	_real_widget->data = new unsigned char[img_width * img_height];
	_robot_widget->data = new unsigned char[img_width * img_height];

	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			// Index key
			int k1 = (i * img_width) + j;
			// Depth key
			int k2 = (i * img_width * img_depth) + (j * img_depth);

			if (img_depth == 1)
			{
				_real_widget->data[k1] = real_bitmap.array[k2];
				_robot_widget->data[k1] = robot_bitmap.array[k2];
			}
			else if (img_depth == 3)
			{
				// Convert to grayscale
				_real_widget->data[k1] = (unsigned char) (0.3 * real_bitmap.array[k2] + 0.59 * real_bitmap.array[k2 + 1] + 0.11 * real_bitmap.array[k2 + 2] + 0.5);
				_robot_widget->data[k1] = (unsigned char) (0.3 * robot_bitmap.array[k2] + 0.59 * robot_bitmap.array[k2 + 1] + 0.11 * robot_bitmap.array[k2 + 2] + 0.5);
			}
			// Unsupported depth
			else
			{
				throw;
			}
		}
	}
	
	_window->end();

	// Set the robot radius
	_real_widget->robot_radius = _robot_widget->robot_radius = 2;

	// Sert the scan radius
	_robot_widget->scan_radius = config.scan_radius;

	// Make the map
	_map = new Map(img_height, img_width);

	// Set current and goal position
	_real_widget->current = _robot_widget->current = (*_map)(config.start.first, config.start.second);
	_real_widget->goal = _robot_widget->goal = (*_map)(config.goal.first, config.goal.second);

	// Build map
	for (int i = 0; i < img_height; i++)
	{
		for (int j = 0; j < img_width; j++)
		{
			int k = (i * img_width) + j;
			double v = (double) _robot_widget->data[k];

			// Cell is unwalkable
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

	// Push start position
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
 * Draws the window.
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
 * Main execution method.
 *
 * @return  int  successfull
 */
int Simulator::execute()
{
	if (_planner->start() == _planner->goal())
	{
		fl_alert("Goal Reached!");
		_real_widget->current = _robot_widget->current = _planner->goal();
		return 1;
	}

	// Check if map was updated
	if (update_map())
	{
		// Replan the path
		if ( ! _planner->replan())
		{
			fl_alert("No Solution Found!");
			throw;
		}

		_robot_widget->path_planned = _planner->path();

		if ( ! _robot_widget->path_planned.empty())
		{
			_robot_widget->path_planned.pop_front();
		}
	}

	// Step
	_real_widget->path_traversed.push_back(_robot_widget->path_planned.front());
	_planner->start(_robot_widget->path_planned.front());
	_real_widget->current = _robot_widget->current = _planner->start();
	_robot_widget->path_planned.pop_front();

	return 0;
}

/**
 * Init the simulator.
 *
 * @return  bool  initialized already
 */
bool Simulator::init()
{
	if (_init)
		return true;

	_init = true;

	if ( ! _planner->replan())
	{
		fl_alert("No Solution Found!");
		throw;
	}

	_robot_widget->path_planned = _planner->path();

	return false;
}

/**
 * Redraws the window.
 *
 * @return  void
 */
void Simulator::redraw()
{
	_window->redraw();
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

	unsigned int x, y;
	x = current->x();
	y = current->y();

	// Radius^2
	unsigned int radius = _robot_widget->scan_radius;
	unsigned int radius2 = radius * radius;

	unsigned int rows, cols;
	rows = _map->rows();
	cols = _map->cols();

	// Make an imaginary box around the scan circle
	unsigned int max_x, max_y, min_x, min_y;
	max_x = (x + radius < cols) ? x + radius : cols;
	max_y = (y + radius < rows) ? y + radius : rows;
	min_x = (x > radius) ? x - radius : 0;
	min_y = (y > radius) ? y - radius : 0;

	for (unsigned int i = min_y; i < max_y; i++)
	{
		int dy = y - i;
		unsigned int dy2 = dy * dy;

		for (unsigned int j = min_x; j < max_x; j++)
		{
			int dx = x - j;
			
			if ((dx * dx) + dy2 < radius2)
			{
				unsigned int k = (i * cols) + j;

				// Check if an update is required
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

	return error;
}