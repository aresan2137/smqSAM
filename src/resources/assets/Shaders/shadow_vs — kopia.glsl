
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec2 o_texCoord;
out vec3 o_normals;

uniform mat4 u_mvp;
uniform mat3 u_normal_mvp;

void main() {
    
    gl_Position = u_mvp * position;
    o_texCoord = texCoord;
    o_normals = u_normal_mvp * normals;
}