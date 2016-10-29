#ifdef GL_ES
precision mediump float;
uniform lowp float u_hazeDistance;
uniform highp float u_slope;
#else
uniform float u_hazeDistance;
uniform float u_slope;
#endif
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;

void main()
{
   //todo reconsider precision modifiers	 
#ifdef GL_ES
    highp vec4 color = vec4(1.0);//todo reimplement as a parameter
    highp float  d = v_texCoord.y * u_slope  +  u_hazeDistance;
    highp vec4 c = texture2D(CC_Texture0, v_texCoord) ; // consider using unpremultiply
#else
    vec4 color = vec4(1.0);//todo reimplement as a parameter
    float  d = v_texCoord.y * u_slope  +  u_hazeDistance;
    vec4 c = texture2D(CC_Texture0, v_texCoord) ; // consider using unpremultiply
#endif
    c = (c - d * color) / (1.0 -d);
    
    gl_FragColor = c; //consider using premultiply(c);
}
