#version 330

uniform sampler2DRect textureSampler;

in vec2 UV;

out vec4 fColor;

void main(void)
{
    fColor = vec4(texture(textureSampler, UV.st).rgb, 1.0);
    //fColor = vec4(UV.st, 0.0, 1.0);
}
