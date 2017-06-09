#include "CCEGLView.h"
#include "UIEditBox.h"
#include "platform/CCEGLView.h"
#include "SDL.h"
#include "SDL_syswm.h"

#if BUILD_FOR(IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

using namespace cocos2d::ui;

@interface CustomUITextField : UITextField
{
}

@end

@implementation CustomUITextField
- (CGRect)textRectForBounds:(CGRect)bounds {
    float padding = 3;
    return CGRectMake(bounds.origin.x, bounds.origin.y + padding, bounds.size.width - padding, bounds.size.height - padding*2);
}

- (CGRect)editingRectForBounds:(CGRect)bounds {
    return [self textRectForBounds:bounds];
}

@end




@interface EditBoxImplIOSInternal : NSObject <UITextFieldDelegate>
{
    CustomUITextField* textField_;
    EditBox* editBox_;
    BOOL editState_;
}

@property(nonatomic, retain) UITextField* textField;
@property(nonatomic, readonly, getter = isEditState) BOOL editState;
@property(nonatomic, assign) void* editBox;

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox;
-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance;
-(void) openKeyboard;
-(void) closeKeyboard;

@end


@implementation EditBoxImplIOSInternal

@synthesize textField = textField_;
@synthesize editState = editState_;
@synthesize editBox = editBox_;

- (void)dealloc
{
    [textField_ resignFirstResponder];
    [textField_ removeFromSuperview];
    self.textField = NULL;
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox
{
    self = [super init];
    
    do
    {
        if (self == nil) break;
        editState_ = NO;
        self.textField = [[[CustomUITextField alloc] initWithFrame: frameRect] autorelease];
        if (!textField_) break;
        [textField_ setTextColor:[UIColor whiteColor]];
        textField_.font = [UIFont systemFontOfSize:frameRect.size.height*2/3]; //TODO need to delete hard code here.
        textField_.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
        textField_.backgroundColor = [UIColor clearColor];
        textField_.borderStyle = UITextBorderStyleNone;
        textField_.delegate = self;
        textField_.hidden = true;
        textField_.returnKeyType = UIReturnKeyDefault;
        [textField_ addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        self.editBox = editBox;
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillShow:)
                                                     name:UIKeyboardWillShowNotification
                                                   object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(keyboardWillHide:)
                                                     name:UIKeyboardWillHideNotification
                                                   object:nil];
        
        return self;
    }while(0);
    
    return nil;
}

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance
{
//    id eglView = [EAGLView sharedEGLView];
//    [eglView doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
}

-(void) keyboardWillShow:(NSNotification*)notification
{
    
}

-(void) keyboardWillHide:(NSNotification*)notification
{
    
}

-(void) openKeyboard
{
    
    SDL_Window* window = (SDL_Window*)cocos2d::CCEGLView::sharedOpenGLView()->getSDLWindow();
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi) )
    {
        return;
    }
    
    [wmi.info.uikit.window addSubview: textField_];
    [wmi.info.uikit.window bringSubviewToFront: textField_];
    
//    [[EAGLView sharedEGLView] addSubview:textField_];
    [textField_ becomeFirstResponder];
}

-(void) closeKeyboard
{
    [textField_ resignFirstResponder];
    [textField_ removeFromSuperview];
    if (editBox_ != nil)
    {
        editBox_->setText([textField_.text UTF8String]);
        editBox_ = nil;
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)sender
{
    [self closeKeyboard];
    return NO;
}

-(void)animationSelector
{
//    id eglView = [EAGLView sharedEGLView];
//    [eglView doAnimationWhenAnotherEditBeClicked];
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)sender        // return NO to disallow editing.
{
    return YES;
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)sender
{
    //    CCLOG("textFieldShouldEndEditing...");
    editState_ = NO;
    
    return YES;
}

/**
 * Delegate method called before the text has been changed.
 * @param textField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)textField:(UITextField *) textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
//    if (getEditBoxImplIOS()->getMaxLength() < 0)
//    {
//        return YES;
//    }
    
    NSUInteger oldLength = [textField.text length];
    NSUInteger replacementLength = [string length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
//    return newLength <= getEditBoxImplIOS()->getMaxLength();
    return YES;
}

/**
 * Called each time when the text field's text has changed.
 */
- (void) textChanged
{
}

@end

namespace cocos2d {
    static void setInputFlag(UITextField* textField, EditBoxInputFlag inputFlag)
    {
        switch (inputFlag)
        {
            case kEditBoxInputFlagPassword:
                textField.secureTextEntry = YES;
                break;
            case kEditBoxInputFlagInitialCapsWord:
                textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
                break;
            case kEditBoxInputFlagInitialCapsSentence:
                textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                break;
            case kEditBoxInputFlagInitialCapsAllCharacters:
                textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
                break;
            case kEditBoxInputFlagSensitive:
                textField.autocorrectionType = UITextAutocorrectionTypeNo;
                break;
            default:
                break;
        }
    }
    
    static UIReturnKeyType setReturnType(KeyboardReturnType returnType)
    {
        switch (returnType) {
            case kKeyboardReturnTypeDefault: return UIReturnKeyDefault;
            case kKeyboardReturnTypeDone:    return UIReturnKeyDone;
            case kKeyboardReturnTypeSend:    return UIReturnKeySend;
            case kKeyboardReturnTypeSearch:  return UIReturnKeySearch;
            case kKeyboardReturnTypeGo:      return UIReturnKeyGo;
        }
        return UIReturnKeyDefault;
    }
    
    static UIKeyboardType setInputMode(EditBoxInputMode inputMode)
    {
        switch (inputMode)
        {
            case kEditBoxInputModeEmailAddr:   return UIKeyboardTypeEmailAddress;
            case kEditBoxInputModeNumeric:     return UIKeyboardTypeNumbersAndPunctuation;
            case kEditBoxInputModePhoneNumber: return UIKeyboardTypePhonePad;
            case kEditBoxInputModeUrl:         return UIKeyboardTypeURL;
            case kEditBoxInputModeDecimal:     return UIKeyboardTypeDecimalPad;
            case kEditBoxInputModeSingleLine:  return UIKeyboardTypeDefault;
        }
        return UIKeyboardTypeDefault;
    }
    
    static float getScaleFactor()
    {
        CCEGLView* eglView = CCEGLView::sharedOpenGLView();
        const CCSize& winSize = eglView->getFrameSize();
        const CCSize& glSize = eglView->getDrawableSize();
        
        return glSize.width / winSize.width;
    }
    
    static CGPoint convertDesignCoordToScreenCoord(const Vec2& designCoord)
    {
        CCEGLView* eglView = CCEGLView::sharedOpenGLView();
        
        float viewH = eglView->getFrameSize().height;// = (float)[[EAGLView sharedEGLView] getHeight];
        
        Vec2 screenGLPos = ccp(designCoord.x, designCoord.y);
        CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
        //        CCLog("[EditBox] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
        return screenPos;
    }

    static EditBoxImplIOSInternal* impl = nil;
    void openKeyboard(ui::EditBox* editbox)
    {
        if (impl == nil) {
            impl = [[EditBoxImplIOSInternal alloc] initWithFrame:CGRectMake(0, 0, 1, 1)
                                                         editBox:editbox];
        }
        
        if (editbox != impl.editBox)
            [impl closeKeyboard];
        
        ccColor3B color = editbox->getColor();
        impl.textField.textColor =[UIColor colorWithRed:color.r / 255.0f green:color.g / 255.0f blue:color.b / 255.0f alpha:1.0f];
        impl.textField.placeholder = [NSString stringWithUTF8String:editbox->getPlaceHolder()];
        impl.textField.hidden = NO;
        impl.textField.text = [NSString stringWithUTF8String: editbox->getText()];
        impl.editBox = editbox;
        
        setInputFlag(impl.textField, editbox->getInputFlag());
        impl.textField.keyboardType = setInputMode(editbox->getInputMode());
        impl.textField.returnKeyType = setReturnType(editbox->getReturnType());
        
        float scale = getScaleFactor();
        
        {
            NSString * fntName = [NSString stringWithUTF8String: editbox->getFontName()];
            int fontSize = editbox->getFontSize();
            UIFont* textFont = [UIFont fontWithName: fntName size: fontSize / scale];
            
            [impl.textField setFont:textFont];
        }
        
        {
            CCSize size = editbox->getSize();
            CGRect frame = [impl.textField frame];
            
            Vec2 anchor = editbox->getAnchorPoint();
            float left = -size.width * anchor.x;
            float cy = size.height * (0.5 - anchor.y) - size.height / 2;
            CCRect rect = CCRect(left, cy, size.width, size.height);
            rect = CCRectApplyAffineTransform(rect, editbox->nodeToWorldTransform());
            
            frame.origin = convertDesignCoordToScreenCoord(Vec2(rect.origin.x, rect.origin.y + rect.size.height));
            frame.size = CGSizeMake(rect.size.width, rect.size.height);
            
            [impl.textField setFrame: frame];
        }
        
        [impl openKeyboard];
    }
                                                       
    void closeKeyboard(ui::EditBox* editbox)
    {
        if (impl.editBox == editbox)
        {
            [impl closeKeyboard];
        }
    }

}
#endif
