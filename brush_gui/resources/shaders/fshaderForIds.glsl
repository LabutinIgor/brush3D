varying mediump vec3 triangleId;
void main(void)
{
    gl_FragColor = vec4(triangleId / 255.0, 1.0);
}
