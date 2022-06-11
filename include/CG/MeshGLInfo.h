#pragma once

#include <glad/glad.h>
#include <string>
#include <memory>

#include "CG/MeshData.h"

namespace cg
{
	class MeshGLInfo
	{
    public:
        MeshGLInfo();
        ~MeshGLInfo();

        GLuint getPositionBufferID() const { return m_positionBuffer; }
        GLuint getColorBufferID() const { return m_colorBuffer; }
        GLuint getIndexBufferID() const { return m_indexBuffer; }
        GLuint getIndexBufferSize() const { return m_drawAmount; }
        GLenum getDrawMode() const { return m_drawMode; }

        static std::shared_ptr<MeshGLInfo> generate(const MeshData& meshData);

    private:
        MeshGLInfo(const MeshGLInfo&) = delete;
        MeshGLInfo(MeshGLInfo&&) = delete;

        MeshGLInfo& operator=(const MeshGLInfo&) = delete;
        MeshGLInfo& operator=(MeshGLInfo&&) = delete;

    private:
        GLuint m_positionBuffer; // ID of vertex-buffer: position
        GLuint m_colorBuffer;    // ID of vertex-buffer: color

        GLuint m_indexBuffer;    // ID of index-buffer

        GLenum m_drawMode = GL_TRIANGLES;

        GLuint m_drawAmount = 0; // How many elements to draw (used in draw call)
	};
}
