#pragma once
#include <cstdint>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <dlfcn.h>
#include <dirent.h>
#include <elf.h>
#include <sys/mman.h>
#include <functional>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>

class memory_utils
{
public:
    static void initialize(int pid);
    static bool read_memory(void *address, void *buffer, size_t size);
    static bool write_memory(void *address, void *buffer, size_t size);
    static bool is_address_valid(uint64_t addr);

    template <typename T>
    static T read(uint64_t addr)
    {
        T data{};
        if (!is_address_valid(addr))
        {
            return data;
        }
        read_memory(reinterpret_cast<void *>(addr), &data, sizeof(T));
        return data;
    }

    static int open_proccess_memory(const pid_t pid)
    {
        char name[255] = {0};
        sprintf(name, "/proc/%d/mem\0", pid);
        return open(name, O_RDWR);
    }

        static ssize_t writemem(uintptr_t addr, void* value, size_t size)
        {
            struct iovec src[1];
            src[0].iov_base = value;
            src[0].iov_len = size;

            struct iovec dst[1];
            dst[0].iov_base = (void *)addr;
            dst[0].iov_len = size;

            auto bytesWritten = process_vm_writev(pid_, src, 1, dst, 1, 0);

            return bytesWritten;
        }
    template<typename T>
    static void write(uintptr_t addr, T newValue) {
        writemem(addr, &newValue, sizeof(T));
    }

private:
    static int pid_;
    static bool process_vm(void *address, void *buffer, size_t size, bool is_write);
    static bool virtual_to_physical(uint64_t vaddr);
    static int get_fd();
};