#pragma once
#include <vector>
#include <random>
#include <algorithm>

constexpr float TWO_PI = 6.2831853f;

struct Agent
{
	float x, y; 
	float angle;
};

enum class PositionMode
{
	RANDOM, CIRCLE
};

class AgentSystem
{
public:
	AgentSystem() : m_numAgents(0) {}
	AgentSystem(int width, int height, size_t numAgents, PositionMode posMode = PositionMode::RANDOM)
		: m_numAgents(numAgents)
	{
		std::mt19937 randGen;
		std::uniform_real_distribution<float> position_x(0.0f, (float)width);
		std::uniform_real_distribution<float> position_y(0.0f, (float)height);
		std::uniform_real_distribution<float> angle(0.0f, TWO_PI);

		m_agents.resize(m_numAgents);

		switch (posMode)
		{
		case PositionMode::CIRCLE:
		{
			const float radius = std::min(width, height) / 3.0f;
			float pos_x, pos_y;
			for (size_t i = 0; i < numAgents; i++)
			{
				do
				{
					pos_x = position_x(randGen);
					pos_y = position_y(randGen);
				} while (powf(pos_x - width * 0.5f, 2) + powf(pos_y - height * 0.5f, 2) > (radius * radius));
				m_agents.at(i) = { pos_x, pos_y, angle(randGen) };
			}
			break;
		}
		case PositionMode::RANDOM:
		default:
			for (size_t i = 0; i < numAgents; i++)
			{
				m_agents.at(i) = { position_x(randGen), position_y(randGen), angle(randGen) };
			}
			break;
		}
	}

	const std::vector<Agent>& getAgents() const { return m_agents; }

private:
	size_t m_numAgents;
	std::vector<Agent> m_agents;
};