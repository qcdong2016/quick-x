precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform float u_factor;
 
void main()
{
  vec2 onePixel = vec2(0.5 / 480.0, 0.5 / 320.0);
 
  // 3
  vec2 texCoord = v_texCoord;
 
  // 4
  vec4 color;
  color.rgb = vec3(0.5);
  color -= texture2D(CC_Texture0, texCoord - onePixel) * 3.0;
  color += texture2D(CC_Texture0, texCoord + onePixel) * 3.0;
  // 5
  color.rgb = vec3((color.r + color.g + color.b) / 3.0);
  gl_FragColor = vec4(color.rgb, 1);
}

