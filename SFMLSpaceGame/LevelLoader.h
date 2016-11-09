#pragma once
#include <string>

class EntityManager;

class LevelLoader
{
private:
	EntityManager& m_entMan;
public:
	explicit LevelLoader(const std::string& filePath, EntityManager& entMan);

	void Load();
};