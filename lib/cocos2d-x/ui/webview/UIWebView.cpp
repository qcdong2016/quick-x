
#include "platform/CCPlatformConfig.h"

/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
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
#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "UIWebViewImpl-android.h"
#elif ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "UIWebViewImpl-ios.h"
#else
#include "UIWebViewImpl-dummy.h"
#endif

#include "UIWebView.h"


namespace cocos2d {
    namespace ui {
        
        WebView::WebView()
        : _impl(new WebViewImpl(this)),
        _onJSCallback(nullptr),
        _onShouldStartLoading(nullptr),
        _onDidFinishLoading(nullptr),
        _onDidFailLoading(nullptr)
        {
        }
        
        WebView::~WebView()
        {
            CC_SAFE_DELETE(_impl);
        }
        
        WebView *WebView::create()
        {
            auto webView = new(std::nothrow) WebView();
            if (webView && webView->init())
            {
                webView->autorelease();
                return webView;
            }
            CC_SAFE_DELETE(webView);
            return nullptr;
        }
        
        void WebView::setJavascriptInterfaceScheme(const std::string &scheme)
        {
            _impl->setJavascriptInterfaceScheme(scheme);
        }
        
        void WebView::loadData(const std::string &data,
                               const std::string &MIMEType,
                               const std::string &encoding,
                               const std::string &baseURL)
        {
            _impl->loadData(data, MIMEType, encoding, baseURL);
        }
        
        void WebView::loadHTMLString(const std::string &string, const std::string &baseURL)
        {
            _impl->loadHTMLString(string, baseURL);
        }
        
        void WebView::loadURL(const std::string &url)
        {
            _impl->loadURL(url);
        }
        
        void WebView::loadFile(const std::string &fileName)
        {
            _impl->loadFile(fileName);
        }
        
        void WebView::stopLoading()
        {
            _impl->stopLoading();
        }
        
        void WebView::reload()
        {
            _impl->reload();
        }
        
        bool WebView::canGoBack()
        {
            return _impl->canGoBack();
        }
        
        bool WebView::canGoForward()
        {
            return _impl->canGoForward();
        }
        
        void WebView::goBack()
        {
            _impl->goBack();
        }
        
        void WebView::goForward()
        {
            _impl->goForward();
        }
        
        void WebView::evaluateJS(const std::string &js)
        {
            _impl->evaluateJS(js);
        }
        
        void WebView::setScalesPageToFit(bool const scalesPageToFit)
        {
            _impl->setScalesPageToFit(scalesPageToFit);
        }
        
        void WebView::draw()
        {
            cocos2d::ui::Layout::draw();
            _impl->draw();
        }
        
        void WebView::setVisible(bool visible)
        {
            CCNode::setVisible(visible);
            _impl->setVisible(visible);
        }
        
        void WebView::setOpaque(bool yes)
        {
            _impl->setOpaque(yes);
        }
        
        cocos2d::ui::Widget* WebView::createCloneInstance()
        {
            return WebView::create();
        }
        
        void WebView::copySpecialProperties(Widget* model)
        {
            WebView* webView = dynamic_cast<WebView*>(model);
            if (webView)
            {
                this->_impl = webView->_impl;
                this->_onShouldStartLoading = webView->_onShouldStartLoading;
                this->_onDidFinishLoading = webView->_onDidFinishLoading;
                this->_onDidFailLoading = webView->_onDidFailLoading;
                this->_onJSCallback = webView->_onJSCallback;
            }
        }
        
        void WebView::setOnDidFailLoading(const ccWebViewCallback &callback)
        {
            _onDidFailLoading = callback;
        }
        
        void WebView::setOnDidFinishLoading(const ccWebViewCallback &callback)
        {
            _onDidFinishLoading = callback;
        }
        
        void WebView::setOnShouldStartLoading(const std::function<bool(WebView *sender, const std::string &url)> &callback)
        {
            _onShouldStartLoading = callback;
        }
        
        void WebView::setOnJSCallback(const ccWebViewCallback &callback)
        {
            _onJSCallback = callback;
        }
        
        std::function<bool(WebView *sender, const std::string &url)> WebView::getOnShouldStartLoading()const
        {
            return _onShouldStartLoading;
        }
        
        WebView::ccWebViewCallback WebView::getOnDidFailLoading()const
        {
            return _onDidFailLoading;
        }
        
        WebView::ccWebViewCallback WebView::getOnDidFinishLoading()const
        {
            return _onDidFinishLoading;
        }
        
        WebView::ccWebViewCallback WebView::getOnJSCallback()const
        {
            return _onJSCallback;
        }
        
    }
}

/// @endcond
