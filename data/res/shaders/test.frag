uniform sampler2D CC_Texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_mask;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main() {
	vec4 texColor0 = texture2D(CC_Texture0, v_texCoord);
	vec4 texColor1 = texture2D(u_texture1, v_texCoord);
	float mask = texture2D(u_mask, v_texCoord).a;
	gl_FragColor = mix(texColor0, texColor1, mask);
}
