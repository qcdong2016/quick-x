uniform sampler2D CC_Texture0;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

const vec3 SEPIA = vec3(1.2, 1.0, 0.8);

void main() {
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
	float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.144));
	vec3 sepiaColor = vec3(gray)*SEPIA;
	gl_FragColor = vec4(mix(texColor.rgb, sepiaColor, 0.75), 1.0)*v_fragmentColor;
}
