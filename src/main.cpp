#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"


//update by hurongping

/**
 * Main.
 *
 * @param   int      number of arguments
 * @param   char**   arguments
 * @return  int
 */
int main(int argc, char **argv)
{
	// Make sure we have the minimum number of arguments
	if (argc != 9)
	{
		printf("Not enough arguments: %d", argc);
		throw;
	}

	Simulator::Config config = Simulator::Config();

	// Set bitmap file locations
	config.real_bitmap = argv[2];
	config.robot_bitmap = argv[3];

	// Start position (x, y)
	config.start.first = atoi(argv[4]);
	config.start.second = atoi(argv[5]);

	// Goal position (x, y)
	config.goal.first = atoi(argv[6]);
	config.goal.second = atoi(argv[7]);

	// Robot scan radius
	config.scan_radius = atoi(argv[8]);

	// Build the simulator and draw
	Simulator sim = Simulator(argv[1], config);
	sim.draw();

	return 0;
}
