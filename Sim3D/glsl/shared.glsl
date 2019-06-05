#version 410 core

precision mediump float;

#define pi 3.14159265358979323846264338327950288

vec3 get(in sampler3D T, ivec3 p) {
    vec3 n = texelFetch(T, p, 0).xyz;
    float x = texelFetch(T, p + ivec3(1, 0, 0), 0).x;
    float y = texelFetch(T, p + ivec3(0, 1, 0), 0).y;
    float z = texelFetch(T, p + ivec3(0, 0, 1), 0).z;
    return 0.5f * (n + vec3(x, y, z));
}

vec3 get(in sampler3D T, vec3 p, vec3 s) {
    vec3 g = 1.0f + s;
    p = (p + s)/(2.0f * g);
    vec3 n = texture(T, p).xyz;
    float x = texture(T, p + vec3(1.0f/g.x, 0.0f, 0.0f)).x;
    float y = texture(T, p + vec3(0.0f, 1.0f/g.y, 0.0f)).y;
    float z = texture(T, p + vec3(0.0f, 0.0f, 1.0f/g.z)).z;
    return 0.5f * (n + vec3(x, y, z));
}

float rand(float seed, vec2 coord) {
    return fract(cos(distance(vec2(seed * 9.22334f - coord.x + 2.2123311f * dot(coord, coord), coord.y * (9.12524f + seed * 0.1631f)), coord.yx)) * seed) * 2.0f - 1.0f;
}
