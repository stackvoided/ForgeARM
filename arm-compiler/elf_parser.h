#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <string>
#include <vector>
#include <cstdint>
#include "elf_header.h"

struct ElfSection {
    std::string name;
    uint32_t type;
    uint64_t flags;
    uint64_t addr;
    uint64_t offset;
    uint64_t size;
    uint64_t addralign;
    std::vector<uint8_t> data;
};

class ElfParser {
public:
    ElfParser() = default;
    ~ElfParser() = default;

    bool parse(const std::string& filepath);
    bool write(const std::string& filepath, bool target_64bit);

    bool is64Bit() const { return is_64bit; }
    uint16_t getMachine() const { return machine; }
    
    ElfSection* getSectionByName(const std::string& name);
    std::vector<ElfSection>& getSections() { return sections; }

private:
    bool is_64bit = false;
    uint16_t machine = 0;
    uint64_t entry_point = 0;
    std::vector<ElfSection> sections;

    bool parse32(const uint8_t* buffer, size_t size);
    bool parse64(const uint8_t* buffer, size_t size);
};

#endif
