uniform sampler2D texture;
varying mediump vec2 texc;
void main(void)
{
    gl_FragColor = texture2D(texture, texc.st);//vec4(texc, 1.0, 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
