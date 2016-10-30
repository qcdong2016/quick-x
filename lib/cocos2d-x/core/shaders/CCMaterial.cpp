#include "CCMaterial.h"
#include "kazmath/mat4.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

void MaterialParam::set1i(int v1)
{
	value.int_ = v1;
	type = INT1;
}

void MaterialParam::set2i(int v1, int v2)
{
	value.int_ = v1;
	value.int2_ = v2;
	type = INT2;
}

void MaterialParam::set3i(int v1, int v2, int v3)
{
	value.int_ = v1;
	value.int2_ = v2;
	value.int3_ = v3;
	type = INT3;
}

void MaterialParam::set4i(int v1, int v2, int v3, int v4)
{
	value.int_ = v1;
	value.int2_ = v2;
	value.int3_ = v3;
	value.int4_ = v4;
	type = INT4;
}

void MaterialParam::set1f(float v1)
{
	value.float_ = v1;
	type = FLOAT1;
}

void MaterialParam::set2f(float v1, float v2)
{
	value.float_ = v1;
	value.float2_ = v2;
	type = FLOAT2;
}

void MaterialParam::set3f(float v1, float v2, float v3)
{
	value.float_ = v1;
	value.float2_ = v2;
	value.float3_ = v3;
	type = FLOAT3;
}

void MaterialParam::set4f(float v1, float v2, float v3, float v4)
{
	value.float_ = v1;
	value.float2_ = v2;
	value.float3_ = v3;
	value.float4_ = v4;
	type = FLOAT4;
}

void MaterialParam::setmat(float* mat)
{
	value.ptr_ = mat;
	type = MAT;
}

void MaterialParam::setSampler(unsigned i)
{
    type = SAMPLER;
}

MaterialParam* Material::getParam(const char* name)
{
	const auto& it = _params.find(name);
	if (it == _params.end())
		return nullptr;

	return it->second.Get();
}


Material::Material(CCGLProgram* program)
{
	_program = program;


	for (auto& itr : program->_uniforms)
	{
		Uniform* u = itr.second.Get();

		SharedPtr<MaterialParam> p(new MaterialParam(u));
		
		_params[itr.first] = p;
	}

	_matrixP = getParam(kCCUniformPMatrix_s);
	_matrixMV = getParam(kCCUniformMVMatrix_s);
	_matrixMVP = getParam(kCCUniformMVPMatrix_s);
    
    MaterialParam* p = getParam(kCCUniformSampler_s);
    // Since sample most probably won't change, set it to 0 now.
    if (p)
        p->setSampler(0);
}

void Material::set1i(const char* name, int v1)                                { getParam(name)->set1i(v1); }
void Material::set2i(const char* name, int v1, int v2)                        { getParam(name)->set2i(v1, v2); }
void Material::set3i(const char* name, int v1, int v2, int v3)                { getParam(name)->set3i(v1, v2, v3); }
void Material::set4i(const char* name, int v1, int v2, int v3, int v4)        { getParam(name)->set4i(v1, v2, v3, v4); }
void Material::set1f(const char* name, float v1)                              { getParam(name)->set1f(v1); }
void Material::set2f(const char* name, float v1, float v2)                    { getParam(name)->set2f(v1, v2); }
void Material::set3f(const char* name, float v1, float v2, float v3)          { getParam(name)->set3f(v1, v2, v3); }
void Material::set4f(const char* name, float v1, float v2, float v3, float v4){ getParam(name)->set4f(v1, v2, v3, v4); }

void Material::use()
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

void Material::bindValue()
{
	_program->use();
	for (const auto& itr : _params)
	{
		MaterialParam* param = itr.second.Get();
		switch (param->type)
		{
		case MaterialParam::INT1: 
			_program->setUniformLocationWith1i(param->_uniform->location, param->value.int_); 
			break;
		case MaterialParam::INT2:
			_program->setUniformLocationWith2i(param->_uniform->location, param->value.int_, param->value.int2_);
			break;
		case MaterialParam::INT3:
			_program->setUniformLocationWith3i(param->_uniform->location, param->value.int_, param->value.int2_, param->value.int3_);
			break;
		case MaterialParam::INT4: 
			_program->setUniformLocationWith4i(param->_uniform->location, param->value.int_, param->value.int2_, param->value.int3_, param->value.int4_);
			break;
		case MaterialParam::FLOAT1:
			_program->setUniformLocationWith1f(param->_uniform->location, param->value.float_);
			break;
		case MaterialParam::FLOAT2:
			_program->setUniformLocationWith2f(param->_uniform->location, param->value.float_, param->value.float2_);
			break;
		case MaterialParam::FLOAT3:
			_program->setUniformLocationWith3f(param->_uniform->location, param->value.float_, param->value.float2_, param->value.float3_);
			break;
		case MaterialParam::FLOAT4:
			_program->setUniformLocationWith4f(param->_uniform->location, param->value.float_, param->value.float2_, param->value.float3_, param->value.float4_);
			break;
		case MaterialParam::MAT:
			_program->setUniformLocationWithMatrix4fv(param->_uniform->location, (GLfloat*)param->value.ptr_, 1);
			break;
        case MaterialParam::SAMPLER:
            _program->setUniformLocationWith1i(param->_uniform->location, 0);
            break;
		default:
			break;
		}
	}
}


NS_CC_END