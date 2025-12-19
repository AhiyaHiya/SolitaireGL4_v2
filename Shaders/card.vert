#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform vec2 uPosition;     // card center in screen pixels
uniform vec2 uSize;         // card size in screen pixels
uniform vec4 uTexRect;      // min_u, min_v, scale_u, scale_v (from JSON)

out vec2 vTexCoord;
out vec4 vTexRect;

void main()
{
    vec2 pos = aPosition.xy * uSize + uPosition;
    gl_Position = uProjection * vec4(pos, 0.0, 1.0);

    vTexCoord = aTexCoord;
    vTexRect = uTexRect;
}
