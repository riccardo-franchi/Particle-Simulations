#pragma once

#include "PhysarumAgents.hpp"
#include "Simulation.hpp"

class PhysarumSimulation : public Simulation
{
public:
	PhysarumSimulation(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
	~PhysarumSimulation();

	void run() override;

private:

	const size_t NUM_AGENTS;

	PhysarumAgentSystem m_agentSystem;
};
