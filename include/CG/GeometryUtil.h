#pragma once

#include "CG/MeshData.h"

namespace cg::GeometryUtil
{
	void generateSphereModel(cg::MeshData* model, uint8_t n, float radius);
	void generateOriginModel(cg::MeshData* model);
}