#include "CG/GeometryUtil.h"

namespace cg::GeometryUtil
{
    static void makeTriangles(cg::MeshData* model, glm::vec3 a, glm::vec3 b, glm::vec3 c, uint8_t n)
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
                model->colors.emplace_back(1.0f, 1.0f, 0.0f);
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

	void generateSphereModel(cg::MeshData* model, uint8_t n, float radius)
	{
        if (n < 0)
        {
            return;
        }

        model->vertices.clear();
        model->colors.clear();
        model->indices.clear();

        using namespace glm;

        // Top Front
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(radius, 0, -radius),
            n);

        // Top Back
        makeTriangles(model,
            vec3(-radius, 0, radius),
            vec3(0, 1, 0),
            vec3(radius, 0, radius),
            n);

        // Top Right
        makeTriangles(model,
            vec3(radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(radius, 0, radius),
            n);

        // Top Left
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, 1, 0),
            vec3(-radius, 0, radius),
            n);

        // Bottom Front
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(radius, 0, -radius),
            n);

        // Bottom Back
        makeTriangles(model,
            vec3(-radius, 0, radius),
            vec3(0, -1, 0),
            vec3(radius, 0, radius),
            n);

        // Bottom Right
        makeTriangles(model,
            vec3(radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(radius, 0, radius),
            n);

        // Bottom Left
        makeTriangles(model,
            vec3(-radius, 0, -radius),
            vec3(0, -1, 0),
            vec3(-radius, 0, radius),
            n);

        // Make octahedron into a sphere shape
        // by pushing vertices away from the center so that
        // every vertex is <radius> units away from the center
        for (size_t i = 0; i < model->vertices.size(); ++i)
        {
            auto vertex = model->vertices[i];
            model->vertices[i] *= radius / glm::length(vertex);
        }

        model->drawMode = GL_TRIANGLES;
	}

	void generateOriginModel(cg::MeshData* model)
	{
        model->vertices.clear();
        model->colors.clear();
        model->indices.clear();

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
}