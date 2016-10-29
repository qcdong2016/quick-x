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


#define GL_ASSERT( gl_code ) do \
    { \
        gl_code; \
        GLuint __gl_error_code = glGetError(); \
        CC_ASSERT(__gl_error_code == GL_NO_ERROR); \
    } while(0)


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
{
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
	CCAssert(fShaderByteArray, "fShaderByteArray == NULL");
	CCAssert(vShaderByteArray, "fShaderByteArray == NULL");

	_fragSrc = fShaderByteArray;
	_vertSrc = vShaderByteArray;
	return loadWithSource();
}

bool cocos2d::CCGLProgram::loadWithSource()
{
	m_uProgram = glCreateProgram();
	CHECK_GL_ERROR_DEBUG();

	m_uVertShader = m_uFragShader = 0;
	if (!compileShader(&m_uVertShader, GL_VERTEX_SHADER, _vertSrc.c_str()))
	{
		CCLOG("cocos2d: ERROR: Failed to compile vertex shader");
		return false;
	}

	// Create and compile fragment shader
	if (!compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, _fragSrc.c_str()))
	{
		CCLOG("cocos2d: ERROR: Failed to compile fragment shader");
		return false;
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

	link();
	updateUniforms();

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
    this->use();
    // Since sample most probably won't change, set it to 0 now.
    this->setUniformLocationWith1i(glGetUniformLocation(m_uProgram, kCCUniformSampler_s), 0);
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
	
	GLint attribLocation;

	attribLocation = glGetAttribLocation(m_uProgram, kCCAttributeNameTexCoord);
	if (attribLocation != -1)
		_vertexAttributes[kCCVertexAttrib_TexCoords] = attribLocation;

	attribLocation = glGetAttribLocation(m_uProgram, kCCAttributeNameColor);
	if (attribLocation != -1)
		_vertexAttributes[kCCVertexAttrib_Color] = attribLocation;

	attribLocation = glGetAttribLocation(m_uProgram, kCCAttributeNamePosition);
	if (attribLocation != -1)
		_vertexAttributes[kCCVertexAttrib_Position] = attribLocation;


	// Query and store uniforms from the program.
	GLint activeUniforms;
	GLint length;
	
	GL_ASSERT(glGetProgramiv(m_uProgram, GL_ACTIVE_UNIFORMS, &activeUniforms));
	if (activeUniforms > 0)
	{
		GL_ASSERT(glGetProgramiv(m_uProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length));
		if (length > 0)
		{
			GLchar* uniformName = new GLchar[length + 1];
			GLint uniformSize;
			GLenum uniformType;
			GLint uniformLocation;
			unsigned int samplerIndex = 0;
			for (int i = 0; i < activeUniforms; ++i)
			{
				// Query uniform info.
				GL_ASSERT(glGetActiveUniform(m_uProgram, i, length, NULL, &uniformSize, &uniformType, uniformName));
				uniformName[length] = '\0';  // null terminate
				if (length > 3)
				{
					// If this is an array uniform, strip array indexers off it since GL does not
					// seem to be consistent across different drivers/implementations in how it returns
					// array uniforms. On some systems it will return "u_matrixArray", while on others
					// it will return "u_matrixArray[0]".
					char* c = strrchr(uniformName, '[');
					if (c)
					{
						*c = '\0';
					}
				}

				// Query the pre-assigned uniform location.
				GL_ASSERT(uniformLocation = glGetUniformLocation(m_uProgram, uniformName));

				SharedPtr<Uniform> uniform(new Uniform());
				uniform->program = this;
				uniform->name = uniformName;
				uniform->location = uniformLocation;
				uniform->type = uniformType;
				if (uniformType == GL_SAMPLER_2D || uniformType == GL_SAMPLER_CUBE)
				{
					samplerIndex += uniformSize;
				}

				_uniforms[uniformName] = uniform;
			}
			CC_SAFE_DELETE_ARRAY(uniformName);
		}
	}

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

bool CCGLProgram::getAttribLocation(GLuint type, GLuint& loc)
{
	auto& it = _vertexAttributes.find(type);
	if (it != _vertexAttributes.end()) {
		loc = it->second;
		return true;
	}
	return false;
}

void CCGLProgram::use()
{
    ccGLUseProgram(this);
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
    
}

void CCGLProgram::reset()
{
    m_uVertShader = m_uFragShader = 0;
	_uniforms.clear();
	_vertexAttributes.clear();
    m_uProgram = 0;
}

NS_CC_END
