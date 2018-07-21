#version 330 core  

uniform vec3 uColor;

in vec3 vColor;
out vec4 FragColor;
	
void main(){
	// Halo shading //
	vec2 radiant = gl_PointCoord - vec2(0.5, 0.5);  
	float radiusSquare = dot(radiant, radiant);
	if(abs(radiant.x) < 0.2 && abs(radiant.y) < 0.2){
		FragColor = vec4(vColor, 1.0);
	}
	else{
		discard;
	}
	// FragColor = vec4(vColor, 1.0);
}