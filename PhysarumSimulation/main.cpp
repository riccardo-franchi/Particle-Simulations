#include "PhysarumSimulation.h"

int main()
{
	auto physarumSimulation = std::make_unique<PhysarumSimulation>(1920, 1080, 0, true);
	physarumSimulation->run();

	return 0;
}