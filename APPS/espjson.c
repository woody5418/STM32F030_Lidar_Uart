//********************************************************************
//
//      代码功能：ESP8266 JSON解析代码
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"


uint8_t   FLAG_CJSON_SEND = 0;     // 定时器发送cJson控制变量
uint8_t   mode_status_value=0;     //模式的值


char json_get_temp[200] = "";  //二级Json存储 数组
char json_temp[70] = "";       //三级Json存储 数组



/**
 * 接收到串口1的ESPP8266数据
 * 判断是否为JSON，并且执行相应的功能
 */
void Apps_Analysis_Data(void) {
    if (USART1_RX_STA & 0x8000) {
        if(cJson_Pars_data() == 0x01) { //数据解析  控制
            json_get_ItemObjectFirst((char *)USART1_RX_BUF,"welcome",json_get_temp);
            if (json_check(json_get_temp)  == 1) { //合法 为0  不合法为1

            }
            memset(USART1_RX_BUF,0,300);
        }
        USART1_RX_STA=0;
    }
}

/**
 * 接收到串口1的ESPP8266数据
 * 判断是否为JSON，并且执行相应的功能
 * {"id":"SWTZ_Cantilever_a0:20:a6:29:a7:dc","to":"user","services":{"strength1":"0.01","strength2":"0.02"}}
*/

char cJson_Pars_data(void) {
    memset(json_get_temp,0,200);//清空数组 写0 大小1024Byte
    memset(json_temp,0,70);//清空数组 写0 大小1024Byte
    //获取子json
    json_get_ItemObject((char *)USART1_RX_BUF,"services",(char *)json_get_temp); //获取子json

    if (json_check(json_get_temp)  == 1) { //合法 为1  不合法为0
        //控制完成以后，直接发送数据到服务器
        //send_data_user();  //发送json到服务器
        return 0;
    } else {
        if(checkJSON("Connection: close")) {
            TIM_Cmd(TIM2, DISABLE);//数据任务关
            Control_Flag_Mode=0;
            Dis_Number = 10;  //进入Airkiss模式
        }
        //灯效果+
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
 * 获取雷达的数据
 * 返回值为-1表示接受错误或者
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
 * 发送数据到服务器
 * 发送的{"id":"SWTZ_LidarAirplane_a0:20:a6:29:a7:dc","to":"user","services":{"switch":"1","data":"125","time":"4096"}}
 * 接收到的
 */
void send_data_user(int v,int t,bool nff) {
    char PRemove[64];
    uint8_t packingData[300];        //最大800字节

    strcpy((char *) packingData, "{\"id\":\"SWTZ_LidarAirplane_");
    strcat((char *) packingData, macbufs);
    strcat((char *) packingData, "\",\"to\":\"user\",");
    strcat((char *) packingData, "\"debug\":{\"variable\":\"");
    sprintf((char *)PRemove,"%d,%d",NetNumber,Lidar_frequency);
    strcat((char *) packingData, PRemove);
    strcat((char *) packingData, "\"},");
    strcat((char *) packingData, "\"services\":{\"switch\":\"");
    sprintf((char *)PRemove,"%d",nff);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\",\"data\":\"");
    sprintf((char *)PRemove,"%d",v);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\",\"time\":\"");
    sprintf((char *)PRemove,"%d",t);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\"}}");
    sendMessagePacket((char *)packingData);
}


////////////********************** JSON  数据处理 ***************************////////////////

/***************************************
 name:       JSON 校验函数
 input:      字符串
 output:     合法JAON 返回1 不合法JSON 返回0
 description:
 ***************************************/
char json_check(char *str) {
    uint16_t str_length;//定义字符长度变量
    str_length = strlen(str);//计算字符长度
    if (str[0] == '{' && str[str_length - 1] == '}') { //通过首尾大括号判断是否为JSON
        return 1;//如果字符串为合法JSON 返回1
    } else {
        return 0;//如果字符串为合法JSON 返回0
    }
}

/***************************************
 name:       JSON 获取键值函数
 input:      JSON字符串 要获取的键名 获取键值的字符串
 output:     如果获取成功返回1 获取失败返回0
 description:
 ***************************************/
char json_get_value(char *json, char *json_key, char *json_value) {
    char *json_key_start;//定义键名开始的位置
    char *json_key_end;//定义键名结束的位置
    char json_key_length;//定义键名长度
    char *json_value_start;//定义键值开始的位置
    char *json_value_end;//定义键值结束的位置
    char json_value_length;//定义键值长度

    json_key_start = strstr(json, json_key);//获取键名开始的位置
    json_key_length = strlen(json_key);//获取键名的长度
    json_key_end = json_key_start + json_key_length;//获取键名结束的位置
    if (json_key_start != 0 && *(json_key_start - 1) == '\"'
            && *(json_key_end) == '\"' && *(json_key_end + 1) == ':'
            && *(json_key_end + 2) == '\"') {
        json_value_start = json_key_end + 3;//获取键值开始的位置
        json_value_end = strstr(json_value_start, "\""); //获取键值结束的位置
        json_value_length = json_value_end - json_value_start;//获取键值的长度
        strncpy(json_value, json_value_start, json_value_length);//将键值存入指针数组
        json_value[json_value_length] = '\0';//指针最后一位补字符串结束符 \0
        return 1;//成功获取键值 返回1
    } else {
        json_value[0] = '\0';
        return 0;//失败获取键值 返回0
    }
}

/***************************************
 name:      JSON 提取子JSON键值的值（一级提取）
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
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
 name:      JSON 提取子JSON键值的值
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
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
 name:      JSON 提取子JSON(二级提取)
 input:     JSON字符串 要获取的键名 获取子json的串
 output:    无
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
 name:       JSON 键值对比函数
 input:      JSON 键值 要匹配的字符
 output:     如果匹配成功返回1 失败返回0
 description:
 ***************************************/
char json_check_value(char *str1, char *str2) {
    if (strcmp(str1, str2) == 0) {
        return 1;//匹配成功返回1
    } else {
        return 0;//匹配成功返回0
    }
}


/***************************************
 name:       string类型转int类型
 input:      存储string的参数
 output:     如果转换成功返回数值 失败返回0
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

