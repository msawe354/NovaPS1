#include "VertexAssembler.hpp"

namespace nova::ps1 {

VertexAssembler::VertexAssembler() {
    Reset();
}

void VertexAssembler::Reset() {
    _primitive.Reset();
    _commandBuffer.clear();
    _vertices.clear();
}

void VertexAssembler::ResetCommands() {
    _commandBuffer.clear();
    _vertices.clear();
}

void VertexAssembler::ParseFirstWord(uint32_t value) {
    _primitive.ParseCommand(value);
}

void VertexAssembler::ParseVertexData() {
    _vertices.clear();

    uint32_t vertexCount = _primitive.GetVertexCount();
    bool gouraud = _primitive.GetShading() == ShadingMode::Gouraud;
    bool textured = _primitive.GetTexture() == TextureMode::Textured;

    uint32_t index = 1;
    for (uint32_t i = 0; i < vertexCount; i++) {
        Vertex v;

        if (gouraud && textured) {
            uint32_t w0 = _commandBuffer[index];
            uint32_t w1 = _commandBuffer[index + 1];
            uint32_t w2 = _commandBuffer[index + 2];

            v.x = static_cast<int16_t>(w0 & 0xFFFF);
            v.y = static_cast<int16_t>((w0 >> 16) & 0xFFFF);
            v.r = static_cast<uint8_t>(w1 & 0xFF);
            v.g = static_cast<uint8_t>((w1 >> 8) & 0xFF);
            v.b = static_cast<uint8_t>((w1 >> 16) & 0xFF);
            v.u = static_cast<uint8_t>(w2 & 0xFF);
            v.v = static_cast<uint8_t>((w2 >> 8) & 0xFF);

            index += 3;
        } else if (gouraud) {
            uint32_t w0 = _commandBuffer[index];
            uint32_t w1 = _commandBuffer[index + 1];

            v.x = static_cast<int16_t>(w0 & 0xFFFF);
            v.y = static_cast<int16_t>((w0 >> 16) & 0xFFFF);
            v.r = static_cast<uint8_t>(w1 & 0xFF);
            v.g = static_cast<uint8_t>((w1 >> 8) & 0xFF);
            v.b = static_cast<uint8_t>((w1 >> 16) & 0xFF);

            index += 2;
        } else if (textured) {
            uint32_t w0 = _commandBuffer[index];
            uint32_t w1 = _commandBuffer[index + 1];

            v.x = static_cast<int16_t>(w0 & 0xFFFF);
            v.y = static_cast<int16_t>((w0 >> 16) & 0xFFFF);
            v.u = static_cast<uint8_t>(w1 & 0xFF);
            v.v = static_cast<uint8_t>((w1 >> 8) & 0xFF);
            v.r = 0xFF;
            v.g = 0xFF;
            v.b = 0xFF;

            index += 2;
        } else {
            uint32_t w0 = _commandBuffer[index];

            v.x = static_cast<int16_t>(w0 & 0xFFFF);
            v.y = static_cast<int16_t>((w0 >> 16) & 0xFFFF);
            v.r = static_cast<uint8_t>((w0 >> 16) & 0xFF);
            v.g = static_cast<uint8_t>((w0 >> 8) & 0xFF);
            v.b = static_cast<uint8_t>(w0 & 0xFF);

            index += 1;
        }

        _vertices.push_back(v);
    }
}

void VertexAssembler::FeedCommand(uint32_t value) {
    _commandBuffer.push_back(value);

    if (_commandBuffer.size() == 1) {
        ParseFirstWord(value);
    }

    uint32_t requiredWords = _primitive.GetWordCount();

    if (_commandBuffer.size() >= requiredWords) {
        ParseVertexData();
    }
}

bool VertexAssembler::IsComplete() const {
    if (_commandBuffer.empty()) return false;
    return _commandBuffer.size() >= _primitive.GetWordCount();
}

}