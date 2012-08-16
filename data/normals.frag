varying vec3 vertex;
varying vec3 vertexEye;
varying vec3 normal;
const vec3 lightDir = vec3(5, 5, -5);
const float shininess = 100.0;

void main() {
	vec3 color = 0.5 * normal + 0.5;
	
	vec3 L = normalize(lightDir - vertex);
	vec3 E = normalize(-vertexEye);
	vec3 R = normalize(-reflect(L,normal));
	
	vec3 Iamb = color * vec3(0.2, 0.2, 0.2);
	vec3 Idiff = color * vec3(0.8, 0.8, 0.8) * max(dot(normal,L), 0.0);
	vec3 Ispec = vec3(0.9, 0.9, 0.9) * pow(max(dot(R,E), 0.0), 0.3*shininess);
	
	gl_FragColor.xyz = Iamb + Idiff + Ispec/* - vec4(1, 1, 1, 0)*/;
}