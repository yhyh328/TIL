#include <iostream>
#include <stdexcept>
#include "sysinfo.h"

using namespace std;

int main() {
    try {
        SysInfo info;
        info.getCpuMemInfo();
        info.printSysInfo();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } 
    return 0;
}