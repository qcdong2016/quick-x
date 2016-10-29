#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D CC_Texture0;
uniform float u_resolution;
uniform float u_radius;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

const float f1 = 0.15;
const float f2 = 0.12;
const float f3 = 0.09;
const float f4 = 0.05;
const float fc = 0.16;

void main() {
	vec4 sum = vec4(0.0);
	vec2 tc = v_texCoord;
	float blur = u_radius/u_resolution; 
	
	sum += texture2D(CC_Texture0, vec2(tc.x - 4.0*blur, tc.y)) * f4;
	sum += texture2D(CC_Texture0, vec2(tc.x - 3.0*blur, tc.y)) * f3;
	sum += texture2D(CC_Texture0, vec2(tc.x - 2.0*blur, tc.y)) * f2;
	sum += texture2D(CC_Texture0, vec2(tc.x - 1.0*blur, tc.y)) * f1;
	
	sum += texture2D(CC_Texture0, vec2(tc.x, tc.y)) * fc;
	
	sum += texture2D(CC_Texture0, vec2(tc.x + 1.0*blur, tc.y)) * f1;
	sum += texture2D(CC_Texture0, vec2(tc.x + 2.0*blur, tc.y)) * f2;
	sum += texture2D(CC_Texture0, vec2(tc.x + 3.0*blur, tc.y)) * f3;
	sum += texture2D(CC_Texture0, vec2(tc.x + 4.0*blur, tc.y)) * f4;

	gl_FragColor = v_fragmentColor * sum;
}
