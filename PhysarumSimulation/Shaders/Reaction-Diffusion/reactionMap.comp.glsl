#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform image2D map;

const float feedrate = 0.055;
const float removeRate = 0.062;
const float diffuseRateA = 1.0;
const float diffuseRateB = 0.5;
const int diffuseRadius = 5;

void main()
{
	ivec2 idCoords = ivec2(gl_GlobalInvocationID.xy);

	ivec2 dims = imageSize(map);
	int width = dims.x;
	int height = dims.y;

	if (idCoords.x >= width || idCoords.y >= height) { return; }

	vec2 curVals = imageLoad(map, idCoords).xy;
	float a = curVals.x;
	float b = curVals.y;

	vec2 diffuseStrength = vec2(0);
	vec2 sum = vec2(0);

	float weightSum;

	for (int offsetY = -diffuseRadius; offsetY <= diffuseRadius; offsetY++)
	{
		for (int offsetX = -diffuseRadius; offsetX <= diffuseRadius; offsetX++)
		{
			if (offsetX == 0 && offsetY == 0) { continue; }

			ivec2 samplePos = idCoords.xy + ivec2(offsetX, offsetY);
			samplePos = min(ivec2(width - 1, height - 1), max(ivec2(0), samplePos));
			
			float sqrDist = offsetX * offsetX + offsetY * offsetY;

			if (sqrDist <= diffuseRadius * diffuseRadius)
			{
				float weight = 1.0 / sqrt(sqrDist);
				sum += imageLoad(map, samplePos).xy * weight;
				weightSum += weight;
			}
		}
	}

	diffuseStrength = sum / weightSum - curVals;

	float deltaA = (diffuseRateA * diffuseStrength.x - a * b * b + feedrate * (1 - a));
	float newA = a + deltaA;

	float deltaB = (diffuseRateB * diffuseStrength.y + a * b * b - (removeRate + feedrate) * b);
	float newB = b + deltaB;

	vec4 outputVec = vec4(max(0, newA), min(newB, 1), deltaA, deltaB);

	imageStore(map, idCoords, outputVec);
}