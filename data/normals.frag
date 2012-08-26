varying vec3 V, N, L;
const float shininess = 100.0;
uniform sampler2D tex;

void main() {
	vec3 R = normalize(-reflect(L,N));
	vec3 E = normalize(-V);
	
	float diffuse = max(0.0, dot(N,L));
	float specular = pow(max(0.0, dot(N,R)), shininess);
	
	//calculate the final color
	gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
	gl_FragColor.xyz *= diffuse * 0.7 + 0.3;
	gl_FragColor.xyz += specular * vec3(0.5);
}