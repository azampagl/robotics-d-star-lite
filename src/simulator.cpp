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
	Fl_BMP_Image bitmap_real(config.bitmap_real);
	Fl_BMP_Image bitmap_robot(config.bitmap_robot);

	// Turn black and white
	bitmap_real.desaturate();
	bitmap_robot.desaturate();

	// Make sure bitmaps were loaded
	//if (real_bitmap.load(config.real_map) < 0 || robot_bitmap.load(config.robot_map) < 0)
	//	throw Exception();

	// Make sure images are same size
	//if (real_bitmap.height() != robot_bitmap.height() || real_bitmap.width() != robot_bitmap.width())
	//	throw Exception();

	// Widths and heights are the same, set a constant
	int img_width = bitmap_real.w();
	int img_height = bitmap_real.h();
	int size = img_width * img_height * bitmap_real.d();

	// Calculate window width and height
	int window_width = img_width * 2 + Simulator::WINDOW_IMG_PADDING + Simulator::WINDOW_WIDTH_PADDING * 2;
	int window_height = img_height + Simulator::WINDOW_HEIGHT_PADDING * 2;

	_window = new Fl_Window(window_width, window_height, name);
	_window->begin();

	_widget_real = new RealWidget(Simulator::WINDOW_WIDTH_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);
	_widget_robot = new RobotWidget(Simulator::WINDOW_WIDTH_PADDING + img_width + Simulator::WINDOW_IMG_PADDING, Simulator::WINDOW_HEIGHT_PADDING, img_width, img_height);

	const char* buff1 = bitmap_real.data()[0];
	_widget_real->data = new unsigned char[size];
	for (int i = 0; i < size; i++)
	{
		_widget_real->data[i] = buff1[i];
	}

	const char* buff2 = bitmap_robot.data()[0];
	_widget_robot->data = new unsigned char[size];
	for (int i = 0; i < size; i++)
	{
		_widget_robot->data[i] = buff2[i];
	}
	
	_window->end();

	//
	_widget_real->robot_radius = _widget_robot->robot_radius = 2;

	//
	_widget_robot->scan_radius = config.scan_radius;

	// Make the map
	_map = new Map(img_height, img_width);
	_widget_real->current = _widget_robot->current = (*_map)(config.start.first, config.start.second);
	_widget_real->goal = _widget_robot->goal = (*_map)(config.goal.first, config.goal.second);

	// Build map
	/*_real_map = new Map(real_bitmap.height(), real_bitmap.width());
	_build_map(_real_map, &real_bitmap);
	
	_real_image = cvLoadImage(config.real_map);
	if ( ! _real_image)
		throw Exception();

	_real_window_name << _name.str();
	_real_window_name << " - Real Map";

	cvNamedWindow(_real_window_name.str().c_str(), CV_WINDOW_AUTOSIZE); 
	cvMoveWindow(_real_window_name.str().c_str(), 100, 100);
	cvShowImage(_real_window_name.str().c_str(), _real_image);

	// Build robot map, image, and window
	_robot_map = new Map(robot_bitmap.height(), robot_bitmap.width());
	_build_map(_robot_map, &robot_bitmap);

	_robot_image = cvLoadImage(config.robot_map);
	if ( ! _robot_image)
		throw Exception();

	_robot_window_name << _name.str();
	_robot_window_name << " - Robot Map";

	cvNamedWindow(_robot_window_name.str().c_str(), CV_WINDOW_AUTOSIZE); 
	cvMoveWindow(_robot_window_name.str().c_str(), 700, 100);
	cvShowImage(_robot_window_name.str().c_str(), _robot_image);

	// Make sure start exists
	if ( ! _robot_map->has(config.start.second, config.start.first))
		throw Exception();

	// Make sure goal exists
	if ( ! _robot_map->has(config.goal.second, config.goal.first))
		throw Exception();

	_path = Path(_robot_map);

	_dstar = new DStarLite(_robot_map, (*_robot_map)(config.start.second, config.start.first), (*_robot_map)(config.goal.second, config.goal.first));
	_dstar->replan();

	_planned_path = _dstar->path();
	_smoothed_path = _path.smooth(_planned_path);
	_smoothed_path = _robot_map->path(_smoothed_path);

	_traversed_path.push_back(_smoothed_path.front());
	//_traversed_path.push_back(_planned_path.front());*/
}

/**
 * Deconstructor.
 */
Simulator::~Simulator()
{
	delete _window;
	delete _map;
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
	/*// Get the image data from actualImage.
	int height = _real_image->height;
	int width = _real_image->width;
	int step = _real_image->widthStep;
	int depth = _real_image->depth;
	int channels = _real_image->nChannels;
	unsigned char* real_data = (unsigned char*) _real_image->imageData;
	unsigned char* robot_data = (unsigned char*) _robot_image->imageData;

	// Colors
	CvScalar red   = CV_RGB(255, 0, 0);
	CvScalar yellow  = CV_RGB(255, 255, 0);
	CvScalar green = CV_RGB(0, 255, 0);
	CvScalar blue  = CV_RGB(0, 0, 255);

	// Reset image data.
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			double real_cost = (*_real_map)(i, j)->cost;
			double robot_cost = (*_robot_map)(i, j)->cost;

			for(int k = 0; k < channels; k++)
			{
				real_data[i * step + j * channels + k] = (unsigned char) ((real_cost == Tile::UNWALKABLE) ? Simulator::UNWALKABLE : real_cost + Simulator::COST_DIFFERENCE - 1.0);
				robot_data[i * step + j * channels + k] = (unsigned char) ((robot_cost == Tile::UNWALKABLE) ? Simulator::UNWALKABLE : robot_cost + Simulator::COST_DIFFERENCE - 1.0);
			}
		}
	}

	Tile* position = _traversed_path.back();
	Tile* goal = _dstar->goal();

	// Draw start and goal points
	CvPoint cv_position = cvPoint(position->x, position->y);
	CvPoint cv_goal = cvPoint(goal->x, goal->y);

	cvCircle(_real_image, cv_position, 3, CV_RGB(255,20,147), -1);
	cvCircle(_real_image, cv_goal, 3, CV_RGB(60,179,113), -1);
	cvCircle(_robot_image, cv_position, 3, CV_RGB(255,20,147), -1);
	cvCircle(_robot_image, cv_goal, 3, CV_RGB(60,179,113), -1);

	// Drawing robot sensor range on the image in BLUE.
	cvCircle(_robot_image, cv_position, _config.scan_radius, blue);

	// Draw traversed path in GREEN.
	for (list<Tile*>::iterator i = _traversed_path.begin(); i != _traversed_path.end(); i++)
	{
		CvPoint p = cvPoint((*i)->x, (*i)->y);
		cvCircle(_robot_image, p, 1, green, -1);
	}

	// Draw planned path in light red.
	for (list<Tile*>::iterator i = _planned_path.begin(); i != _planned_path.end(); i++)
	{
		CvPoint p = cvPoint((*i)->x, (*i)->y);
		cvCircle(_robot_image, p, 1, yellow, -1);
	}

	// Draw smoothed path in RED.
	for (list<Tile*>::iterator i = _smoothed_path.begin(); i != _smoothed_path.end(); i++)
	{
		CvPoint p = cvPoint((*i)->x, (*i)->y);
		cvCircle(_robot_image, p, 1, red, -1);
	}

	// Show the images
	cvShowImage(_real_window_name.str().c_str(), _real_image);
	cvShowImage(_robot_window_name.str().c_str(), _robot_image);

	cvWaitKey(5);*/
}

/**
 * Execute.
 *
 * @return  int  successfull
 */
int Simulator::execute()
{
	/*if (_dstar->start() == _dstar->goal())
		return 1;

	if (update_map())
	{
		if ( ! _dstar->replan())
			return -1;

		_planned_path = _dstar->path();
		_smoothed_path = _path.smooth(_planned_path);
		_smoothed_path = _robot_map->path(_smoothed_path);

		if ( ! _smoothed_path.empty())
		{
			_smoothed_path.pop_front();
		}	
		if ( ! _planned_path.empty())
		{
			_planned_path.pop_front();
		}

		//traversed_path.push_back(_planned_path.front());
		//_dstar->start(_planned_path.front());
		_traversed_path.push_back(_smoothed_path.front());
		_dstar->start(_smoothed_path.front());
	}
	else
	{
		if ( ! _smoothed_path.empty())
		{
			_smoothed_path.pop_front();
		}	
		if ( ! _planned_path.empty())
		{
			_planned_path.pop_front();
		}

		//_traversed_path.push_back(_planned_path.front());
		//_dstar->start(_planned_path.front());
		_traversed_path.push_back(_smoothed_path.front());
		_dstar->start(_smoothed_path.front());
	}

	return 0;*/
	return 0;
}

/*
 * Scans map for updated tiles.
 *
 * @return  bool  updates found
 */
bool Simulator::update_map()
{
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