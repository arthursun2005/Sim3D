layout (location = 0) out float U;

uniform sampler3D P;
uniform sampler3D D;
uniform sampler3D W;
uniform vec3 invSize;

in float depth;

void main() {
    vec3 p = vec3(gl_FragCoord.xy, depth);
    vec3 coord = p * invSize;
    
    float n0 = texture(P, coord + vec3(invSize.x, 0.0f, 0.0f)).x;
    float n1 = texture(P, coord + vec3(0.0f, invSize.y, 0.0f)).x;
    float n2 = texture(P, coord + vec3(0.0f, 0.0f, invSize.z)).x;
    float n3 = texture(P, coord - vec3(invSize.x, 0.0f, 0.0f)).x;
    float n4 = texture(P, coord - vec3(0.0f, invSize.y, 0.0f)).x;
    float n5 = texture(P, coord - vec3(0.0f, 0.0f, invSize.z)).x;
    
    float d = texelFetch(D, ivec3(p), 0).x;
    U = (n0 + n1 + n2 + n3 + n4 + n5 - d) / 6.0f * min(texelFetch(W, ivec3(p), 0).x, 1.0f);
}
