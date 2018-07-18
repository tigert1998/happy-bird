#version 330 core  

layout (location = 0) in vec4 aPoint;  

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()  
{
  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPoint.xyz,1.0f);  
  gl_PointSize = aPoint.w; 
}