uniform sampler2D tex;
uniform float d;

void main() {
	vec2 c = gl_TexCoord[0].st;
	vec4 sum = vec4(0);
	
	float weight[5];
	weight[0] = 0.1423;
	weight[1] = 0.1346;
	weight[2] = 0.1139;
	weight[3] = 0.0863;
	weight[4] = 0.0585;
	
	int i;
	sum += texture2D(tex, c) * weight[0];
	for (i = 1; i < 10; i++) {
		sum += texture2D(tex, c + vec2(0,float( i)*d)) * weight[i/2] * 0.5;
		sum += texture2D(tex, c + vec2(0,float(-i)*d)) * weight[i/2] * 0.5;
	}
	
	gl_FragColor = sum;
}