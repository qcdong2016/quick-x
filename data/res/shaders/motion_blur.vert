attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform vec2 u_directionalTexelStep;

varying vec2 v_texCoord;
varying vec2 v_oneStepBackTextureCoordinate;
varying vec2 v_twoStepsBackTextureCoordinate;
varying vec2 v_threeStepsBackTextureCoordinate;
varying vec2 v_fourStepsBackTextureCoordinate;
varying vec2 v_oneStepForwardTextureCoordinate;
varying vec2 v_twoStepsForwardTextureCoordinate;
varying vec2 v_threeStepsForwardTextureCoordinate;
varying vec2 v_fourStepsForwardTextureCoordinate;

void main()
{
	gl_Position = CC_MVPMatrix * a_position;
    //gl_Position = a_position;
    
    v_texCoord = a_texCoord;
    v_oneStepBackTextureCoordinate = a_texCoord - u_directionalTexelStep;
    v_twoStepsBackTextureCoordinate = a_texCoord - 2.0 * u_directionalTexelStep;
    v_threeStepsBackTextureCoordinate = a_texCoord - 3.0 * u_directionalTexelStep;
    v_fourStepsBackTextureCoordinate = a_texCoord - 4.0 * u_directionalTexelStep;
    v_oneStepForwardTextureCoordinate = a_texCoord + u_directionalTexelStep;
    v_twoStepsForwardTextureCoordinate = a_texCoord + 2.0 * u_directionalTexelStep;
    v_threeStepsForwardTextureCoordinate = a_texCoord + 3.0 * u_directionalTexelStep;
    v_fourStepsForwardTextureCoordinate = a_texCoord + 4.0 * u_directionalTexelStep;
}
