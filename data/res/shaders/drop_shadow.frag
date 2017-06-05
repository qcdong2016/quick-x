#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D CC_Texture0;
uniform vec2 u_resolution;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

const float RADIUS = 0.75;
const float SOFTNESS = 0.45;
const vec3 SEPIA = vec3(1.2, 1.0, 0.8);

void main() {
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
	vec2 pos = (gl_FragCoord.xy/u_resolution.xy) - 0.5;
	pos.x *= u_resolution.x/u_resolution.y;
	float len = length(pos);
	////gl_FragColor = vec4(texColor.rgb*(1.0-len), 1.0);
	float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);
	//texColor.rgb *= mix(texColor.rgb, texColor.rgb*vignette, 0.5);
	texColor.rgb = mix(texColor.rgb, texColor.rgb*vignette, 0.5);
	//gl_FragColor=texColor;
	//gl_FragColor=texColor*v_fragmentColor;
	//gl_FragColor = vec4(texColor.rgb*(1.0-len), 1);
	float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.144));
	vec3 sepiaColor = vec3(gray)*SEPIA;
	gl_FragColor = vec4(mix(texColor.rgb, sepiaColor, 0.75), 1.0);
}
