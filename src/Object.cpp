#include "CG/Object.h"

#include "CG/GeometryUtil.h"
#include "CG/ShaderManager.h"

namespace cg
{
	static void updateNormalsModel(std::shared_ptr<cg::Object> normalsObj, const MeshData* mesh);

	Object::Object(const std::string& debugName)
		: m_debugName(debugName)
	{
		
	}

	Object::~Object()
	{

	}

	void Object::setShader(GLSLProgram* shader)
	{
		m_shader = shader;
		updateVAO();
	}

	void Object::setMesh(const MeshData& mesh)
	{
		m_meshInfo = MeshGLInfo::generate(mesh);
		updateVAO();
		//updateNormalsModel(m_normalsDisplayObj, &mesh);
	}

	void Object::updateVAO()
	{
		if (m_meshInfo == nullptr || m_shader == nullptr)
		{
			m_vao.deleteVAO();
			return;
		}

		m_vao.deleteVAO();
		m_vao.generateVAO();

		m_vao.bindShaderAttribVec3f(m_meshInfo->getPositionBufferID(), m_shader, "position");
		m_vao.bindShaderAttribVec3f(m_meshInfo->getColorBufferID(), m_shader, "color");
		m_vao.bindShaderAttribVec3f(m_meshInfo->getNormalBufferID(), m_shader, "normal");
		m_vao.bindIndexBuffer(m_meshInfo->getIndexBufferID());
	}

	void Object::rotateAroundOrigin(float deg, const glm::vec3& axis)
	{
		auto m = glm::rotate(glm::mat4x4(1.0f), glm::radians(deg), axis);

		position = glm::make_vec3(m * glm::vec4(position, 0.0f));
	}

	void updateNormalsModel(std::shared_ptr<cg::Object> normalsObj, const MeshData* mesh)
	{
		// Generate model for displaying normals
		cg::MeshData normalsMesh;
		cg::GeometryUtil::generateNormalDisplayObj(&normalsMesh, mesh);

		normalsObj->setMesh(normalsMesh);
	}

	void Object::showNormals()
	{
		/*
		if (!hasChild(m_normalsDisplayObj))
		{
			addChild(m_normalsDisplayObj);
		}
		*/
	}

	void Object::hideNormals()
	{
		/*
		removeChild(m_normalsDisplayObj);
		*/
	}
}
