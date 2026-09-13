#pragma once
#include "Vertex.hpp"
#include "Primitive.hpp"
#include <cstdint>
#include <vector>

namespace nova::ps1 {

class VertexAssembler {
public:
    VertexAssembler();
    ~VertexAssembler() = default;

    void Reset();

    void SetPrimitive(const Primitive& primitive) { _primitive = primitive; }
    const Primitive& GetPrimitive() const { return _primitive; }

    void FeedCommand(uint32_t value);
    bool IsComplete() const;
    void ResetCommands();

    const std::vector<Vertex>& GetVertices() const { return _vertices; }

private:
    Primitive _primitive;
    std::vector<uint32_t> _commandBuffer;
    std::vector<Vertex> _vertices;

    void ParseFirstWord(uint32_t value);
    void ParseVertexData();
};

}