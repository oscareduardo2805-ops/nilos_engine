# Phase 3 Demo - Gameplay Foundations

## What's New in Phase 3

### 1. **AABB Collision System**
- Axis-Aligned Bounding Box collision detection
- Fast and simple collision checks
- Raycast support for line-of-sight checks

### 2. **Physics World**
- Basic gravity simulation
- Simple collision resolution
- Foundation for rigid body physics

### 3. **A* Pathfinding**
- Grid-based navigation
- Manhattan heuristic for speed
- Obstacle avoidance
- Perfect for NPC navigation

### 4. **Model Loader Foundation**
- Structure ready for Assimp integration
- Placeholder implementation
- Will be expanded when models are needed

## Testing Phase 3

Run the engine:
```powershell
.\run.ps1
```

### Current Demo Scene:
- Multiple cubes with Phong lighting
- Rotating objects to show lighting
- Physics system running in background
- Pathfinding available for future NPCs

### Debug Visualization (Coming Soon):
- Green wireframes around collision boxes
- Yellow lines showing pathfinding routes
- Red/Green indicators for collision states

## Code Examples

### Using AABB Collisions:
```cpp
// Create bounding box
AABB box = AABB::FromCenterSize(glm::vec3(0, 1, 0), glm::vec3(2, 2, 2));

// Check collision
AABB other = AABB::FromCenterSize(glm::vec3(1, 1, 0), glm::vec3(1, 1, 1));
if (box.Intersects(other)) {
    // Handle collision
}

// Raycast
Ray ray(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
float tMin, tMax;
if (ray.Intersects(box, tMin, tMax)) {
    glm::vec3 hitPoint = ray.GetPoint(tMin);
}
```

### Using Pathfinding:
```cpp
Pathfinding pathfinder(1.0f); // 1 meter cell size

glm::vec3 start(0, 0, 0);
glm::vec3 goal(10, 0, 10);

std::vector<glm::vec3> obstacles = { /* obstacle positions */ };

std::vector<glm::vec3> path = pathfinder.FindPath(start, goal, obstacles);

// Follow path
for (const glm::vec3& waypoint : path) {
    // Move NPC to waypoint
}
```

### Using Physics World:
```cpp
// Update physics each frame
m_PhysicsWorld->Update(deltaTime);

// Register collider
m_PhysicsWorld->RegisterCollider(collider, transform);

// Raycast
glm::vec3 hitPoint;
Entity hitEntity;
Ray ray(origin, direction);
if (m_PhysicsWorld->Raycast(ray, 100.0f, hitPoint, hitEntity)) {
    // Hit something!
}
```

## Performance Notes

### Phase 3 is Lightweight:
- AABB checks are O(1) - extremely fast
- A* is optimized for small grids
- Physics is simplified (no rigid body dynamics)
- Everything runs at 60 FPS easily

### Scalability:
- Can handle hundreds of colliders
- Pathfinding works for grids up to 100x100
- Physics system is placeholder for now (no heavy simulation)

## Next Steps (Phase 4)

Phase 3 provides the foundation for:
- **AI Behavior**: NPCs can navigate and avoid obstacles
- **Interactive Objects**: Collision detection enables interactions
- **Game Mechanics**: Physics + pathfinding = gameplay

Ready for Phase 4: AI Systems!

