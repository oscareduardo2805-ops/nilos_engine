#pragma once

#include <chrono>

namespace Nilos {

/**
 * @brief Time management system for the engine
 * 
 * Tracks frame time, delta time, and provides time utilities.
 * Essential for frame-independent movement and physics calculations.
 */
class Time {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<float>;

    /**
     * @brief Get the singleton instance
     */
    static Time& Get() {
        static Time instance;
        return instance;
    }

    /**
     * @brief Initialize the time system
     */
    void Initialize() {
        m_StartTime = Clock::now();
        m_LastFrameTime = m_StartTime;
        m_DeltaTime = 0.0f;
        m_TotalTime = 0.0f;
        m_FrameCount = 0;
        m_FPS = 0.0f;
        m_FPSAccumulator = 0.0f;
        m_FPSFrameCount = 0;
    }

    /**
     * @brief Update time at the beginning of each frame
     */
    void Update() {
        TimePoint currentTime = Clock::now();
        Duration delta = currentTime - m_LastFrameTime;
        m_DeltaTime = delta.count();
        
        m_TotalTime += m_DeltaTime;
        m_LastFrameTime = currentTime;
        m_FrameCount++;

        // Calculate FPS (updated every second)
        m_FPSAccumulator += m_DeltaTime;
        m_FPSFrameCount++;
        
        if (m_FPSAccumulator >= 1.0f) {
            m_FPS = static_cast<float>(m_FPSFrameCount) / m_FPSAccumulator;
            m_FPSAccumulator = 0.0f;
            m_FPSFrameCount = 0;
        }
    }

    /**
     * @brief Get time elapsed since last frame (in seconds)
     */
    float GetDeltaTime() const { return m_DeltaTime; }

    /**
     * @brief Get total time since engine start (in seconds)
     */
    float GetTotalTime() const { return m_TotalTime; }

    /**
     * @brief Get total number of frames rendered
     */
    uint64_t GetFrameCount() const { return m_FrameCount; }

    /**
     * @brief Get current frames per second
     */
    float GetFPS() const { return m_FPS; }

    /**
     * @brief Get time scale for slow-motion/fast-forward effects
     */
    float GetTimeScale() const { return m_TimeScale; }

    /**
     * @brief Set time scale (1.0 = normal, 0.5 = half speed, 2.0 = double speed)
     */
    void SetTimeScale(float scale) { m_TimeScale = scale; }

    /**
     * @brief Get scaled delta time (affected by time scale)
     */
    float GetScaledDeltaTime() const { return m_DeltaTime * m_TimeScale; }

private:
    Time() : m_DeltaTime(0.0f), m_TotalTime(0.0f), m_FrameCount(0), 
             m_FPS(0.0f), m_FPSAccumulator(0.0f), m_FPSFrameCount(0),
             m_TimeScale(1.0f) {}

    TimePoint m_StartTime;
    TimePoint m_LastFrameTime;
    float m_DeltaTime;
    float m_TotalTime;
    uint64_t m_FrameCount;
    float m_FPS;
    float m_FPSAccumulator;
    uint32_t m_FPSFrameCount;
    float m_TimeScale;
};

} // namespace Nilos

