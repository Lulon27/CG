#include "CG/OBJFile.h"

#include <fstream>
#include <iostream>
#include <cstring>

namespace cg
{
	#define ASSERT_NORMALS_IN_BOUNDS(face, normals) \
		if (face.fvn >= normals.size()) { \
			std::cout << "Normal index is out of range. (" << face.fvn << " >= " << normals.size() << ")\n"; \
			continue; }

	struct OBJData
	{
		struct OBJFace
		{
			unsigned int fv;
			unsigned int fvt;
			unsigned int fvn;
		};

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<OBJFace>> faces;
	};

	static bool parseLine(const std::string& line, OBJData* objData);

	bool OBJFile::load(const std::string& path, MeshData* meshData, float scale)
	{
		meshData->clearAll();

		std::fstream file(path);

		if (!file.good())
		{
			std::cout << "could not open file \"" << path << "\"";
			return false;
		}

		std::string line;

		size_t ln = 0;
		OBJData objData;

		while (!file.eof())
		{
			getline(file, line);
			ln++;
			//std::cout << ln << ": " << line << '\n';
			if (!parseLine(line, &objData))
			{
				file.close();
				return false;
			}
		}

		file.close();

		meshData->vertices = objData.vertices;
		for (size_t i = 0; i < meshData->vertices.size(); ++i)
		{
			meshData->vertices[i] *= scale;
			meshData->colors.emplace_back(0.8f, 0.1f, 0.1f);
		}
		meshData->normals.resize(meshData->vertices.size());

		for (size_t i = 0; i < objData.faces.size(); ++i)
		{
			auto& face = objData.faces[i];
			if (face.size() > 4)
			{
				std::cout << "Skipping face with more than 4 vertices\n";
				continue;
			}
			else if (face.size() == 4)
			{
				meshData->indices.push_back(face[0].fv);
				meshData->indices.push_back(face[1].fv);
				meshData->indices.push_back(face[2].fv);

				meshData->indices.push_back(face[0].fv);
				meshData->indices.push_back(face[2].fv);
				meshData->indices.push_back(face[3].fv);

				ASSERT_NORMALS_IN_BOUNDS(face[0], objData.normals);
				ASSERT_NORMALS_IN_BOUNDS(face[1], objData.normals);
				ASSERT_NORMALS_IN_BOUNDS(face[2], objData.normals);
				ASSERT_NORMALS_IN_BOUNDS(face[3], objData.normals);

				meshData->normals[face[0].fv] = objData.normals[face[0].fvn];
				meshData->normals[face[1].fv] = objData.normals[face[1].fvn];
				meshData->normals[face[2].fv] = objData.normals[face[2].fvn];
				meshData->normals[face[3].fv] = objData.normals[face[3].fvn];
			}
			else if (face.size() == 3)
			{
				meshData->indices.push_back(face[0].fv);
				meshData->indices.push_back(face[1].fv);
				meshData->indices.push_back(face[2].fv);

				ASSERT_NORMALS_IN_BOUNDS(face[0], objData.normals);
				ASSERT_NORMALS_IN_BOUNDS(face[1], objData.normals);
				ASSERT_NORMALS_IN_BOUNDS(face[2], objData.normals);

				meshData->normals[face[0].fv] = objData.normals[face[0].fvn];
				meshData->normals[face[1].fv] = objData.normals[face[1].fvn];
				meshData->normals[face[2].fv] = objData.normals[face[2].fvn];
			}
			else
			{
				std::cout << "Skipping face with less than 3 vertices\n";
				continue;
			}
		}
		return true;
	}

	static float nextFloat(std::string& str)
	{
		size_t idx = 0;
		float f = std::stof(str, &idx);
		str = str.substr(idx);
		return f;
	}

	static float nextInt(std::string& str)
	{
		size_t idx = 0;
		int i = std::stoi(str, &idx);
		str = str.substr(idx);
		return i;
	}

	static void skipWhitespace(std::string& str)
	{
		if (str.empty())
		{
			return;
		}
		size_t i;
		for (i = 0; i < str.size(); ++i)
		{
			if (!(str[i] == ' ' && str[i] == '\t'))
			{
				break;
			}
		}
		str = str.substr(i + 1);
	}

	static bool isNextWhitespace(std::string& str)
	{
		return !str.empty() && !(str[0] == ' ' && str[0] == '\t');
	}

	static void skipOne(std::string& str)
	{
		if (str.empty())
		{
			return;
		}
		str = str.substr(1);
	}

	bool parseLine(const std::string& line, OBJData* objData)
	{
		if (line.starts_with("#"))
		{
			return true;
		}


		if (line.starts_with("v "))
		{
			std::string l = line.substr(2);
			float vx = nextFloat(l);
			float vy = nextFloat(l);
			float vz = nextFloat(l);

			objData->vertices.emplace_back(vx, vy, vz);
		}

		if (line.starts_with("vn "))
		{
			std::string l = line.substr(3);
			float vx = nextFloat(l);
			float vy = nextFloat(l);
			float vz = nextFloat(l);
			objData->normals.emplace_back(vx, vy, vz);
		}

		if (line.starts_with("f "))
		{
			std::vector<OBJData::OBJFace> face;
			face.reserve(4);

			std::string l = line.substr(2);
			int fv, fvt, fvn;
			while (!l.empty())
			{
				// x...
				fv = nextInt(l) - 1;
				if (l[0] == '/')
				{
					// x/...
					skipOne(l);
					if (l[0] == '/')
					{
						// x//x
						skipOne(l);
						fvn = nextInt(l) - 1;
					}
					else
					{
						// x/x/x
						fvt = nextInt(l) - 1;
						skipOne(l);
						fvn = nextInt(l) - 1;
					}
				}
				skipWhitespace(l);
				face.emplace_back(fv, fvt, fvn);
			}

			objData->faces.push_back(face);
		}

		//std::cout << line << '\n';

		return true;
	}
}