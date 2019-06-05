layout (location = 0) out vec3 vel;

uniform sampler3D V;
uniform sampler3D W;
uniform vec3 invSize;

in float depth;

#define R 3

void main() {
    vec3 p = vec3(gl_FragCoord.xy, depth);
    vec3 coord = p * invSize;
    
    vec3 cv = texelFetch(V, ivec3(p), 0).xyz;
    vec3 cw = texelFetch(W, ivec3(p), 0).xyz;
    
    vec3 qw = 1.0f - sign(cw);
    
    vec3 sum = vec3(0.0f);
    vec3 weight = vec3(0.0f);
    int z = 0;
    for(int x = -R; x <= R; ++x) {
        for(int y = -R; y <= R; ++y) {
            //for(int z = -R; z <= R; ++z) {
                vec3 cd = coord + vec3(x, y, z) * invSize;
                vec3 v = texture(V, cd).xyz;
                vec3 w = texture(W, cd).xyz;
                float dsq = x * x + y * y + z * z;
                dsq = dsq + (1.0f - sign(dsq));
                sum += v / dsq;
                weight += w / dsq;
            //}
        }
    }
    
    weight = weight + (1.0f - sign(weight));
    vel = cv / (cw + qw) + qw * (sum / weight);
}
