layout(points) in;
layout(points, max_vertices = 1) out;

in vs {
    int depth;
    vec3 vel;
    vec3 weight;
} gs_in[];

out vec3 vel;
out vec3 weight;

void main() {
    gl_Position = gl_in[0].gl_Position;
    gl_Layer = gs_in[0].depth;
    vel = gs_in[0].vel;
    weight = gs_in[0].weight;
    EmitVertex();
    
    EndPrimitive();
}

