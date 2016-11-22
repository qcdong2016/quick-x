/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __COCOSGUI_H__
#define __COCOSGUI_H__


#include "UIWidget.h"
#include "Layouts/UILayout.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIImageView.h"
#include "UILabel.h"
#include "UILabelAtlas.h"
#include "UILoadingBar.h"
#include "ScrollWidget/UIScrollView.h"
#include "UISlider.h"
#include "UITextField.h"
#include "ScrollWidget/UIListView.h"
#include "UILabelBMFont.h"
#include "ScrollWidget/UIPageView.h"
#include "UITouchGroup.h"
#include "UIRichText.h"
#include "ui/webview/UIWebView.h"


//class type define
NS_CC_BEGIN
//
namespace ui {
    
    typedef cocos2d::ui::Layout UIPanel;
    typedef cocos2d::ui::Layout UILayout;
    typedef cocos2d::ui::Widget UIWidget;
    
    typedef cocos2d::ui::LayoutParameter UILayoutParameter;
    typedef cocos2d::ui::Margin UIMargin;
    
    typedef cocos2d::ui::ListView UIListView;
    typedef cocos2d::ui::PageView UIPageView;
    typedef cocos2d::ui::ScrollView UIDragPanel;
    typedef cocos2d::ui::ScrollView UIScrollView;
    typedef cocos2d::ui::Button UIButton;
    typedef cocos2d::ui::Button UITextButton;
    typedef cocos2d::ui::CheckBox UICheckBox;
    typedef cocos2d::ui::ImageView UIImageView;
    typedef cocos2d::ui::Label UILabel;
    typedef cocos2d::ui::LabelAtlas UILabelAtlas;
    typedef cocos2d::ui::LabelBMFont UILabelBMFont;
    typedef cocos2d::ui::LoadingBar UILoadingBar;
    typedef cocos2d::ui::Slider UISlider;
    typedef cocos2d::ui::TextField UITextField;
    typedef cocos2d::ui::TouchGroup UILayer;
    
    typedef cocos2d::ui::LinearGravity UILinearGravity;
    typedef cocos2d::ui::RelativeAlign UIRelativeAlign;
    typedef cocos2d::ui::LayoutParameter UILayoutParameter;
    typedef cocos2d::ui::LinearLayoutParameter UILinearLayoutParameter;
    typedef cocos2d::ui::RelativeLayoutParameter UIRelativeLayoutParameter;
}
NS_CC_END


#endif /* defined(__CocosGUITest__Cocos__) */
