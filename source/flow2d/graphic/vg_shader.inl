const static char* vg_vertex = 
"#version 330 core\n"
"uniform mat3 transform;\n"
"layout(location = 0) in vec2 in_position;\n"
"out vec2 position;\n"
"void main() {\n"
"  position = in_position;\n"
"  gl_Position = vec4((transform * vec3(in_position, 1.0f)).xy, 0.f, 1.f);\n"
"}\n";

const static char* vg_fragment =
"#version 330 core\n"
"uniform mat3  paint;\n"
"uniform vec4  inner_color;\n"
"uniform vec4  outer_color;\n"
"uniform float radius;\n"
"uniform float feather;\n"
"uniform int   operations;\n"
"in vec2 position;\n"
"out vec4 color;\n"
"\n"
"void main(void)\n"
"{\n"
"    int  firstPass = operations & 0xF;\n"
"    int  secondPass = operations & 0xF0;\n"
"\n"
"    if ( firstPass == 0 ) // solid color\n"
"    {\n"
"        color = inner_color;\n"
"    }\n"
"    else if( firstPass == 1 ) // linear gradient\n"
"    {\n"
"        vec2 pt = (paint*vec3(position, 1.0f)).xy;\n"
"        color = mix( inner_color, outer_color, clamp(dot(pt, vec2(radius, feather)), 0, 1) ); \n"
"    }\n"
"    else if( firstPass == 2 ) // radial gradient\n"
"    {\n"
"        vec2 pt = (paint*vec3(position, 1.0f)).xy;\n"
"        float ratio = (length(pt) - radius) / feather;\n"
"        color = mix( inner_color, outer_color, clamp(ratio, 0, 1) );\n"
"    }\n"
"\n"
"    if( secondPass == 16 )\n"
"    {\n"
"        float gray = dot( color.rgb, vec3(0.3, 0.59, 0.11) );\n"
"        color.rgb = vec3(gray, gray, gray);\n"
"    }\n"
"}\n";

const static char* vg_uniforms[] = {
    "transform", "paint", "inner_color", "outer_color", "radius", "feather", "operations"
};

// void main(void)
// {
//     int  firstPass = operations & 0xF;
//     int  secondPass = operations & 0xF0;

//     if ( firstPass == 0 ) // solid color
//     {
//         color = inner_color;
//     }
//     else if( firstPass == 1 ) // linear gradient
//     {
//         vec2 pt = (paint*vec3(position, 1.0f)).xy;
//         color = mix( inner_color, outer_color, clamp(dot(pt, vec2(radius, feather)), 0, 1) ); 
//     }
//     else if( firstPass == 2 ) // radial gradient
//     {
//         vec2 pt = (paint*vec3(position, 1.0f)).xy;
//         float ratio = (length(pt) - radius) / feather;
//         color = mix( inner_color, outer_color, clamp(ratio, 0, 1) );
//     }

//     if( secondPass == 1 )
//     {
//         float gray = dot( color.rgb, float3(0.3, 0.59, 0.11) );
//         color.rgb = vec3(gray, gray, gray);
//     }
// }