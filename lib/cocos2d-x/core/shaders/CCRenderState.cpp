#include "CCRenderState.h"
#include "kazmath/mat4.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

void ShaderParam::set1i(int v1)
{
	value.int_ = v1;
	type = INT1;
}

void ShaderParam::set2i(int v1, int v2)
{
	value.int_ = v1;
	value.int2_ = v2;
	type = INT2;
}

void ShaderParam::set3i(int v1, int v2, int v3)
{
	value.int_ = v1;
	value.int2_ = v2;
	value.int3_ = v3;
	type = INT3;
}

void ShaderParam::set4i(int v1, int v2, int v3, int v4)
{
	value.int_ = v1;
	value.int2_ = v2;
	value.int3_ = v3;
	value.int4_ = v4;
	type = INT4;
}

void ShaderParam::set1f(float v1)
{
	value.float_ = v1;
	type = FLOAT1;
}

void ShaderParam::set2f(float v1, float v2)
{
	value.float_ = v1;
	value.float2_ = v2;
	type = FLOAT2;
}

void ShaderParam::set3f(float v1, float v2, float v3)
{
	value.float_ = v1;
	value.float2_ = v2;
	value.float3_ = v3;
	type = FLOAT3;
}

void ShaderParam::set4f(float v1, float v2, float v3, float v4)
{
	value.float_ = v1;
	value.float2_ = v2;
	value.float3_ = v3;
	value.float4_ = v4;
	type = FLOAT4;
}

void ShaderParam::setmat(float* mat)
{
	value.ptr_ = mat;
	type = MAT;
}

void ShaderParam::setSampler(unsigned i)
{
    type = SAMPLER;
}

ShaderParam* RenderState::getParam(const char* name)
{
	const auto& it = _params.find(name);
	if (it == _params.end())
		return nullptr;

	return it->second.Get();
}


RenderState::RenderState(CCGLProgram* program)
{
	_program = program;


	for (auto& itr : program->_uniforms)
	{
		Uniform* u = itr.second.Get();

		SharedPtr<ShaderParam> p(new ShaderParam(u));
		
		_params[itr.first] = p;
	}

	_matrixP = getParam(kCCUniformPMatrix_s);
	_matrixMV = getParam(kCCUniformMVMatrix_s);
	_matrixMVP = getParam(kCCUniformMVPMatrix_s);
    
    ShaderParam* p = getParam(kCCUniformSampler_s);
    // Since sample most probably won't change, set it to 0 now.
    if (p)
        p->setSampler(0);
}

void RenderState::set1i(const char* name, int v1)                                { getParam(name)->set1i(v1); }
void RenderState::set2i(const char* name, int v1, int v2)                        { getParam(name)->set2i(v1, v2); }
void RenderState::set3i(const char* name, int v1, int v2, int v3)                { getParam(name)->set3i(v1, v2, v3); }
void RenderState::set4i(const char* name, int v1, int v2, int v3, int v4)        { getParam(name)->set4i(v1, v2, v3, v4); }
void RenderState::set1f(const char* name, float v1)                              { getParam(name)->set1f(v1); }
void RenderState::set2f(const char* name, float v1, float v2)                    { getParam(name)->set2f(v1, v2); }
void RenderState::set3f(const char* name, float v1, float v2, float v3)          { getParam(name)->set3f(v1, v2, v3); }
void RenderState::set4f(const char* name, float v1, float v2, float v3, float v4){ getParam(name)->set4f(v1, v2, v3, v4); }

void RenderState::use()
{
	kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;

	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);

	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	
	if (_matrixP)
		_matrixP->setmat(matrixP.mat);
	if (_matrixMV)
		_matrixMV->setmat(matrixMV.mat);
	if (_matrixMVP)
		_matrixMVP->setmat(matrixMVP.mat);

	bindValue();
}

void RenderState::bindValue()
{
	_program->use();
	for (const auto& itr : _params)
	{
		ShaderParam* param = itr.second.Get();
		switch (param->type)
		{
		case ShaderParam::INT1: 
			_program->setUniformLocationWith1i(param->_uniform->location, param->value.int_); 
			break;
		case ShaderParam::INT2:
			_program->setUniformLocationWith2i(param->_uniform->location, param->value.int_, param->value.int2_);
			break;
		case ShaderParam::INT3:
			_program->setUniformLocationWith3i(param->_uniform->location, param->value.int_, param->value.int2_, param->value.int3_);
			break;
		case ShaderParam::INT4: 
			_program->setUniformLocationWith4i(param->_uniform->location, param->value.int_, param->value.int2_, param->value.int3_, param->value.int4_);
			break;
		case ShaderParam::FLOAT1:
			_program->setUniformLocationWith1f(param->_uniform->location, param->value.float_);
			break;
		case ShaderParam::FLOAT2:
			_program->setUniformLocationWith2f(param->_uniform->location, param->value.float_, param->value.float2_);
			break;
		case ShaderParam::FLOAT3:
			_program->setUniformLocationWith3f(param->_uniform->location, param->value.float_, param->value.float2_, param->value.float3_);
			break;
		case ShaderParam::FLOAT4:
			_program->setUniformLocationWith4f(param->_uniform->location, param->value.float_, param->value.float2_, param->value.float3_, param->value.float4_);
			break;
		case ShaderParam::MAT:
			_program->setUniformLocationWithMatrix4fv(param->_uniform->location, (GLfloat*)param->value.ptr_, 1);
			break;
        case ShaderParam::SAMPLER:
            _program->setUniformLocationWith1i(param->_uniform->location, 0);
            break;
		default:
			break;
		}
	}
}


NS_CC_END