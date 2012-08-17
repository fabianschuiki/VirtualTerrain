uniform sampler2D tex;

void main() {
	vec3 c = texture2D(tex, gl_TexCoord[0].st).xyz;
	gl_FragColor.xyz = 1.0 - exp(-c * 1.0);
}