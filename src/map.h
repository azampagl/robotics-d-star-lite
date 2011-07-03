/**
 * Map.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#ifndef DSTARLITE_MAP_H
#define DSTARLITE_MAP_H

#include <functional>

using namespace std;

namespace DStarLite
{
	class Map
	{
		public:

			class Cell
			{
				public:

					/**
					 * Hash
					 */
					class Hash : public unary_function<Cell*, size_t>
					{
						public:

							/**
							 * @var  int  hash "constant" (may need to change if large map)
							 */
							static const int C;

							/**
							 * Hashes cell based on coordinates.
							 *
							 * @param   Cell*
							 * @return  size_t
							 */
							size_t operator()(Cell* c) const;
					};

					/*
					 * @var  static const int  number of cell neighbors
					 */
					static const unsigned int NUM_NBRS;

					/*
					 * @var  static const double  cost of an unwalkable cell
					 */
					static const double COST_UNWALKABLE;

					/**
					 * @var  double  cost of cell
					 */
					double cost;

					/**
					 * Constructor.
					 *
					 * @param   unsigned int   x-coordinate
					 * @param   unsigned int   y-coordinate
					 */
					Cell(unsigned int x, unsigned int y);

					/**
					 * Deconstructor.
					 */
					~Cell();

					/**
					 * Initialize.
					 *
					 * @param   Cell**  cell neighbors
					 * @return  void
					 */
					void init(Cell** nbrs);

					/**
					 * Get cell neighbors.
					 *
					 * @return  Cell**
					 */
					Cell** nbrs();

					/**
					 * Get x-coordinate.
					 *
					 * @return  unsigned int
					 */
					unsigned int x();

					/**
					 * Get y-coordinate.
					 *
					 * @return  unsigned int
					 */
					unsigned int y();

				protected:

					/**
					 * @var  bool  initialized
					 */
					bool _init;

					/**
					 * @var  Cell**  neighbors
					 */
					Cell** _nbrs;

					/**
					 * @var  unsigned int  x-coordinate
					 */
					unsigned int _x;

					/**
					 * @var  unsigned int  y-coordinate
					 */
					unsigned int _y;
			};

			/**
			 * Default Constructor.
			 */
			Map();

			/**
			 * Constructor.
			 *
			 * @param  unsigned int   rows
			 * @param  unsigned int   columns
			 */
			Map(unsigned int rows, unsigned int cols);

			/**
			 * Deconstructor.
			 */
			~Map();

			/**
			 * Retrieve a cell.
			 *
			 * @param   unsigned int   row
			 * @param   unsigned int   column
			 * @return  Cell*
			 */
			Cell* operator()(const unsigned int row, const unsigned int col);

			/**
			 * Number of cols.
			 *
			 * @return  unsigned int
			 */
			unsigned int cols();

			/**
			 * Checks if movement grazes any possible blocked tiles.
			 *
			 * @param   Cell*   start
			 * @param   Cell*   destination
			 * @return  bool
			 */
			bool grazes(Cell* a, Cell* b);

			/**
			 * Checks if row/col exists.
			 *
			 * @param   unsigned int   row
			 * @param   unsigned int   col
			 * @return  bool
			 */
			bool has(unsigned int row, unsigned int col);

			/**
			 * Number of rows.
			 *
			 * @return  unsigned int
			 */
			unsigned int rows();

	protected:

			/**
			 * @var  unsigned int columns
			 */
			unsigned int _cols;

			/**
			 * @var  unsigned int  rows
			 */
			unsigned int _rows;

			/**
			 * @var  Cell***  cells of the map
			 */
			Cell*** _cells;
	};
};

#endif // DSTARLITE_MAP_H