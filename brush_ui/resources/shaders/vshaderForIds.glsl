attribute highp vec3 vertex;
attribute mediump vec3 id;
varying mediump vec3 triangleId;
uniform mediump mat4 matrix;
void main(void)
{
    gl_Position = matrix * vec4(vertex, 1);
    triangleId = id;
}
