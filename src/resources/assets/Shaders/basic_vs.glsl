
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normals;

out vec2 o_uv;

uniform mat4 u_mvp;
uniform mat3 u_normal_mvp;
uniform mat4 u_model;

void main() {
    gl_Position = u_mvp * position;
    o_uv = uv;
}