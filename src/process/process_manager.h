#pragma once
#include <cstdint>
#include <string>

class process_manager {
public:
    process_manager(const std::string& package_name);
    bool initialize();
    int get_pid() const;
    uint64_t get_libunity_base() const;

private:
    std::string package_name_;
    int pid_;
    uint64_t libunity_base_;

    int find_process_id();
    uint64_t find_module_base(const char* module_name, int mod);
};