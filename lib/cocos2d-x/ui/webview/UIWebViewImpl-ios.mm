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
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import <Foundation/Foundation.h>
#import <UIKit/UIWebView.h>


#include "UIWebViewImpl-ios.h"
#include "UIWebView.h"
//#include "platform/ios/EAGLView.h"
#include "engine/CCDirector.h"
#include "engine/CCFileSystem.h"
#include "CCEGLView.h"

#include "SDL.h"
#include "SDL_syswm.h"
#include "nodes/CCDrawingPrimitives.h"

static std::string getFixedBaseUrl(const std::string& baseUrl)
{
    std::string fixedBaseUrl;
    if (baseUrl.empty() || baseUrl.c_str()[0] != '/') {
        fixedBaseUrl = [[[NSBundle mainBundle] resourcePath] UTF8String];
        fixedBaseUrl += "/";
        fixedBaseUrl += baseUrl;
    }
    else {
        fixedBaseUrl = baseUrl;
    }
    
    size_t pos = 0;
    while ((pos = fixedBaseUrl.find(" ")) != std::string::npos) {
        fixedBaseUrl.replace(pos, 1, "%20");
    }
    
    if (fixedBaseUrl.c_str()[fixedBaseUrl.length() - 1] != '/') {
        fixedBaseUrl += "/";
    }
    
    return fixedBaseUrl;
}

@interface UIWebViewWrapper : NSObject<UIWebViewDelegate>
@property (nonatomic) std::function<bool(std::string url)> shouldStartLoading;
@property (nonatomic) std::function<void(std::string url)> didFinishLoading;
@property (nonatomic) std::function<void(std::string url)> didFailLoading;
@property (nonatomic) std::function<void(std::string url)> onJsCallback;

@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

@property(nonatomic, retain) UIWebView *uiWebView;
@property(nonatomic, copy) NSString *jsScheme;

+ (instancetype)webViewWrapper;

- (void)setVisible:(bool)visible;

- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height;

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme;

- (void)loadData:(const std::string &)data MIMEType:(const std::string &)MIMEType textEncodingName:(const std::string &)encodingName baseURL:(const std::string &)baseURL;

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL;

- (void)loadUrl:(const std::string &)urlString;

- (void)loadFile:(const std::string &)filePath;

- (void)stopLoading;

- (void)reload;

- (NSString*)evaluateJS:(const std::string &)js;

- (void)goBack;

- (void)goForward;

- (void)setScalesPageToFit:(const bool)scalesPageToFit;

- (void)setOpaque: (bool) yes;
@end


@implementation UIWebViewWrapper {
    
}

+ (instancetype)webViewWrapper {
    return [[self alloc] init] ;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.uiWebView = nil;
        self.shouldStartLoading = nullptr;
        self.didFinishLoading = nullptr;
        self.didFailLoading = nullptr;
    }
    return self;
}

- (void)dealloc {
    self.uiWebView.delegate = nil;
    [self.uiWebView removeFromSuperview];
    self.uiWebView = nil;
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView {
    if (!self.uiWebView) {
        self.uiWebView = [[[UIWebView alloc] init] autorelease];
        self.uiWebView.delegate = self;
        self.uiWebView.detectsPhoneNumbers = NO;
    }
    if (!self.uiWebView.superview) {
        SDL_Window* window = (SDL_Window*)cocos2d::CCEGLView::sharedOpenGLView()->getSDLWindow();
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(window, &wmi) )
        {
            return;
        }
        
        [wmi.info.uikit.window addSubview: self.uiWebView];
        [wmi.info.uikit.window bringSubviewToFront:  self.uiWebView];
        
        //    [[EAGLView sharedEGLView] addSubview:textField_];
        //[textField_ becomeFirstResponder];
//        EAGLView* eaglview = [EAGLView sharedEGLView];
//        [eaglview addSubview:self.uiWebView];
    }
}

- (void)setVisible:(bool)visible {
    self.uiWebView.hidden = !visible;
}

- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height {
    if (!self.uiWebView) {[self setupWebView];}
    CGRect newFrame = CGRectMake(x, y, width, height);
    if (!CGRectEqualToRect(self.uiWebView.frame, newFrame)) {
        self.uiWebView.frame = CGRectMake(x, y, width, height);
    }
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme {
    self.jsScheme = @(scheme.c_str());
}

- (void)loadData:(const std::string &)data MIMEType:(const std::string &)MIMEType textEncodingName:(const std::string &)encodingName baseURL:(const std::string &)baseURL {
    [self.uiWebView loadData:[NSData dataWithBytes:data.c_str() length:data.length()]
                    MIMEType:@(MIMEType.c_str())
            textEncodingName:@(encodingName.c_str())
                     baseURL:[NSURL URLWithString:@(getFixedBaseUrl(baseURL).c_str())]];
}

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL {
    if (!self.uiWebView) {[self setupWebView];}
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(getFixedBaseUrl(baseURL).c_str())]];
}

- (void)loadUrl:(const std::string &)urlString {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)loadFile:(const std::string &)filePath {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)stopLoading {
    [self.uiWebView stopLoading];
}

- (void)reload {
    [self.uiWebView reload];
}

- (BOOL)canGoForward {
    return self.uiWebView.canGoForward;
}

- (BOOL)canGoBack {
    return self.uiWebView.canGoBack;
}

- (void)goBack {
    [self.uiWebView goBack];
}

- (void)goForward {
    [self.uiWebView goForward];
}

- (NSString*)evaluateJS:(const std::string &)js {
    if (!self.uiWebView) {[self setupWebView];}
    
    return [self.uiWebView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit {
    if (!self.uiWebView) {[self setupWebView];}
    self.uiWebView.scalesPageToFit = scalesPageToFit;
}

- (void)setOpaque: (bool) yes {
    if (!self.uiWebView) {[self setupWebView];}
    [self.uiWebView setBackgroundColor: [UIColor clearColor]];
    [self.uiWebView setOpaque: yes];
}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *url = [[request URL] absoluteString];
    if ([[[request URL] scheme] isEqualToString:self.jsScheme]) {
        self.onJsCallback([url UTF8String]);
        return NO;
    }
    if (self.shouldStartLoading && url) {
        return self.shouldStartLoading([url UTF8String]);
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    if (self.didFinishLoading) {
        NSString *url = [[webView.request URL] absoluteString];
        self.didFinishLoading([url UTF8String]);
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    if (self.didFailLoading) {
        NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
        if (url) {
            self.didFailLoading([url UTF8String]);
        }
    }
}

@end


namespace cocos2d {
    namespace ui {
        
WebViewImpl::WebViewImpl(WebView *webView)
        : _webView(webView)
{
    
    id ww = [UIWebViewWrapper webViewWrapper];
    _uiWebViewWrapper = (void*)ww;
    
    UIWebViewWrapper* webview = (UIWebViewWrapper*)_uiWebViewWrapper;
            
    webview.shouldStartLoading = [this](std::string url) {
        if (this->_webView->_onShouldStartLoading) {
            return this->_webView->_onShouldStartLoading(this->_webView, url);
        }
        return true;
    };
    webview.didFinishLoading = [this](std::string url) {
        if (this->_webView->_onDidFinishLoading) {
            this->_webView->_onDidFinishLoading(this->_webView, url);
        }
    };
    webview.didFailLoading = [this](std::string url) {
        if (this->_webView->_onDidFailLoading) {
            this->_webView->_onDidFailLoading(this->_webView, url);
        }
    };
    webview.onJsCallback = [this](std::string url) {
        if (this->_webView->_onJSCallback) {
            this->_webView->_onJSCallback(this->_webView, url);
        }
    };
}

WebViewImpl::~WebViewImpl(){
    [(id)_uiWebViewWrapper release];
    _uiWebViewWrapper = nullptr;
}

void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    [(id)_uiWebViewWrapper setJavascriptInterfaceScheme:scheme];
}

void WebViewImpl::loadData(const std::string &dataString,
                           const std::string &MIMEType,
                           const std::string &encoding,
                           const std::string &baseURL) {
    
    [(id)_uiWebViewWrapper loadData:dataString MIMEType:MIMEType textEncodingName:encoding baseURL:baseURL];
}

void WebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
    [(id)_uiWebViewWrapper loadHTMLString:string baseURL:baseURL];
}

void WebViewImpl::loadURL(const std::string &url) {
    [(id)_uiWebViewWrapper loadUrl:url];
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = FileSystem::fullPathOfFile(fileName);
    [(id)_uiWebViewWrapper loadFile:fullPath];
}

void WebViewImpl::stopLoading() {
    [(id)_uiWebViewWrapper stopLoading];
}

void WebViewImpl::reload() {
    [(id)_uiWebViewWrapper reload];
}

bool WebViewImpl::canGoBack() {
    UIWebViewWrapper* webview = (UIWebViewWrapper*)_uiWebViewWrapper;
    return webview.canGoBack;
}

bool WebViewImpl::canGoForward() {
    UIWebViewWrapper* webview = (UIWebViewWrapper*)_uiWebViewWrapper;
    return webview.canGoForward;
}

void WebViewImpl::goBack() {
    [(id)_uiWebViewWrapper goBack];
}

void WebViewImpl::goForward() {
    [(id)_uiWebViewWrapper goForward];
}

std::string WebViewImpl::evaluateJS(const std::string &js) {
    return [[(id)_uiWebViewWrapper evaluateJS:js] UTF8String];
}

void WebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
    [(id)_uiWebViewWrapper setScalesPageToFit:scalesPageToFit];
}

void WebViewImpl::setOpaque(bool yes)
{
    [(id)_uiWebViewWrapper setOpaque : yes];
}
        
static Vec2 convertDesignCoordToScreenCoord(const Vec2& designCoord)
{
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    
    float viewH = eglView->getFrameSize().height;// = (float)[[EAGLView sharedEGLView] getHeight];
    
    Vec2 screenGLPos = ccp(designCoord.x, designCoord.y);
    return Vec2(screenGLPos.x, viewH - screenGLPos.y);
}
        
void WebViewImpl::draw()
{
    CCSize size = this->_webView->getSize();
    
    Vec2 anchor = Vec2(0, 0);
    float left = -size.width * anchor.x;
    float cy   = size.height * (0.5 - anchor.y) -size.height / 2;
    CCRect rect = CCRect(left, cy, size.width, size.height);
    
    rect = CCRectApplyAffineTransform(rect, this->_webView->nodeToWorldTransform());
    
    Vec2 pos = convertDesignCoordToScreenCoord(Vec2(rect.origin.x, rect.origin.y + rect.size.height));
    
    [_uiWebViewWrapper setFrameWithX: pos.x
                                   y: pos.y
                               width: rect.size.width
                              height: rect.size.height];
}

void WebViewImpl::setVisible(bool visible){
    [(id)_uiWebViewWrapper setVisible:visible];
}

}}

#endif
