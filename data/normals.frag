varying vec3 V, N, L;
const float shininess = 20.0;

void main() {
	vec3 R = normalize(-reflect(L,N));
	vec3 E = normalize(-V);
	
	float diffuse = max(0.0, dot(N,L));
	float specular = pow(max(0.0, dot(N,R)), shininess);
	
	//calculate the final color
	gl_FragColor = gl_Color;
	gl_FragColor.xyz *= diffuse * 1.5 + 0.1;
	gl_FragColor.xyz += specular * vec3(1.5);
}