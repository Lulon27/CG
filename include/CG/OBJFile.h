#pragma once

#include <string>

#include "CG/MeshData.h"

namespace cg
{
	class OBJFile
	{
	public:
		static bool load(const std::string& path, MeshData* meshData, float scale = 1.0f);
	};
}