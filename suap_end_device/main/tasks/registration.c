#include "tasks.h"



esp_netif_ip_info_t ip_info;
unsigned char mac[6] = {0};
char podaci[100], odgovor[100];
char mac_addr[30], IP_addr[20];
char query[200];

int register_end_device(){
    esp_netif_get_ip_info(IP_EVENT_STA_GOT_IP,&ip_info);

    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    sprintf(IP_addr,IPSTR, IP2STR(&ip_info.ip));
    strcat(query, "{\"ID\": null,\"IP\": ");
    strcat(query, IP_addr);
    strcat(query, ",\"MAC\": ");
    strcat(query, mac_addr);
    strcat(query, ", \"ďevices\":");
    strcat(query, PERIPHERAL_DEVICES); //priprema za dinamičko generiranje
    strcat(query, "}");

    post_rest_function(podaci, odgovor, BASE_URL"/Registration", query);

    const cJSON *main_request = NULL;
    cJSON *pom = NULL;

    main_request = cJSON_Parse(podaci);
    if(main_request == NULL){
        ESP_LOGE(predef_tasks, "JSON je NULL");
        return -1;
    }
    
        pom = cJSON_GetObjectItemCaseSensitive(main_request, "ID");
        if (!cJSON_IsNumber(pom)){
           ESP_LOGE(predef_tasks, "ID nije int");
           return -2;
        }
        my_ID = pom->valueint;

    //napraviti task za povlačenje podataka o susjedima...a


    return 0;
}

int get_neigbours(){
    get_rest_function(podaci, odgovor, API_POST_URL, NULL);
    char podaci[100], odgovor[100];
    char mac_addr[30], IP_addr[20];
    char query[200];


    return 0;
}