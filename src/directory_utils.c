#include "directory_utils.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "options.h"

extern struct options to_avoid;

//function to understand should we avoid something
int should_avoid(char* filename) {
  if (to_avoid.type == 0) {
    return 0;
  }
  else if (to_avoid.type == 1) {
    if (regexec(&to_avoid.regex, filename, 0, NULL, 0) == 0) {
      return 1;
    }
    return 0;
  }
  else if (to_avoid.type == 2) {
    if (strcmp(to_avoid.name, filename) == 0) {
      return 1;
    }
    return 0;
  }
  return 0;
}

//receptively function to check all files and directories inside current directory
void manifest_directory(const char *path){
  DIR *dir = opendir(path);
  if (dir == NULL){
    printf("Failed to open directory %s\n", path);
    return;
  }
  
  char full_path[1024];
  struct dirent *entry;
  
  while ((entry = readdir(dir)) != NULL){
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    
    snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);    
    struct stat path_stat;
    if (!should_avoid(entry->d_name)) {
      if (stat(full_path, &path_stat) == 0) {
        if (S_ISDIR(path_stat.st_mode)) {
          if (!should_avoid(full_path)) {
            manifest_directory(full_path);
          }
        } else if (S_ISREG(path_stat.st_mode)) {
          if (!should_avoid(full_path)) {
            unsigned hashvalue = hash(full_path);
            printf("%s : %08X\n", full_path, hashvalue);
          }
        }
      }
    }
  }
  
  closedir(dir);
}
