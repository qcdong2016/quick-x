varying vec2 v_texCoord;
varying vec2 v_leftTexCoord;
varying vec2 v_rightTexCoord; 
varying vec2 v_topTexCoord;
varying vec2 v_bottomTexCoord;

varying float v_centerMultiplier;
varying float v_edgeMultiplier;

uniform sampler2D CC_Texture0;

void main()
{
    vec3 textureColor = texture2D(CC_Texture0, v_texCoord).rgb;
    vec3 leftTexColor = texture2D(CC_Texture0, v_leftTexCoord).rgb;
    vec3 rightTexColor = texture2D(CC_Texture0, v_rightTexCoord).rgb;
    vec3 topTexColor = texture2D(CC_Texture0, v_topTexCoord).rgb;
    vec3 bottomTexColor = texture2D(CC_Texture0, v_bottomTexCoord).rgb;

	vec3 v1 = leftTexColor * v_edgeMultiplier + rightTexColor * v_edgeMultiplier + topTexColor * v_edgeMultiplier + bottomTexColor * v_edgeMultiplier;
	vec3 v2 = textureColor * v_centerMultiplier - v1;
	vec4 v3 = texture2D(CC_Texture0, v_bottomTexCoord);

    gl_FragColor = vec4(v2, v3.w);
}
