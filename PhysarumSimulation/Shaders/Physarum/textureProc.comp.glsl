#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f) uniform readonly image2D imgInput;
layout (rgba32f) uniform writeonly image2D imgOutput;

uniform float deltaTime;

const float evaporateSpeed = 0.9;
const float diffuseSpeed = 20.0;

void main()
{
    ivec2 imgSize = imageSize(imgInput);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 originalValue = imageLoad(imgInput, pixelCoords);

    vec4 sum = vec4(0.0);
    for (int x_off = -1; x_off <= 1; x_off++)
        for (int y_off = -1; y_off <= 1; y_off++)
        {
            ivec2 sampleCoords = ivec2(pixelCoords.x + x_off, pixelCoords.y + y_off);
            if (sampleCoords.x >= 0 && sampleCoords.x < imgSize.x && sampleCoords.y >= 0 && sampleCoords.y < imgSize.y)
                sum += imageLoad(imgInput, sampleCoords);
        }

    vec4 blurred = sum / 9.0;

    vec4 diffusedValue = mix(originalValue, blurred, diffuseSpeed * deltaTime);
    vec4 diffusedEvaporatedValue = diffusedValue - evaporateSpeed * deltaTime;

    vec4 processedValue = vec4(diffusedEvaporatedValue.r - 0.015, diffusedEvaporatedValue.g - 0.05, diffusedEvaporatedValue.b -0.1, 1.0);

    imageStore(imgOutput, pixelCoords, processedValue);
}