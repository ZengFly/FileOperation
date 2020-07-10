#ifndef ITC_INI_H
#define ITC_INI_H

/***********************************INCLUDES************************************/

/*******************************************************************************/


/*******************************GLOBAL VARIABLES********************************/

/*******************************************************************************/


/*****************************FUNCTION DECLARATION******************************/
/**
 * @brief           读取配置
 * @param title     标题
 * @param key       键
 * @param buf       接收值缓冲区
 * @return  0-success other-failed
 * */
int ReadIniKeyString(char *title, char *key, char *buf, char *filename);

/**
 * @brief           写入(修改)配置
 * @param title     标题
 * @param key       键
 * @param buf       值
 * @return  0-success other-failed
 * */
int WriteIniKeyString(char *title, char *key, char *val, char *filename);
/*******************************************************************************/
#endif //ITC_INI_H
