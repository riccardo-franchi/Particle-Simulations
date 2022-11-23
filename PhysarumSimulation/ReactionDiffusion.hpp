#pragma once

#include "Simulation.hpp"

class ReactionDiffusion : public Simulation
{
public:
	ReactionDiffusion(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
	~ReactionDiffusion();

	void run() override;

private:
	std::unique_ptr<ComputeShader> m_textureInitComputeProgram;
};

