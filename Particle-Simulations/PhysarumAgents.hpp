#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <numbers>

constexpr float TWO_PI{ 2 * static_cast<float>(std::numbers::pi) };

class PhysarumAgentSystem
{
public:

	struct Agent
	{
		float x, y;
		float angle;
		int32_t speciesMask;
	};

	enum class PositionMode
	{
		RANDOM, CIRCLE, CENTER
	};

	PhysarumAgentSystem() : m_numAgents(0) {}
	PhysarumAgentSystem(int width, int height, size_t numAgents, int8_t numSpecies, PositionMode posMode = PositionMode::RANDOM)
		: m_numAgents(numAgents)
	{
		if (numSpecies > 3) numSpecies = 3;
		else if (numSpecies < -1) numSpecies = -1;

		std::default_random_engine randGen;
		std::uniform_real_distribution<float> position_x(0.0f, (float)width);
		std::uniform_real_distribution<float> position_y(0.0f, (float)height);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);
		std::uniform_int_distribution<int32_t> species(0, numSpecies - 1);

		m_agents.resize(m_numAgents);

		int32_t species_mask{ (numSpecies < 2) ? -1 : species(randGen) };

		switch (posMode)
		{
		case PositionMode::CIRCLE:
		{
			const float radius{ std::min(width, height) / 3.0f };
			float pos_x, pos_y;
			
			for (Agent& a : m_agents)
			{
				do
				{
					pos_x = position_x(randGen);
					pos_y = position_y(randGen);
				} while (powf(pos_x - width * 0.5f, 2) + powf(pos_y - height * 0.5f, 2) > (radius * radius));
				a = { pos_x, pos_y, angle(randGen), species_mask};
			}
			break;
		}
		case PositionMode::CENTER:
		{
			float pos_x{ width * 0.5f };
			float pos_y{ height * 0.5f };
			for (Agent& a : m_agents)
			{
				a = { pos_x, pos_y, angle(randGen), species_mask };
			}
			break;
		}
		case PositionMode::RANDOM:
		default:
			for (Agent& a : m_agents)
			{
				a = { position_x(randGen), position_y(randGen), angle(randGen), species_mask };
			}
			break;
		}
	}

	const std::vector<Agent>& getAgents() const { return m_agents; }

private:
	size_t m_numAgents;
	std::vector<Agent> m_agents;
};