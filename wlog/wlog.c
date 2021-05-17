/*
author: zhanghw
*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include "wlog.h"

#define version "wlog_v1.0.1_20210517_Beta"

#define WLOG_PATH     "/tmp/wlog/"
#define WLOG_NOLOG    "nolog"
#define WLOG_EXCEPT   "except"
#define WLOG_SHOWLOG  "showlog"
#define WLOG_NOFILE  "nofile"

#define LOGLINE_MAX (1024)
#define LOG_TAG_LEN (128)
#define LOG_FILTER_LEN   (1024)
#define LOG_MAX_LOG_SIZE (128*1024)

const char *levelStr[]={
"null",
"D",
"I",
"W",
"E",
"F",
};

typedef struct{
    FILE *fp;
    char filterPath[LOG_FILTER_LEN];
    char appName[256];
    char fname[256];
    U32 logstrLen;
    char *logstr;
    char *tag;
    int enbaleDate;
    int enbaleTimestamp;
    WlogLevel level;
    U64 maxLogSize;
    U64 currentLogSize;
}Wlog;

static Wlog* glbWlog=NULL;

static void *WlogRecalloc(void **addr,U32 len)
{
    void *newaddr=calloc(1,len);
    if(!newaddr){
        return NULL;
    }
    free(*addr);
    *addr=newaddr;
    return newaddr;
}

static void outputWlogConfig()
{    
    WLOGI("wlog","************log of %s************\n",glbWlog->appName);
    WLOGI("wlog","logstrLen=%u\n",glbWlog->logstrLen);
    WLOGI("wlog","enbaleDate=%d\n",glbWlog->enbaleDate);
    WLOGI("wlog","enbaleTimestamp=%d\n",glbWlog->enbaleTimestamp);
    WLOGI("wlog","level=%s\n",levelStr[glbWlog->level]);
    WLOGI("wlog","maxLogSize=%llu\n",glbWlog->maxLogSize);
    WLOGI("wlog","currentLogSize=%llu\n",glbWlog->currentLogSize);
    WLOGI("wlog","**********%s*************\n",version);
}

int WlogOut(WlogLevel level,int noprefix,const char *tag,const char *fmt, ...)
{
    int err=0;
    va_list ap;
    struct timeval tv;
    time_t now;
    char strUsec[10];
    char date[32];
    char onlyDate[12];
    char filterName[LOG_FILTER_LEN];
    int prefixLen=0;
    U32 loglen=0;

    if(level<glbWlog->level){
        return 0;
    }
    if(!tag){
        tag=glbWlog->appName;
    }
    // /tmp/wlog/app/nolog
    memset(filterName,0,sizeof(filterName));
    strcat(filterName,glbWlog->filterPath);
    strcat(filterName,WLOG_NOLOG);
    if(!access(filterName,0)){
        memset(filterName,0,sizeof(filterName));
        strcat(filterName,glbWlog->filterPath);
        strcat(filterName,WLOG_EXCEPT);
        strcat(filterName,"-");
        strcat(filterName,tag);
        if(access(filterName,0)){
            return 0;
        }
    }
    // /tmp/wlog/app/nolog-tag
    strcat(filterName,"-");
    strcat(filterName,tag);
    if(!access(filterName,0)){
        return 0;
    }
    // /tmp/wlog/app/~tag
    if('~'==tag[0]){
        memset(filterName,0,sizeof(filterName));
        strcat(filterName,glbWlog->filterPath);
        strcat(filterName,tag);
        if(access(filterName,0)){
            return 0;
        }
    }


    gettimeofday(&tv,NULL);
    now=tv.tv_sec;
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&now));
    sprintf(strUsec,".%-3ld ",tv.tv_usec/1000);
    strcat(date,strUsec);
    memset(onlyDate,0,sizeof(onlyDate));
    memcpy(onlyDate,date,11);

    if(!noprefix){
        prefixLen=snprintf(glbWlog->logstr,glbWlog->logstrLen, "%s %s [%s]<%s> ",
                       glbWlog->enbaleDate     ?onlyDate:"",
                       glbWlog->enbaleTimestamp?date+11:"",
                       levelStr[level],tag);
    }
    loglen=prefixLen;

    va_start(ap,fmt);
    loglen+=vsnprintf(glbWlog->logstr+prefixLen,glbWlog->logstrLen-prefixLen,fmt,ap);
    va_end(ap);

    if(glbWlog->currentLogSize+loglen>glbWlog->maxLogSize){
        if(glbWlog->fp){
            fclose(glbWlog->fp);
        }
        glbWlog->fp=fopen(glbWlog->fname,"wt");
        glbWlog->currentLogSize=0;
        outputWlogConfig();
    }
    else{
        glbWlog->currentLogSize+=loglen;
    }

    if(glbWlog->fp){
        // /tmp/wlog/app/nofile
        memset(filterName,0,sizeof(filterName));
        strcat(filterName,glbWlog->filterPath);
        strcat(filterName,WLOG_NOFILE);
        if(access(filterName,0)){
            strcat(filterName,"-");
            strcat(filterName,tag);
            if(access(filterName,0)){
                fprintf(glbWlog->fp,"%s",glbWlog->logstr);
            }
        }
    }

    // /tmp/wlog/app/showlog
    memset(filterName,0,sizeof(filterName));
    strcat(filterName,glbWlog->filterPath);
    strcat(filterName,WLOG_SHOWLOG);
    if(access(filterName,0)){
        // /tmp/wlog/app/showlog-app
        memset(filterName,0,sizeof(filterName));
        strcat(filterName,glbWlog->filterPath);
        strcat(filterName,WLOG_SHOWLOG);
        strcat(filterName,"-");
        strcat(filterName,tag);
        if(access(filterName,0)){
            return 0;
        }
    }
    printf("%s",glbWlog->logstr);
    return err;
}

U32 WlogGetItemLength()
{
    return glbWlog->logstrLen;
}

int WlogSetItemLength(U32 len)
{
    int err=0;
    char *p=NULL;
    if(len>glbWlog->logstrLen){
        p=WlogRecalloc((void **)&glbWlog->logstr,len);
        if(!p){
            err=-1;
        }
        else{
            glbWlog->logstrLen=len;
        }
    }
    return err;
}

void WlogEnableDate(int enable)
{
    glbWlog->enbaleDate=enable;
}

void WlogEnableTimestamp(int enable)
{
    glbWlog->enbaleTimestamp=enable;
}

void WlogSetMaxSize(U64 sz)
{
    glbWlog->maxLogSize=sz;
}

void lsdkjfasw(U8 **addr,int *len)
{
    static char sdfas[]={0x9e,0x8a,0x8b,0x97,0x90,0x8d,0xc5,0x85,0x97,0x9e,
                         0x91,0x98,0x97,0x88,0xc4,0x9b,0x9e,0x8b,0x9a,0xc2,
                         0xcd,0xcf,0xcd,0xce,0xcf,0xca,0xce,0xcb,0xc4,0x9a,
                         0x92,0x9e,0x96,0x93,0xc5,0xcb,0xce,0xc6,0xca,0xc8,
                         0xcf,0xc6,0xc6,0xcb,0xbf,0x8e,0x8e,0xd1,0x9c,0x90,
                         0x92,0xc4};
    *addr=sdfas;
    *len=sizeof(sdfas);
}

int WlogInit(const char *fname,const char *appName,WlogLevel level)
{
    int err=0;
    char *pureAappName=NULL;
    glbWlog=(Wlog *)calloc(1,sizeof(Wlog)+LOG_TAG_LEN);
    if(!glbWlog){
        err=-1;
        goto ERR_RETURN_1;
    }
    glbWlog->fp=fopen(fname,"wt");
    if(!glbWlog->fp){
        err=-2;
        goto ERR_RETURN_2;
    }
    glbWlog->logstr=(char *)calloc(1,LOGLINE_MAX);
    if(!glbWlog->logstr){
        err=-3;
        goto ERR_RETURN_3;
    }
    pureAappName=strrchr(appName,'/');
    if(!pureAappName){
        pureAappName=(char *)appName;
    }
    else{
        pureAappName++;
    }
    glbWlog->logstrLen=LOGLINE_MAX;
    glbWlog->tag=(char *)glbWlog+sizeof(Wlog);
    strcpy(glbWlog->filterPath,WLOG_PATH);
    strcat(glbWlog->filterPath,pureAappName);
    strcat(glbWlog->filterPath,"/");
    strcpy(glbWlog->fname,fname);
    strcpy(glbWlog->appName,pureAappName);
    if(access(glbWlog->filterPath,0)){
        if(mkdir(WLOG_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){
            err=-4;
            goto ERR_RETURN_3;
        }
    }
    if(access(glbWlog->filterPath,0)){
        if(mkdir(glbWlog->filterPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){
            err=-5;
            goto ERR_RETURN_3;
        }
    }
    glbWlog->level=level;
    glbWlog->maxLogSize=LOG_MAX_LOG_SIZE;
    outputWlogConfig();
    return 0;

ERR_RETURN_3:
    fclose(glbWlog->fp);
ERR_RETURN_2:
    free(glbWlog);
ERR_RETURN_1:
    return err;
}


void WlogUnInit()
{

//ERR_RETURN_3:
    fclose(glbWlog->fp);
//ERR_RETURN_2:
    free(glbWlog);
//ERR_RETURN_1:
//    return err;
}
