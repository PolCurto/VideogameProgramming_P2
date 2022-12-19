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

vec3 rotateAxis(vec3 p, vec3 axis, float angle) {
return mix(dot(axis, p)*axis, p, cos(angle)) + cross(axis,p)*sin(angle);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec4 texColor = texture(tex0, uvs);
    //FragColor = vec4(texColor.r, texColor.g, texColor.b, texColor.a);

    //mat4 rotation = rotationMatrix(vec3(0, 1, 0), rot);
    //vec3 rotatedLD = (vec3(rotation.r, rotation.g, rotation.b) * lightDirection);

    vec3 rotatedLD = rotateAxis(lightDirection, vec3(0, 1, 0), rot);

    float dotProduct = 1- dot(rotatedLD, normals);

    dotProduct = clamp(dotProduct, 0, 1);

    vec4 directionalLight = vec4(dotProduct, dotProduct, dotProduct, 1);

    FragColor = texColor * (directionalLight / 2 + 0.5);

   //FragColor = vec4((normals.z+1.)/2., (normals.z+1.)/2., (normals.z+1.)/2., 1.0f);
   //FragColor = vec4(uvs, 0.0f, 1.0f);
   //FragColor = vec4(color, 1.0f);
}