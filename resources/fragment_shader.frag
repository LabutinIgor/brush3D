#version 330 core

in vec2 UV;

out vec3 fColor;

uniform sampler2D textureSampler;

void main(){
    //fColor = vec4(UV.st, 0.0, 1.0);
	fColor = texture(textureSampler, UV).rgb;
}