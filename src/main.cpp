#include <stdio.h>

#include "simulator.h"

int main(int argc, char **argv)
{
	FILE * log = fopen("log.txt","w");
	/*Simulator::Config config = Simulator::Config();

	config.real_bitmap = "C:/Users/sysadmin/Pictures/test03.bmp";
	config.robot_bitmap = "C:/Users/sysadmin/Pictures/test03.bmp";

	config.start.first = 2;
	config.start.second = 2;

	config.goal.first = 50;
	config.goal.second = 50;

	config.scan_radius = 20;

	Simulator sim = Simulator("Simulator 1", config);

	int result = sim.execute();
	sim.draw();*/

	Map map(10, 10);

	Map::Cell* start = map(0, 0);
	Map::Cell* goal = map(9, 9);

	Planner planner(&map, start, goal);

	planner.replan();

	list<Map::Cell*> path = planner.path();

	for (list<Map::Cell*>::iterator i = path.begin(); i != path.end(); i++)
	{
		fprintf(log, "%d, %d\n", (*i)->x(), (*i)->y());
	}

	/*for (int i = 0; i < map.rows(); i++)
	{
		for (int j = 0; j < map.cols(); j++)
		{
			fprintf(log, "%.2f\n", map(i, j)->cost);
		}
	}*/

	fclose(log);
	return 0;
}