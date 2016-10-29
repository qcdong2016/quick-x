#ifdef GL_ES
precision highp float;
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

uniform sampler2D CC_Texture0;
uniform float u_exposure;

void main()
{
    vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = vec4(textureColor.rgb * pow(2.0, u_exposure), textureColor.w);
}

