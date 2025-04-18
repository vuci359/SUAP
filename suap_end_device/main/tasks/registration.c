#include "tasks.h"



esp_netif_ip_info_t ip_info;
unsigned char mac_base[6] = {0};
char odgovor[50];
char IP_addr[12];
char query[200];

int register_end_device(){
    esp_netif_get_ip_info(IP_EVENT_STA_GOT_IP,&ip_info);

    esp_read_mac(mac_base, ESP_MAC_WIFI_STA);
    sprintf(IP_addr,IPSTR, IP2STR(&ip_info.ip));
    strcat(query, "{\"ID\": null,\"IP\": ");
    strcat(query, IP_addr);
    strcat(query, ",\"MAC\": ");
    strcat(query, mac_base);
    strcat(query, ", \"ďevices\":");
    strcat(query, PERIPHERAL_DEVICES); //priprema za dinamičko generiranje
    strcat(query, "}");



    post_rest_function(*odgovor, API_POST_URL,query);


    return 0;
}