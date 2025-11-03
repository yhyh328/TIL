#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysinfo.h"

void get_cpu_mem_info(struct SysInfo *info) {
    /* ---------- CPU ---------- */
    {
        FILE *fp = fopen("/proc/stat", "r");
        if (!fp) {
            info->cpu_usage = 0.0;
        } else {
            // /proc/stat 첫 줄: cpu user nice system idle iowait irq softirq steal guest guest_nice
            unsigned long long user=0, nice=0, system=0, idle=0;
            unsigned long long iowait=0, irq=0, softirq=0, steal=0, guest=0, guest_nice=0;
            // cpu 레이블은 버립니다(%*s)
            if (fscanf(fp, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                       &user, &nice, &system, &idle,
                       &iowait, &irq, &softirq, &steal, &guest, &guest_nice) >= 4) {
                unsigned long long busy  = user + nice + system + irq + softirq + steal;
                unsigned long long total = busy + idle + iowait;
                info->cpu_usage = (total == 0) ? 0.0
                                               : 100.0 * (double)busy / (double)total;
            } else {
                info->cpu_usage = 0.0;
            }
            fclose(fp);
        }
    }

    /* ---------- MEM ---------- */
    {
        FILE *fp = fopen("/proc/meminfo", "r");
        if (!fp) {
            info->mem_usage = 0.0;
            return;
        }

        unsigned long mem_total_kb = 0;
        unsigned long mem_available_kb = 0;
        unsigned long mem_free_kb = 0;

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            unsigned long v = 0;
            // 단위는 반드시 kB (대문자 B) 이므로 포맷을 정확히 맞춥니다.
            if (sscanf(line, "MemTotal: %lu kB", &v) == 1) {
                mem_total_kb = v;
            } else if (sscanf(line, "MemAvailable: %lu kB", &v) == 1) {
                mem_available_kb = v;
            } else if (sscanf(line, "MemFree: %lu kB", &v) == 1) {
                mem_free_kb = v;
            }
        }
        fclose(fp);

        if (mem_total_kb == 0) {
            info->mem_usage = 0.0;
            return;
        }

        unsigned long used_kb;
        if (mem_available_kb > 0) {
            // 현실적인 사용량 (캐시/버퍼는 가용으로 간주)
            used_kb = mem_total_kb - mem_available_kb;
        } else {
            // 구형 커널 폴백 (러프함)
            used_kb = mem_total_kb - mem_free_kb;
        }

        info->mem_usage = 100.0 * (double)used_kb / (double)mem_total_kb;
    }
}

void print_sysinfo(const struct SysInfo *info) {
    printf("CPU Usage: %.2f%%\n", info->cpu_usage);
    printf("Memory Usage: %.2f%%\n", info->mem_usage);
}
