layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normals;

out vec2 o_uv;
out vec3 o_normals;
out vec3 o_pos;
out vec4 o_posLightSpace; 

uniform mat4 u_mvp;
uniform mat3 u_normal_mvp;
uniform mat4 u_model;
uniform mat4 u_lightSpaceMatrix; 

void main() {
    gl_Position = u_mvp * position;
    
    o_uv = uv;
    o_normals = u_normal_mvp * normals;
    o_pos = vec3(u_model * vec4(position.xyz, 1.0));
    
    o_posLightSpace = u_lightSpaceMatrix * vec4(o_pos, 1.0);
}