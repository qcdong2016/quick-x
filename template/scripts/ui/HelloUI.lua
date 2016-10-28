local UILayer = require "UILayer"

local HelloUI = class('HelloUI', UILayer)

function HelloUI:ctor()
    UILayer.ctor(self)
    
    local hello = display.newSprite('HelloWorld.png'):addTo(self)

    hello:runAction(CCRepeatForever:create(CCSequence:create(
        CCScaleTo:create(3, 0.5),
        CCScaleTo:create(1, 1)
    )))

    hello:runAction(CCScaleTo:create(3, 0.5))

    ui.newTTFLabel({size= 25, text = 'Hello World'}):addTo(self, 1);
    
    imgui.addDraw(function()
        imgui.Button('hello lua')
    end)

    imgui.addDraw(function()
        imgui.Button('hello lua')
    end)

     CCLayerColor:create(ccc4(0,55,0,255)):addTo(self)
     display.newRect(100,100):addTo(self)

    local spine = SkeletonAnimation:createWithFile("spineboy.json", "spineboy.atlas", 1)
        :addTo(self)
        :pos(-200+math.random(-100,100), -display.cy)
    spine:setAnimation(0, 'run', true)

    local touchLayer = TouchGroup:create()
    self:addChild(touchLayer)

    function Button(text, func)
        local btn = Label:create()
        btn:ignoreContentAdaptWithSize(false)
        btn:setSize(CCSize(500, 50))
        btn:setFontSize(40)
        btn:setText(text)
        btn:setTouchEnabled(true)
        btn:setTextHorizontalAlignment(kCCTextAlignmentCenter)
        btn:setTextVerticalAlignment(kCCVerticalTextAlignmentCenter)
        btn:addTouchEventListener(function(_, type)
            if type == TOUCH_EVENT_ENDED then
                if func then func(btn); end
            end
        end)
        touchLayer:addWidget(btn)
        return btn
    end

    local btn = Button('send Event 1', function(self)
        self:sendEvent(1)
    end):pos(100, 100)

    local btn2 = Button('send Event 2', function(self)
        self:sendEvent(2)
    end):pos(100, 200)


    local btn3 = Button('remove 1', function(self)
        print('unsubscribeFromAllEvents')
        -- btn:removeSelf()
        self:unsubscribeFromEvent(1)
    end):pos(300, 100)

    btn:subscribeToEvent(1, function(type, data)
        print('onEvent 1,1')
    end)

    btn2:subscribeToEvent(2, function(type, data)
        print('onEvent 2')
    end)

    btn3:subscribeToEvent(1, function(type, data)
        print('onEvent 3,1')
    end)

    btn3:subscribeToEvent(btn, 1, function(type, data)
        print('onEvent 3,11')
    end)


local ccFilterShader_hblur_vert = 
[[                                            
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
        vec2 c = vec2(u_radius / 7.0*(7.0 - float(i)), 0.0);
        v_blurTexCoords[i] = v_texCoord - c;
        v_blurTexCoords[13 - i] = v_texCoord + c;
    }
}                                                   
]];

local ccFilterShader_blur_frag = [[
#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[14];
uniform sampler2D CC_Texture0;

void main()
{
    gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 0])*0.0044299121055113265;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 1])*0.00895781211794;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 2])*0.0215963866053;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 3])*0.0443683338718;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 4])*0.0776744219933;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 5])*0.115876621105;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 6])*0.147308056121;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord         )*0.159576912161;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 7])*0.147308056121;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 8])*0.115876621105;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[ 9])*0.0776744219933;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[10])*0.0443683338718;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[11])*0.0215963866053;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[12])*0.00895781211794;
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[13])*0.0044299121055113265;
}]];


    local shader = CCShaderCache:sharedShaderCache():addShader(ccFilterShader_hblur_vert, ccFilterShader_blur_frag)
    shader:set1f(shader:get('u_radius'), 0.1)
    hello:setShaderProgram(shader)
end

return HelloUI