#include "tasks.h"

const char *predef_tasks = "predefined tasks";


int my_ID;
int neighbour_count;
end_device neighbours[100];


esp_netif_ip_info_t ip_info;
esp_netif_t* netif=NULL;
unsigned char mac[6] = {0};
char podaci[500], odgovor[100];
char mac_addr[30], IP_addr[20];
char query[500];


int register_end_device(){
    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    esp_netif_get_ip_info(netif,&ip_info);

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    sprintf(IP_addr,IPSTR, IP2STR(&ip_info.ip));
    //printf(IPSTR, IP2STR(&ip_info.ip));
    strcat(query, "{\"ID\": -1, \"IP\": \"");
    strcat(query, IP_addr);
    strcat(query, "\", \"MAC\": \"");
    strcat(query, mac_addr);
    strcat(query, "\", \"devices\": ");
    strcat(query, PERIPHERAL_DEVICES); //priprema za dinamičko generiranje
    strcat(query, "}\0");
    printf("%s", query);
    ESP_LOGI(predef_tasks, "PPPP");


    int status = post_rest_function(podaci, odgovor, BASE_URL"/Registration", query);
    printf("podaci %s \n", podaci);

    ESP_LOGI(predef_tasks, "GRES");

    if(status == 0){

        const cJSON *main_request = NULL;
        cJSON *pom = NULL;

        main_request = cJSON_Parse(podaci);
        if(main_request == NULL){
            ESP_LOGE(predef_tasks, "pod JSON je NULL");
            return -1;
        }
        
            pom = cJSON_GetObjectItem(main_request, "ID");
            if (!cJSON_IsNumber(pom)){
            ESP_LOGE(predef_tasks, "ID nije int");
            return -2;
            }
            else{
                my_ID = pom->valueint;
            }
    }
    ESP_LOGI(predef_tasks, "CCCC");


    //napraviti task za povlačenje podataka o susjedima...a


    return 0;
}

int get_neighbours(){
    char podaci[2000], odgovor[100];

    get_rest_function(podaci, odgovor, BASE_URL"/Registration", NULL);
  //  char mac_addr[30], IP_addr[20];
    printf("%s", podaci);

    cJSON *elem, *subelem;
    cJSON *pom, *subpom;
    cJSON *root = cJSON_Parse(podaci);
    if(root == NULL){
        ESP_LOGE(predef_tasks, "root JSON je NULL");
        return -1;
    }
    int n = cJSON_GetArraySize(root);
    int p = 0;
    for (int i = 0; i < n; i++) {
        elem = cJSON_GetArrayItem(root, i);
        pom = cJSON_GetObjectItem(elem, "ID");
        if (!cJSON_IsNumber(pom)){
            ESP_LOGE(predef_tasks, "ID nije int");
            return -2;
        }
        if(pom->valueint == my_ID){
            continue;   //preskače sam sebe
        }
        neighbours[p].ID = pom->valueint;
        pom = cJSON_GetObjectItem(elem, "IP");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(predef_tasks, "IP nije string");
            return -3;
        }
        free(neighbours[p].IP); //brisanje starog
        neighbours[p].IP = malloc(strlen(pom->valuestring)+1); //rezerviranje za novo
        strcpy(neighbours[p].IP, pom->valuestring); //punjenje

        pom = cJSON_GetObjectItem(elem, "MAC");
        if (!cJSON_IsString(pom)){
            ESP_LOGE(predef_tasks, "MAC nije string");
            return -4;
        }
        free(neighbours[p].MAC); //brisanje starog
        neighbours[p].MAC = malloc(strlen(pom->valuestring)+1); //rezerviranje za novo
        strcpy(neighbours[p].MAC, pom->valuestring); //punjenje
        
        pom = cJSON_GetObjectItem(elem, "devices");
        int k = cJSON_GetArraySize(pom);
        
        free(neighbours[p].devices);
        neighbours[p].devices = malloc(k*sizeof(attached_device));

        for(int j = 0; j < k; j++){
            subelem = cJSON_GetArrayItem(pom, j);
            subpom = cJSON_GetObjectItem(subelem, "device_type");
            if (!cJSON_IsNumber(subpom)){
                ESP_LOGE(predef_tasks, "Tip uredjaja nije INT");
                return -5;
            }

            neighbours[p].devices[j].device_type = subpom->valueint;

            subpom = cJSON_GetObjectItem(subelem, "device_id");
            if (!cJSON_IsNumber(subpom)){
                ESP_LOGE(predef_tasks, "Id uredjaja nije INT");
                return -6;
            }

            neighbours[p].devices[j].device_id = subpom->valueint;
        }

        p++;
    }
    neighbour_count = n-1;

    return 0;
}

void find_neighbours(){
    while(1){
        int rez = get_neighbours();
        ESP_LOGI(predef_tasks, "Susjedi %d", rez);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}