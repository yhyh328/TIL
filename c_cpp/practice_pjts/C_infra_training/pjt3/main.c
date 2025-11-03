#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char LOG_PATH[128];
int INTERVAL;

void load_config() {
    FILE *fp = fopen("config.conf", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open config.conf\n");
        exit(1);
    }
    char key[64], val[64];
    while (fscanf(fp, "%[^=]=%s\n", key, val) != EOF) {
        if (strcmp(key, "LOG_PATH") == 0) strcpy(LOG_PATH, val);
        if (strcmp(key, "INTERVAL") == 0) INTERVAL = atoi(val);
    }
    fclose(fp);
}

void write_log(const char *msg) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/log_%04d%02d%02d.txt", LOG_PATH, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
    
    // Create directory if it doesn't exist
    struct stat st = {0};
    if (stat(LOG_PATH, &st) == -1) {
        mkdir(LOG_PATH, 0755);
    }
    
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open log file %s\n", filename);
        return;
    }
    fprintf(fp, "%02d:%02d:%02d %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
    fclose(fp);
    printf("%02d:%02d:%02d Log written: %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
}

int main() {
    load_config();
    printf("Logging service started. Log path: %s, Interval: %d seconds\n", LOG_PATH, INTERVAL);
    printf("Press Ctrl+C to stop.\n\n");
    while (1) {
        write_log("System OK");
        sleep(INTERVAL);
    }
}