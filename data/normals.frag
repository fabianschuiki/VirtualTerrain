varying vec3 V, N, L;
const float shininess = 100.0;
uniform sampler2D tex_color;
uniform sampler2D tex_normals;

void main() {
	vec3 pN = texture2D(tex_normals, gl_TexCoord[0].st).rgb;
	vec3 vTp = gl_TexCoord[1].xyz;
	vec3 vTt = cross(N,vTp);
	vec3 pNp = normalize(pN.x*vTp + pN.y*N + pN.z*vTt);
	
	vec3 R = normalize(-reflect(L,pNp));
	vec3 E = normalize(-V);
	
	float diffuse = max(0.0, dot(pNp,L));
	float specular = pow(max(0.0, dot(pNp,R)), shininess);
	
	//calculate the final color
	gl_FragColor = texture2D(tex_color, gl_TexCoord[0].st);
	gl_FragColor.xyz *= diffuse * 0.7 + 0.3;
	gl_FragColor.xyz += specular * vec3(0.5);
}