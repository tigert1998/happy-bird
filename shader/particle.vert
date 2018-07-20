#version 330 core  

layout (location = 0) in vec3 aPoint;  
layout (location = 1) in float aRadius;  

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()  
{
  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPoint,1.0f);  
  gl_PointSize = aRadius * 2; 
}