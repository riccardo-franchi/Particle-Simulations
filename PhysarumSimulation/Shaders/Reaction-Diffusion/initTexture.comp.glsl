#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform image2D map;

#define POSITION_MODE_CENTER 0
#define POSITION_MODE_RANDOM 1
#define POSITION_MODE_LINES 2
const int bPositionMode = POSITION_MODE_RANDOM;

// www.cs.ubc.ca/~rbridson/docs/schechter-sca08-turbulence.pdf
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

float scaleRandomTo01(uint random)
{
    return float(random) / 4294967295.0;
}

void main() 
{
	const ivec2 imgSize = imageSize(map);
	const ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

	const vec4 a = vec4(1.0, 0.0, 0.0, 0.0);
	const vec4 b = vec4(1.0, 1.0, 0.0, 0.0);

	if (bPositionMode == POSITION_MODE_CENTER)
	{
		const int RADIUS = 20;

		if (pow((coords.x - imgSize.x / 2), 2) + pow((coords.y - imgSize.y / 2), 2) <= pow(RADIUS, 2))
		{
			imageStore(map, coords, b);
		}
		else 
		{
			imageStore(map, coords, a);
		}
	}
	else if (bPositionMode == POSITION_MODE_RANDOM)
	{
		const float RANDOM_THRESHOLD = 0.004;
		const int RADIUS = 4;

		const float val = scaleRandomTo01(hash(uint(coords.y * imgSize.y + coords.x)));
		if (val < RANDOM_THRESHOLD)
		{
			for (int i = -RADIUS; i<= RADIUS; i++)
			{
				imageStore(map, ivec2(coords.x + i, coords.y), b);
			}
		}
		else if (imageLoad(map,coords).g < 0.0005)
		{
			imageStore(map, coords, a);
		}
	}
	else if (bPositionMode == POSITION_MODE_LINES)
	{
		const int N = 600;
		const int X_RADIUS = 2;
		if (coords.x % N == N / 2)
		{
			for (int i = -X_RADIUS; i<=X_RADIUS; i++)
			{
				imageStore(map, ivec2(coords.x + i, coords.y), b);
			}
		}
		else if (imageLoad(map,coords).g < 0.0005)
		{
			imageStore(map, coords, a);
		}
	}
}