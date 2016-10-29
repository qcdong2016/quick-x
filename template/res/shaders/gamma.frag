#ifdef GL_ES
precision mediump float;
#endif
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;
uniform float u_gamma;

void main()
{
    vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = vec4(pow(textureColor.rgb, vec3(u_gamma)), textureColor.w);
}
