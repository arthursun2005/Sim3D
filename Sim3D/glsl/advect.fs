layout (location = 0) out vec3 A;

uniform sampler3D V;
uniform sampler3D T;
uniform float dt;
uniform vec3 invSize;

in float depth;

void main() {
    vec3 p = vec3(gl_FragCoord.xy, depth);
    vec3 v = texelFetch(V, ivec3(p), 0).xyz;
    vec3 size = 1.0f/invSize;
    A = texture(T, (p - v * dt) * invSize).xyz;
}
