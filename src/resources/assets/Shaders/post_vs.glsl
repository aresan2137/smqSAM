
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 o_uv;

void main() {
    
    gl_Position = position;
    o_uv = uv;

}