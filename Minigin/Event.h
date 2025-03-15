#pragma once

#include <cstdint>
#include <vector>
#include <any>
#include <string>

// ==================== Event ID Hashing ====================
constexpr unsigned int make_sdbm_hash(const char* str, unsigned int hash = 0) {
    return (*str) ? make_sdbm_hash(str + 1, *str + (hash << 6) + (hash << 16) - hash) : hash;
}

using EventId = unsigned int;

// ==================== Event Argument Structure ====================
struct EventArg {};

// ==================== Event Structure ====================
struct Event {
    static const uint8_t MAX_ARGS = 8;

    const EventId id;
    std::string name;  // Human-readable event name
    uint8_t nbArgs = 0;
    std::vector<std::any> args; // Dynamic argument list

    explicit Event(EventId _id, std::string _name = "")
        : id{ _id }, name{ std::move(_name) } {
    }

    template <typename T>
    void AddArgument(T arg) {
        if (nbArgs < MAX_ARGS) {
            args.push_back(arg);
            nbArgs++;
        }
    }
};
