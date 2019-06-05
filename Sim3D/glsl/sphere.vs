layout (location = 0) in vec2 uv;
layout (location = 1) in vec3 shape;

out vec4 u;

uniform mat4 m;
uniform float size;
uniform sampler2D T;
uniform sampler2D V;
uniform vec2 ts;

uniform vec3 light;
uniform float minPower;
uniform vec4 colour;
uniform float vel;
uniform vec4 lightColour;
uniform float power;

void main() {
    vec4 p = vec4(texelFetch(T, ivec2(uv * ts), 0).xyz + shape * size, 1.0f);
    vec3 d = light - p.xyz;
    vec3 l = normalize(d);
    float ct = clamp(dot(shape, l), 0.0f, 1.0f);
    u = (colour + length(texelFetch(V, ivec2(uv * ts), 0).xyz) * vel) * lightColour * power * (ct / dot(d, d) + minPower);
    gl_Position = m * p;
}
