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
const int Map::Cell::CellHash::C = 1000000;

/**
 * Builds a map based on a bitmap.
 *
 * @param   Bitmap*   bitmap
 * @return  Map*
 */
/*Map* Map::factory(Bitmap* bitmap)
{
	unsigned int rows = bitmap->height();
	unsigned int cols = bitmap->width();

	Map* map = new Map(rows, cols);

	unsigned char** data = new unsigned char*[rows];
	for (unsigned int i = 0; i < rows; i++)
	{
		data[i] = new unsigned char[cols];
	}

	bitmap->as_array(data);

	for (unsigned int i = 0; i < rows; i++)
	{
		for (unsigned int j = 0; j < cols; j++)
		{
			if (data[i][j] == Bitmap::MIN_VALUE)
			{
				(*map)(i, j)->cost = Cell::COST_UNWALKABLE;
			}
			else
			{
				(*map)(i, j)->cost = (double) Bitmap::MAX_VALUE - data[i][j] + 1.0;
			}
		}
	}

	for (unsigned int i = 0; i < rows; i++)
	{
		delete[] data[i];
	}

	delete[] data;

	return map;
}*/

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
			_cells[i][j] = new Cell();
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
	int dx = b->x - a->x;
	int dy = b->y - a->y;

	// Not considered grazing if not a diagonal move
	if ((abs(dx) + abs(dy)) == 1)
		return false;

	// Verify that it is in the bounds of the map
	if ( ( ! has(a->y, a->x + dx)) ||  ( ! has(a->y + dy, a->x)))
		return true;

	return (_cells[a->y][a->x + dx]->cost == Cell::COST_UNWALKABLE || _cells[a->y + dy][a->x]->cost == Cell::COST_UNWALKABLE);
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

Map::Cell::Cell()
{
	_init = false;
	_nbrs = NULL;
}

Map::Cell::~Cell()
{
	if (_nbrs != NULL)
		delete[] _nbrs;
}

void Map::Cell::init(Cell** nbrs)
{
	if (_init)
		return;

	_init = true;

	_nbrs = nbrs;
}