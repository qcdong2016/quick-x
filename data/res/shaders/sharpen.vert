attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;	

uniform float u_widthFactor; 
uniform float u_heightFactor; 
uniform float u_sharpness;

varying vec2 v_texCoord;
varying vec2 v_leftTexCoord;
varying vec2 v_rightTexCoord; 
varying vec2 v_topTexCoord;
varying vec2 v_bottomTexCoord;

varying float v_centerMultiplier;
varying float v_edgeMultiplier;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;		
    //gl_Position = a_position;
    
    vec2 widthStep = vec2(u_widthFactor, 0.0);
    vec2 heightStep = vec2(0.0, u_heightFactor);
    
    v_texCoord = a_texCoord;
    v_leftTexCoord = a_texCoord - widthStep;
    v_rightTexCoord = a_texCoord + widthStep;
    v_topTexCoord = a_texCoord + heightStep;
    v_bottomTexCoord = a_texCoord - heightStep;
    
    v_centerMultiplier = 1.0 + 4.0 * u_sharpness;
    v_edgeMultiplier = u_sharpness;
}
