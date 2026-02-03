
layout(location = 0) out vec4 color;

in vec2 o_texCoord;
in vec3 o_normals;
in vec3 o_pos;

uniform sampler2D u_tex;

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std430, binding = 0) readonly buffer LightBuffer {
    PointLight lights[];
};

uniform int u_lightCount;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightPos   = light.position.xyz;
    float radius    = light.position.w;
    vec3 lightColor = light.color.rgb;
    float intensity = light.color.w;

    vec3 lightVec = lightPos - fragPos;
    float dist = length(lightVec);

    if(dist > radius) {
        return vec3(0.0);
    }

    vec3 lightDir = normalize(lightVec);

    float diff = max(dot(normal, lightDir), 0.0);

    float attenuation = clamp(1.0 - dist / radius, 0.0, 1.0);

    return lightColor * intensity * diff * attenuation;
}

void main() {
    vec4 texColor = texture(u_tex, o_texCoord);
    vec3 norm = normalize(o_normals);
    
    vec3 lighting = vec3(0.0);

    for(int i = 0; i < u_lightCount; i++) {
        lighting += CalculatePointLight(lights[i], norm, o_pos);
    }

    vec3 ambient = vec3(0.05) * texColor.rgb;
    
    vec3 finalColor = ambient + (lighting * texColor.rgb);
    color = vec4(finalColor, 1.0);
}