#include "meshloader.h"

Mesh::Mesh(const std::string& path)
{
    this->path = path;
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string buf;
        while (file >> buf)
        {
            if (buf[0] == '#')//comment
            {
                file.ignore(1 << 15, '\n');
            }
            else if (buf == "v")//vertices
            {
                float x, y, z;
                file >> x >> y >> z;
                float r2 = x * x + y * y + z * z;
                if (r2 > radius2) { radius2 = r2; }
                vertices.emplace_back(x, y, -z);
            }
            else if (buf == "vt")
            {
                float u, v;
                file >> u >> v;
                vertices_texture.emplace_back(u, v);
            }
            else if (buf == "f")//  vertex_index / texture_index / normal_index
            {
                std::getline(file, buf);
                std::stringstream ss;
                ss << buf;
                int v = -1;
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
                    if (vt != -1)
                    {
                        indices_texture.push_back(vt - 1);
                    }
                }
            }
            else if (buf == "mtllib")
            {
                file >> buf;
                material = new Material;
                LoadMaterial(buf);
            }
            else
            {
                file.ignore(1 << 15, '\n');//not able to parse
            }

        }
        file.close();
    }
    
}

const std::vector<rt::float3>* Mesh::GetVertices()const
{
    return &vertices;
}

const std::vector<int>* Mesh::GetIndices()const
{
    return &indices;
}

rt::float3 Mesh::Sampler(rt::float2 uv, int type)
{
    rt::BMP* tex;
    int x, y;
    switch (type)
    {
        case TEXTURESAMPLER:
        {
            tex = &texture;


            break;
        }
        case NORMALSAMPLER:
        {
            tex = &normal;
            break;
        }
        default:return{ 0,0,0 };
    }
    x = uv.x * tex->GetWidthMax();
    y = uv.y * tex->GetHeightMax();
    rt::Color color = tex->GetData(x, y);
    float a = static_cast<float>(color.b) / 255.0f;
    float b = static_cast<float>(color.g) / 255.0f;
    float c = static_cast<float>(color.r) / 255.0f;
    switch (type)
    {
        case TEXTURESAMPLER:
        {
            return{ c,b,a };
        }
        case NORMALSAMPLER:
        {
            return{ c,a,b };
        }
    }
    return{ 0,0,0 };
}

void Mesh::LoadMaterial(std::string Path)
{
    std::ifstream file(Path);
    if (file.is_open())
    {
        float Ns = -1;//1-diffuse
        float Ka = -1;//ambient
        float Kd = -1;//color
        float Ks = -1;//reflectivity
        float Ni = -1;//refractive
        float d = -1;//opaque
        std::string buf;
        while (file >> buf)
        {
            if (buf[0] == '#')//comment
            {
                file.ignore(1 << 15, '\n');
            }
            else if (buf == "newmtl")
            {
                file >> buf;

            }
            else if (buf == "Ns")
            {
                file >> Ns;
            }
            else if (buf == "Ka")
            {
                file >> Ka >> Ka >> Ka;
            }
            else if (buf == "Kd")
            {
                file >> Kd >> Kd >> Kd;
            }
            else if (buf == "Ks")
            {
                file >> Ks >> Ks >> Ks;
            }
            else if (buf == "Ni")
            {
                file >> Ni;
            }
            else if (buf == "d")
            {
                file >> d;
            }
            else if (buf == "map_Kd")
            {
                file >> buf;
                material->has_texture_map = true;
                texture.Load(buf);
            }
            else if (buf == "map_Bump")
            {
                file >> buf;
                material->has_normal_map = true;
                normal.Load(buf);
            }
            else
            {
                file.ignore(1 << 15, '\n');
            }

        }
        material->ambient = Ka != -1 ? Ka : 0;
        material->color = (Kd != -1) ? rt::float3{ Kd, Kd, Kd } : rt::float3{ 1,0,1 };
        material->diffuse = Ns != -1 ? 1 - log2(Ns + 1) / 9.967226259f : 0;
        material->reflectivity = Ks != -1 ? Ks : 0;
        material->refractive = Ni != -1 ? Ni : 0;
        material->transparent = (d == 0);
        file.close();
    }
}
