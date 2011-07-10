#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

int main(int argc, char **argv)
{
	if (argc != 9)
		throw;

	Simulator::Config config = Simulator::Config();

	config.real_bitmap = argv[2];
	config.robot_bitmap = argv[3];

	config.start.first = atoi(argv[4]);
	config.start.second = atoi(argv[5]);

	config.goal.first = atoi(argv[6]);
	config.goal.second = atoi(argv[7]);

	config.scan_radius = atoi(argv[8]);

	Simulator sim = Simulator(argv[1], config);

	sim.draw();

	return 0;
}