/****************************************************************************
Copyright 2012 cocos2d-x.org
Copyright 2011 Jeff Lamarche
Copyright 2012 Goffredo Marocchi
Copyright 2012 Ricardo Quesada

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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCDirector.h"
#include "CCGLProgram.h"
#include "ccGLStateCache.h"
#include "ccMacros.h"
#include "support/data_support/uthash.h"
#include "cocoa/CCString.h"
// extern
#include "kazmath/GL/matrix.h"
#include "kazmath/kazmath.h"
#include "IO/FileSystem.h"

NS_CC_BEGIN

typedef struct _hashUniformEntry
{
    GLvoid*         value;       // value
    unsigned int    location;    // Key
    UT_hash_handle  hh;          // hash entry
} tHashUniformEntry;

CCGLProgram::CCGLProgram()
: m_uProgram(0)
, m_uVertShader(0)
, m_uFragShader(0)
, m_bUsesTime(false)
, m_hasShaderCompiler(true)
{
    memset(m_uUniforms, 0, sizeof(m_uUniforms));
}

CCGLProgram::~CCGLProgram()
{
    CCLOGINFO("cocos2d: %s %d deallocing 0x%X", __FUNCTION__, __LINE__, this);

    // there is no need to delete the shaders. They should have been already deleted.
    CCAssert(m_uVertShader == 0, "Vertex Shaders should have been already deleted");
    CCAssert(m_uFragShader == 0, "Fragment Shaders should have been already deleted");

    if (m_uProgram) 
    {
        ccGLDeleteProgram(m_uProgram);
    }
}

bool CCGLProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    m_uProgram = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    m_uVertShader = m_uFragShader = 0;

    if (vShaderByteArray)
    {
        if (!compileShader(&m_uVertShader, GL_VERTEX_SHADER, vShaderByteArray))
        {
            CCLOG("cocos2d: ERROR: Failed to compile vertex shader");
 			return false;
       }
    }

    // Create and compile fragment shader
    if (fShaderByteArray)
    {
        if (!compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray))
        {
            CCLOG("cocos2d: ERROR: Failed to compile fragment shader");
			return false;
        }
    }

    if (m_uVertShader)
    {
        glAttachShader(m_uProgram, m_uVertShader);
    }
    CHECK_GL_ERROR_DEBUG();

    if (m_uFragShader)
    {
        glAttachShader(m_uProgram, m_uFragShader);
    }
    
    CHECK_GL_ERROR_DEBUG();

    return true;
}

bool CCGLProgram::initWithVertexShaderFilename(const char* vShaderFilename, const char* fShaderFilename)
{
    const GLchar * vertexSource = (GLchar*) CCString::createWithContentsOfFile(FileSystem::fullPathOfFile(vShaderFilename).c_str())->getCString();
    const GLchar * fragmentSource = (GLchar*) CCString::createWithContentsOfFile(FileSystem::fullPathOfFile(fShaderFilename).c_str())->getCString();

    return initWithVertexShaderByteArray(vertexSource, fragmentSource);
}

const char* CCGLProgram::description()
{
    return CCString::createWithFormat("<CCGLProgram = "
                                      CC_FORMAT_PRINTF_SIZE_T
                                      " | Program = %i, VertexShader = %i, FragmentShader = %i>",
                                      (size_t)this, m_uProgram, m_uVertShader, m_uFragShader)->getCString();
}

bool CCGLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#if CC_TARGET_PLATFORM == CC_PLATFORM_NACL
        "precision highp float;\n"
#else
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
    	GLsizei log_length,src_length;
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &log_length);
	glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &src_length);

	GLchar* log = (GLchar *)malloc(sizeof(GLchar) * log_length);
	GLchar* src = (GLchar *)malloc(sizeof(GLchar) * src_length);


	glGetShaderInfoLog(*shader,log_length,NULL,log); //get shader compile error info

	glGetShaderSource(*shader, src_length, NULL, src);	//get shader source code

	CCLOG("cocos2d: ERROR: Failed to compile shader:\n%s\nerror log:%s\n", src,log);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLOG("cocos2d: %s", vertexShaderLog());
        }
        else
        {
            CCLOG("cocos2d: %s", fragmentShaderLog());
        }
        free(log);
        free(src);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        return false;
#else
		abort();
#endif
    }
    return (status == GL_TRUE);
}

void CCGLProgram::addAttribute(const char* attributeName, GLuint index)
{
    glBindAttribLocation(m_uProgram, index, attributeName);
}

void CCGLProgram::updateUniforms()
{
    m_uUniforms[kCCUniformPMatrix] = glGetUniformLocation(m_uProgram, kCCUniformPMatrix_s);
	m_uUniforms[kCCUniformMVMatrix] = glGetUniformLocation(m_uProgram, kCCUniformMVMatrix_s);
	m_uUniforms[kCCUniformMVPMatrix] = glGetUniformLocation(m_uProgram, kCCUniformMVPMatrix_s);
	
	m_uUniforms[kCCUniformTime] = glGetUniformLocation(m_uProgram, kCCUniformTime_s);
	m_uUniforms[kCCUniformSinTime] = glGetUniformLocation(m_uProgram, kCCUniformSinTime_s);
	m_uUniforms[kCCUniformCosTime] = glGetUniformLocation(m_uProgram, kCCUniformCosTime_s);
	
	m_bUsesTime = (
                 m_uUniforms[kCCUniformTime] != -1 ||
                 m_uUniforms[kCCUniformSinTime] != -1 ||
                 m_uUniforms[kCCUniformCosTime] != -1
                 );
    
	m_uUniforms[kCCUniformRandom01] = glGetUniformLocation(m_uProgram, kCCUniformRandom01_s);

    m_uUniforms[kCCUniformSampler] = glGetUniformLocation(m_uProgram, kCCUniformSampler_s);

    this->use();
    
    // Since sample most probably won't change, set it to 0 now.
    this->setUniformLocationWith1i(m_uUniforms[kCCUniformSampler], 0);
}

bool CCGLProgram::link()
{
    CCAssert(m_uProgram != 0, "Cannot link invalid program");

    GLint status = GL_TRUE;
    
    glLinkProgram(m_uProgram);

    if (m_uVertShader)
    {
        glDeleteShader(m_uVertShader);
    }
    
    if (m_uFragShader)
    {
        glDeleteShader(m_uFragShader);
    }
    
    m_uVertShader = m_uFragShader = 0;
	
#if _DEBUG
    glGetProgramiv(m_uProgram, GL_LINK_STATUS, &status);
	
    if (status == GL_FALSE)
    {
        CCLOG("cocos2d: ERROR: Failed to link program: %i", m_uProgram);
        ccGLDeleteProgram(m_uProgram);
        m_uProgram = 0;
    }
#endif

    return (status == GL_TRUE);
}

void CCGLProgram::use()
{
    ccGLUseProgram(m_uProgram);
}

const char* CCGLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc)
{
    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return 0;

    char *logBytes = (char*)malloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);

    CCString* log = CCString::create(logBytes);

    free(logBytes);
    return log->getCString();
}

const char* CCGLProgram::vertexShaderLog()
{
    return this->logForOpenGLObject(m_uVertShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* CCGLProgram::fragmentShaderLog()
{
    return this->logForOpenGLObject(m_uFragShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* CCGLProgram::programLog()
{
    return this->logForOpenGLObject(m_uProgram, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

GLint CCGLProgram::getUniformLocationForName(const char* name)
{
    CCAssert(name != NULL, "Invalid uniform name" );
    CCAssert(m_uProgram != 0, "Invalid operation. Cannot get uniform location when program is not initialized");
    
    return glGetUniformLocation(m_uProgram, name);
}

void CCGLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    glUniform1i( (GLint)location, i1);
}

void CCGLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
    glUniform2i( (GLint)location, i1, i2);
}

void CCGLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
    glUniform3i( (GLint)location, i1, i2, i3);
}

void CCGLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    glUniform4i( (GLint)location, i1, i2, i3, i4);
}

void CCGLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    glUniform2iv( (GLint)location, (GLsizei)numberOfArrays, ints );
}

void CCGLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    glUniform3iv( (GLint)location, (GLsizei)numberOfArrays, ints );
}

void CCGLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    glUniform4iv( (GLint)location, (GLsizei)numberOfArrays, ints );
}

void CCGLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
    glUniform1f( (GLint)location, f1);
}

void CCGLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
    glUniform2f( (GLint)location, f1, f2);
}

void CCGLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    glUniform3f( (GLint)location, f1, f2, f3);
}

void CCGLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    glUniform4f( (GLint)location, f1, f2, f3,f4);
}

void CCGLProgram::setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
}

void CCGLProgram::setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
}

void CCGLProgram::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
}


void CCGLProgram::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
}

void CCGLProgram::setUniformsForBuiltins()
{
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
    setUniformLocationWithMatrix4fv(m_uUniforms[kCCUniformPMatrix], matrixP.mat, 1);
    setUniformLocationWithMatrix4fv(m_uUniforms[kCCUniformMVMatrix], matrixMV.mat, 1);
    setUniformLocationWithMatrix4fv(m_uUniforms[kCCUniformMVPMatrix], matrixMVP.mat, 1);
	
	if(m_bUsesTime)
    {
		CCDirector *director = CCDirector::sharedDirector();
		// This doesn't give the most accurate global time value.
		// Cocos2D doesn't store a high precision time value, so this will have to do.
		// Getting Mach time per frame per shader using time could be extremely expensive.
        float time = director->getTotalFrames() * director->getAnimationInterval();
		
        setUniformLocationWith4f(m_uUniforms[kCCUniformTime], time/10.0, time, time*2, time*4);
        setUniformLocationWith4f(m_uUniforms[kCCUniformSinTime], time/8.0, time/4.0, time/2.0, sinf(time));
        setUniformLocationWith4f(m_uUniforms[kCCUniformCosTime], time/8.0, time/4.0, time/2.0, cosf(time));
	}
	
	if (m_uUniforms[kCCUniformRandom01] != -1)
    {
        setUniformLocationWith4f(m_uUniforms[kCCUniformRandom01], CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
	}
}

void CCGLProgram::reset()
{
    m_uVertShader = m_uFragShader = 0;
    memset(m_uUniforms, 0, sizeof(m_uUniforms));
    // it is already deallocated by android
    //ccGLDeleteProgram(m_uProgram);
    m_uProgram = 0;
}

NS_CC_END
