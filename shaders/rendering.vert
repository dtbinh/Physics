#version 150

// input attributes (world space) 
in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 coord;

// input uniforms 
uniform mat4 mdvMat;      // modelview matrix 
uniform mat4 projMat;     // projection matrix
uniform mat3 normalMat;   // normal matrix
uniform mat4 mvpDepthMat; // mvp depth matrix

// output vectors (camera space)
out vec3 normalView;
out vec3 tangentView;
out vec3 eyeView;
out vec2 uvcoord;

void main() {
  gl_Position = projMat*mdvMat*vec4(position,1.0);
  normalView  = normalize(normalMat*normal);
  tangentView = normalize(normalMat*tangent);
  eyeView     = normalize((mdvMat*vec4(position,1.0)).xyz);
  uvcoord     = coord*5.0;
  
  // *** TODO: project position in light space to be able to compare depths ***
  
}

