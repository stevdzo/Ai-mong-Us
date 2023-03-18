#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)

#include <map>
#include <cassert>
#include <string>

class BaseGameEntity;

#define entity_manager EntityManager::instance()

class EntityManager {

private:

    typedef std::map<int, BaseGameEntity*> typedef_entity_map;

private:

    typedef_entity_map entity_map;

    EntityManager() {}

    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);

public:

    static EntityManager* instance();

    void add_entity(BaseGameEntity*);
    BaseGameEntity* get_entity_from_id(int) const;
    void remove_entity(BaseGameEntity*);
};
#endif