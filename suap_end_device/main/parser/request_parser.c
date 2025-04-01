static int extract_relevant_data(char *json){

    const cJSON *main_request = NULL;
    const cJSON *request_body = NULL;
    const cJSON *pom = NULL;

    cJSON *json_data = cJSON_Parse(json);
    if(json_data == NULL){
        printf("JSON data IS NULL\n");
        return -1;
    }
    
        zadnja = cJSON_GetObjectItemCaseSensitive(json_data, ZADNJA_VRIJEDONST);
        if (!cJSON_IsNumber(zadnja)){
            return -5;
        }
        //provjera dali su poslani podaci za dobar uređaj
        br = cJSON_GetObjectItemCaseSensitive(json_data, BROJAC);
        if (!cJSON_IsNumber(br)){
            return -5;
        }
        pr = cJSON_GetObjectItemCaseSensitive(json_data, PROSJEK);
        if (!cJSON_IsNumber(pr)){
            return -5;
        }

        zadnja_vrijednost = zadnja->valueint;
        brojac = br->valueint;
        prosjek = pr->valuedouble;
        printf("%d %d %f\n", zadnja_vrijednost, brojac, prosjek);
    

    cJSON_Delete(json_data);
    return 0;
}
