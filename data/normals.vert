varying vec3 vertex;
varying vec3 vertexEye;
varying vec3 normal;

void main() {
	vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vertexEye = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
	normal = normalize(gl_Normal);
	
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}