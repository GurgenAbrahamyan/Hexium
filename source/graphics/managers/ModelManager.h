#pragma once

#include <unordered_map>
#include <memory>
#include <string>


class Model;
class EventBus;

class ModelManager {
public:
    ModelManager(
        EventBus* bus
    );

    int addModel(const std::string& name, const std::string& modelPath);

    Model* getModel(int id);
    int getModelID(const std::string& name) const;

private:
	EventBus* bus;

    std::unordered_map<int, std::unique_ptr<Model>> idMap;
    std::unordered_map<std::string, int> nameToIDMap;

    int nextID = 0;
};
