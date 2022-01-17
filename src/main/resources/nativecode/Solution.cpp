#include "Solution.h"
#include "FileCrc32.h"
#include "Sha256.h"

#include <string>
#include <cstdint>
#include <cstring>
#include <ctime>

static const int CPU_CURRENT = 2400;       // 鲲鹏920CPU的主频 xxxx MHz
static const int CPU_EXTERNAL_CLOCK = 100; // 鲲鹏920CPU的外频 100 MHz
static const int RATIO_HMZ_TO_HZ = 1E6;    // 1MHz=1E6Hz

const char *GetSha256Digest(const char *jstr)
{
    static const Sha256 sha256;
    std::string message(jstr);
    std::string digest = sha256.GetHexMessageDiges(message);

    size_t jDigestLen = digest.size() + 1;
    char *jDigest = new char[jDigestLen];
    memcpy(jDigest, digest.c_str(), jDigestLen);
    return jDigest;
}

const char *GetCrc32Digest(const char *jstr)
{
    static FileCrc32 fileCrc32;
    std::string filePath(jstr);
    std::string crc = fileCrc32.GetFileCrc(filePath);

    size_t jCrcLen = crc.size() + 1;
    char *jCrc = new char[jCrcLen];
    memcpy(jCrc, crc.c_str(), jCrcLen);
    return jCrc;
}

const char *GetCpuClocks(void)
{
    uint64_t clocks;
    uint32_t lo, hi;
#ifdef __x86_64__
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    clocks = (uint64_t)hi << 32 | lo;
#elif defined __aarch64__
    __asm__ __volatile__("mrs %0, cntvct_el0"
                         : "=r"(clocks));
    clocks = clocks * CPU_CURRENT / CPU_EXTERNAL_CLOCK;
#else
    clocks = clock() / CLOCKS_PER_SEC * CPU_CURRENT * RATIO_HMZ_TO_HZ;
#endif
    std::string strClocks = std::to_string(clocks);
    return strClocks.c_str();
}
