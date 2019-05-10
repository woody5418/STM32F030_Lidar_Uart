//********************************************************************
//
//      ���빦�ܣ�ESP8266 JSON��������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"


uint8_t   FLAG_CJSON_SEND = 0;     // ��ʱ������cJson���Ʊ���
uint8_t   mode_status_value=0;     //ģʽ��ֵ


char json_get_temp[200] = "";  //����Json�洢 ����
char json_temp[70] = "";       //����Json�洢 ����



/**
 * ���յ�����1��ESPP8266����
 * �ж��Ƿ�ΪJSON������ִ����Ӧ�Ĺ���
 */
void Apps_Analysis_Data(void) {
    if (USART1_RX_STA & 0x8000) {
        if(cJson_Pars_data() == 0x01) { //���ݽ���  ����
            json_get_ItemObjectFirst((char *)USART1_RX_BUF,"welcome",json_get_temp);
            if (json_check(json_get_temp)  == 1) { //�Ϸ� Ϊ0  ���Ϸ�Ϊ1

            }
            memset(USART1_RX_BUF,0,300);
        }
        USART1_RX_STA=0;
    }
}

/**
 * ���յ�����1��ESPP8266����
 * �ж��Ƿ�ΪJSON������ִ����Ӧ�Ĺ���
 * {"id":"SWTZ_Cantilever_a0:20:a6:29:a7:dc","to":"user","services":{"strength1":"0.01","strength2":"0.02"}}
*/

char cJson_Pars_data(void) {
    memset(json_get_temp,0,200);//������� д0 ��С1024Byte
    memset(json_temp,0,70);//������� д0 ��С1024Byte
    //��ȡ��json
    json_get_ItemObject((char *)USART1_RX_BUF,"services",(char *)json_get_temp); //��ȡ��json

    if (json_check(json_get_temp)  == 1) { //�Ϸ� Ϊ1  ���Ϸ�Ϊ0
        //��������Ժ�ֱ�ӷ������ݵ�������
        //send_data_user();  //����json��������
        return 0;
    } else {
        if(checkJSON("Connection: close")) {
            TIM_Cmd(TIM2, DISABLE);//���������
            Control_Flag_Mode=0;
            Dis_Number = 10;  //����Airkissģʽ
        }
        //��Ч��+
        /*
        LED_Normal_H;
        delay_ms(40);
        LED_Normal_L;
        delay_ms(40);
        LED_Normal_H;
        delay_ms(40);
        LED_Normal_L;
        delay_ms(40);
        */
        return 1;
    }
}


/**
 * ��ȡ�״������
 * ����ֵΪ-1��ʾ���ܴ������
 */
int Get_Lidar_value(uint8_t *buf) {
    int temp=0;
    if(USART2_RX_STA&0x80) {
        temp = DecodeLaseData3Byte(buf);
        USART2_RX_STA=0;
        return temp;
    } else return -1;
}


/**
 * �������ݵ�������
 * ���͵�{"id":"SWTZ_LidarAirplane_a0:20:a6:29:a7:dc","to":"user","services":{"switch":"1","data":"125","time":"4096"}}
 * ���յ���
 */
void send_data_user(int v,int t,bool nff) {
    char PRemove[64];
    uint8_t packingData[300];        //���800�ֽ�

    strcpy((char *) packingData, "{\"id\":\"SWTZ_LidarAirplane_");
    strcat((char *) packingData, macbufs);
    strcat((char *) packingData, "\",\"to\":\"user\",");
    strcat((char *) packingData, "\"debug\":{\"variable\":\"");
    sprintf((char *)PRemove,"%d,%d",NetNumber,Lidar_frequency);
    strcat((char *) packingData, PRemove);
    strcat((char *) packingData, "\"},");
    strcat((char *) packingData, "\"services\":{\"switch\":\"");
    sprintf((char *)PRemove,"%d",nff);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\",\"data\":\"");
    sprintf((char *)PRemove,"%d",v);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\",\"time\":\"");
    sprintf((char *)PRemove,"%d",t);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\"}}");
    sendMessagePacket((char *)packingData);
}


////////////********************** JSON  ���ݴ��� ***************************////////////////

/***************************************
 name:       JSON У�麯��
 input:      �ַ���
 output:     �Ϸ�JAON ����1 ���Ϸ�JSON ����0
 description:
 ***************************************/
char json_check(char *str) {
    uint16_t str_length;//�����ַ����ȱ���
    str_length = strlen(str);//�����ַ�����
    if (str[0] == '{' && str[str_length - 1] == '}') { //ͨ����β�������ж��Ƿ�ΪJSON
        return 1;//����ַ���Ϊ�Ϸ�JSON ����1
    } else {
        return 0;//����ַ���Ϊ�Ϸ�JSON ����0
    }
}

/***************************************
 name:       JSON ��ȡ��ֵ����
 input:      JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ���ַ���
 output:     �����ȡ�ɹ�����1 ��ȡʧ�ܷ���0
 description:
 ***************************************/
char json_get_value(char *json, char *json_key, char *json_value) {
    char *json_key_start;//���������ʼ��λ��
    char *json_key_end;//�������������λ��
    char json_key_length;//�����������
    char *json_value_start;//�����ֵ��ʼ��λ��
    char *json_value_end;//�����ֵ������λ��
    char json_value_length;//�����ֵ����

    json_key_start = strstr(json, json_key);//��ȡ������ʼ��λ��
    json_key_length = strlen(json_key);//��ȡ�����ĳ���
    json_key_end = json_key_start + json_key_length;//��ȡ����������λ��
    if (json_key_start != 0 && *(json_key_start - 1) == '\"'
            && *(json_key_end) == '\"' && *(json_key_end + 1) == ':'
            && *(json_key_end + 2) == '\"') {
        json_value_start = json_key_end + 3;//��ȡ��ֵ��ʼ��λ��
        json_value_end = strstr(json_value_start, "\""); //��ȡ��ֵ������λ��
        json_value_length = json_value_end - json_value_start;//��ȡ��ֵ�ĳ���
        strncpy(json_value, json_value_start, json_value_length);//����ֵ����ָ������
        json_value[json_value_length] = '\0';//ָ�����һλ���ַ��������� \0
        return 1;//�ɹ���ȡ��ֵ ����1
    } else {
        json_value[0] = '\0';
        return 0;//ʧ�ܻ�ȡ��ֵ ����0
    }
}

/***************************************
 name:      JSON ��ȡ��JSON��ֵ��ֵ��һ����ȡ��
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ��ֵ
 output:    ��
 description:
 ***************************************/
void json_get_ItemObject(char *json, char *json_key, char *json_value) {
    int  json_len;
    uint16_t jsoncls_0=0;
    char *json_get_Item_obj;
    char *ptr_obj;

    while(json[jsoncls_0++] != '\0');
    json[jsoncls_0-1] = 0x30;
    json_get_Item_obj = strstr(json, json_key);
    json_get_Item_obj = strstr(json_get_Item_obj, "{");
    ptr_obj = strrchr(json_get_Item_obj, ',');
    json_len = strlen(json_get_Item_obj) - strlen(ptr_obj);
    strncpy(json_value, json_get_Item_obj, json_len);
    json_value[json_len] = '\0';
}

/***************************************
 name:      JSON ��ȡ��JSON��ֵ��ֵ
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ��ֵ
 output:    ��
 description:
 ***************************************/
void json_get_ItemObjectFirst(char *json, char *json_key, char *json_value) {
    int  json_len_Fir;
    char *json_get_Item_obj_Fir;
    char *ptr_obj_Fir;

    json_get_Item_obj_Fir = strstr(json, json_key);
    json_get_Item_obj_Fir = strstr(json_get_Item_obj_Fir, "{");
    ptr_obj_Fir = strrchr(json_get_Item_obj_Fir, '}');
    json_len_Fir = strlen(json_get_Item_obj_Fir) - strlen(ptr_obj_Fir);
    strncpy(json_value, json_get_Item_obj_Fir, json_len_Fir);
    json_value[json_len_Fir] = '\0';
}


/***************************************
 name:      JSON ��ȡ��JSON(������ȡ)
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��json�Ĵ�
 output:    ��
 description:
 ***************************************/
void json_get_obj(char *json, char *json_key,char *json_objectItem) {
    int json_len_t;
    char *json_get_temp_obj_t;
    char *ptr_t;

    json_get_temp_obj_t = strstr(json,json_key);
    json_get_temp_obj_t = strstr(json_get_temp_obj_t,"{");
    ptr_t = strchr(json_get_temp_obj_t,'}');
    json_len_t =strlen(json_get_temp_obj_t)-strlen(ptr_t)+1;
    strncpy(json_objectItem, json_get_temp_obj_t, json_len_t);
    json_objectItem[json_len_t] = '\0';
}


/***************************************
 name:       JSON ��ֵ�ԱȺ���
 input:      JSON ��ֵ Ҫƥ����ַ�
 output:     ���ƥ��ɹ�����1 ʧ�ܷ���0
 description:
 ***************************************/
char json_check_value(char *str1, char *str2) {
    if (strcmp(str1, str2) == 0) {
        return 1;//ƥ��ɹ�����1
    } else {
        return 0;//ƥ��ɹ�����0
    }
}


/***************************************
 name:       string����תint����
 input:      �洢string�Ĳ���
 output:     ���ת���ɹ�������ֵ ʧ�ܷ���0
 description:
 ***************************************/
uint32_t stringtoint(char *a) {
    uint32_t val=0;
    uint8_t i=0;
    while(1) {
        if( *(a+i) == '\0' )break;
        else {
            if(*(a+i)<='9'&&*(a+i)>='0')
                val=(val*10)+(*(a+(i++))&0x0F);
            else
                return 0;
        }
    }
    return val;
}

//
char* checkJSON(char *str) {
    char *strx=0;
    strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    return (char*)strx;
}

