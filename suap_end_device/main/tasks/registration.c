#include "tasks.h"



esp_netif_ip_info_t ip_info;
unsigned char mac[6] = {0};
char odgovor[100];
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



    post_rest_function(*odgovor, API_POST_URL, query);


    return 0;
}