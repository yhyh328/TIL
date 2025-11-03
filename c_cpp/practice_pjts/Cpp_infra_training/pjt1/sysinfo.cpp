#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include "sysinfo.h"

void SysInfo::parseCpuInfo() {
    std::ifstream fp("/proc/stat");
    if (!fp.is_open()) {
        throw std::runtime_error("Cannot open /proc/stat");
    }
    
    unsigned long long user, nice, system, idle;
    std::string cpu_label;
    fp >> cpu_label >> user >> nice >> system >> idle;
    fp.close();
    
    cpu_usage = 100.0 * (user + nice + system) / (user + nice + system + idle);
}

void SysInfo::parseMemInfo() {
    std::ifstream fp("/proc/meminfo");
    if (!fp.is_open()) {
        throw std::runtime_error("Cannot open /proc/meminfo");
    }
    
    std::string label;
    unsigned long total, free;
    
    fp >> label >> total >> label;
    fp >> label >> free >> label;
    fp.close();
    
    mem_usage = 100.0 * (total - free) / total;
}

void SysInfo::getCpuMemInfo() {
    parseCpuInfo();
    parseMemInfo();
}

void SysInfo::printSysInfo() const {
    std::cout << "CPU Usage: " << std::fixed << std::setprecision(2) 
              << cpu_usage << "%" << std::endl;
    std::cout << "Memory Usage: " << std::fixed << std::setprecision(2) 
              << mem_usage << "%" << std::endl;
}