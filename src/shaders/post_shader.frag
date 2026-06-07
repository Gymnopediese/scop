

#version 450

layout(location = 0) in vec2 uv;
layout(set = 0, binding = 0) uniform sampler2D screen_texture;

layout(location = 0) out vec4 COLOR;

void main()
{
    vec2 resolution = vec2(textureSize(screen_texture, 0));

    float pixelSize = 16.0;

    vec2 pixelUV = floor(uv * resolution / pixelSize) * pixelSize / resolution;

    COLOR = texture(screen_texture, pixelUV);
}