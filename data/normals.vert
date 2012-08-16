const vec3 lightPos = vec3(5, 5, -5);
varying vec3 V, N, L;

void main()
{
	//eye-space normal
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	//eye-space light vector
	V = vec3(gl_ModelViewMatrix * gl_Vertex);
	L = normalize(gl_NormalMatrix * (lightPos - gl_Vertex.xyz));
	
	//bypass
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}