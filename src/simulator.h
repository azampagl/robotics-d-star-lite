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
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_ask.H>

#include "planner.h"
#include "map.h"
#include "widgets/widget_real.h"
#include "widgets/widget_robot.h"

using namespace DStarLite;

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
					char* real_bitmap;

					/**
					 * @var  char*  robot map file
					 */
					char* robot_bitmap;

					/**
					 * @var  pair<unsigned int, unsigned int>  start position
					 */
					pair<unsigned int, unsigned int> start;

					/**
					 * @var pair<unsigned int, unsigned int>  goal position
					 */
					pair<unsigned int, unsigned int> goal;

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
			 * @var  double  unwalkable value of bitmap
			 */
			static const double UNWALKABLE_CELL;

			/**
			 * Executes the simulator when the start button is clicked.
			 *
			 * @param   Fl_Widget*   widget
			 * @param   void*        simulator
			 * @return  void
			 */
			static void callback(Fl_Widget* w, void* p);

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
			 * Draws the window.
			 *
			 * @return  int
			 */
			int draw();

			/**
			 * Main execution method.
			 *
			 * @return  int  successfull
			 */
			int execute();

			/**
			 * Init the simulator.
			 *
			 * @return  bool  initialized already
			 */
			bool init();

			/**
			 * Redraws the window.
			 *
			 * @return  void
			 */
			void redraw();

			/*
			 * Scans map for updated cells.
			 *
			 * @return  bool  updates found
			 */
			bool update_map();

		protected:

			/**
			 * @var  Config  simulator config options
			 */
			Config _config;

			/**
			 * @var  bool  simulator initialized
			 */
			bool _init;

			/**
			 * @var  Map*  real map, with all obstacles
			 */
			Map* _map;

			/**
			 * @var  char*  name of the simulator
			 */
			char* _name;

			/**
			 * @var  Planner*  planner
			 */
			Planner* _planner;

			/**
			 * @var  RealWidget*  real widget
			 */
			RealWidget* _real_widget;

			/**
			 * @var  RobotWidget*  robot widget
			 */
			RobotWidget* _robot_widget;

			/**
			 * @var  Fl_Button*  start button
			 */
			Fl_Button* _start_button;

			/**
			 * @var  Fl_Window*  window
			 */
			Fl_Window* _window;
	};
};

#endif // DSTARLITE_SIMULATOR_H
