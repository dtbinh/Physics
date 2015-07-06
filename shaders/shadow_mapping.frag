#version 330 core
out vec4 FragColor;

uniform struct LightInfo {
    vec4 position;
    vec3 intensity;
} light;

struct MaterialInfo
{
    vec3 ka;            // Ambient reflectivity
    vec3 kd;            // Diffuse reflectivity
    vec3 ks;            // Specular reflectivity
    float shininess;    // Specular shininess factor
} ;
uniform MaterialInfo material;
in vec3 position;
in vec3 normal;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool shadows;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 adsModel( const in vec3 pos, const in vec3 norm )
{
    // Calculate the vector from the light to the fragment
    vec3 s = normalize( vec3( light.position ) - vec3(0,0,0) );

    float fator = max( (dot(norm,s))/(length(s)) ,0.0);


    // Calculate the vector from the fragment to the eye position
    // (origin since this is in "eye" or "camera" space)
    vec3 v = normalize( -position.xyz );

    // Reflect the light beam using the normal at this fragment
    vec3 r = reflect( -s, norm );

    // Calculate the diffuse component
    float diffuse = fator;//max( dot( s, norm ), 0.0 );

    // Calculate the specular component
    float specular = 0.0;

 //calculo da contribuicao especular
    s = normalize(s);
    r = norm*(2*dot(s,norm)) - s;
    v = (v-pos)/length(v-pos);
    r = (r+v)/length(r+v);

    //float fator2 = fmax(pow((r*v),m->shininess*128),0);
    //if(r*n<0) fator2 = 0;
    //Vec4 especular;




    if ( dot( r, norm ) > 0.0 )
        specular = pow( max( dot( r, v ), 0.0 ), material.shininess );

    vec3 lightColor = vec3(0.8);
    return (lightColor * ( material.ka + material.kd * diffuse + material.ks * specular ));

}

void main()
{           
    vec3 normal = normalize(fs_in.Normal);
    vec3 color = adsModel(lightPos, normal); //texture(diffuseTexture, fs_in.TexCoords).rgb; // adsModel(lightPos, normal);//;

    vec3 lightColor = vec3(1.0);
    // Ambient
    vec3 ambient = 0.5 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // Calculate shadow
    float shadow = shadows ? ShadowCalculation(fs_in.FragPosLightSpace) : 0.0;   
    //vec3 lighting = color;
    //if (shadow > 0.0)                   
    vec3  lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    FragColor = vec4(lighting, 1.0f);
}
