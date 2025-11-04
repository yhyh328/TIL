#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>

class ConfigManager {
private:
    std::map<std::string, std::string> config;
    
public:
    void loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config.conf");
        }
        
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                config[key] = value;
            }
        }
        file.close();
    }
    
    std::string get(const std::string& key, const std::string& default_value = "") const {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : default_value;
    }
    
    int getInt(const std::string& key, int default_value = 0) const {
        auto it = config.find(key);
        return (it != config.end()) ? std::stoi(it->second) : default_value;
    }
};

class Logger {
private:
    std::string log_path;
    
    void ensureDirectoryExists(const std::string& path) {
        struct stat st = {0};
        if (stat(path.c_str(), &st) == -1) {
            mkdir(path.c_str(), 0755);
        }
    }
    
    std::string getLogFilename() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        
        std::ostringstream oss;
        oss << log_path << "/log_" 
            << std::setfill('0') << std::setw(4) << (tm->tm_year + 1900)
            << std::setw(2) << (tm->tm_mon + 1)
            << std::setw(2) << tm->tm_mday << ".txt";
        return oss.str();
    }
    
public:
    Logger(const std::string& path) : log_path(path) {
        ensureDirectoryExists(log_path);
    }
    
    void writeLog(const std::string& msg) {
        std::string filename = getLogFilename();
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open log file " << filename << std::endl;
            return;
        }
        
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        
        file << std::setfill('0') << std::setw(2) << tm->tm_hour << ":"
             << std::setw(2) << tm->tm_min << ":"
             << std::setw(2) << tm->tm_sec << " " << msg << std::endl;
        file.close();
        
        std::cout << std::setfill('0') << std::setw(2) << tm->tm_hour << ":"
                  << std::setw(2) << tm->tm_min << ":"
                  << std::setw(2) << tm->tm_sec << " Log written: " << msg << std::endl;
    }
};

int main() {
    try {
        ConfigManager config;
        config.loadConfig("config.conf");
        
        std::string log_path = config.get("LOG_PATH", "./logs");
        int interval = config.getInt("INTERVAL", 5);
        
        Logger logger(log_path);
        
        std::cout << "Logging service started. Log path: " << log_path 
                  << ", Interval: " << interval << " seconds" << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl << std::endl;
        
        while (true) {
            logger.writeLog("System OK");
            sleep(interval);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}