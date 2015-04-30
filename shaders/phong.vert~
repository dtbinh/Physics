

varying vec4 p;
varying vec3 n;

void main(void)
{
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;

    p = gl_ModelViewMatrix * gl_Vertex;
    n = gl_NormalMatrix * gl_Normal;
}

