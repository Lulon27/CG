#include "CG/GeometryUtil.h"

namespace cg::GeometryUtil
{
    static void makeTriangles(cg::MeshData* model, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint8_t n, glm::vec3 color)
    {
        if (n < 0)
        {
            return;
        }

        /*
         *        C
         *       / \
         *      /   \
         *     /     \
         *    /-------\  <- Bridge. On every level an imaginary bridge is used to place vertices.
         *   /         \
         *  A-----------B
         */

         // Point between A and B
        glm::vec3 ab_slide;

        // Point between B and C
        glm::vec3 bc_slide;

        // Iteration variables
        size_t x, y;

        // Vector that represents a "bridge" between the left and right side at a cetrain hight
        glm::vec3 currentLine;

        // Edge length of a small triangle
        float edgeLengthOuter = 1.0f / (n + 1);

        // Percentage of the "bridge" that one small triangle takes up
        float edgeLengthInner;

        // Number of triangles that fit on the bridge on the current level
        uint32_t edgeDivInner;

        // Next index because there may already be vertices in <model>
        GLushort nextIndex = model->vertices.size();

        GLushort nextTriUp[3] = { 0, 1, 2 + n };
        GLushort nextTriDown[3] = { 3 + n, 2 + n, 1 };

        for (x = 0; x < 3; ++x)
        {
            nextTriUp[x] += model->vertices.size();
            nextTriDown[x] += model->vertices.size();
        }

        for (y = 0; y < n + 2; ++y)
        {
            // Place the "bridge", one higher every time
            ab_slide = a + (b - a) * (y * edgeLengthOuter);
            bc_slide = c + (b - c) * (y * edgeLengthOuter);
            currentLine = bc_slide - ab_slide;
            edgeDivInner = n + 1 - y;

            // Percentage of the "bridge" that one small triangle takes up
            edgeLengthInner = edgeDivInner == 0 ? 0.0f : 1.0f / edgeDivInner;
            for (x = 0; x < n + 2 - y; ++x)
            {
                // Put vertices along the bridge
                glm::vec3 vertex = ab_slide + currentLine * (x * edgeLengthInner);
                model->vertices.push_back(vertex);
                model->colors.push_back(color);
            }
        }

        uint32_t trianglesUp = n + 1;
        uint32_t trianglesDown = n;

        //Generate up pointing triangle indices
        for (y = 0; y < trianglesUp; ++y)
        {
            for (x = y; x < trianglesUp; ++x)
            {
                model->indices.push_back(nextTriUp[0]);
                model->indices.push_back(nextTriUp[1]);
                model->indices.push_back(nextTriUp[2]);
                ++nextTriUp[0]; ++nextTriUp[1]; ++nextTriUp[2];
            }
            ++nextTriUp[0]; ++nextTriUp[1];
        }

        //Generate down pointing triangle indices
        for (y = 0; y < trianglesDown; ++y)
        {
            for (x = y; x < trianglesDown; ++x)
            {
                model->indices.push_back(nextTriDown[0]);
                model->indices.push_back(nextTriDown[1]);
                model->indices.push_back(nextTriDown[2]);
                ++nextTriDown[0]; ++nextTriDown[1]; ++nextTriDown[2];
            }
            ++nextTriDown[0]; ++nextTriDown[1]; nextTriDown[2] += 2;
        }
    }

	void generateSphereModel(cg::MeshData* model, uint8_t n, float radius, const glm::vec3& color)
	{
        if (n < 0)
        {
            return;
        }

        model->clearAll();

        using namespace glm;

        // Top Front
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(radius, 0, -radius),
            n, color);

        // Top Back
        makeTriangles(model,
            vec3(-radius, 0, radius),
            vec3(0, 1, 0),
            vec3(radius, 0, radius),
            n, color);

        // Top Right
        makeTriangles(model,
            vec3(radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(radius, 0, radius),
            n, color);

        // Top Left
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(-radius, 0, radius),
            n, color);

        // Bottom Front
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(radius, 0, -radius),
            n, color);

        // Bottom Back
        makeTriangles(model,
            vec3(-radius, 0, radius),
            vec3(0, -1, 0),
            vec3(radius, 0, radius),
            n, color);

        // Bottom Right
        makeTriangles(model,
            vec3(radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(radius, 0, radius),
            n, color);

        // Bottom Left
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(-radius, 0, radius),
            n, color);

        // Make octahedron into a sphere shape
        // by pushing vertices away from the center so that
        // every vertex is <radius> units away from the center
        for (size_t i = 0; i < model->vertices.size(); ++i)
        {
            auto vertex = model->vertices[i];
            vec3 normal = vertex / glm::length(vertex);
            model->normals.push_back(normal);
            model->vertices[i] = radius * normal;
        }

        model->drawMode = GL_TRIANGLES;
	}

	void generateOriginModel(cg::MeshData* model)
	{
        model->clearAll();

        // Origin symbol
        model->vertices =
        {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };

        model->colors =
        {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };

        model->indices =
        {
            0, 1, 2, 3, 4, 5
        };

        model->drawMode = GL_LINES;
	}

    void generateLineModel(cg::MeshData* model, float length, const glm::vec3& dir, const glm::vec3& color, const glm::vec3& center)
    {
        model->clearAll();

        model->drawMode = GL_LINES;

        model->vertices =
        {
            center + dir * 0.5f * length,
            center - dir * 0.5f * length
        };

        model->colors = { color, color };

        model->indices = { 0, 1 };
    }

    void generateNormalDisplayObj(cg::MeshData* model, const cg::MeshData* modelWithNormals, float normalLength, const glm::vec3& color)
    {
        model->clearAll();

        model->drawMode = GL_LINES;

        if (modelWithNormals->normals.size() != modelWithNormals->vertices.size())
        {
            return;
        }

        for (size_t i = 0; i < modelWithNormals->normals.size(); ++i)
        {
            model->vertices.push_back(modelWithNormals->vertices[i]);
            model->vertices.push_back(modelWithNormals->vertices[i] + modelWithNormals->normals[i] * normalLength);

            model->indices.push_back(i * 2);
            model->indices.push_back(i * 2 + 1);

            model->colors.push_back(color);
            model->colors.push_back(color);
        }
    }
}