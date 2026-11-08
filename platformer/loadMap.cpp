#include "loadMap.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

#include "EntityManager.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "main.h"

struct TileType {
    char symbol;
    ECSEngine::SpriteID sprite;
    ECSEngine::Rect boundingBox;
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

bool ReadDictionaryLine(Engine& engine, std::ifstream& file, int dictionaryType,
                        std::unordered_map<char, TileType>& dictionary) {
    std::string symbolStr;
    file >> symbolStr;

    if (symbolStr.length() != 1) {
        file.seekg(-symbolStr.length(), std::fstream::cur);
        return false;
    }
    char symbol = symbolStr[0];

    std::string sheetPath;
    file >> std::ws >> sheetPath >> std::ws;
    if (file.eof()) {
        throw std::runtime_error("Unexpected end of line");
    }

    ECSEngine::Rect sheetLocation;
    ReadRect(file, sheetLocation);
    file >> std::ws;

    ECSEngine::SpriteID spriteId =
        engine.GetSpriteManager().RegisterTexture(sheetPath, sheetLocation);

    if (dictionaryType == 2) {
        ECSEngine::Rect boundingBox;
        ReadRect(file, boundingBox);
        file >> std::ws;
        dictionary[symbol] = TileType{symbol, spriteId, boundingBox};
    } else {
        dictionary[symbol] = TileType{symbol, spriteId, {}};
    }

    return true;
}

std::unordered_map<char, TileType> ReadDictionary(Engine& engine, std::ifstream& file) {
    int dictionaryType;

    expectStr(file, "dictionary");
    file >> std::ws >> dictionaryType >> std::ws;

    bool validType = dictionaryType == 1 || dictionaryType == 2;
    if (file.fail() || !validType) {
        throw std::runtime_error("Invalid dictionary type");
    }

    std::unordered_map<char, TileType> dictionary;
    while (ReadDictionaryLine(engine, file, dictionaryType, dictionary));

    return dictionary;
}

void AddTile(Engine& engine, TileType& type, ECSEngine::Point2D position) {
    ECSEngine::EntityManager<COMPONENTS>& entityManager = engine.GetEntityManager();
    ECSEngine::EntityID entity = entityManager.CreateEntity("Tile");

    entityManager.AddComponent(entity, ECSEngine::LocationComponent{position});

    // TODO: do something with type
    (void)type;
}

void ReadMap(Engine& engine, std::ifstream& file, std::unordered_map<char, TileType>& dictonary) {
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
                    AddTile(engine, dictonary[symbol], position);
                } else {
                    throw std::runtime_error("Undefined tile \"" + std::string{symbol} + "\"");
                }
            }
        }
        file >> std::ws;
    }
}

void LoadMap(Engine& engine, const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open map \"" + path + "\"");
    }

    std::unordered_map<char, TileType> dictonary = ReadDictionary(engine, file);
    ReadMap(engine, file, dictonary);

    if (!file.eof()) {
        throw std::runtime_error("Expected end of file");
    }

    file.close();
}
