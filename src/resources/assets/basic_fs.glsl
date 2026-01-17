
layout(location = 0) out vec4 color;

in vec3 o_normals;
in vec2 o_texCoord;

uniform sampler2D u_tex;

void main() {
    vec3 norm = normalize(o_normals);

    vec3 lightDir = normalize(vec3(0.3f, 1.0f, 0.2f));

    vec3 lightColor = vec3(1.0f, 0.9f, 0.6f);

    vec3 ambient = vec3(0.2f, 0.18f, 0.12f);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 texColor = texture(u_tex, o_texCoord).rgb;

    vec3 finalColor = texColor * (ambient + diff * lightColor);

    color = vec4(finalColor, 1.0f);
}