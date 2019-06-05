layout (location = 0) in vec2 uv;

uniform sampler2D T;
uniform vec3 scl;

uniform int z;

out vs {
    int depth;
    float weight;
} vs_out;

void main() {
    gl_PointSize = 1.0f;
    vec3 p = texture(T, uv).xyz;
    float d = clamp(0.5f * (p.z + scl.z) - 0.5f, 0.0f, scl.z - 1.0f);
    int di = int(floor(d));
    int ds[] = int[](di, di + 1);
    vs_out.depth = ds[z];
    vs_out.weight = 1.0f - abs(d - vs_out.depth);
    gl_Position = vec4(p.xy * scl.xy, 0.0f, 1.0f);
}
