#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int text;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 2) uniform sampler2D tex;

void main()
{

    if (text == 1)
        outColor = texture(tex, fragTexCoord) * vec4(fragColor, 1.0);
    else
        outColor = vec4(fragColor, 1.0);
}