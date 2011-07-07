
#include "simulator.h"

int main(int argc, char **argv)
{
	Simulator::Config config = Simulator::Config();

	config.bitmap_real = "C:/Users/sysadmin/Pictures/test03.bmp";
	config.bitmap_robot = "C:/Users/sysadmin/Pictures/test03.bmp";

	config.start.first = 2;
	config.start.second = 2;

	config.goal.first = 50;
	config.goal.second = 50;

	config.scan_radius = 20;

	Simulator sim = Simulator("Simulator 1", config);

	//int result = sim.execute();
	sim.draw();

	return 0;
}