/***********************************INCLUDES************************************/
#include <stdio.h>
#include <string.h>
#include "ini.h"
/*******************************************************************************/


/*******************************GLOBAL VARIABLES********************************/
/*******************************************************************************/


/*****************************FUNCTION DECLARATION******************************/
/*******************************************************************************/


/*****************************FUNCTION DEFINITION*******************************/
/**
 * @brief           读取配置
 * @param title     标题
 * @param key       键
 * @param buf       接收值缓冲区
 * @return  0-success other-failed
 * */
int ReadIniKeyString(char *title, char *key, char *buf, char *filename)
{
    FILE *fp;
    int  flag = 0;
    char sTitle[64], *wTmp;
    char sLine[1024];

    memset(sTitle, 0, sizeof(sTitle));
    sprintf(sTitle, "[%s]", title);

    if(NULL == (fp = fopen(filename, "r"))) {
        perror("fopen");
        return -1;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // 这是注释行
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while(*(wTmp + 1) == ' '){
                    wTmp++;
                }
                strcpy(buf,wTmp + 1);
                return 0;
            }
        } else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                flag = 1; // 找到标题位置
            }
        }
    }

    fclose(fp);
    return -1;
}

/**
 * @brief           写入(修改)配置
 * @param title     标题
 * @param key       键
 * @param buf       值
 * @return  0-success other-failed
 * */
int WriteIniKeyString(char *title, char *key, char *val, char *filename)
{
    FILE *fpw = NULL,*fpr = NULL;
    int flag = 0, wFlag = 0;
    char sLine[1024], sTitle[32], wTmp[1024];

    /*尝试打开读文件*/
    fpr = fopen(filename, "r");
    if(NULL == fpr) {
        /*文件不存在，创建文件*/
        if (NULL == (fpw = fopen(filename, "wt+"))) {
            return -1;
        }
        /*写入配置*/
        memset(sTitle, 0, sizeof(sTitle));
        snprintf(sTitle, sizeof(sTitle), "[%s]\n", title);
        fputs(sTitle, fpw);
        /*写入键值*/
        memset(sLine, 0, sizeof(sLine));
        snprintf(sLine, sizeof(sLine), "%s=%s\n", key, val);
        fputs(sLine, fpw);
        /*退出写入操作*/
        fclose(fpw);

        return 0;
    }
    else
    {
        /*文件存在*/
        memset(sLine, 0, sizeof(sLine));
        snprintf(sLine, sizeof(sLine), "%s.tmp", filename);
        /*创建临时文件*/
        if (NULL == (fpw = fopen(sLine, "wt+"))) {
            return -1;
        }
        /*格式化title和键值*/
        memset(sTitle, 0, sizeof(sTitle));
        snprintf(sTitle, sizeof(sTitle), "[%s]\n", title);
        memset(wTmp, 0, sizeof(wTmp));
        snprintf(wTmp, sizeof(wTmp), "%s=%s\n", key, val);

        while (!ferror(fpr) && (NULL != fgets(sLine, sizeof(sLine), fpr))) {
            /*根据标志位判断是否要修改数据*/
            if (1 == flag) {
                /*匹配键*/
                if (0 == memcmp(sLine, key, strlen(key))) {
                    /*更新数据*/
                    memset(sLine, 0, sizeof(sLine));
                    snprintf(sLine, sizeof(sLine), "%s=%s\n", key, val);
                    /*修改标志位*/
                    flag = 0;
                    /*修改已写入标志*/
                    wFlag = 1;
                }
                /*比较第一个字符判断是否读取到下一个title*/
                if ((0 == memcmp(sLine, sTitle, 1))) {
                    /*是title标志且写入标志未修改，说明内容不存在，增加新内容*/
                    fputs(wTmp, fpw);
                    /*修改标志位*/
                    flag = 0;
                    /*修改已写入标志*/
                    wFlag = 1;
                }
            }

            /*判断是否为Title*/
            if (0 == memcmp(sLine, sTitle, strlen(sTitle))) {
                /*匹配到title，修改写入标志*/
                flag = 1;
            }

            /*写入临时文件*/
            fputs(sLine, fpw);
        }

        /*判断是否写入内容，此部分解决只存在一个title，无法匹配到title标志的情况*/
        if (wFlag != 1 ) {
            if (flag != 0) {
                /*不写入Title*/
                fputs(wTmp, fpw);
            } else {
                /*写入Title*/
                fputs(sTitle, fpw);
                fputs(wTmp, fpw);
            }
        }

        /*关闭文件操作指针*/
        fclose(fpr);
        fclose(fpw);

        /*覆盖原文件*/
        memset(sLine, 0, sizeof(sLine));
        snprintf(sLine, sizeof(sLine), "%s.tmp", filename);
        return rename(sLine, filename);
    }
}
/*******************************************************************************/
