attribute highp vec3 vertex;
attribute mediump vec2 texCoord;
//attribute mediump vec3 id;
varying mediump vec2 texc;
uniform mediump mat4 matrix;
void main(void)
{
    gl_Position = matrix * vec4(vertex, 1);
    texc = texCoord;
}
