/**
 * Map.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "map.h"

using namespace DStarLite;

/*
 * @var  unsigned int  number of cell neighbors
 */
const unsigned int Map::Cell::NUM_NBRS = 8;

/*
 * @var  double  cost of an unwalkable tile
 */
const double Map::Cell::COST_UNWALKABLE = 100.0;

/**
 * @var  int  hash "constant" (may need to change if large map)
 */
const int Map::Cell::Hash::C = 1000000;

/**
 * Default Constructor.
 */
Map::Map()
{
	_rows = 0;
	_cols = 0;

	_cells = NULL;
}

/**
 * Constructor.
 *
 * @param  unsigned int   rows
 * @param  unsigned int   columns
 */
Map::Map(unsigned int rows, unsigned int cols)
{
	_rows = rows;
	_cols = cols;

	_cells = new Cell**[rows];

	for (unsigned int i = 0; i < rows; i++)
	{
		_cells[i] = new Cell*[cols];

		for (unsigned int j = 0; j < cols; j++)
		{
			_cells[i][j] = new Cell(j, i);
		}
	}

	// Attach neighbors
	for (unsigned int i = 0; i < _rows; i++)
	{
		for (unsigned int j = 0; j < _cols; j++)
		{
			Cell** nbrs = new Cell*[Cell::NUM_NBRS];
			for (unsigned int k = 0; k < Cell::NUM_NBRS; k++)
			{
				nbrs[k] = NULL;
			}

			// Top
			if (i != 0)
			{
				if (j != 0)
				{
					// Top left
					nbrs[0] = _cells[i - 1][j - 1];
				}

				// Top middle
				nbrs[1] = _cells[i - 1][j];

				if (j != cols)
				{
					// Top right
					nbrs[2] = _cells[i - 1][j + 1];
				}
			}

			if (j != cols)
			{
				// Middle right
				nbrs[3] = _cells[i][j + 1];
			}
			
			// Bottom
			if (i != rows)
			{
				if (j != cols)
				{
					// Bottom right
					nbrs[4] = _cells[i + 1][j + 1];
				}

				// Bottom middle
				nbrs[5] = _cells[i + 1][j];

				if (j != 0)
				{
					// Bottom left
					nbrs[6] = _cells[i + 1][j - 1];
				}
			}

			if (j != 0)
			{
				// Middle left
				nbrs[7] = _cells[i][j - 1];
			}

			_cells[i][j]->init(nbrs);
		}
	}
}

/**
 * Deconstructor.
 */
Map::~Map()
{
	if (_rows > 0 && _cols > 0)
	{
		for (unsigned int i = 0; i < _rows; i++)
		{
			for (unsigned int j = 0; j < _cols; j++)
			{
				delete _cells[i][j];
			}

			delete[] _cells[i];
		}

		delete[] _cells;
	}
}

/**
 * Retrieve a cell.
 *
 * @param   unsigned int   row
 * @param   unsigned int   column
 * @return  Cell*
 */
Map::Cell* Map::operator()(const unsigned int row, const unsigned int col)
{
	return _cells[row][col];
}

/**
 * Number of cols.
 *
 * @return  unsigned int
 */
unsigned int Map::cols()
{
	return _cols;
}

/**
 * Checks if movement grazes any possible blocked cells.
 *
 * @param   Cell*   start
 * @param   Cell*   destination
 * @return  bool
 */
bool Map::grazes(Cell* a, Cell* b)
{
	int dx = b->x() - a->x();
	int dy = b->y() - a->y();

	// Not considered grazing if not a diagonal move
	if ((abs(dx) + abs(dy)) == 1)
		return false;

	// Verify that it is in the bounds of the map
	if ( ( ! has(a->y(), a->x() + dx)) ||  ( ! has(a->y() + dy, a->x())))
		return true;

	return (_cells[a->y()][a->x() + dx]->cost == Cell::COST_UNWALKABLE || _cells[a->y() + dy][a->x()]->cost == Cell::COST_UNWALKABLE);
}

/**
 * Checks if row/col exists.
 *
 * @param   unsigned int   row
 * @param   unsigned int   col
 * @return  bool
 */
bool Map::has(unsigned int row, unsigned int col)
{
	return (row >= 0 && row < _rows && col >= 0 && col < _cols);
}

/**
 * Number of rows.
 *
 * @return  unsigned int
 */
unsigned int Map::rows()
{
	return _rows;
}

/**
 * Default constructor.
 */
Map::Cell::Cell()
{
	_init = false;

	_nbrs = NULL;

	_x = 0;
	_y = 0;
}

/**
 * Constructor.
 *
 * @param   unsigned int   x-coordinate
 * @param   unsigned int   y-coordinate
 */				
Map::Cell::Cell(unsigned int x, unsigned int y)
{
	_init = false;

	_nbrs = NULL;

	_x = x;
	_y = y;
}

/**
 * Deconstructor.
 */
Map::Cell::~Cell()
{
	if (_nbrs != NULL)
		delete[] _nbrs;
}

/**
 * Initialize.
 *
 * @param   Cell**  cell neighbors
 * @return  void
 */
void Map::Cell::init(Cell** nbrs)
{
	if (_init)
		return;

	_init = true;

	_nbrs = nbrs;
}

/**
 * Get x-coordinate.
 *
 * @return  unsigned int
 */
unsigned int Map::Cell::x()
{
	return _x;
}

/**
 * Get y-coordinate.
 *
 * @return  unsigned int
 */
unsigned int Map::Cell::y()
{
	return _y;
}

/**
 * Hashes cell based on coordinates.
 *
 * @param   Cell*
 * @return  size_t
 */
size_t Map::Cell::Hash::operator()(Cell* c) const
{
	return c->x() + Cell::Hash::C * c->y();
}