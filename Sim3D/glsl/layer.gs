layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vs {
    int depth;
} gs_in[];

out float depth;

void main() {
    gl_Position = gl_in[0].gl_Position;
    gl_Layer = gs_in[0].depth;
    depth = gs_in[0].depth + 0.5f;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    gl_Layer = gs_in[1].depth;
    depth = gs_in[1].depth + 0.5f;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    gl_Layer = gs_in[2].depth;
    depth = gs_in[2].depth + 0.5f;
    EmitVertex();
    
    EndPrimitive();
}
