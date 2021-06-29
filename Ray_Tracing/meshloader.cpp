#include "meshloader.h"

Mesh::Mesh(std::string Path)
{
    std::ifstream file(Path);
    std::string buf;
    while (file >> buf)
    {
        if (buf[0] == '#')//comment
        {
            file.ignore(1<<15,'\n');
        }
        else if (buf == "v")//vertices
        {
            float x, y, z;
            file >> x >> y >> z;
            float r2 = x * x + y * y + z * z;
            if (r2 > radius2) { radius2 = r2; }
            vertices.emplace_back(x, y, z);
        }
        else if (buf == "f")//  vertex_index / texture_index / normal_index
        {
            std::getline(file, buf);
            std::stringstream ss;
            ss << buf;
            int v  = -1;
            int vt = -1;
            int vn = -1;
            char c;
            while (ss >> v)
            {
                ss.get(c);
                if (c != ' ')
                {
                    ss >> c;
                    if (c != '/')
                    {
                        ss.seekg(-1, std::ios_base::cur);
                        ss >> vt >> c >> vn;
                    }
                    else
                    {
                        ss >> vn;
                    }

                }
                if (v != -1)//only use vertex for now
                {
                    indices.push_back(v - 1);
                }
            }
        }
        else
        {
            file.ignore(1<<15, '\n');//not able to parse
        }
    }
}

const std::vector<float3>* Mesh::GetVertices()const
{
    return &vertices;
}

const std::vector<int>* Mesh::GetIndices()const
{
    return &indices;
}
