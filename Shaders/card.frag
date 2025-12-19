#version 460 core

in vec2 vTexCoord;
in vec4 vTexRect;

out vec4 FragColor;

uniform sampler2D uCardTextures;

void main()
{
    // Apply per-card UV offset and scale from the atlas
    vec2 uv = vTexCoord * vTexRect.zw + vTexRect.xy;

    vec3 texColor = texture(uCardTextures, uv).rgb;

    // Rounded corners
    vec2 cornerUV = abs(vTexCoord - 0.5) * 2.0;  // [0,1] -> [-1,1] centered
    vec2 cornerDist = max(cornerUV - vec2(0.8), 0.0);
    float dist = length(cornerDist);
    if (dist > 0.2) discard;

    FragColor = vec4(texColor, 1.0);
}
