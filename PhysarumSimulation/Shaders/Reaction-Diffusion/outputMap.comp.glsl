#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform readonly image2D inputMap;
layout (rgba32f, binding = 1) uniform writeonly image2D imgOutput;

uniform int displayMode;

void main()
{
		ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

		vec4 m = imageLoad(inputMap, pixelCoords);
		float a = m.x;
		float b = m.y;
		float deltaA = m.z;
		float deltaB = m.w;

		const float deltaThreshold = 0.0005;
		deltaA = max(0, deltaA - deltaThreshold);

		if (displayMode == 0) 
		{
			imageStore(imgOutput, pixelCoords, m); 
		}
		else if (displayMode == 1)
		{
			imageStore(imgOutput, pixelCoords, vec4(vec3(a-b), 1.0));
		}
		else if (displayMode == 2)
		{
			vec4 col = vec4(deltaA * 350, deltaB * 250, 0, 0) + vec4(0, 0, b, 0);
			col = col.gbra;
			col.g = pow(col.g, 2) * 6;
			col = min(vec4(1.0), max(vec4(0.0), col));
			imageStore(imgOutput, pixelCoords, col);
		}
		else if (displayMode == 3)
		{
			float v = a - b;
			vec4 col = (1 - pow(abs(1 - (v + 1 - max(deltaB, deltaA) * 350)), 0.6)) + vec4(deltaB, 0, 0, 0) * 400; 
			imageStore(imgOutput, pixelCoords, col);
		}
}