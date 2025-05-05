#include "tasks.h"



int getMessage(char *message){

    ESP_LOGI(predef_tasks, "Poruka...");
  //  printf("abc");
    char  odgovor[100], URL[300] = "";
  //  printf("aaa");
    strcat(URL, BASE_URL"/Datagram/PopMessage");
   // printf("bbb");
    strcat(URL, "/");
 //   printf("ccc");
    char pom[5] = ""; sprintf(pom, "%d", my_ID);
    strcat(URL, pom);
    printf("%s\n", URL);

    get_rest_function(message, odgovor, URL, NULL);

    return 0;
}

int getMessageCount(int *message_count){
    ESP_LOGI(predef_tasks, "Broj_poruka...");
    *message_count = -1;
  //  printf("abc");
    char podaci[10], odgovor[100], URL[300] = "";
  //  printf("aaa");
    strcat(URL, BASE_URL"/Datagram/MessageCount");
   // printf("bbb");
    strcat(URL, "/");
 //   printf("ccc");
    char pom[5] = ""; sprintf(pom, "%d", my_ID);
    strcat(URL, pom);
    printf("%s\n", URL);
    get_rest_function(podaci, odgovor, URL, NULL);
    printf("eee%s", podaci);
    *message_count = atoi(podaci);

    return 0;
}

void get_messages(){
    while(1){
        ESP_LOGI(predef_tasks, "Povlacim...");
        int pom = -2;
        char message[MAX_HTTP_RECV_BUFFER];
        ESP_LOGI(predef_tasks, "Palim...");
        getMessageCount(&pom);
        ESP_LOGI(predef_tasks, "Imam %d poruka.", pom);
        for(int i = 0; i < pom; i++){
            getMessage(message);
            int status = parse_request(message);
            ESP_LOGI(predef_tasks, "Poruka obradjena sa statusom %d", status);
        }


        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}