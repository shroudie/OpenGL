#version 330 core

layout(location = 0) in vec3 vertexPostion;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;
uniform mat4 ml_matrix;

uniform vec3 uLightPosition;
uniform vec3 uAmbientLightColor;
uniform vec3 uDiffuseLightColor;
uniform vec3 uSpecularLightColor;

uniform vec3 uKAmbient;
uniform vec3 uKDiffuse;
uniform vec3 uKSpecular;
uniform float uShininess;

mat3 uNMatrix = inverse(transpose(mat3(vw_matrix*ml_matrix)));

out vec4 color;
out vec3 position;

void main() {
	position = vertexPostion;
    vec4 vertexPositionEye4 = vw_matrix * ml_matrix * vec4(vertexPostion, 1.0);

    vec3 vertexPositionEye3 = vertexPositionEye4.xyz / vertexPositionEye4.w;

  

    // Calculate the vector (l) to the light source

    vec3 lightEye=(vec4(uLightPosition,1)).xyz;

    vec3 vectorToLightSource = normalize(lightEye - vertexPositionEye3);

    

    // Transform the normal (n) to eye coordinates

    vec3 normalEye = normalize(uNMatrix * vertexNormal);

    

    // Calculate n dot l for diffuse lighting

    float diffuseLightWeightning = max(dot(normalEye, 

                                    vectorToLightSource), 0.0);

                                       

    // Calculate the reflection vector (r) that is needed for specular light

    vec3 reflectionVector = normalize(reflect(-vectorToLightSource, 

                                             normalEye));

    

    // The camera in eye coordinates is located in the origin and is pointing

    // along the negative z-axis. Calculate viewVector (v) 

    // in eye coordinates as:

    // (0.0, 0.0, 0.0) - vertexPositionEye3

    vec3 viewVectorEye = -normalize(vertexPositionEye3);

    

    float rdotv = max(dot(reflectionVector, viewVectorEye), 0.0);

    

    float specularLightWeightning = pow(rdotv, uShininess);



    // Sum up all three reflection components and send to the fragment shader

    color = vec4(((uAmbientLightColor*uKAmbient)

                + (uDiffuseLightColor*uKDiffuse) * diffuseLightWeightning

                + ((uSpecularLightColor*uKSpecular) * specularLightWeightning)),1.0);

       

    //color = pr_matrix*vw_matrix*vec4(vertexPostion, 1.0);     

    gl_Position = pr_matrix*vw_matrix*ml_matrix*vec4(vertexPostion, 1.0);
}
