attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 color;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 lightModelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 lightMatrix;	//modelviewmatrix for the light
uniform vec3 inLightPosition;  //model-space

varying vec4 lightVertexPosition;
varying vec3 outColor;
varying vec3 position;
varying vec3 outNormal;
varying vec3 lightPosition;  //world-space

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	lightVertexPosition=lightModelViewProjectionMatrix*vec4(vertex,1.0);
	position=vec3(modelViewMatrix*vec4(vertex,1.0));
	outNormal=normalMatrix*normal;
	outColor=color;	
	lightPosition=vec3(lightMatrix*vec4(inLightPosition,1.0));
}
