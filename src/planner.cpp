/**
 * DStarLite.
 *
 * @package		DStarLite
 * @author		Aaron Zampaglione <azampagl@gmail.com>
 * @copyright	Copyright (C) 2011 Aaron Zampaglione
 * @license		MIT
 */
#include "planner.h"

using namespace DStarLite;

/*
 * @var  static const double  max steps before assuming no path possible
 */
const double Planner::MAX_STEPS = 1000000;

/**
 *
 */
Planner::Planner(Map* map, Map::Cell* start, Map::Cell* goal)
{
	_open_list.clear();
	_open_hash.clear();
	_path.clear();
	
	_km = 0;

	_map = map;
	_start = start;
	_goal = goal;
	_last = _start;

	_g(_start, Math::INFINITY);
	_rhs(_start, 0.0);

	_g(_goal, Math::INFINITY);
	_rhs(_goal, Math::INFINITY);

	_list_insert(_start, _k(_start));
}

/**
 * Deconstructor.
 */
Planner::~Planner()
{
}

/**
 * Returns the generated path.
 *
 * @return  list<Map::Cell*>
 */
list<Map::Cell*> Planner::path()
{
	return _path;
}

/**
 * Gets/Sets a new goal.
 *
 * @param   Map::Cell*
 * @return  Map::Cell*
 */
Map::Cell* Planner::goal(Map::Cell* u)
{
	if (u == NULL)
		return _goal;

	// Hack implementation
	_goal = u;

	return _goal;
}

/**
 * Replan.
 *
 * @return  bool   solution found
 */
/*bool DStarLite::replan()
{
	_path.clear();
	
	int result = _compute();
	
	if (result < 0)
	  return false;

	list<Tile*> succ;
	list<Tile*>::iterator i;
	Tile* min_t;
	double min_cost, min_dist, cost, dist;

	Tile* cur = _start;

	while(cur != _goal)
	{
		if (_g(cur) == Math::INFINITY)
			return false;

		_path.push_back(cur);

		_succ(cur, succ);

		min_t = NULL;
		min_cost = Math::INFINITY;
		min_dist = Math::INFINITY;
		
		for (i = succ.begin(); i != succ.end(); i++)
		{
			cost = _cost(cur, *i) + _g(*i);
			dist = (*i)->euclidean_distance(*_goal) + _start->euclidean_distance(*(*i));
			if (Math::close(min_cost, cost))
			{
				if (dist < min_dist)
				{
					min_t = *i;
					min_cost = cost;
					min_dist = dist;
				}
			}
			else if (cost < min_cost)
			{
				min_t = *i;
				min_cost = cost;
				min_dist = dist;
			}
		}

		if (min_t == NULL)
			return false;

		cur = min_t;
	}

	_path.push_back(_goal);

	return true;
}*/

/**
 * Gets/Sets a new start.
 *
 * @param   Map::Cell*
 * @return  Map::Cell*
 */
Map::Cell* Planner::start(Map::Cell* u)
{
	if (u == NULL)
		return _start;

	_start = u;

	return _start;
}

/**
 *
 */
void Planner::update(Map::Cell* u, double cost)
{
	if (u == _goal)
		return;

	// Update km
	_km += _h(_last, _start);
	_last = _start;

	_cell(u);
	u->cost = cost;

	_update(u);
}

/**
 *
 */
void Planner::_cell(Map::Cell* u)
{
	if (_cell_hash.find(u) != _cell_hash.end())
		return;
	
	double h = Math::INFINITY;
	_cell_hash[u] = pair<double,double>(h, h);
}

/**
 *
 */
bool Planner::_compute()
{
	if (_open_list.empty())
		return -1;

	Map::Cell* u;
	pair<double,double> k_old;
	pair<double,double> k_new;

	int attempts = 0;

	while (((! _open_list.empty()) && (_open_list.begin()->first < _k(_start))) || (_rhs(_start) != _g(_start)))
	{
		// Reached max steps, quit
		if (++attempts > Planner::MAX_STEPS)
			return false;

		u = _open_list.begin()->second;
		k_old = _open_list.begin()->first;
		k_new = _k(u);

		_list_remove(_open_list.begin()->second);

		if (k_old < k_new)
		{
			_list_insert(u, k_new);
		}
		else if (_g(u) > _rhs(u))
		{
			_g(u, _rhs(u));

			_pred(u, pred);
			for (i = pred.begin(); i != pred.end(); i++)
			{
				_update(*i);
			}
		}
		else
		{
			_g(u, Math::INFINITY);

			_pred(u, pred);
			pred.push_back(u);

			for (i = pred.begin(); i != pred.end(); i++)
			{
				_update(*i);
			}
		}
	}

	return 0;
}

/**
 *
 */
double Planner::_cost(Map::Cell* a, Map::Cell* b)
{
	unsigned int dx = abs(a->x() - b->x());
	unsigned int dy = abs(a->y() - b->y());
	double scale = 1.0;

	if ((dx + dy) > 1)
	{
		scale = Math::SQRT2;
	}

	return scale * ((a->cost + b->cost) / 2);
}

/**
 *
 */
double Planner::_g(Map::Cell* u, double value)
{
	_cell(u);
	pair<double,double>* g_rhs = &_cell_hash[u];

	if (value != DBL_MIN)
	{
		g_rhs->first = value;
	}

	return g_rhs->first;
}

/**
 *
 */
double Planner::_h(Map::Cell* a, Map::Cell* b)
{
	int x = a->x() - b->x();
	int y = a->y() - b->y();
	
	return sqrt(x*x + y*y);
}

/**
 *
 */
pair<double,double> Planner::_k(Map::Cell* u)
{
	double min_c = min(_g(u), _rhs(u));
	return pair<double,double>((min_c + _h(_start, u) + _km), min_c);
}

/**
 *
 */
void Planner::_list_insert(Map::Cell* u, pair<double,double> k)
{
	OL::iterator pos = _open_list.insert(OL_PAIR(k, u));
	_open_hash[u] = pos;
}

/**
 *
 */
void Planner::_list_remove(Map::Cell* u)
{
	_open_list.erase(_open_hash[u]);
	_open_hash.erase(_open_hash.find(u));
}

/**
 *
 */
double Planner::_rhs(Map::Cell* u, double value)
{
	if (u == _goal)
		return 0;

	_cell(u);
	pair<double,double>* g_rhs = &_cell_hash[u];

	if (value != DBL_MIN)
	{
		g_rhs->second = value;
	}
	
	return g_rhs->second;
}

/**
 * Updates tile.
 *
 * @param   Tile*
 * @return  void
 */
/*void DStarLite::_update(Tile* u)
{
	if (u != _goal)
	{
		list<Tile*> succ;
		_succ(u, succ);

		double min_rhs = Math::INFINITY;
		double rhs;
		
		for (list<Tile*>::iterator i = succ.begin(); i != succ.end(); i++)
		{
			rhs = _cost(u, *i) + _g(*i);
			if (min_rhs > rhs)
			{
				min_rhs = rhs;
			}
		}

		_rhs(u, min_rhs);
	}
	if (_open_hash.find(u) != _open_hash.end())
	{
		_list_remove(u);
	}
	if (_g(u) != _rhs(u))
	{
		_list_insert(u, _k(u));
	}
}*/

/**
 * Key compare function.
 */
bool Planner::KeyCompare::operator()(const pair<double,double>& p1, const pair<double,double>& p2) const
{
	if (p1.first + 0.000000001 < p2.first) return true;
	else if (p1.first - 0.000000001 > p2.first) return false;
	else if (p1.second + 0.000000001 < p2.second) return true;
	else if (p1.second - 0.000000001 > p2.second) return false;
	return false;
}