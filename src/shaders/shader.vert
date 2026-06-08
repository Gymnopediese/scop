#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 1) uniform Transform3D {
    mat4 transform;
    int  has_texture;
    int  shadow;
} transform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) flat out int text;

void main()
{
    gl_Position = ubo.proj * ubo.view * ubo.model * transform.transform * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
    text = transform.has_texture;
    fragColor = inColor;

    // if (transform.shadow == 1) fragColor = fragColor / max(0.3, gl_Position.z / 10.);
}