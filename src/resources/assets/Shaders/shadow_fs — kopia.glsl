
layout(location = 0) out vec4 color;

in vec2 o_texCoord;
in vec3 o_normals;

uniform sampler2D u_tex;

void main() {
    vec3 n = normalize(o_normals);

    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = dot(n, lightDir);

    vec3 ambient = vec3(0.4); 
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 texColor = texture(u_tex, o_texCoord).rgb;

    vec3 finalColor = texColor * (ambient + diff * lightColor * 0.45);
    finalColor = clamp(finalColor, 0.0, 1.0);

    color = vec4(finalColor, 1.0);
}