#include "PhysarumSimulation.hpp"

#ifdef _WIN32
	#include <Windows.h>
	#ifdef __cplusplus
	extern "C" {
	#endif

		__declspec(dllexport) DWORD NvOptimusEnablement = 1;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

	#ifdef __cplusplus
	}
	#endif
#endif

int main()
{
	auto physarumSimulation = std::make_unique<PhysarumSimulation>(2560, 1600, 1, true);
	physarumSimulation->run();

	return 0;
}