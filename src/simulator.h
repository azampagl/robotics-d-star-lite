/**
 * Simulator.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_SIMULATOR_H
#define DSTARLITE_SIMULATOR_H

#include <FL/Fl.H>
#include <FL/FL_BMP_Image.H>
#include <FL/FL_Window.H>

#include "planner.h"
#include "map.h"
#include "widgets/widget_real.h"
#include "widgets/widget_robot.h"

namespace DStarLite
{
	class Simulator
	{
		public:

			/**
			 * Config class.
			 */
			class Config
			{
				public:

					/**
					 * @var  char*  real map file
					 */
					char* bitmap_real;

					/**
					 * @var  char*  robot map file
					 */
					char* bitmap_robot;

					/**
					 * @var  pair<int,int>  start position
					 */
					pair<int,int> start;

					/**
					 * @var  pair<int,int>  goal position
					 */
					pair<int,int> goal;

					/**
					 * @var  unsigned int  scanner radius
					 */
					unsigned int scan_radius;
			};

			/**
			 * @var  int  window width padding
			 */
			static const int WINDOW_WIDTH_PADDING;

			/**
			 * @var  int  window height padding
			 */
			static const int WINDOW_HEIGHT_PADDING;

			/**
			 * @var  int  window image padding
			 */
			static const int WINDOW_IMG_PADDING;

			/**
			 * @var  double  cost difference between bitmap and tile
			 */
			static const double COST_DIFFERENCE;

			/**
			 * @var  unsigned char  unwalkable value of bitmap
			 */
			static const unsigned char UNWALKABLE_CELL;

			/**
			 * Constructor.
			 * 
			 * @param  char*    name of the simulator
			 * @param  Config   config options
			 */
			Simulator(char* name, Config confg);

			/**
			 * Deconstructor.
			 */
			~Simulator();

			/**
			 * Draw.
			 *
			 * @return  void
			 */
			void draw();

			/**
			 * Execute.
			 *
			 * @return  bool  successfull
			 */
			int execute();

			/*
			 * Scans map for updated cells.
			 *
			 * @return  bool  updates found
			 */
			bool update_map();

		protected:

			/**
			 * @var  char*  name of the simulator
			 */
			char* _name;

			/**
			 * @var  Config  simulator config options
			 */
			Config _config;

			/**
			 * @var  Map*  real map, with all obstacles
			 */
			Map* _map;

			/**
			 * @var  Planner*  planner
			 */
			Planner* _planner;

			/**
			 * @var  Fl_Window*  window
			 */
			Fl_Window* _window;

			/**
			 * @var  RealWidget*  real widget
			 */
			RealWidget* _widget_real;

			/**
			 * @var  RobotWidget*  robot widget
			 */
			RobotWidget* _widget_robot;
	};
};

#endif // DSTARLITE_SIMULATOR_H