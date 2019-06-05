layout (location = 0) out vec3 a1;
layout (location = 1) out vec3 a2;

in vec3 vel;
in vec3 weight;

void main() {
    a1 = vel;
    a2 = weight;
}
