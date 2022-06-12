#pragma once

#include "CG/MeshData.h"

namespace cg::GeometryUtil
{
	void generateSphereModel(cg::MeshData* model, uint8_t n, float radius, const glm::vec3& color = { 1.0f, 1.0f, 0.0f });
	void generateOriginModel(cg::MeshData* model);
	void generateLineModel(cg::MeshData* model, float length, const glm::vec3& dir = { 0.0f, 1.0f, 0.0f }, const glm::vec3& color = { 1.0f, 0.0f, 0.0f }, const glm::vec3& center = { 0.0f, 0.0f, 0.0f });
}