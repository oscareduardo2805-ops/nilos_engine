#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>

namespace Nilos {

/**
 * @brief Logging levels for the engine
 */
enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

/**
 * @brief Simple logging system for engine diagnostics
 * 
 * Provides formatted console output with timestamps and log levels.
 * Future enhancement: file logging, colored output, log filtering
 */
class Logger {
public:
    /**
     * @brief Get the singleton instance
     */
    static Logger& Get() {
        static Logger instance;
        return instance;
    }

    /**
     * @brief Set the minimum log level to display
     */
    void SetLogLevel(LogLevel level) {
        m_MinLevel = level;
    }

    /**
     * @brief Log a trace message (most verbose)
     */
    template<typename... Args>
    void Trace(Args&&... args) {
        Log(LogLevel::Trace, "TRACE", std::forward<Args>(args)...);
    }

    /**
     * @brief Log a debug message
     */
    template<typename... Args>
    void Debug(Args&&... args) {
        Log(LogLevel::Debug, "DEBUG", std::forward<Args>(args)...);
    }

    /**
     * @brief Log an info message
     */
    template<typename... Args>
    void Info(Args&&... args) {
        Log(LogLevel::Info, "INFO", std::forward<Args>(args)...);
    }

    /**
     * @brief Log a warning message
     */
    template<typename... Args>
    void Warning(Args&&... args) {
        Log(LogLevel::Warning, "WARNING", std::forward<Args>(args)...);
    }

    /**
     * @brief Log an error message
     */
    template<typename... Args>
    void Error(Args&&... args) {
        Log(LogLevel::Error, "ERROR", std::forward<Args>(args)...);
    }

    /**
     * @brief Log a critical error message
     */
    template<typename... Args>
    void Critical(Args&&... args) {
        Log(LogLevel::Critical, "CRITICAL", std::forward<Args>(args)...);
    }

private:
    Logger() : m_MinLevel(LogLevel::Trace) {}
    
    template<typename... Args>
    void Log(LogLevel level, const char* levelStr, Args&&... args) {
        if (level < m_MinLevel) return;

        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::tm tm;
        #ifdef _WIN32
            localtime_s(&tm, &time);
        #else
            localtime_r(&time, &tm);
        #endif

        // Build message
        std::ostringstream oss;
        oss << "[" << std::put_time(&tm, "%H:%M:%S") << "." 
            << std::setfill('0') << std::setw(3) << ms.count() << "] "
            << "[" << levelStr << "] ";
        
        // Concatenate all arguments
        ((oss << std::forward<Args>(args)), ...);
        
        // Output to console (stderr for errors)
        if (level >= LogLevel::Error) {
            std::cerr << oss.str() << std::endl;
        } else {
            std::cout << oss.str() << std::endl;
        }
    }

    LogLevel m_MinLevel;
};

// Convenience macros
#define NILOS_TRACE(...)    ::Nilos::Logger::Get().Trace(__VA_ARGS__)
#define NILOS_DEBUG(...)    ::Nilos::Logger::Get().Debug(__VA_ARGS__)
#define NILOS_INFO(...)     ::Nilos::Logger::Get().Info(__VA_ARGS__)
#define NILOS_WARNING(...)  ::Nilos::Logger::Get().Warning(__VA_ARGS__)
#define NILOS_ERROR(...)    ::Nilos::Logger::Get().Error(__VA_ARGS__)
#define NILOS_CRITICAL(...) ::Nilos::Logger::Get().Critical(__VA_ARGS__)

} // namespace Nilos

