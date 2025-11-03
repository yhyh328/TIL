#ifndef SYSINFO_H
#define SYSINFO_H

struct SysInfo {
    double cpu_usage;
    double mem_usage;
};

void get_cpu_mem_info(struct SysInfo *info);
void print_sysinfo(const struct SysInfo *info);

#endif