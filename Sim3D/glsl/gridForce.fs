layout (location = 0) out vec3 vel;

uniform sampler3D V;
uniform vec3 exf;
uniform float dt;
uniform vec3 size;

in float depth;

void main() {
    vec3 pos = vec3(gl_FragCoord.xy, depth);
    vec3 v = texelFetch(V, ivec3(pos), 0).xyz + exf * dt;
    pos += (v * dt - 1.0f) * 0.5f;
    v.x *= (sign(pos.x - 1.0f) * 0.5f + 0.5f) * (sign(size.x - pos.x - 1.0f) * 0.5f + 0.5f);
    v.y *= (sign(pos.y - 1.0f) * 0.5f + 0.5f) * (sign(size.y - pos.y - 1.0f) * 0.5f + 0.5f);
    v.z *= (sign(pos.z - 1.0f) * 0.5f + 0.5f) * (sign(size.z - pos.z - 1.0f) * 0.5f + 0.5f);
    vel = v;
}
