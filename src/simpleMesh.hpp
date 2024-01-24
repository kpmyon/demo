#include "polyscope/polyscope.h"
#include <string>
#include <vector>

class SimpleMesh {
public:
    std::string                      name;
    std::vector<glm::vec3>           vertexPositions;
    std::vector<std::vector<size_t>> faceIndices;
    std::vector<glm::vec2>           texturePositions;
    std::vector<glm::vec3>           normalPositions;
    float                            initialScale = 1.0;
    float                            currScale    = 1.0;

    SimpleMesh() = default;
    SimpleMesh(std::string                      &name,
               std::vector<glm::vec3>           &vertexPositions,
               std::vector<std::vector<size_t>> &faceIndices)
        : name(name)
        , vertexPositions(vertexPositions)
        , faceIndices(faceIndices){

          };
    /*
    [[nodiscard]] std::string const                      &GetName();
    [[nodiscard]] std::vector<glm::vec3> const           &GetVertexPositions();
    [[nodiscard]] std::vector<std::vector<size_t>> const &GetFaceIndices();
    [[nodiscard]] std::vector<glm::vec2> const           &GetTexturePositions();
    [[nodiscard]] std::vector<glm::vec3> const           &GetNormalPositions();
    [[nodiscard]] float const                            &GetInitialScale();
    [[nodiscard]] float const                            &GetCurrScale();

    void SetName(std::string const &r_name);
    void SetVertexPositions(std::vector<glm::vec3> const &r_vertexPositions);
    void SetfaceIndices(std::vector<std::vector<size_t>> const &r_faceIndices);
    void SetTexturePositions(std::vector<glm::vec2> const &r_texturePositions);
    void SetNormalPositions(std::vector<glm::vec3> const &r_normalPositions);
    void SetInitialScale(float const &r_initialScale);
    void SetCurrScale(float const &r_currScale);
*/
    std::tuple<glm::vec3, glm::vec3> getBoundingBox();
};