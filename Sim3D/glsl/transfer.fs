layout (location = 0) out vec3 vel;

uniform sampler2D P;
uniform sampler2D V;
uniform sampler3D G;
uniform sampler3D G2;
uniform vec3 size;

void main() {
    ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 pos0 = texelFetch(P, coord, 0).xyz;
    vec3 vel0 = texelFetch(V, coord, 0).xyz;
    vec3 gv = get(G, pos0, size);
    vec3 gt = get(G2, pos0, size);
    vec3 fv = vel0 + (gv - gt);
    vel = mix(gv, fv, 0.99f);
}
