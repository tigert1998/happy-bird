#version 330 core  

uniform vec3 uColor;

in vec3 vColor;
out vec4 FragColor;
	
void main(){
	// Halo shading //
	vec2 radiant = gl_PointCoord - vec2(0.5, 0.5);  
	float radiusSquare = dot(radiant, radiant);
	if(abs(radiant.y) < 0.1 && abs(radiant.x) < 0.2){
		FragColor = vec4(vColor, 1.0);
	}
	else if(radiusSquare < 0.15){ // from 0.0625 to 0.25
		FragColor = vec4(vColor, 0.15 - radiusSquare);
	}
	else{
		discard;
	}
	// FragColor = vec4(vColor, 1.0);
}