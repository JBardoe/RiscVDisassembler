#include "disassembler/ELFFile.hpp"

#include <cstdint>

#include "utils/BadFileException.hpp"

namespace ELFParser {

void ELFFile::parseSections() {
    if (header->shnum != 0 && header->shentrySize != sizeof(SectionHeader))
        throw BadFileException("Section header table entry size is invalid");

    SectionHeader stringTableHeader;
    stream->seekg(header->shoff + header->shstrndx * header->shentrySize);
    stream->read(reinterpret_cast<char*>(&stringTableHeader),
                 sizeof(stringTableHeader));

    for (uint16_t i = 0; i < header->shnum; i++) {
        stream->seekg(header->shoff + (i * header->shentrySize));
        auto sectionhdr = std::make_unique<SectionHeader>();

        stream->read(reinterpret_cast<char*>(sectionhdr.get()),
                     sizeof(*sectionhdr));

        stream->seekg(stringTableHeader.offset + sectionhdr->name);

        auto section =
            std::make_unique<ELFSection>(this, std::move(sectionhdr));

        std::string name;
        std::getline(*stream, name, '\0');

        if (name.empty())
            name = "Unknown Section: " + std::to_string(i);  // Should not trigger

        sections[name] = std::move(section);
        sectionIndexes[i] = name;
    }
}
void ELFFile::parseSegments() {
    if (header->phnum != 0 && header->phentrySize != sizeof(SegmentHeader))
        throw BadFileException("Segment header table entry size is invalid");

    for (uint16_t i = 0; i < header->phnum; i++) {
        stream->seekg(header->phoff + (i * header->phentrySize));
        auto segmentHeader = std::make_unique<SegmentHeader>();

        if (!stream->read(reinterpret_cast<char*>(segmentHeader.get()),
                          sizeof(*segmentHeader))) {
            throw BadFileException("Failed to read program header");
        }

        segments.emplace_back(std::move(
            std::make_unique<ELFSegment>(this, std::move(segmentHeader))));
    }
}

const char* ELFSection::getData() {
    if (loaded) return data;
    if (header->size == 0) {
        loaded = true;
        return nullptr;
    }
    if (!data) data = new char[header->size];
    if (!file || !file->stream || !file->stream->good())
        throw BadFileException("Cannot find stream");

    file->stream->seekg(header->offset);
    file->stream->read(data, header->size);

    if (file->stream->gcount() != static_cast<std::streamsize>(header->size))
        throw BadFileException("Section cut off");

    loaded = true;
    return data;
}

char* ELFSegment::getData() {
    if (loaded) return data;
    if (header->fileSize == 0) {
        loaded = true;
        return nullptr;
    }
    if (!data) data = new char[header->fileSize];
    if (!file || !file->stream || !file->stream->good())
        throw BadFileException("Cannot find stream");

    file->stream->seekg(header->offset);
    file->stream->read(data, header->fileSize);

    if (file->stream->gcount() !=
        static_cast<std::streamsize>(header->fileSize))
        throw BadFileException("Segment cut off");

    loaded = true;
    return data;
}
}  // namespace ELFParser