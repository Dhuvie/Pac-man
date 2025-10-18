#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

void main()
{
    // Create circular particle with smooth falloff
    vec2 coord = TexCoords * 2.0 - 1.0;
    float dist = length(coord);
    float alpha = 1.0 - smoothstep(0.0, 1.0, dist);
    
    color = vec4(ParticleColor.rgb, ParticleColor.a * alpha);
}
