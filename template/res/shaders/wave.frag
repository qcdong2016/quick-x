// come from
// http://blog.csdn.net/zmzsoftware/article/details/32737605
//


varying vec4 v_fragmentColor; 
varying vec2 v_texCoord;

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D CC_Texture0;
uniform float u_time;  
uniform vec2 u_resolution;  
const float PI = 3.1415926535897932;  

const float speed = 0.2;  
const float speed_x = 0.3;  
const float speed_y = 0.3;  
 
const float intensity = 3.0;  
const int steps = 8;  
const float frequency = 4.0;  
const int angle = 7; 
 
const float delta = 10.0;  
const float intence = 400.0;  
const float emboss = 0.3;  
 
float col(vec2 coord)  
{  
    float delta_theta = 2.0 * PI / float(angle);  
    float col = 0.0;  
    float theta = 0.0;  
    for (int i = 0; i < steps; i++)  
    {
        vec2 adjc = coord;
        theta = delta_theta * float(i);
        adjc.x += cos(theta)*u_time*speed + u_time * speed_x;  
        adjc.y -= sin(theta)*u_time*speed - u_time * speed_y;  
        col = col + cos((adjc.x*cos(theta) - adjc.y*sin(theta))  
            *frequency)*intensity;  
    }
    return cos(col);
}  

void main(void)  
{  
    vec2 p = v_texCoord.xy, c1 = p, c2 = p;  
    float cc1 = col(c1);  
    
    //u_resolution.x = u_resolution.y;
    c2.x += u_resolution.y/delta;
    float dx = emboss*(cc1-col(c2))/delta;  
 
    c2.x = p.x;  
    c2.y += u_resolution.y/delta;  
    float dy = emboss*(cc1-col(c2))/delta;  
 
    c1.x += dx;  
    c1.y += dy;  
    float alpha = 1.+dot(dx,dy)*intence;  
    gl_FragColor = texture2D(CC_Texture0,c1) *(alpha) *v_fragmentColor*(alpha);  
}
