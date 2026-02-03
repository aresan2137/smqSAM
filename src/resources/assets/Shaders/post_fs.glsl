
layout(location = 0) out vec4 color;

in vec2 o_uv;

uniform sampler2D u_tex;
uniform sampler2D u_depth;

void main() {
    //float d = texture(u_depot, o_uv).r;
    //float z = (2.0 * 0.1) / (100.1 - d * (100.0 - 0.1)); 
    //color = vec4(vec3(z), 1.0);
    color = texture(u_tex, o_uv);
}