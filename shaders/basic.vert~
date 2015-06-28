#version 330

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Pass through the color to the fragment shader
    color = vertexColor;

    // Calculate the vertex position
    gl_Position = projection * view * vec4( vertexPosition, 1.0 );
}
