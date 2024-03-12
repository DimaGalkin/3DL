#include "objects.hpp"
#include "utils.hpp"

ThreeDL::Object::Object(
    const std::string& model_path,
    const ThreeDL::FILEFORMAT ff,
    const uint32_t color
) : position_({0, 0, 0}),
    rotation_({0, 0, 0}),
    texture_h_(1),
    texture_w_(1)
{
    fillTriangleBuffer(model_path, ff);
    fillTextureBuffer(color);
}


ThreeDL::Object::Object(
    const std::string& model_path,
    const ThreeDL::FILEFORMAT ff,
    const std::string& texture_path
) : position_({0, 0, 0}),
    rotation_({0, 0, 0}),
    texture_w_(1),
    texture_h_(1)
{
    fillTriangleBuffer(model_path, ff);
    fillTextureBuffer(texture_path);
}

void ThreeDL::Object::fillTextureBuffer(const std::string& texture_path) {
    SDL_Surface* texture = IMG_Load(texture_path.c_str());

    if (texture == nullptr) {
        throw std::runtime_error("Could not load texture: " + texture_path);
    }

    std::vector<uint32_t> tex_data;
    tex_data.resize(texture->w * texture->h);
    texture_.resize(texture->w * texture->h);

    for (int i = 0; i < texture->w; ++i) {
        for (int j = 0; j < texture->h; ++j) {
            uint8_t r, g, b, a;
            SDL_GetRGBA(Utils::samplePixel(texture, i, j), texture->format, &r, &g, &b, &a);
            tex_data.at(i + j * texture->w) = a << 24 | r << 16 | g << 8 | b;
        }
    }

    texture_w_ = texture->w;
    texture_h_ = texture->h;

    cl::SVMAllocator<Triangle, cl::SVMTraitCoarse<>> svm_allocator;
    texture_ = std::vector<uint32_t, tex_allocator_t>(tex_data.size(), 1, svm_allocator);

    std::copy(tex_data.begin(), tex_data.end(), texture_.begin());

    tex_data.clear();

    SDL_FreeSurface(texture);
}

void ThreeDL::Object::fillTextureBuffer(const uint32_t color) {
    cl::SVMAllocator<Triangle, cl::SVMTraitCoarse<>> svm_allocator;
    texture_ = std::vector<uint32_t, tex_allocator_t>(1, 1, svm_allocator);
    texture_[0] = color;

    texture_w_ = 0;
    texture_h_ = 0;
}

void ThreeDL::Object::fillTriangleBuffer(
    const std::string &model_path,
    const ThreeDL::FILEFORMAT ff
) {
    switch (ff) {
        case FILEFORMAT::OBJ: {
            std::vector<Triangle> loaded = OBJLoader::load(model_path);
            triangles_.resize(loaded.size());

            cl::SVMAllocator<Triangle, cl::SVMTraitCoarse<>> svm_allocator;
            triangles_ = std::vector<Triangle, tri_allocator_t>(loaded.size(), {}, svm_allocator);
            std::copy(loaded.begin(), loaded.end(), triangles_.begin());

            loaded.clear();

            break;
        }

        default:
            throw std::runtime_error("Unsupported file format");
    }
}

std::vector<Triangle> ThreeDL::OBJLoader::load(const std::string& path) {
    std::string line;
    std::ifstream vertex_data {path};

    if (!vertex_data.is_open())
        throw std::runtime_error("Could not open OBJ file: " + path);

    int vertex_index = 1;
    int texture_index = 1;
    int normal_index = 1;

    std::vector<Triangle> triangles;

    std::unordered_map<int, Vector3> vertices;
    std::unordered_map<int, Vector3> normals;
    std::unordered_map<int, Vector2> uvs;

    while(std::getline(vertex_data, line)) {
        if (line.empty()) continue;
        if (line[0] == '\r') continue;
        if (line[0] == '#') continue;

        if (line[0] == 'v' && line[1] == 't') {
            std::vector<std::string> tokens = Utils::split(line.substr(3), ' ');

            uvs.emplace(
                texture_index,
                Vector2 {
                    std::stod(tokens[0]),
                    std::stod(tokens[1])
                }
            );

            ++texture_index;
        } else if (line[0] == 'v' && line[1] == 'n') {
            std::vector<std::string> tokens = Utils::split(line.substr(3), ' ');

            normals.emplace(
                    normal_index,
                    Vector3 {
                            std::stod(tokens[0]),
                            std::stod(tokens[1]),
                            std::stod(tokens[2])
                    }
            );

            ++normal_index;
        } else if (line[0] == 'v') {
            std::vector<std::string> tokens = Utils::split(line.substr(2), ' ');

            vertices.emplace(
                    vertex_index,
                    Vector3 {
                            std::stod(tokens[0]),
                            std::stod(tokens[1]),
                            std::stod(tokens[2])
                    }
            );

            ++vertex_index;
        } else if (line[0] == 'f') {
            std::vector<std::string> tokens = Utils::split(line.substr(2), ' ');
            std::vector<std::string> first = Utils::split(tokens[0], '/');
            std::vector<std::string> second = Utils::split(tokens[1], '/');
            std::vector<std::string> third = Utils::split(tokens[2], '/');

            triangles.emplace_back(
                vertices[std::stoi(first[0])],
                vertices[std::stoi(second[0])],
                vertices[std::stoi(third[0])],
                uvs[std::stoi(first[1])],
                uvs[std::stoi(second[1])],
                uvs[std::stoi(third[1])],
                normals[std::stoi(first[2])],
                normals[std::stoi(second[2])],
                normals[std::stoi(third[2])],
                static_cast<unsigned int>(0xaaaaaa),
                static_cast<unsigned int>(0xdddddd),
                10.0
            );
        }
    }

    return triangles;
}