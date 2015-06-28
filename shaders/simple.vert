#version 330
 
in vec4 vertex;
 
void main( void )
{
 gl_Position = vec4(vertex.x,vertex.y,vertex.z,1.0);
}
