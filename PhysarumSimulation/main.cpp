#include "PhysarumSimulation.hpp"

int main()
{
	auto physarumSimulation = std::make_unique<PhysarumSimulation>(1920, 1080, 1, true);
	physarumSimulation->run();

	return 0;
}