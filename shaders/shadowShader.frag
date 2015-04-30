//uniform sampler2DShadow shadowMap;
uniform sampler2D shadowMap;
uniform vec3 cameraPosition;

varying vec4 lightVertexPosition;
varying vec3 outNormal;
varying vec3 outColor;
varying vec3 lightPosition;
varying vec3 position;

const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);

void main()
{
	float shadowValue=0.0;
	vec4 lightVertexPosition2=lightVertexPosition;
	lightVertexPosition2/=lightVertexPosition2.w;
	for(float x=-0.001;x<=0.001;x+=0.0005)
		for(float y=-0.001;y<=0.001;y+=0.0005)
		{
			if(texture2D(shadowMap,lightVertexPosition2.xy+vec2(x,y)).r>=lightVertexPosition2.z)
				shadowValue+=1.0;	
		}
	shadowValue/=16.0;
	
	
	
	//float shadowValue=shadow2DProj(shadowMap,lightVertexPosition).r;	//r==1.0 not in shader
																									//r==0.0 it's in shadow
																									
																									
		vec3 normal=normalize(outNormal);
		vec3 surf2light=normalize(lightPosition-position);
		vec3 surf2camera=normalize(-position);
		vec3 reflection=-reflect(surf2camera,normal);
		float diffuseContribution=max(0.0,dot(normal,surf2light));
		float specularContribution=pow(max(0.0,dot(reflection,surf2light)),32.0);

																
		gl_FragColor=vec4(ambientColor*outColor+(shadowValue+0.05)*diffuseContribution *diffuseColor*outColor+ (shadowValue<0.5 ? vec3(0.0,0.0,0.0) : specularContribution*specularColor*shadowValue),1.0);
}
