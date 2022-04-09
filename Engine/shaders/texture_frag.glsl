#version 460
uniform sampler3D ourTexture;

in vec4 frag_tex_coord;
out vec4 shaded_color;

const float DISCARD_EPS = 0.01f;

void main()
{
    const vec4 outColor = texture(ourTexture, frag_tex_coord.xyz);
    if(length(outColor.xyz) < DISCARD_EPS)
    {
        discard;
    }

    shaded_color = outColor;
}
