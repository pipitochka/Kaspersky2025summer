#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

//test of help flag
void test_help_flag(){
  printf("test_help_flag\n");
  
  FILE *fp = popen("./mangen -h", "r");
  assert(fp != NULL);
  
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  
  char *expected_output = "run ./mangen [DIR_PATH] [OPTIONS]";  
  fgets(buffer, sizeof(buffer), fp);
  assert(buffer != NULL);
  
  assert(strstr(buffer, expected_output));
 
  pclose(fp);
}

//test version flag
void test_version_flag(){
  printf("test_version_flag\n");
    
  FILE *fp = popen("./mangen -v", "r");
  assert(fp != NULL);
  
  char buffer[256];
  char *expected_output = "version 1.0";  
  
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    assert(strstr(buffer, expected_output) != NULL); 
  } else {
    assert(0);  
  }

  expected_output = "author: Akulov Artem";  
  
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    assert(strstr(buffer, expected_output) != NULL);  
  } else {
    assert(0);  
  }
 
  pclose(fp);
}

//addition function to create directory for test
void create_directory(char* directory_name){
  if (mkdir(directory_name, 0777) != 0) {
    printf("mkdir error\n");
  }
}

//addition function to create file for test
void create_file(char* file_name, char* content){
  FILE* fp = fopen(file_name, "w");
  assert(fp != NULL);
  fprintf(fp, "%s", content);
  fclose(fp);
}

//addition function to delete directory created for test
void free_directory(char* directory_path){
  char command[1024];
  snprintf(command, sizeof(command), "rm -rf %s", directory_path);
  int ret = system(command);
  assert(ret == 0);
}

//function to test mangen functionality
void test_mangen(){
  printf("test_mangen\n");
  
  create_directory("./test");
  create_directory("./test/1");
  create_directory("./test/2");
  create_file("./test/1/file1.txt", "privet");
  create_file("./test/2/file2.txt", "kak dela");
  create_file("./test/1/file3.c", "int main");
  
  FILE *fp = popen("./mangen test", "r");
  
  char buffer[1024];
  
  char *expected_output = "test/1/file1.txt : 0000029A";  
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    assert(strstr(buffer, expected_output) != NULL); 
  } else {
    assert(0);  
  }
  
  expected_output = "test/1/file3.c : 00000310";  
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    assert(strstr(buffer, expected_output) != NULL); 
  } else {
    assert(0);  
  }
  
  expected_output = "test/2/file2.txt : 000002ED";  
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    assert(strstr(buffer, expected_output) != NULL); 
  } else {
    assert(0);  
  }
  
  pclose(fp);
}

//test cases
int main() {
  test_version_flag();
  test_help_flag();
  test_mangen();
}