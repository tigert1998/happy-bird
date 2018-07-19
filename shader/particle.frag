#version 330 core  

uniform vec3 uColor;

out vec4 FragColor;
	
void main(){
	// Halo shading //
	vec2 radiant = gl_PointCoord - vec2(0.5, 0.5);  
	float radiusSquare = dot(radiant, radiant);
	if(abs(radiant.x) < 0.07 && abs(radiant.y) < 0.07){
		FragColor = vec4(uColor, 1.0);
	}
	else if(radiusSquare < 0.25){ // from 0.0625 to 0.25
		FragColor = vec4(uColor, 0.25 - radiusSquare);
	}
	else{
		discard;
	}
	// FragColor = vec4(uColor, 1.0);
}