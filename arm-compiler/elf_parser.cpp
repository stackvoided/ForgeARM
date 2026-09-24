#include "elf_parser.h"
#include <fstream>
#include <iostream>
#include <cstring>

bool ElfParser::parse(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize < 16) return false;

    std::vector<uint8_t> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) return false;

    if (buffer[0] != 0x7F || buffer[1] != 'E' || buffer[2] != 'L' || buffer[3] != 'F') {
        return false;
    }

    is_64bit = (buffer[4] == 2);

    if (is_64bit) {
        return parse64(buffer.data(), fileSize);
    } else {
        return parse32(buffer.data(), fileSize);
    }
}

bool ElfParser::parse32(const uint8_t* buffer, size_t size) {
    if (size < sizeof(Elf32_Ehdr)) return false;

    const auto* ehdr = reinterpret_cast<const Elf32_Ehdr*>(buffer);
    machine = ehdr->e_machine;
    entry_point = ehdr->e_entry;

    if (ehdr->e_shoff + (ehdr->e_shnum * sizeof(Elf32_Shdr)) > size) return false;

    const auto* shdrs = reinterpret_cast<const Elf32_Shdr*>(buffer + ehdr->e_shoff);

    const char* strtab = nullptr;
    if (ehdr->e_shstrndx < ehdr->e_shnum) {
        const auto& strShdr = shdrs[ehdr->e_shstrndx];
        if (strShdr.sh_offset + strShdr.sh_size <= size) {
            strtab = reinterpret_cast<const char*>(buffer + strShdr.sh_offset);
        }
    }

    sections.clear();
    sections.reserve(ehdr->e_shnum);

    for (uint16_t i = 0; i < ehdr->e_shnum; ++i) {
        const auto& sh = shdrs[i];
        ElfSection sec;
        
        if (strtab && sh.sh_name != 0) {
            sec.name = std::string(strtab + sh.sh_name);
        } else {
            sec.name = "sec_" + std::to_string(i);
        }

        sec.type = sh.sh_type;
        sec.flags = sh.sh_flags;
        sec.addr = sh.sh_addr;
        sec.offset = sh.sh_offset;
        sec.size = sh.sh_size;
        sec.addralign = sh.sh_addralign;

        if (sh.sh_offset + sh.sh_size <= size) {
            sec.data.assign(buffer + sh.sh_offset, buffer + sh.sh_offset + sh.sh_size);
        }

        sections.push_back(std::move(sec));
    }

    return true;
}

bool ElfParser::parse64(const uint8_t* buffer, size_t size) {
    if (size < sizeof(Elf64_Ehdr)) return false;

    const auto* ehdr = reinterpret_cast<const Elf64_Ehdr*>(buffer);
    machine = ehdr->e_machine;
    entry_point = ehdr->e_entry;

    if (ehdr->e_shoff + (ehdr->e_shnum * sizeof(Elf64_Shdr)) > size) return false;

    const auto* shdrs = reinterpret_cast<const Elf64_Shdr*>(buffer + ehdr->e_shoff);

    const char* strtab = nullptr;
    if (ehdr->e_shstrndx < ehdr->e_shnum) {
        const auto& strShdr = shdrs[ehdr->e_shstrndx];
        if (strShdr.sh_offset + strShdr.sh_size <= size) {
            strtab = reinterpret_cast<const char*>(buffer + strShdr.sh_offset);
        }
    }

    sections.clear();
    sections.reserve(ehdr->e_shnum);

    for (uint16_t i = 0; i < ehdr->e_shnum; ++i) {
        const auto& sh = shdrs[i];
        ElfSection sec;

        if (strtab && sh.sh_name != 0) {
            sec.name = std::string(strtab + sh.sh_name);
        } else {
            sec.name = "sec_" + std::to_string(i);
        }

        sec.type = sh.sh_type;
        sec.flags = sh.sh_flags;
        sec.addr = sh.sh_addr;
        sec.offset = sh.sh_offset;
        sec.size = sh.sh_size;
        sec.addralign = sh.sh_addralign;

        if (sh.sh_offset + sh.sh_size <= size) {
            sec.data.assign(buffer + sh.sh_offset, buffer + sh.sh_offset + sh.sh_size);
        }

        sections.push_back(std::move(sec));
    }

    return true;
}

ElfSection* ElfParser::getSectionByName(const std::string& name) {
    for (auto& sec : sections) {
        if (sec.name == name) return &sec;
    }
    return nullptr;
}

bool ElfParser::write(const std::string& filepath, bool target_64bit) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    if (target_64bit) {
        Elf64_Ehdr ehdr;
        std::memset(&ehdr, 0, sizeof(ehdr));
        ehdr.e_ident[0] = 0x7F; ehdr.e_ident[1] = 'E'; ehdr.e_ident[2] = 'L'; ehdr.e_ident[3] = 'F';
        ehdr.e_ident[4] = 2;
        ehdr.e_ident[5] = 1;
        ehdr.e_ident[6] = 1;
        ehdr.e_type = 3;
        ehdr.e_machine = EM_AARCH64;
        ehdr.e_version = 1;
        ehdr.e_ehsize = sizeof(Elf64_Ehdr);
        ehdr.e_shentsize = sizeof(Elf64_Shdr);
        ehdr.e_shnum = static_cast<uint16_t>(sections.size());

        file.write(reinterpret_cast<const char*>(&ehdr), sizeof(ehdr));
    } else {
        Elf32_Ehdr ehdr;
        std::memset(&ehdr, 0, sizeof(ehdr));
        ehdr.e_ident[0] = 0x7F; ehdr.e_ident[1] = 'E'; ehdr.e_ident[2] = 'L'; ehdr.e_ident[3] = 'F';
        ehdr.e_ident[4] = 1;
        ehdr.e_ident[5] = 1;
        ehdr.e_ident[6] = 1;
        ehdr.e_type = 3;
        ehdr.e_machine = EM_ARM;
        ehdr.e_version = 1;
        ehdr.e_ehsize = sizeof(Elf32_Ehdr);
        ehdr.e_shentsize = sizeof(Elf32_Shdr);
        ehdr.e_shnum = static_cast<uint16_t>(sections.size());

        file.write(reinterpret_cast<const char*>(&ehdr), sizeof(ehdr));
    }

    for (const auto& sec : sections) {
        if (!sec.data.empty()) {
            file.write(reinterpret_cast<const char*>(sec.data.data()), sec.data.size());
        }
    }

    return true;
}
