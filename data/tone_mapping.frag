uniform sampler2D tex;

void main() {
	vec3 c = texture2D(tex, gl_TexCoord[0].st).xyz;
	
	//calculate the luminance
	float luminance = 0.2126*c.x + 0.7152*c.y + 0.0722*c.z;

	gl_FragColor.xyz = 1.0 - exp(-c * 1.0);
	/*gl_FragColor = c;*/
}