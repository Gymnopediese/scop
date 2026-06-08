#version 450

layout(location = 0) in vec2 UV;
layout(set = 0, binding = 0) uniform sampler2D screen_texture;

layout(location = 0) out vec4 COLOR;

// void main() {
//     float cshift = 1;
//     float intensity = 2;
//     // 获取屏幕分辨率
//     vec2 iResolution = vec2(textureSize(screen_texture, 0));
//     // 基础UV坐标（适配Godot坐标系）
//     vec2 uv = UV;
//     uv.y = 1.0 - uv.y; // 翻转Y轴

//     // 宽高比计算
//     float aspectRatio = iResolution.x / iResolution.y;

//     // 计算强度参数
//     float strength = intensity * 0.03;
//     vec2 tintensity = vec2(
//         strength * aspectRatio,
//         strength * aspectRatio
//     );

//     // 坐标转换到[-1, 1]范围
//     vec2 coords = uv;
//     coords = (coords - 0.5) * 2.0;

//     // 计算坐标偏移量
//     vec2 realCoordOffs;
//     realCoordOffs.x = (1.0 - coords.y * coords.y) * tintensity.y * coords.x;
//     realCoordOffs.y = (1.0 - coords.x * coords.x) * tintensity.x * coords.y;

//     // 应用偏移
//     vec2 fuv = uv - realCoordOffs;
//     // 边界检查
//     if(fuv.x < 0.0 || fuv.x > 1.0 || fuv.y < 0.0 || fuv.y > 1.0) {
//         COLOR = vec4(0.0); // 超出范围显示黑
// 		vec4 color = texture(screen_texture,fuv);
//         if (cshift == 1)
// 		COLOR.rgb=mix(color.rgb,color.bgr,length(fuv-0.5));
//     } else {
//         // 采样时再次翻转Y轴适配Godot坐标系
//         fuv.y = 1.0 - fuv.y;
//         COLOR = texture(screen_texture, fuv);
// 		vec4 color = texture(screen_texture,fuv);
//         if (cshift == 1)
// 		COLOR.rgb=mix(color.rgb,color.bgr,length(fuv-0.5));
//     }
// }

// #version 450

// layout(location = 0) in vec2 uv;
// layout(set = 0, binding = 0) uniform sampler2D screen_texture;

// layout(location = 0) out vec4 COLOR;

void main()
{

    // COLOR = texture(screen_texture, uv);
    vec2 resolution = vec2(textureSize(screen_texture, 0)) ;

    float pixelSize = 1.0;

    vec2 pixelUV = floor((UV * resolution) / pixelSize) * pixelSize / resolution;

    pixelUV = clamp(pixelUV, pixelSize / resolution, vec2(1, 1));
    COLOR = texture(screen_texture, pixelUV);
    // COLOR = vec4(pixelUV, 0, 1);
}