#pragma once

#include <cstdint>
#include <limits>

namespace Nilos {

/**
 * @brief Entity is just a unique ID
 * 
 * In pure ECS architecture, entities are lightweight identifiers.
 * All data is stored in components, all logic in systems.
 */
using Entity = uint32_t;

/**
 * @brief Special value representing an invalid/null entity
 */
constexpr Entity NULL_ENTITY = std::numeric_limits<Entity>::max();

/**
 * @brief Maximum number of entities supported
 */
constexpr uint32_t MAX_ENTITIES = 10000;

/**
 * @brief Maximum number of component types supported
 */
constexpr uint32_t MAX_COMPONENTS = 64;

} // namespace Nilos

