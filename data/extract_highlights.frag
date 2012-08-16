uniform sampler2D tex;

void main() {
	vec4 c = texture2D(tex, gl_TexCoord[0].st);
	c = max(c - 0.9, 0.0);
	gl_FragColor = c;
}