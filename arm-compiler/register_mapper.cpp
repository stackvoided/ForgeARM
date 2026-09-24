#include "register_mapper.h"

RegisterMapper::RegisterMapper() {
    for (uint8_t i = 0; i <= 12; ++i) {
        auto r64 = static_cast<RegA64>(i);
        auto r32 = static_cast<RegA32>(i);
        a64_to_a32[r64] = r32;
        a32_to_a64[r32] = r64;
    }

    a64_to_a32[RegA64::X29] = RegA32::R11;
    a64_to_a32[RegA64::X30] = RegA32::LR;
    a64_to_a32[RegA64::SP]  = RegA32::SP;

    a32_to_a64[RegA32::R11] = RegA64::X29;
    a32_to_a64[RegA32::LR]  = RegA64::X30;
    a32_to_a64[RegA32::SP]  = RegA64::SP;
}

RegA32 RegisterMapper::mapToA32(RegA64 reg) {
    auto it = a64_to_a32.find(reg);
    if (it != a64_to_a32.end()) {
        return it->second;
    }
    return RegA32::INVALID;
}

RegA64 RegisterMapper::mapToA64(RegA32 reg) {
    auto it = a32_to_a64.find(reg);
    if (it != a32_to_a64.end()) {
        return it->second;
    }
    return RegA64::INVALID;
}

bool RegisterMapper::requiresSpill(RegA64 reg) const {
    uint8_t val = static_cast<uint8_t>(reg);
    return (val >= 13 && val <= 28);
}

int RegisterMapper::getSpillOffset(RegA64 reg) const {
    if (!requiresSpill(reg)) return -1;
    return (static_cast<int>(reg) - 13) * 4;
}
