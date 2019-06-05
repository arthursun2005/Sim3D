layout (location = 0) out vec3 vel;

uniform sampler3D V;
uniform sampler3D U;
uniform vec3 invSize;

in float depth;

void main() {
    vec3 p = vec3(gl_FragCoord.xy, depth);
    vec3 vel0 = texelFetch(V, ivec3(p), 0).xyz;
    float n = texture(U, p * invSize).x;
    
    float x = texture(U, (p - vec3(1.0f, 0.0f, 0.0f)) * invSize).x;
    float y = texture(U, (p - vec3(0.0f, 1.0f, 0.0f)) * invSize).x;
    float z = texture(U, (p - vec3(0.0f, 0.0f, 1.0f)) * invSize).x;
    
    vel0 -= n - vec3(x, y, z);
    vel = vel0;
}
