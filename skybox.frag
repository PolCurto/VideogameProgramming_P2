#version 330 core
out vec4 FragColor;

in vec3 color;

in vec3 normals;
in vec2 uvs;

uniform sampler2D tex0;

uniform vec3 tint = vec3(1.0, 1.0, 1.0);

uniform vec3 lightDirection = vec3(-0.5, -1, -0.75);
uniform float ambient = 0.2;
uniform float rot = 0;

void main()
{
    vec4 texColor = texture(tex0, uvs);

    FragColor = texColor 

   //FragColor = vec4((normals.z+1.)/2., (normals.z+1.)/2., (normals.z+1.)/2., 1.0f);
   //FragColor = vec4(uvs, 0.0f, 1.0f);
   //FragColor = vec4(color, 1.0f);
}