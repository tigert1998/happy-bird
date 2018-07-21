#version 330 core  

in vec3 vColor;
out vec4 FragColor;
	
void main(){
	// Halo shading //
	vec2 radiant = gl_PointCoord - vec2(0.5, 0.5);  
	float radiusSquare = dot(radiant, radiant);
	if(abs(radiant.x) < 0.07 && abs(radiant.y) < 0.07){
		FragColor = vec4(vColor, 1.0);
	}
	else if(radiusSquare < 0.25){ // from 0.0625 to 0.25
		FragColor = vec4(vColor, 0.25 - radiusSquare);
	}
	else{
		discard;
	}
	// FragColor = vec4(vColor, 1.0);
}