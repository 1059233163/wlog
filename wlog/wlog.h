/*
*
1.输出方式一：自动添加前缀，输出受log等级控制
  WLOGD/WLOGI/WLOGW/WLOGE/WLOGF
2.输出方式二：不添加，输出受log等级控制
  WLOGPD/WLOGPI/WLOGPW/WLOGPE/WLOGPF
3.日志级别
  两种输出方式均收日志级别控制。日志级别在init的时候设置。小于设置的级别的日志不会输出。
4.日期和时间戳
  缺省是不添加日期和时间戳的。需要调用WlogEnableDate和WlogEnableTimestamp来开启。
5.log过滤
  (1)./tmp/wlog/app
    过滤规则目录,app为程序名称
  (2)./tmp/wlog/app/nolog
    禁止产生log
  (3)./tmp/wlog/app/showlog
    打印log到前台
  (4)./tmp/wlog/app/nolog-tag
    禁止带有tag的log产生
  (5)./tmp/wlog/app/showlog-tag
    把带有tag的log打印到前台
   注意：nolog优先，有nolog的情况下，showlog无效
6.单条日志输出上限
  缺省为1024，超出部分丢失，可以通过WlogSetItemLength设置
5.日志空间限制
  默认最大保存128KB日志，如果需要改动，可以调用WlogSetMaxSize进行测试。
*/
#ifndef __WLOG_H
#define __WLOG_H

#if __cplusplus
extern "C"{
#endif

typedef unsigned long long int U64;
typedef unsigned int U32;
typedef unsigned char U8;

typedef enum{
    Wlog_Level_Debug=1,
    Wlog_Level_Info,
    Wlog_Level_Warn,
    Wlog_Level_Error,
    Wlog_Level_Fatal,
}WlogLevel;

#define WLOGD(tag,fmt,arg...) WlogOut(Wlog_Level_Debug,0,tag,"[%s-%d]: "fmt,__func__,__LINE__,##arg);
#define WLOGI(tag,fmt,arg...) WlogOut(Wlog_Level_Info ,0,tag,"[%s-%d]: "fmt,__func__,__LINE__,##arg);
#define WLOGW(tag,fmt,arg...) WlogOut(Wlog_Level_Warn ,0,tag,"[%s-%d]: "fmt,__func__,__LINE__,##arg);
#define WLOGE(tag,fmt,arg...) WlogOut(Wlog_Level_Error,0,tag,"[%s-%d]: "fmt,__func__,__LINE__,##arg);
#define WLOGF(tag,fmt,arg...) WlogOut(Wlog_Level_Fatal,0,tag,"[%s-%d]: "fmt,__func__,__LINE__,##arg);

#define WLOGPD(tag,fmt,arg...) WlogOut(Wlog_Level_Debug,1,tag,fmt,##arg);
#define WLOGPI(tag,fmt,arg...) WlogOut(Wlog_Level_Info ,1,tag,fmt,##arg);
#define WLOGPW(tag,fmt,arg...) WlogOut(Wlog_Level_Warn ,1,tag,fmt,##arg);
#define WLOGPE(tag,fmt,arg...) WlogOut(Wlog_Level_Error,1,tag,fmt,##arg);
#define WLOGPF(tag,fmt,arg...) WlogOut(Wlog_Level_Fatal,1,tag,fmt,##arg);

/*
*初始化wlog，输出log前必须先调用
*@fname: 日志保存路径
*@appName: 当前应用程序的名称
*@level: 日志打印级别
*@retval:0 or err
*/
int WlogInit(const char *fname,const char *appName,WlogLevel level);
/*
*获取每条log允许最大的长度
*@retval: 每条log允许最大的长度
*/
U32 WlogGetItemLength();
/*
*设置每条log允许最大的长度，缺省为1024
*@len: 每条log允许最大的长度，小于1024设置无效
*@retval: 0 or err
*/
int WlogSetItemLength(U32 len);
/*
*设置最大日志文件大小，缺省为100K
*@sz：最大日志文件大小
*/
void WlogSetMaxSize(U64 sz);
/*
*设置是否添加Date前缀，缺省为不添加
*enable: 0不添加，1添加
*/
void WlogEnableDate(int enable);
/*
**设置是否添加Timestamp前缀，缺省为不添加
*enable: 0不添加，1添加
*/
void WlogEnableTimestamp(int enable);
/*
*输出日志
*@level: 日志打印级别
*@noprefix: 是否添加前缀,0不添加，1添加
*@tag: 日志tag，NULL为默认使用app名称
*@fmt: 格式化输出
*@...: 格式化输出参数
*@retval: 0 or err
*/
int  WlogOut(WlogLevel level,int noprefix,const char *tag,const char *fmt, ...);
/*
*wlog去初始化
*/
void WlogUnInit();


#if __cplusplus
}
#endif

#endif
