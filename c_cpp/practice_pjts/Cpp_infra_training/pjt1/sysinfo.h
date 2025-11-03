#ifndef SYSINFO_H
#define SYSINFO_H

class SysInfo {
    
    private:
        double cpu_usage;
        double mem_usage;
        void parseCpuInfo();
        void parseMemInfo();
    
    public:
        SysInfo() : cpu_usage(0.0), mem_usage(0.0) {}
        void getCpuMemInfo();
        void printSysInfo() const;
        double getCpuUsage() const { return cpu_usage; }
        double getMemUsage() const { return mem_usage; }

};

#endif