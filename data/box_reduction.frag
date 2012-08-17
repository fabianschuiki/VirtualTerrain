uniform sampler2D tex;
uniform float dw;
uniform float dh;

void main() {
	vec4 c = vec4(0);
	c += texture2D(tex, gl_TexCoord[0].st + vec2( 0,  0));
	c += texture2D(tex, gl_TexCoord[0].st + vec2(dw,  0));
	c += texture2D(tex, gl_TexCoord[0].st + vec2( 0, dh));
	c += texture2D(tex, gl_TexCoord[0].st + vec2(dw, dh));
	gl_FragColor = c * 0.25;
}