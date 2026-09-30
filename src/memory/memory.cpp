#include "memory.h"
int memory_utils::pid_ = -1;
static bool can_write_ = false;

#ifndef SYS_process_vm_writev
#define SYS_process_vm_writev 310
#endif

void memory_utils::initialize(int pid)
{
    pid_ = pid;
    
    char maps_path[64];
    sprintf(maps_path, "/proc/%d/maps", pid);
    
    FILE* mapsfd = fopen(maps_path, "r");
    if (!mapsfd) {
        can_write_ = false;
        return;
    }
    
    uintptr_t addr = 0;
    char ln[256];
    if (fgets(ln, sizeof(ln), mapsfd)) {
        uintptr_t s, e;
        if (sscanf(ln, "%lx-%lx", &s, &e) == 2) {
            addr = s + 0x1000;
        }
    }
    fclose(mapsfd);
    
    struct iovec l[1], r[1];
    char test[4];
    
    l[0].iov_base = test;
    l[0].iov_len = sizeof(test);
    
    r[0].iov_base = (void *)(addr > 0 ? addr : 0x10000);
    r[0].iov_len = sizeof(test);
    
    ssize_t res = process_vm_readv(pid, l, 1, r, 1, 0);
    if (res < 0) {
        for(int i = 0; i < 5 && res < 0; i++) {
            usleep(10000);
            res = process_vm_readv(pid, l, 1, r, 1, 0);
        }
    }
    
    can_write_ = (res >= 0);
}

bool memory_utils::read_memory(void *address, void *buffer, size_t size)
{
    return process_vm(address, buffer, size, false);
}

bool memory_utils::write_memory(void *address, void *buffer, size_t size)
{
    return process_vm(address, buffer, size, true);
}

bool memory_utils::is_address_valid(uint64_t addr)
{
    if (addr == 0 || addr <= 0x1000)
    {
        return false;
    }
    return virtual_to_physical(addr);
}

bool memory_utils::process_vm(void *address, void *buffer, size_t size, bool is_write)
{
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;

    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (pid_ < 0)
    {
        return false;
    }

    ssize_t bytes;
    if (is_write) {
        bytes = process_vm_writev(pid_, local, 1, remote, 1, 0);
    } else {
        bytes = process_vm_readv(pid_, local, 1, remote, 1, 0);
    }
    
    return bytes == static_cast<ssize_t>(size);
}

bool memory_utils::virtual_to_physical(uint64_t vaddr)
{
    static int page_size = getpagesize();
    int fd = get_fd();
    if (fd < 0)
    {
        return false;
    }

    unsigned long v_page_index = vaddr / page_size;
    off_t pfn_item_offset = static_cast<off_t>(v_page_index * sizeof(uint64_t));
    uint64_t item;
    if (lseek(fd, pfn_item_offset, SEEK_SET) < 0)
    {
        return false;
    }
    if (::read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))
    {
        return false;
    }

    return (item & (1ULL << 63)) != 0;
}

int memory_utils::get_fd()
{
    static int fd = -1;
    if (fd < 0)
    {
        char filename[32];
        snprintf(filename, sizeof(filename), "/proc/%d/pagemap", pid_);
        fd = open(filename, O_RDONLY);
    }
    return fd;
}