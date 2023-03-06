#pragma once

#include "Simulation.hpp"

class ReactionDiffusion : public Simulation
{
public:
	ReactionDiffusion(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
	~ReactionDiffusion();

	void run() override;

private:

	enum class DisplayMode
	{
		AB, GREYSCALE, DELTA1, DELTA2
	};

	std::unique_ptr<ComputeShader> m_textureInitComputeProgram;

	DisplayMode m_displayMode{ DisplayMode::AB };
};

