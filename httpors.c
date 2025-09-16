#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <pthread.h>

#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BGRAY "\e[1;90m"
#define BWHT "\e[1;37m"

#define reset "\e[0m"

FILE *fd = NULL;
FILE *out = NULL;

char BUFFER[128];
size_t total_size = 0;

int show_status = 0;
int show_location = 0;
int show_content_length = 0;
int show_content_type = 0;
int show_ip = 0;
int output = 0;

int ARGC;
char **ARGV;

typedef struct{
  char domain[256];
} thread_arg;

size_t write_callback(void *ptr,size_t size,size_t nmemb,void *usrdata){
  total_size += size * nmemb;
  return size * nmemb;
}

int ck_std();
void banner();
void usage();
void help();
void handler();
void httpors();
void * thread();

int main(int argc, char *argv[]){

  ARGC = argc;
  ARGV = argv;

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

    } else if (strcmp(argv[i],"--output") == 0 || strcmp(argv[i],"-o") == 0) {
      out = fopen(argv[i+1],"w");
      output = 1;
      i++;
    }
    else if (strcmp(argv[i],"--status-code") == 0 || strcmp(argv[i],"-sc") == 0) show_status = 1;
    else if (strcmp(argv[i],"--location") == 0) show_location = 1;
    else if (strcmp(argv[i],"--content-length") == 0 || strcmp(argv[i],"-cl") == 0) show_content_length = 1;
    else if (strcmp(argv[i],"--content-type") == 0 || strcmp(argv[i],"-ct") == 0) show_content_type = 1;
    else if (strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"-h") == 0) {
      help();
      return 0;
    }
    else if (strcmp(argv[i],"-ip") == 0) show_ip = 1;
    else {
      printf("Unknown option %s\n",argv[i]);
      return 1;
    }
  }

  handler();

  if (output && out){
    fclose(out);
  }
  if (fd) {
    fclose(fd);
  }

  return 0;
}

void banner() {
  printf(BGRN"\n");
  printf("    __    __  __                       \n");
  printf("   / /_  / /_/ /_____  ____  __________\n");
  printf("  / __ \\/ __/ __/ __ \\/ __ \\/ ___/ ___/\n");
  printf(" / / / / /_/ /_/ /_/ / /_/ / /  (__  ) \n");
  printf("/_/ /_/\\__/\\__/ .___/\\____/_/  /____/  \n");
  printf("             /_/                        \n");
  printf(BBLU"\nAuthor: William Steven (Anon404)\n\n"reset);
}

int ck_std(){
  if (isatty(fileno(stdin))) return 1;
  return 0;
}

void help() {
    printf("\n");

    // Title
    printf(BWHT "Usage: httpors [options]\n\n");

    // Options header
    printf(BWHT "Options:\n");

    // Flag + Description
    printf(BGRN "  -f, --file <file>");
    printf(BWHT "           Read domains from file\n");

    printf(BGRN "  -sc, --status-code");
    printf(BWHT "          Show HTTP status code\n");

    printf(BGRN "  -ip");
    printf(BWHT "                         Show resolved IP address\n");

    printf(BGRN "  -cl, --content-length");
    printf(BWHT "       Show response content length\n");

    printf(BGRN "  -ct, --content-type");
    printf(BWHT "         Show response content type\n");

    printf(BGRN "  --location");
    printf(BWHT "                  Show redirect location (if any)\n");

    printf(BGRN "  -h, --help");
    printf(BWHT "                  Show this help menu\n");

    printf(BGRN "  -o, --output <out file>");
    printf(BWHT "     Saving output on txt file\n\n");

    // Examples
    printf(BWHT "Examples:\n");
    printf(BYEL "  ./httpors -f subdomains.txt --sc --ip --cl\n");
    printf(BYEL "  subfinder -d example.com | ./httpors --sc --ip\n\n");

    // Author
    printf(BWHT "Author: " BCYN "Anon404\n\n");
}

void usage(char *arg){
  printf("%s --help to read user manual\n",arg);
  exit(EXIT_FAILURE);
}

void handler(){

  pthread_t tids[3000];
  int tcount = 0;

  if (fd == NULL){
    while (fgets(BUFFER,sizeof(BUFFER),stdin)) {
      BUFFER[strcspn(BUFFER,"\n")] = 0;

      thread_arg *targ = malloc(sizeof(thread_arg));
      strcpy(targ->domain, BUFFER);

      pthread_create(&tids[tcount], NULL, thread,targ);                                                                                                       tcount++;

    }
  } else {
    while (fgets(BUFFER,sizeof(BUFFER),fd)) {
     BUFFER[strcspn(BUFFER,"\n")] = 0;

      thread_arg *targ = malloc(sizeof(thread_arg));
      strcpy(targ->domain, BUFFER);

      pthread_create(&tids[tcount], NULL, thread,targ);
      tcount++;

    }
  }

  for (int i = 0; i < tcount; i++) {
    pthread_join(tids[i], NULL);
  }

}


void * thread(void * arg){
  thread_arg *targ = (thread_arg *) arg;
  httpors(targ->domain);
  free(targ);
  return NULL;
}

void httpors(char *dom){
  CURL *curl;
  CURLcode result;
  total_size = 0;

  int status;
  char *ip;
  char *cont_type;
  curl_off_t  cont_len;
  char *loc;



  curl = curl_easy_init();

  if (curl == NULL) {
    fprintf(stderr, "Failed to make http request\n");
  }

  char url[3024];
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
    //printf("%s   ",url);

    if (show_status){
      status = 0;
      if (curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&status) == CURLE_OK) {
        //printf("[%d]",status);
      }else {
        //printf("[--]",status);
      }
    }

    if (show_ip) {
      ip = NULL;
      if (curl_easy_getinfo(curl,CURLINFO_PRIMARY_IP,&ip) == CURLE_OK && ip) {
        //printf(" [%s]", ip);
      }else{
        //printf("\t");
      }
    }

    if (show_content_type) {
      cont_type = NULL;
      if (curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cont_type) == CURLE_OK && cont_type > 0) {
        //printf(" [%s]",cont_type);
      } else {
        //printf(" [?/?]");
      }
    }

    if (show_content_length) {
      cont_len = 0;
      if (curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &cont_len) == CURLE_OK && cont_len > 0) {
        //printf(" [%zu]", total_size);
      } else {
        //printf(" [0]");
      }
    }

    if (show_location) {
      loc = NULL;
      if (curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &loc) == CURLE_OK && loc) {
        //printf(" [location: %s]", loc);
      } else {
        //printf("");
      }
    }
  }


  printf(BWHT"%s  ",url);
  if (output && out) fprintf(out,"%s  ",url);
  for (int i = 1 ;i < ARGC; i++) {
    if (strcmp(ARGV[i],"--status-code") == 0 || strcmp(ARGV[i],"-sc") == 0){
      if (status != 0) {
        if (status >= 200 && status < 300) {
          printf(BGRN" [%d]", status);
          if (output && out) fprintf(out," [%d]",status);

        }else if (status >= 300 && status < 400) {
          printf(BYEL" [%d]", status);
          if (output && out) fprintf(out," [%d]",status);

        }else if (status >= 400) {
          printf(BRED" [%d]", status);
          if (output && out) fprintf(out," [%d]",status);
        }

      } else {
        printf(BGRAY" [???]",status);
        if (output && out) fprintf(out," [???]");
      }
    }

    if (strcmp(ARGV[i],"-ip") == 0) {
      if (ip != 0) {
        printf(BCYN" [%s]",ip);
        if (output && out) fprintf(out," [%s]",ip);
      } else {
        printf("\t");
        if (output && out) fprintf(out,"\t");
      }
    }

    if (strcmp(ARGV[i],"--content-type") == 0 || strcmp(ARGV[i],"-ct") == 0) {
      if (cont_type != NULL) {
        printf(BBLU" [%s]",cont_type);
        if (output && out) fprintf(out," [%s]",cont_type);
      } else {
        printf(BGRAY" [?/?]");
        if (output && out) fprintf(out," [?/?]");
      }
    }

    if (strcmp(ARGV[i],"--content-length") == 0 || strcmp(ARGV[i],"-cl") == 0){
      if (cont_len != 0) {
        printf(BMAG" [%zu]", total_size);
        if (output && out) fprintf(out," [%zu]",total_size);
      } else {
        printf(BGRAY" [0]");
        if (output && out) fprintf(out," [0]");
      }
    }

    if (strcmp(ARGV[i],"--location") == 0) {
      if (loc != NULL) {
        printf(BBLU" [loc: %s]", loc);
        if (output && out) fprintf(out," [loc: %s]",loc);
      } else {
        printf("");
      }
    }

  }

  printf("\n");
  if (output && out) fprintf(out,"\n");
  curl_easy_cleanup(curl);

}
