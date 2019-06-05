layout (location = 0) in vec2 uv;

uniform sampler2D P;
uniform sampler2D V;
uniform vec3 size;

uniform int x;
uniform int y;
uniform int z;

uniform int d;

out vs {
    int depth;
    vec3 vel;
    vec3 weight;
} vs_out;

void main() {
    gl_PointSize = 1.0f;
    
    vec3 p = texture(P, uv).xyz + size;
    vec3 v = texture(V, uv).xyz;
    
    float kx[2] = float[](p.x, p.x + 1.0f);
    float ky[2] = float[](p.y, p.y + 1.0f);
    float kz[2] = float[](p.z, p.z + 1.0f);
    float k[2] = float[](0.0f, 1.0f);
    
    float dp = clamp((0.5f * kz[z]) - 0.5f, 0.0f, size.z);
    
    int di = int(floor(dp));
    int ds[] = int[](di, di + 1);
    
    vs_out.depth = ds[d];
    vs_out.weight = (1.0f - abs(dp - vs_out.depth)) * vec3(k[x], k[y], k[z]);
    vs_out.vel = v * vs_out.weight;
    
    gl_Position = vec4(vec2(kx[x], ky[y]) / (size.xy + 1.0f) - 1.0f, 0.0f, 1.0f);
}
