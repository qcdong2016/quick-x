#ifdef GL_ES
precision mediump float;
#endif
 varying vec2 v_texCoord;
 
 uniform sampler2D CC_Texture0;
 
 uniform float u_redAdj;
 uniform float u_greenAdj;
 uniform float u_blueAdj;
 
 void main()
 {
     vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
     gl_FragColor = vec4(textureColor.r * u_redAdj, textureColor.g * u_greenAdj, textureColor.b * u_blueAdj, textureColor.a);
 }
