#include <stdio.h>
#include "sysinfo.h"

int main(void) {
    struct SysInfo info;
    get_cpu_mem_info(&info);
    print_sysinfo(&info);
    return 0;
}