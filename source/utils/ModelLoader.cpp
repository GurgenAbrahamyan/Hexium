#include "ModelLoader.h"
#include <fstream>
#include <cstring>
#include <stdexcept>
#include "../graphics/Vertex.h"
#include "../math_custom/Quat.h"
#include "math.h"
#include "../math_custom/GLAdapter.h"


// ---------------- Constructor ----------------
ModelLoader::ModelLoader(const std::string& filePath)
    : filePath(filePath)
{
    directory = filePath.substr(0, filePath.find_last_of('/') + 1);
    std::ifstream file(filePath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open model file");
    file >> JSON;
    file.close();


    bufferData = loadBufferData(JSON["buffers"][0]["uri"]);
    parseGLTF();
}

void ModelLoader::parseGLTF()
{
    // Start with identity transforms
    Vector3 rootPos(0.0f, 0.0f, 0.0f);
    Quat rootRot; // Identity quaternion
    Vector3 rootScale(1.0f, 1.0f, 1.0f);

    traverseNode(0, rootPos, rootRot, rootScale);
}

void ModelLoader::traverseNode(unsigned int nextNode, const Vector3& parentPos,
    const Quat& parentRot, const Vector3& parentScale)
{
    json node = JSON["nodes"][nextNode];
    std::string nodeName = node.value("name", "node_" + std::to_string(nextNode));
    std::cout << "Parsing node: " << nodeName << "\n";

    
    Vector3 localTranslation(0.0f, 0.0f, 0.0f);
    if (node.contains("translation"))
    {
        Vector3 gltf_t(node["translation"][0], node["translation"][1], node["translation"][2]);
        
        localTranslation.x = -gltf_t.x;   // right
        localTranslation.y = gltf_t.z; // forward (GLTF Z -> Engine Y)
        localTranslation.z = gltf_t.y;  // up (GLTF Y -> Engine Z)
        
       
        std::cout << "Local Translation: " << localTranslation.x << ", " << localTranslation.y << ", " << localTranslation.z << "\n";
    }

    Quat localRotation; // Identity
    if (node.contains("rotation"))
    {
       
        Quat gltf_q(node["rotation"][0], node["rotation"][1], node["rotation"][2], node["rotation"][3]);

        localRotation.x = gltf_q.x;     
        localRotation.z = gltf_q.z;
        localRotation.y = gltf_q.y;
        localRotation.w = gltf_q.w;

        std::cout << "Local Rotation: " << localRotation.x << ", " << localRotation.y << ", " << localRotation.z << ", " << localRotation.w << "\n";
    }

    Vector3 localScale(1.0f, 1.0f, 1.0f);
    if (node.contains("scale"))
    {
        Vector3 gltf_s(node["scale"][0], node["scale"][1], node["scale"][2]);
       
        localScale.x = gltf_s.x;
        localScale.y = gltf_s.y;
        localScale.z = gltf_s.z; 
        std::cout << "Local Scale: " << localScale.x << ", " << localScale.y << ", " << localScale.z << "\n";
    }

	Mat4 matNode; 
    if (node.find("matrix") != node.end())
    {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++)
            matValues[i] = (node["matrix"][i]);
		matNode.setDataFromGLTF(matValues);
    }

    // Accumulate transforms
    Quat worldRotation = parentRot * localRotation;

    Vector3 worldScale(
        parentScale.x * localScale.x,
        parentScale.y * localScale.y,
        parentScale.z * localScale.z
    );

    Vector3 rotatedTranslation = parentRot.rotate(localTranslation);
    Vector3 scaledTranslation(
        rotatedTranslation.x * parentScale.x,
        rotatedTranslation.y * parentScale.y,
        rotatedTranslation.z * parentScale.z
    );

    // FIXED: Add instead of multiply!
    Vector3 worldPosition = parentPos.add(scaledTranslation);

    std::cout << "World Position: " << worldPosition.x << ", " << worldPosition.y << ", " << worldPosition.z << "\n";

    if (node.contains("mesh"))
    {
        unsigned int meshInd = node["mesh"];
        parseMesh(meshInd);

        MeshData& meshData = model.meshes.back();

        NodeData nodeData;
        nodeData.position = worldPosition;
        nodeData.rotation = worldRotation;
        nodeData.scale = worldScale;
        nodeData.meshIndex = model.meshes.size() - 1;
        nodeData.name = nodeName;

        if (node.contains("children"))
        {
            for (auto& c : node["children"])
                nodeData.children.push_back(c);
        }

        model.nodes.push_back(nodeData);
    }

    if (node.contains("children"))
    {
        for (auto& child : node["children"])
            traverseNode(child, worldPosition, worldRotation, worldScale);
    }
}
/*void ModelLoader::bakeTransformToMesh(MeshData& meshData, const Mat4& transform)
{
   
   // Mat4 rotationOnly = Mat4::fromQuat(Quat::fromMat4(transform));

    for (Vertex& v : meshData.vertices)
    {
        // Transform position by full transform
       // Vector3 transformedPos = transform.transformPoint(v.position);
        v.position = transformedPos;

        // Transform normal by rotation only
     //   Vector3 transformedNorm = rotationOnly.transformDirection(v.normal);
        v.normal = transformedNorm.normalize();
    }

    std::cout << "Baked transform into " << meshData.name << " vertices\n";
}
*/
void ModelLoader::parseMesh(unsigned int meshIndex)
{
    const json& mesh = JSON["meshes"][meshIndex];
    const json& prim = mesh["primitives"][0];


    unsigned int posAcc = prim["attributes"]["POSITION"];
    unsigned int normAcc = prim["attributes"]["NORMAL"];
    unsigned int uvAcc = prim["attributes"]["TEXCOORD_0"];
    unsigned int indAcc = prim["indices"];

    auto positions = groupVec3(getFloats(JSON["accessors"][posAcc]));
    auto normals = groupVec3(getFloats(JSON["accessors"][normAcc]));
    auto uvs = groupVec2(getFloats(JSON["accessors"][uvAcc]));

    MeshData meshData;
    meshData.vertices = assembleVertices(positions, normals, uvs);
    meshData.indices = getIndices(JSON["accessors"][indAcc]);
    meshData.texturePaths = getTexturesForMesh(meshIndex);


    meshData.name = mesh.value("name", "mesh_" + std::to_string(meshIndex));
	std::cout << "Parsed mesh: " << meshData.name << " with " << meshData.vertices.size() << " vertices and " << meshData.indices.size() << " indices.\n";
    if (prim.contains("material"))
    {
        unsigned int matIndex = prim["material"];
        parseMaterial(matIndex);
    }

    model.meshes.push_back(meshData);
}

void ModelLoader::parseMaterial(unsigned int materialIndex)
{

    if (materialIndex < model.materials.size())
        return;

    MaterialData mat;
    const json& matJSON = JSON["materials"][materialIndex];


    mat.name = matJSON.value("name", "material_" + std::to_string(materialIndex));

	std::cout << "Parsing material: " << mat.name << "\n";
    if (matJSON.contains("pbrMetallicRoughness"))
    {
        const json& pbr = matJSON["pbrMetallicRoughness"];
        mat.metallic = pbr.value("metallicFactor", 1.0f);
        mat.roughness = pbr.value("roughnessFactor", 1.0f);

        if (pbr.contains("baseColorTexture"))
        {
            unsigned int texIndex = pbr["baseColorTexture"]["index"];
            unsigned int imgIndex = JSON["textures"][texIndex]["source"];
            std::string uri = JSON["images"][imgIndex]["uri"];
			std::cout << "Base Color Texture: " << uri << "\n";
            mat.texturePaths.push_back(directory + uri);
        }
    }

    model.materials.push_back(mat);
}

std::vector<Vertex> ModelLoader::assembleVertices(
    const std::vector<Vector3>& positions,
    const std::vector<Vector3>& normals,
    const std::vector<Vector2>& uvs)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++)
    {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.uv = uvs[i];
        v.color = Vector3(1.0f, 1.0f, 1.0f);
        vertices.push_back(v);
    }
    return vertices;
}
std::vector<Vector3> ModelLoader::groupVec3(const std::vector<float>& floats) {
    std::vector<Vector3> out;
    for (size_t i = 0; i < floats.size(); i += 3) {
        float gltf_x = floats[i + 0];
        float gltf_y = floats[i + 1];
        float gltf_z = floats[i + 2];

        Vector3 v;
        v.x = -gltf_x;  // flip X to make +X point right
        v.y = gltf_z; // up stays up
        v.z = gltf_y;

        out.emplace_back(v);
    }
    return out;
}

std::vector<Vector2> ModelLoader::groupVec2(const std::vector<float>& floats)
{
    std::vector<Vector2> out;
    for (size_t i = 0; i < floats.size(); i += 2)
        out.emplace_back(floats[i], floats[i + 1]);
    return out;
}
std::vector<float> ModelLoader::getFloats(const json& accessor)
{
    std::vector<float> floatVec;
    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];
    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];
    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4)
    {
        unsigned char bytes[] = { bufferData[i], bufferData[i + 1], bufferData[i + 2], bufferData[i + 3] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }
    return floatVec;
}
std::vector<GLuint> ModelLoader::getIndices(const json& accessor)
{
    std::vector<GLuint> indices;
    unsigned int bufferView = accessor["bufferView"];
    unsigned int count = accessor["count"];
    unsigned int offset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];
    const json& view = JSON["bufferViews"][bufferView];
    unsigned int byteOffset = view.value("byteOffset", 0);
    unsigned int start = byteOffset + offset;
    if (componentType == 5125)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            GLuint v;
            std::memcpy(&v, &bufferData[start + i * 4], 4);
            indices.push_back(v);
        }
    }
    else if (componentType == 5123)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            unsigned short v;
            std::memcpy(&v, &bufferData[start + i * 2], 2);
            indices.push_back(v);
        }
    }
    return indices;
}
std::vector<std::string> ModelLoader::getTexturesForMesh(unsigned int)
{
    std::vector<std::string> textures;
    if (!JSON.contains("images")) return textures;
    for (unsigned int i = 0; i < JSON["images"].size(); i++)
    {
        std::string texPath = JSON["images"][i]["uri"];
        if (texPath.find("baseColor") != std::string::npos ||
            texPath.find("metallicRoughness") != std::string::npos)
        {
            textures.push_back(directory + texPath);
        }
    }
    return textures;
}
std::vector<unsigned char> ModelLoader::loadBufferData(const std::string& uri)
{

    std::ifstream file(directory + uri, std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("Failed to load buffer");
    return std::vector<unsigned char>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}
Mat4 ModelLoader::fromArray(const json& arr)
{
    Mat4 m;
    for (int i = 0; i < 16; i++)
        m.data[i] = arr[i];
    return m;
}

