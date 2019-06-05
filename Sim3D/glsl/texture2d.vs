layout (location = 0) in vec2 uv;

void main() {
    gl_PointSize = 1.0f;
    gl_Position = vec4(uv * 2.0f - 1.0f, 0.0f, 1.0f);
}
