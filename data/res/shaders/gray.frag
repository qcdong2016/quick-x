#ifdef GL_ES
precision mediump float;
#endif
varying vec2 v_texCoord;                    
varying vec4 v_fragmentColor;

uniform sampler2D CC_Texture0;
uniform vec4 u_grayParam; 

void main(void)                             
{                                           
// Convert to greyscale using NTSC weightings    
vec4 texColor = texture2D(CC_Texture0, v_texCoord);   
//float grey = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));       
float grey = dot(texColor.rgba, u_grayParam);       
gl_FragColor = vec4(vec3(grey), texColor.a) * v_fragmentColor; 
}
