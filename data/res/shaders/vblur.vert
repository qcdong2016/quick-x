												
attribute vec4 a_position;							
attribute vec2 a_texCoord;							
attribute vec4 a_color;								
													
#ifdef GL_ES										
varying lowp vec4 v_fragmentColor;					
varying mediump vec2 v_texCoord;					
#else												
varying vec4 v_fragmentColor;						
varying vec2 v_texCoord;							
#endif												

uniform float u_radius; 
varying vec2 v_blurTexCoords[14]; 
													
void main()											
{													
    gl_Position = CC_MVPMatrix * a_position;		
	v_fragmentColor = a_color;						
	v_texCoord = a_texCoord;						
	for (int i = 0; i < 7; ++i) {
		vec2 c = vec2(0.0, u_radius / 7.0*(7.0 - float(i)));
		v_blurTexCoords[i] = v_texCoord - c;
		v_blurTexCoords[13 - i] = v_texCoord + c;
	}
}													

