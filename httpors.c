#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>

FILE *fd = NULL;
char BUFFER[128];
size_t total_size = 0;

int show_status = 0;
int show_location = 0;
int show_content_length = 0;
int show_ip = 0;

size_t write_callback(void *ptr,size_t size,size_t nmemb,void *usrdata){
  total_size += size * nmemb;
  return size * nmemb;
}

int ck_std();
void banner();
void usage();
void handler();
void httpors();

int main(int argc, char *argv[]){

  banner();
  if (argc < 2 && ck_std()){
    usage(argv[0]);
  }

  for (int i = 1;i < argc;i++){
    if (strcmp(argv[i],"-f") == 0 || strcmp(argv[i],"--file") == 0) {
      fd = fopen(argv[i+1],"r");
      if (!fd) {
        printf("Error to open file\n");
        return 1;
      }
      i++;

    } else if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i],"--domain") == 0) {
      printf("domain %s\n",argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"--status-code") == 0) show_status = 1;
    else if (strcmp(argv[i],"--location") == 0) show_location = 1;
    else if (strcmp(argv[i],"--content-length") == 0) show_content_length = 1;
    else if (strcmp(argv[i],"--ip") == 0) show_ip = 1;
    else {
      printf("Unknown option %s\n",argv[i]);
      return 1;
    }
  }

  handler();

  return 0;
}

void banner() {
  printf("\n");
  printf("    __    __  __                       \n");
  printf("   / /_  / /_/ /_____  ____  __________\n");
  printf("  / __ \\/ __/ __/ __ \\/ __ \\/ ___/ ___/\n");
  printf(" / / / / /_/ /_/ /_/ / /_/ / /  (__  ) \n");
  printf("/_/ /_/\\__/\\__/ .___/\\____/_/  /____/  \n");
  printf("             /_/                        \n");
  printf("\nAuthor: William Steven (Anon404)\n\n");
}

int ck_std(){
  if (isatty(fileno(stdin))) return 1;
  return 0;
}

void usage(char *arg){
  printf("%s --help to read user manual\n",arg);
  exit(EXIT_FAILURE);
}

void handler(){
  if (fd == NULL){
    while (fgets(BUFFER,sizeof(BUFFER),stdin)) {
      BUFFER[strcspn(BUFFER,"\n")] = 0;
      httpors(BUFFER);
    }
  } else {
    while (fgets(BUFFER,sizeof(BUFFER),fd)) {
     BUFFER[strcspn(BUFFER,"\n")] = 0;
      httpors(BUFFER);
    }
  }
}

void httpors(char *dom){
  CURL *curl;
  CURLcode result;
  total_size = 0;

  curl = curl_easy_init();

  if (curl == NULL) {
    fprintf(stderr, "Failed to make http request\n");
  }

  char url[1024];
  int n = snprintf(url, sizeof(url), "https://%s", dom);

  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_TIMEOUT,10L);
  curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10L);
  curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback);

  result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    //fprintf(stderr, "%s\n",curl_easy_strerror(result));
    curl_easy_cleanup(curl);
    return;
  }else {
    printf("%s   ",url);

    if (show_status){
      int status = 0;
      if (curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&status) == CURLE_OK) {
        printf("[%d]",status);
      }else {
        printf("[--]",status);
      }
    }

    if (show_ip) {
      char *ip = NULL;
      if (curl_easy_getinfo(curl,CURLINFO_PRIMARY_IP,&ip) == CURLE_OK && ip) {
        printf(" [%s]", ip);
      }else{
        printf("\t");
      }
    }

    if (show_content_length) {
      size_t cont_len = 0;
      if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &cont_len) == CURLE_OK && cont_len > 0) {
        printf(" [%d]", total_size);
      } else {
        printf(" [0]");
      }
    }

    if (show_location) {
      char *loc = NULL;
      if (curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, loc) == CURLE_OK && loc) {
        printf(" [location: %s]", loc);
      } else {
        printf("");
      }
    }
  }

  printf("\n");
  curl_easy_cleanup(curl);

}      if (!fd) {
        printf("Error to open file\n");
        return 1;
      }
      i++;

    } else if (strcmp(argv[i],"-d") == 0 || strcmp(argv[i],"--domain") == 0) {
      printf("domain %s\n",argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"--status-code") == 0) show_status = 1;
    else if (strcmp(argv[i],"--location") == 0) show_location = 1;
    else if (strcmp(argv[i],"--content-length") == 0) show_content_length = 1;
    else if (strcmp(argv[i],"--ip") == 0) show_ip = 1;
    else {
      printf("Unknown option %s\n",argv[i]);
      return 1;
    }
  }

  handler();

  return 0;
}

void banner() {
  printf("\n");
  printf("    __    __  __                       \n");
  printf("   / /_  / /_/ /_____  ____  __________\n");
  printf("  / __ \\/ __/ __/ __ \\/ __ \\/ ___/ ___/\n");
  printf(" / / / / /_/ /_/ /_/ / /_/ / /  (__  ) \n");
  printf("/_/ /_/\\__/\\__/ .___/\\____/_/  /____/  \n");
  printf("             /_/                        \n");
  printf("\nAuthor: William Steven (Anon404)\n\n");
}

int ck_std(){
  if (isatty(fileno(stdin))) return 1;
  return 0;
}

void usage(char *arg){
  printf("%s --help to read user manual\n",arg);
  exit(EXIT_FAILURE);
}

void handler(){
  if (fd == NULL){
    while (fgets(BUFFER,sizeof(BUFFER),stdin)) {
      BUFFER[strcspn(BUFFER,"\n")] = 0;
      httpors(BUFFER);
    }
  } else {
    while (fgets(BUFFER,sizeof(BUFFER),fd)) {
     BUFFER[strcspn(BUFFER,"\n")] = 0;
      httpors(BUFFER);
    }
  }
}

void httpors(char *dom){
  CURL *curl;
  CURLcode result;
  total_size = 0;

  curl = curl_easy_init();

  if (curl == NULL) {
    fprintf(stderr, "Failed to make http request\n");
  }

  char url[1024];
  int n = snprintf(url, sizeof(url), "https://%s", dom);

  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_TIMEOUT,10L);
  curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10L);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback);

  result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    //fprintf(stderr, "%s\n",curl_easy_strerror(result));
    curl_easy_cleanup(curl);
    return;
  }else {
    printf("%s\t",url);

    if (show_status){
      int status = 0;
      if (curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&status) == CURLE_OK) {
        printf("[%d]",status);
      }else {
        printf("[--]",status);
      }
    }

    if (show_ip) {
      char *ip = NULL;
      if (curl_easy_getinfo(curl,CURLINFO_PRIMARY_IP,&ip) == CURLE_OK && ip) {
        printf(" [%s]", ip);
      }else{
        printf("\t");
      }
    }

    if (show_content_length) {
      size_t cont_len = 0;
      if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &cont_len) == CURLE_OK && cont_len > 0) {
        printf(" [%d]", total_size);
      } else {
        printf(" [--]");
      }
    }

    if (show_location) {
      char *loc = NULL;
      if (curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, loc) == CURLE_OK && loc) {
        printf(" [location: %s]", loc);
      } else {
        printf("");
      }
    }
  }

  printf("\n");
  curl_easy_cleanup(curl);

}
