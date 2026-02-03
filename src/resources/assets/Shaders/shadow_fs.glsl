layout(location = 0) out vec4 color;

in vec2 o_uv;
in vec3 o_normals;
in vec3 o_pos;
in vec4 o_posLightSpace; 

uniform sampler2D u_tex;
uniform sampler2D u_shadowMap;

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std430, binding = 0) readonly buffer LightBuffer {
    PointLight lights[];
};

uniform int u_lightCount;

float CalculateShadow(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(u_shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(o_normals);
    vec3 lightDir = normalize(vec3(0.0, 100.0, 0.0) - o_pos); 
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 

    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, float shadow) {
    vec3 lightPos   = light.position.xyz;
    float radius    = light.position.w;
    vec3 lightColor = light.color.rgb;
    float intensity = light.color.w;

    vec3 lightVec = lightPos - fragPos;
    float dist = length(lightVec);

    if(dist > radius) return vec3(0.0);

    vec3 lightDir = normalize(lightVec);
    float diff = max(dot(normal, lightDir), 0.0);
    float attenuation = clamp(1.0 - dist / radius, 0.0, 1.0);

    return lightColor * intensity * diff * attenuation * (1.0 - shadow);
}

void main() {
    vec4 texColor = texture(u_tex, o_uv);
    vec3 norm = normalize(o_normals);
    
    float shadow = CalculateShadow(o_posLightSpace);

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < u_lightCount; i++) {
        float s = (i == 0) ? shadow : 0.0; 
        lighting += CalculatePointLight(lights[i], norm, o_pos, s);
    }

    vec3 ambient = vec3(0.05) * texColor.rgb;
    vec3 finalColor = ambient + (lighting * texColor.rgb);
    color = vec4(finalColor, 1.0);
}