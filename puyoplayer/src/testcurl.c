#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(int argc, char* argv[]) {
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunkMem;
  chunkMem.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunkMem.size = 0;    /* no data at this point */
  //char *fp = malloc(sizeof(char) * CURL_MAX_WRITE_SIZE);
  curl = curl_easy_init();
  if (curl) {
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "Accept: text/plain");

    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_URL, "https://puyopuyo.vermeille.fr/turn?name=bab&move=DOWN");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunkMem);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    printf ("%s\n", chunkMem.memory);
    free(chunkMem.memory);
  }

  return 0;
}