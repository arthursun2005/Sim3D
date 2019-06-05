layout (location = 0) out float D;

uniform sampler3D V;
uniform sampler3D W;
uniform float w0;

in float depth;

void main() {
    ivec3 coord = ivec3(gl_FragCoord.xy, depth);
    float w = texelFetch(W, coord, 0).x;
    vec3 n = texelFetch(V, coord, 0).xyz;
    float x = texelFetch(V, coord + ivec3(1, 0, 0), 0).x;
    float y = texelFetch(V, coord + ivec3(0, 1, 0), 0).y;
    float z = texelFetch(V, coord + ivec3(0, 0, 1), 0).z;
    float div = ((x - n.x) + (y - n.y) + (z - n.z));
    div -= max(w - w0, 0.0f);
    D = div;
}
