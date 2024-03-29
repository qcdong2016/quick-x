
#ifdef GL_ES
precision mediump float;
#endif
 
 varying vec2 v_texCoord;
 uniform sampler2D u_texture;
 uniform float u_time;
 
void main()
{
  vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);
 
  vec2 texCoord = v_texCoord;
	texCoord.x += sin(u_time) * (onePixel.x * 20.0);
	texCoord.y += cos(u_time) * (onePixel.y * 20.0);
 
  vec4 color;
  color.rgb = vec3(0.5);
  color -= texture2D(u_texture, texCoord - onePixel) * 2.0;
  color += texture2D(u_texture, texCoord + onePixel) * 2.0;
  color.rgb = vec3((color.r + color.g + color.b) / 3.0);
  gl_FragColor = vec4(color.rgb, 1);
}

