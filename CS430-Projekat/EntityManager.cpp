#include "EntityManager.h"
#include "BaseGameEntity.h"

EntityManager* EntityManager::instance() {

    static EntityManager instance;

    return &instance;
}

BaseGameEntity* EntityManager::get_entity_from_id(int id) const {

    typedef_entity_map::const_iterator ent = entity_map.find(id);

    assert((ent != entity_map.end()) && "invalid id");

    return ent->second;
}

void EntityManager::remove_entity(BaseGameEntity* pEntity) {

    entity_map.erase(entity_map.find(pEntity->get_id()));
}

void EntityManager::add_entity(BaseGameEntity* new_entity) {

    entity_map.insert(std::make_pair(new_entity->get_id(), new_entity));
}