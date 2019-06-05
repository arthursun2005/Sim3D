layout (location = 0) out vec4 A;

uniform sampler3D T;

in float depth;

void main() {
    ivec3 p = ivec3(gl_FragCoord.xy, depth);
    A = texelFetch(T, p, 0);
}
