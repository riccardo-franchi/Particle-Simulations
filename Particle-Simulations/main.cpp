#include "PhysarumSimulation.hpp"
#include "ReactionDiffusion.hpp"

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

constexpr int HEIGHT{ 720 };
constexpr int WIDTH{ HEIGHT * 16 / 10 };

int main()
{
	auto simulation = std::make_unique<PhysarumSimulation>(WIDTH, HEIGHT, 1, true);
	simulation->run();

	return 0;
}