#pragma once

#include "../ECS/System.h"
#include "../ECS/World.h"
#include "../Core/Logger.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Nilos {

/**
 * @brief AI System for NPC behavior and decision making (Placeholder for Phase 5)
 * 
 * Future implementation will include:
 * 
 * 1. Behavior Trees:
 *    - Composite nodes (Sequence, Selector, Parallel)
 *    - Decorator nodes (Inverter, Repeater, Condition)
 *    - Leaf nodes (Actions, Conditions)
 * 
 * 2. GOAP (Goal-Oriented Action Planning):
 *    - Action planning based on current state and goals
 *    - Dynamic replanning when conditions change
 *    - Integration with world state
 * 
 * 3. External AI Integration:
 *    - REST API clients for OpenAI, Anthropic, etc.
 *    - Local model support (llama.cpp, ONNX Runtime)
 *    - Prompt management and context building
 *    - Token budget management
 * 
 * 4. Memory System:
 *    - Short-term memory (recent events)
 *    - Long-term memory (persistent storage)
 *    - Embedding-based retrieval (vector database)
 *    - Memory consolidation and importance scoring
 * 
 * 5. Sensory System:
 *    - Visual perception (raycast-based line of sight)
 *    - Audio perception (sound propagation)
 *    - Touch/collision sensing
 *    - Perception filtering and attention
 * 
 * 6. Learning and Adaptation:
 *    - Reinforcement learning integration
 *    - Behavior evolution based on outcomes
 *    - Skill acquisition and improvement
 * 
 * Current status: Basic placeholder structure only
 */
class AISystem : public System {
public:
    AISystem() = default;

    void Initialize() override {
        NILOS_INFO("AISystem initialized (placeholder mode)");
        NILOS_INFO("  Future: Behavior trees, GOAP, external AI APIs");
    }

    void Update(float deltaTime) override {
        // Placeholder for AI update loop
        
        // Future implementation:
        // 1. Update perception for all AI agents
        // 2. Process sensory data and update world knowledge
        // 3. Run decision-making systems (behavior tree/GOAP)
        // 4. Execute actions based on decisions
        // 5. Update memory systems
        // 6. Handle AI-to-AI communication
        
        // Example structure:
        // auto agents = m_World->GetEntitiesWithComponent<AIAgentComponent>();
        // for (auto entity : agents) {
        //     auto* agent = m_World->GetComponent<AIAgentComponent>(entity);
        //     
        //     // Update at specified intervals to save CPU
        //     agent->TimeSinceLastUpdate += deltaTime;
        //     if (agent->TimeSinceLastUpdate >= agent->UpdateInterval) {
        //         UpdatePerception(entity, agent);
        //         ProcessDecisions(entity, agent);
        //         agent->TimeSinceLastUpdate = 0.0f;
        //     }
        // }
    }

    void Shutdown() override {
        NILOS_INFO("AISystem shutdown");
    }

    const char* GetName() const override {
        return "AISystem";
    }

private:
    // Future members:
    // std::unique_ptr<BehaviorTreeManager> m_BehaviorTrees;
    // std::unique_ptr<GOAPPlanner> m_GOAPPlanner;
    // std::unique_ptr<AIMemoryDatabase> m_MemorySystem;
    // std::unique_ptr<AIApiClient> m_ExternalAIClient;
    // PerceptionManager m_PerceptionManager;
};

/**
 * @brief Behavior tree node types (Placeholder)
 * 
 * This demonstrates the structure for future behavior tree implementation.
 */
enum class BehaviorNodeType {
    // Composite nodes
    Sequence,      // Execute children in order, fail if any fails
    Selector,      // Execute children until one succeeds
    Parallel,      // Execute multiple children simultaneously
    
    // Decorator nodes
    Inverter,      // Invert result of child
    Repeater,      // Repeat child N times or forever
    Condition,     // Only execute if condition is met
    
    // Leaf nodes
    Action,        // Execute an action
    Wait          // Wait for specified time
};

/**
 * @brief GOAP action structure (Placeholder)
 * 
 * Actions in GOAP have preconditions and effects.
 */
struct GOAPAction {
    std::string Name;
    float Cost = 1.0f;
    
    // Preconditions (world state requirements)
    std::unordered_map<std::string, bool> Preconditions;
    
    // Effects (how this action changes world state)
    std::unordered_map<std::string, bool> Effects;
    
    // Function pointer to actual action implementation
    // std::function<bool(Entity)> Execute;
};

/**
 * @brief AI memory entry structure (Placeholder)
 */
struct MemoryEntry {
    uint64_t Timestamp;           // When this memory was created
    std::string Description;       // What happened
    float Importance;              // How important this memory is (0-1)
    std::vector<float> Embedding;  // Vector embedding for similarity search
    
    // Future: associated entities, locations, emotions, etc.
};

/**
 * @brief External AI API configuration (Placeholder)
 */
struct AIApiConfig {
    enum class Provider {
        OpenAI,
        Anthropic,
        LocalModel
    };
    
    Provider ApiProvider = Provider::OpenAI;
    std::string ApiKey;
    std::string ModelName = "gpt-4";
    float Temperature = 0.7f;
    int MaxTokens = 500;
    
    // For local models
    std::string ModelPath;
};

} // namespace Nilos

