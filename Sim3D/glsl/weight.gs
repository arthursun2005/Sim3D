layout(points) in;
layout(points, max_vertices = 1) out;

in vs {
    int depth;
    float weight;
} gs_in[];

out float weight;

void main() {
    gl_Position = gl_in[0].gl_Position;
    gl_Layer = gs_in[0].depth;
    weight = gs_in[0].weight;
    EmitVertex();
    
    EndPrimitive();
}
