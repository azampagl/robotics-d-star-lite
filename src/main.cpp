#include <stdio.h>

#include "simulator.h"

int main(int argc, char **argv)
{
	//FILE * log = fopen("log.txt","w");
	Simulator::Config config = Simulator::Config();

	config.real_bitmap = "../../../../maps/map-01-real.bmp";
	//config.robot_bitmap = "../../../../maps/map-01-robot.bmp";
	config.robot_bitmap = "../../../../maps/map-01-real.bmp";

	config.start.first = 0;
	config.start.second = 0;

	config.goal.first = 499;
	config.goal.second = 499;

	//config.goal.first = 19;
	//config.goal.second = 19;

	config.scan_radius = 20;

	Simulator sim = Simulator("Simulator 1", config);

	sim.draw();

	/*Map map(10, 10);

	Map::Cell* start = map(0, 0);
	Map::Cell* goal = map(9, 9);

	Planner planner(&map, start, goal);

	planner.replan();

	list<Map::Cell*> path = planner.path();

	for (list<Map::Cell*>::iterator i = path.begin(); i != path.end(); i++)
	{
		fprintf(log, "%d, %d\n", (*i)->x(), (*i)->y());
	}

	for (int i = 0; i < map.rows(); i++)
	{
		for (int j = 0; j < map.cols(); j++)
		{
			fprintf(log, "%.2f\n", map(i, j)->cost);
		}
	}

	fclose(log);*/
	return 0;
}