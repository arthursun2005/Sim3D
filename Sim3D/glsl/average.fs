layout (location = 0) out vec3 vel;

uniform sampler3D V;
uniform sampler3D W;
uniform vec3 invSize;

in float depth;

void main() {
    vec3 p = vec3(gl_FragCoord.xy, depth);
    vec3 coord = p * invSize;
    
    vec3 cv = texelFetch(V, ivec3(p), 0).xyz;
    vec3 cw = texelFetch(W, ivec3(p), 0).xyz;
    
    vel = cv / (cw + (1.0f - sign(cw)));
}

