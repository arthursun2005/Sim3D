layout (location = 0) out vec3 A;

uniform vec3 pos;
uniform float sp;
uniform int hx;
uniform int hy;
uniform int hz;
uniform int root;
uniform int count;

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    int k = u.x + u.y * root;
    int on = k - count;
    int l = hx * hy;
    A = pos + sp * (vec3(on%hx, (on%l)/hx, on/l) - 0.5f * vec3(hx, hy, hz));
}
