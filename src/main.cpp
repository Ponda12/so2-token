#include "memory/memory.h"
#include "process/process_manager.h"
#include <cstdint>
#include <cstdio>
#include <string>

std::string get_token(uint64_t il2cpp) {
    uint64_t typeinfo = memory_utils::read<uint64_t>(il2cpp + 9886336);
    if (!typeinfo) return "";

    uint64_t staticfields = memory_utils::read<uint64_t>(typeinfo + 0x40);   
    if (!staticfields) return "";

    uint64_t authmanager = memory_utils::read<uint64_t>(staticfields + 0x0);
    if (!authmanager) return "";

    uint64_t state = memory_utils::read<uint64_t>(authmanager + 0xC8);       
    if (!state) return "";

    uint64_t ticket = memory_utils::read<uint64_t>(state + 0x20);            
    if (!ticket) return "";

    uint32_t len = memory_utils::read<uint32_t>(ticket + 0x18);              
    if (!len || len > 1024) return "";

    std::string token;
    token.reserve(len);
    for (uint32_t i = 0; i < len; i++) {
        token.push_back((char)memory_utils::read<uint8_t>(ticket + 0x20 + i)); 
    }
    return token;
}

int main() {
    process_manager proc("com.axlebolt.standoff2");
    if (!proc.initialize()) return -1;

    memory_utils::initialize(proc.get_pid());

    uint64_t il2cpp = proc.get_libunity_base();
    std::string token = get_token(il2cpp);

    if (token.empty()) return -1;

    printf("External Token Getter by @auron_creator (1.0)\n");
    printf("Token: %s\n", token.c_str());
    return 0;
}