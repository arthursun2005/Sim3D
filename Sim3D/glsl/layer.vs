layout (location = 0) in vec2 p;

out vs {
    int depth;
} vs_out;

void main() {
    vs_out.depth = gl_InstanceID;
    gl_Position = vec4(p, 0.0f, 1.0f);
}
