#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D CC_Texture0;

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
#ifdef GL_ES
    lowp vec4 fragmentColor = texture2D(CC_Texture0, v_texCoord) * 0.18;
#else
    vec4 fragmentColor = texture2D(CC_Texture0, v_texCoord) * 0.18;
#endif
    fragmentColor += texture2D(CC_Texture0, v_oneStepBackTextureCoordinate) * 0.15;
    fragmentColor += texture2D(CC_Texture0, v_twoStepsBackTextureCoordinate) *  0.12;
    fragmentColor += texture2D(CC_Texture0, v_threeStepsBackTextureCoordinate) * 0.09;
    fragmentColor += texture2D(CC_Texture0, v_fourStepsBackTextureCoordinate) * 0.05;
    fragmentColor += texture2D(CC_Texture0, v_oneStepForwardTextureCoordinate) * 0.15;
    fragmentColor += texture2D(CC_Texture0, v_twoStepsForwardTextureCoordinate) *  0.12;
    fragmentColor += texture2D(CC_Texture0, v_threeStepsForwardTextureCoordinate) * 0.09;
    fragmentColor += texture2D(CC_Texture0, v_fourStepsForwardTextureCoordinate) * 0.05;

    gl_FragColor = fragmentColor;
}
