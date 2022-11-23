#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform writeonly image2D map;

void main() 
{
	ivec2 imgSize = imageSize(map);
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

	vec4 initialCol = vec4(0.0);

	if (
		   coords.x >= imgSize.x * 3.0 / 7.0 
		&& coords.x <= imgSize.x * 4.0 / 7.0
		&& coords.y >= imgSize.y * 3.0 / 7.0
		&& coords.y <= imgSize.y * 4.0 / 7.0
		)
	{
		initialCol.g = 1.0;
	}
	initialCol.r = 1.0;

	imageStore(map, ivec2(gl_GlobalInvocationID.xy), initialCol);
}