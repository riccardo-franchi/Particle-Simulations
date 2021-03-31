#version 460 core

#define TWO_PI 6.2831853

struct Agent
{
    float x, y;
    float angle;
};

layout (local_size_x = 64, local_size_y = 1) in;
layout (rgba32f) uniform image2D imgOutput;
layout (std430, binding = 2) buffer Agents
{
    Agent agents[];
};

uniform float deltaTime;

const ivec2 imgSize = imageSize(imgOutput);

const float agentSpeed = 180.0;
const float turnSpeed = 100.0;
const float sensorOffsetDst = 50.0;
const float sensorAngleSpacing = 0.1;
const int sensorSize = 3;

uint hash(uint state) 
{
    state ^= 2747636419;
    state *= 2654435769;
    state ^= state >> 16;
    state *= 2654435769;
    state ^= state >> 16;
    state *= 2654435769;
    return state;
}

float scaleRantomTo01(uint random)
{
    return float(random) / 4294967295.0;
}

float sense(Agent agent, float sensorAngleOffset)
{
    float sensorAngle = agent.angle + sensorAngleOffset;
    vec2 sensorDir = vec2(cos(sensorAngle), sin(sensorAngle));
    ivec2 sensorCenter = ivec2(vec2(agent.x, agent.y) + sensorDir * sensorOffsetDst);
    float sum = 0.0;

    for (int x_off = -sensorSize; x_off <= sensorSize; x_off++)
        for (int y_off = -sensorSize; y_off <= sensorSize; y_off++)
        {
            ivec2 pos = sensorCenter + ivec2(x_off, y_off);
            if (pos.x >= 0 && pos.x < imgSize.x && pos.y >= 0 && pos.y < imgSize.y)
                sum += imageLoad(imgOutput, pos).r;
        }

        return sum;
}

void main() 
{
    uint i = gl_GlobalInvocationID.x;
    Agent currAgent = agents[i];

    uint random = hash(uint(currAgent.y * imgSize.y + currAgent.x + hash(i)));

    vec2 position = vec2(currAgent.x, currAgent.y);
    vec2 direction = vec2(cos(currAgent.angle), sin(currAgent.angle));

    position += direction * agentSpeed * deltaTime;

    if (position.x < 0 || position.x >= imgSize.x || position.y < 0 || position.y >= imgSize.y)
    {
        position.x = min(imgSize.x - 0.01, max(0, position.x));
        position.y = min(imgSize.y - 0.01, max(0, position.y));
        agents[i].angle = scaleRantomTo01(random) * TWO_PI;
    }

    float weightForward = sense(currAgent, 0);
    float weightLeft = sense(currAgent, sensorAngleSpacing);
    float weightRight = sense(currAgent, -sensorAngleSpacing);

    float randomSteerStrength = scaleRantomTo01(random);

    if (weightForward > weightLeft && weightForward > weightRight) {}

    else if (weightForward < weightLeft && weightForward < weightRight)
        agents[i].angle += (randomSteerStrength - 0.5) * 2 * turnSpeed * deltaTime;

    else if (weightRight > weightLeft)
        agents[i].angle -= randomSteerStrength * turnSpeed * deltaTime;

    else if (weightLeft > weightRight)
        agents[i].angle += randomSteerStrength * turnSpeed * deltaTime;


    agents[i].x = position.x;
    agents[i].y = position.y;

    imageStore(imgOutput, ivec2(position), vec4(1.0));
}