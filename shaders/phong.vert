
	// Used for shadow lookup
	varying vec4 ShadowCoord;
varying vec4 p;
varying vec3 n;

void main(void)
{
    ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;

    p = gl_ModelViewMatrix * gl_Vertex;
    n = gl_NormalMatrix * gl_Normal;
}

