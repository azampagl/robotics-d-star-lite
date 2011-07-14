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
#include <stdlib.h>

#include "math.h"

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
							 * @var  int  hash "constant" (may need to change if width exceeds this value)
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

					/**
					 * @var  static const int  number of cell neighbors
					 */
					static const unsigned int NUM_NBRS;

					/**
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
					 * @param   unsigned int        x-coordinate
					 * @param   unsigned int        y-coordinate
					 * @param   double [optional]   cost of the cell
					 */
					Cell(unsigned int x, unsigned int y, double cost = 1.0);

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
					 * Gets cell neighbors.
					 *
					 * @return  Cell**
					 */
					Cell** nbrs();

					/**
					 * Gets x-coordinate.
					 *
					 * @return  unsigned int
					 */
					unsigned int x();

					/**
					 * Gets y-coordinate.
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
			 * Retrieves a cell.
			 *
			 * @param   unsigned int   row
			 * @param   unsigned int   column
			 * @return  Map::Cell*
			 */
			Cell* operator()(const unsigned int row, const unsigned int col);

			/**
			 * Gets number of cols.
			 *
			 * @return  unsigned int
			 */
			unsigned int cols();

			/**
			 * Checks if row/col exists.
			 *
			 * @param   unsigned int   row
			 * @param   unsigned int   col
			 * @return  bool
			 */
			bool has(unsigned int row, unsigned int col);

			/**
			 * Gets number of rows.
			 *
			 * @return  unsigned int
			 */
			unsigned int rows();

	protected:
			
			/**
			 * @var  Cell***  cells of the map
			 */
			Cell*** _cells;

			/**
			 * @var  unsigned int columns
			 */
			unsigned int _cols;

			/**
			 * @var  unsigned int  rows
			 */
			unsigned int _rows;
	};
};

#endif // DSTARLITE_MAP_H
