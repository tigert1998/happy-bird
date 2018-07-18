#version 330 core  

layout (location = 0) in vec4 aPoint;  

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

// out vec3 vPosition;

void main()  
{
  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPoint.xyz,1.0f);  

	// vPosition = vec3(uModelMatrix * vec4(aPoint.x, aPoint.y, aPoint.z, 1));

	// vec4 temp_without_projection = uViewMatrix * uModelMatrix * vec4(aPoint.x, aPoint.y, aPoint.z, 1.0f);
	// float dist = abs( temp_without_projection.z / temp_without_projection.w );
  
  gl_PointSize = aPoint.w; 
  // gl_PointSize = aPoint.w ;//* sqrt(1000000.0f / (4.0f *dist*dist + dist + 1.0f ));
}