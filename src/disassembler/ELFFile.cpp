#include "disassembler/ELFFile.hpp"

#include "utils/BadFileException.hpp"

void ELFFile::parseSections() {
    if (header.shnum != 0 && header.shentrySize != sizeof(SectionHeader))
        throw BadFileException("Section header table entry size is invalid");

    SectionHeader stringTableHeader;
    stream->seekg(header.shoff + header.shstrndx * header.shentrySize);
    stream->read(reinterpret_cast<char*>(&stringTableHeader),
                 sizeof(stringTableHeader));

    for (uint16_t i = 0; i < header.shnum; i++) {
        stream->seekg(header.shoff + (i * header.shentrySize));
        SectionHeader sectionhdr;

        stream->read(reinterpret_cast<char*>(&sectionhdr), sizeof(sectionhdr));

        std::unique_ptr<ELFSection> section =
            std::make_unique<ELFSection>(this, sectionhdr);

        stream->seekg(stringTableHeader.offset + sectionhdr.name);
        std::string name;
        std::getline(*stream, name, '\0');

        if (name.empty())
            name = "Unknown Section" + std::to_string(i);  // Should not trigger

        sections[name] = std::move(section);
    }
}
void ELFFile::parseSegments() {
    if (header.phnum != 0 && header.phentrySize != sizeof(SegmentHeader))
        throw BadFileException("Segment header table entry size is invalid");

    for (uint16_t i = 0; i < header.phnum; i++) {
        stream->seekg(header.phoff + (i * header.phentrySize));
        SegmentHeader segmentHeader;

        if (!stream->read(reinterpret_cast<char*>(&segmentHeader),
                          sizeof(segmentHeader))) {
            throw BadFileException("Failed to read program header");
        }

        segments.push_back(std::make_unique<ELFSegment>(this, segmentHeader));
    }
}

char* ELFSection::getData() {
    if (loaded) return data;

    loaded = true;
    return nullptr;  // TODO
}

char* ELFSegment::getData() {
    if (loaded) return data;

    loaded = true;
    return nullptr;  // TODO
}