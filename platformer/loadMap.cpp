#include "loadMap.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

#include "MathUtil.h"
#include "main.h"

struct SpriteType {
    std::string sheetPath;  // TODO: probably replace later
    ECSEngine::Rect sheetLocation;
    ECSEngine::Rect boundingBox;
    bool hasBoundingBox;
};

std::ifstream& expectStr(std::ifstream& os, const std::string expected) {
    std::string found;
    os >> found;

    if (found != expected) {
        throw std::runtime_error("Expected \"" + expected + "\" but found \"" + found + "\"");
    }

    return os;
}

std::ifstream& ReadPoint(std::ifstream& os, ECSEngine::Point2D& point) {
    float x, y;
    os >> x >> std::ws >> y;

    if (os.fail()) {
        throw std::runtime_error("Invalid rect");
    } else {
        point = ECSEngine::Point2D(x, y);
    }

    return os;
}

std::ifstream& ReadRect(std::ifstream& os, ECSEngine::Rect& rect) {
    float x, y, width, height;
    os >> x >> std::ws >> y >> std::ws >> width >> std::ws >> height >> std::ws;

    if (os.fail()) {
        throw std::runtime_error("Invalid rect");
    } else {
        rect = ECSEngine::Rect(x, y, width, height);
    }

    return os;
}

bool ReadDictionaryLine(std::ifstream& file, int dictionaryType,
                        std::unordered_map<char, SpriteType>& spriteTypes) {
    std::string symbol;
    file >> symbol;

    if (symbol.length() != 1) {
        file.seekg(-symbol.length(), std::fstream::cur);
        return false;
    }

    std::string sheetPath;
    file >> std::ws >> sheetPath >> std::ws;
    if (file.eof()) {
        throw std::runtime_error("Unexpected end of line");
    }

    ECSEngine::Rect sheetLocation;
    ReadRect(file, sheetLocation);
    file >> std::ws;

    if (dictionaryType == 2) {
        ECSEngine::Rect boundingBox;
        ReadRect(file, boundingBox);
        file >> std::ws;
        spriteTypes[symbol[0]] = SpriteType{sheetPath, sheetLocation, boundingBox, true};
    } else {
        spriteTypes[symbol[0]] = SpriteType{sheetPath, sheetLocation, {}, false};
    }

    return true;
}

std::unordered_map<char, SpriteType> ReadDictionary(std::ifstream& file) {
    int dictionaryType;

    expectStr(file, "dictionary");
    file >> std::ws >> dictionaryType >> std::ws;

    bool validType = dictionaryType == 1 || dictionaryType == 2;
    if (file.fail() || !validType) {
        throw std::runtime_error("Invalid dictionary type");
    }

    std::unordered_map<char, SpriteType> spriteTypes;
    while (ReadDictionaryLine(file, dictionaryType, spriteTypes));

    return spriteTypes;
}

void AddSprite(Engine& engine, SpriteType& type, ECSEngine::Point2D position) {
    // TODO: do something with all this data
}

void ReadMap(Engine& engine, std::ifstream& file, std::unordered_map<char, SpriteType>& dictonary) {
    expectStr(file, "map");
    file >> std::ws;

    expectStr(file, "origin");
    ECSEngine::Point2D origin;
    ReadPoint(file, origin);
    file >> std::ws;

    expectStr(file, "tile");
    ECSEngine::Point2D tileStride;
    ReadPoint(file, tileStride);
    file >> std::ws;

    expectStr(file, "size");
    ECSEngine::Point2D mapSize;
    ReadPoint(file, mapSize);
    file >> std::ws;

    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {
            ECSEngine::Point2D position = origin + tileStride * ECSEngine::Point2D(x, y);

            char symbol;
            file >> symbol;
            if (file.eof()) {
                throw std::runtime_error("Unexpected end of line");
            }

            if (symbol != '.') {
                if (dictonary.contains(symbol)) {
                    AddSprite(engine, dictonary[symbol], position);
                } else {
                    throw std::runtime_error("Undefined sprite \"" + std::string{symbol} + "\"");
                }
            }
        }
        file >> std::ws;
    }
}

void LoadMap(Engine& engine, const std::string& path) {
    std::ifstream file(path);

    std::unordered_map<char, SpriteType> dictonary = ReadDictionary(file);
    ReadMap(engine, file, dictonary);

    if (!file.eof()) {
        throw std::runtime_error("Expected end of file");
    }

    file.close();
}
