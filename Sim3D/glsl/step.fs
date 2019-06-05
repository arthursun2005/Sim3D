layout (location = 0) out vec3 pos;

uniform sampler2D P;
uniform sampler3D V;

uniform float dt;
uniform vec3 size;

void main() {
    vec3 pos0 = texelFetch(P, ivec2(gl_FragCoord.xy), 0).xyz;
    vec3 vel0 = get(V, pos0, size);
    vec3 vel1 = get(V, pos0 + vel0 * dt * 0.5f, size);
    pos = clamp(pos0 + vel1 * dt, -size, size);
}
