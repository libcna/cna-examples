// SPDX-License-Identifier: MIT
#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include "System/IO/MemoryStream.hpp"
#include "Microsoft/Xna/Framework/Color.hpp"
#include "Microsoft/Xna/Framework/Vector3.hpp"
#include "Microsoft/Xna/Framework/Net/PacketReader.hpp"
#include "Microsoft/Xna/Framework/Net/PacketWriter.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Net::NetworkSessionDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::Vector3;
using Microsoft::Xna::Framework::Net::PacketReader;
using Microsoft::Xna::Framework::Net::PacketWriter;

namespace Detail {
inline std::vector<SharpRuntime::bytecs> DrainToBytes(PacketWriter& writer) {
    auto* mem = dynamic_cast<System::IO::MemoryStream*>(writer.getBaseStreamProperty());
    return mem != nullptr ? mem->ToArray() : std::vector<SharpRuntime::bytecs>{};
}

// The same MemoryStream::ToArray()/Stream::Write() dance LocalNetworkGamer::SendData/
// ReceiveData use to move a packet's raw bytes across a real gamer-to-gamer hop.
inline void LoadIntoReader(PacketReader& reader, const std::vector<SharpRuntime::bytecs>& bytes) {
    reader.setPositionProperty(0);
    reader.getBaseStreamProperty()->Write(bytes.data(), 0, (int)bytes.size());
    reader.setPositionProperty(0);
}

inline float RandFloat() { return ((float)std::rand() / (float)RAND_MAX) * 2000.0f - 1000.0f; }
} // namespace Detail

// Demonstrates PacketWriter/PacketReader round-tripping XNA value types. Color is the one
// worth a second look: PacketWriter::Write(Color) writes four bytes and, as in XNA,
// PacketReader::ReadColor() reads exactly those four bytes back. (FNA's reader reads four
// 32-bit floats instead, so the pair is not an inverse there.)
class PacketRoundTripScreen : public DemoScreen {
public:
    PacketRoundTripScreen() : DemoScreen("NetworkSession: Packet Round-Trip") {}

protected:
    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            RunVector3Roundtrip();
        } else if (input.IsNewKeyPress(Keys::C, ControllingPlayer(), playerIndex)) {
            RunColorRoundtrip();
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: Write+Read a random Vector3 (correct round-trip)");
        lines.push_back("C: Write a Color (4 bytes), then ReadColor() it back (4 bytes, as in");
        lines.push_back("   XNA; FNA's reader reads 4 floats instead)");
        lines.emplace_back();
        lines.push_back("Vector3 round-trips: " + std::to_string(vector3Count_));
        if (!lastVector3Result_.empty()) lines.push_back(lastVector3Result_);
        lines.emplace_back();
        lines.push_back("Color round-trips: " + std::to_string(colorCount_));
        if (!lastColorResult_.empty()) lines.push_back(lastColorResult_);

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    void RunVector3Roundtrip() {
        PacketWriter writer;
        const Vector3 value(Detail::RandFloat(), Detail::RandFloat(), Detail::RandFloat());
        writer.Write(value);
        PacketReader reader;
        Detail::LoadIntoReader(reader, Detail::DrainToBytes(writer));
        const Vector3 back = reader.ReadVector3();
        vector3Count_++;
        lastVector3Result_ = "wrote " + value.ToString() + ", read " + back.ToString() +
                              (back == value ? "  [MATCH]" : "  [MISMATCH]");
    }

    void RunColorRoundtrip() {
        PacketWriter writer;
        const Color value((SharpRuntime::bytecs)(std::rand() % 256), (SharpRuntime::bytecs)(std::rand() % 256),
                           (SharpRuntime::bytecs)(std::rand() % 256), (SharpRuntime::bytecs)(std::rand() % 256));
        writer.Write(value);
        PacketReader reader;
        Detail::LoadIntoReader(reader, Detail::DrainToBytes(writer));
        colorCount_++;
        const Color back = reader.ReadColor();
        lastColorResult_ = "wrote " + value.ToString() + ", read " + back.ToString() +
                           (back == value ? "  [MATCH]" : "  [MISMATCH]");
    }

    int vector3Count_ = 0;
    std::string lastVector3Result_;
    int colorCount_ = 0;
    std::string lastColorResult_;
};

} // namespace CnaExamples::Demos::Net::NetworkSessionDemos
