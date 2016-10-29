#ifdef GL_ES
precision lowp float;
varying highp vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

uniform sampler2D CC_Texture0;
uniform float u_saturation;

const vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

void main()
{
   vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
   float luminance = dot(textureColor.rgb, luminanceWeighting);
   vec3 greyScaleColor = vec3(luminance);
   
   gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, u_saturation), textureColor.w);
}

