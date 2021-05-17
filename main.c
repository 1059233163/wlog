#include <stdio.h>

#include "wlog.h"

int main(int argc,char **argv)
{
    int i=0,err=0,cnt=0;
    if((err=WlogInit("./my.log",argv[0],Wlog_Level_Debug))){
        printf("wlog init err=%d\n",err);
        return -1;
    }
    WlogEnableDate(0);
    WlogEnableTimestamp(1);
    WLOGD("zhanghw","test wlog %d\n",100);
    WLOGI("zhanghw","test wlog %d\n",100);
    WLOGW("zhanghw","test wlog %d\n",100);
    WLOGE("zhanghw","test wlog %d\n",100);
    WLOGF("zhanghw","test wlog %d\n",100);


    WLOGF("~zhanghw","~test wlog %d\n",100);

    WLOGD("jack","test wlog %d-%d\n",100,200);
    WLOGI("jack","test wlog %d-%d\n",100,200);
    WLOGW("jack","test wlog %d-%d\n",100,200);
    WLOGE("jack","test wlog %d-%d\n",100,200);
    WLOGF("jack","test wlog %d-%d\n",100,200);

    WLOGPD("jack","test wlogp\n");
    for(i=0;i<256;i++){
        WLOGPD("jack","%02x ",i);
    }
    WLOGPD("jack","\n\n");
    WLOGPI("jack","test wlogp\n");
    for(i=0;i<256;i++){
        WLOGPI("jack","%02x ",i);
    }
    WLOGPI("jack","\n\n");
    WLOGPW("jack","test wlogp\n");
    for(i=0;i<256;i++){
        WLOGPW("jack","%02x ",i);
    }
    WLOGPW("jack","\n\n");
    WLOGPE("jack","test wlogp\n");
    for(i=0;i<256;i++){
        WLOGPE("jack","%02x ",i);
    }
    WLOGPE("jack","\n\n");
    WLOGPF("jack","test wlogp\n");
    for(i=0;i<256;i++){
        WLOGPF("jack","%02x ",i);
    }
    WLOGPF("jack","\n\n");

    WLOGPF("jack","");

//    WlogSetMaxSize(1024*1024);//如果不设置，日志溢出，会发生覆盖
//    while(cnt++<10000){
//        WLOGF("tom","test wlog cnt=%d\n",cnt);
//    }

    WlogSetItemLength(2048);//如果不设置，下面的输出会被截断
    WLOGPF("congcong",
    "Hurriedly\n"
    "Swallows may have gone, but there is a time of return; willow\n"
    "trees, there is a time of regreening; peach blossom, bloom one day.However,\n"
    "smart, you tell me, why should our days gone forever?-- perhaps they have been\n"
    "stolen by someone: who is that?Where could he hide?Could they be: where are they\n"
    "now?I don not know how many days I have been given; but my hands are\n"
    "empty.Counting up silently, I find that more than 8000 days have already slipped\n"
    "away through my fingers; like a drop in the ocean, my days are dripping into the\n"
    "stream of time, no sound, no shadow.And tears welling up in my eyes.What is gone\n"
    "is gone, what is to come; the middle go to, how hurry?When I get up in the\n"
    "morning, hut two or three shot into the sun slanting.The sun has his feet,\n"
    "lightly and furtively; and I am caught blankly.So -- when I wash my hands, in\n"
    "the past day from the basin; when eating, time from their jobs in the past;\n"
    "silently, then pass through staring eyes.I perceived him to rush, hand hide in,\n"
    "he from the cover of the hand in the past, when it was dark, I lie in bed, he\n"
    "nimbly crossed from me, from my feet.When I open my eyes and meet the sun again,\n"
    "another day has gone by.I bury my face in my hands.But the new day begins to\n"
    "flash past in the sigh.Fleeting days, in numerous households in the world of\n"
    "what can I do?Nothing but to hesitate, to rush; in that eight-thousand-day rush,\n"
    "apart from hesitating, what is left?Over the past days, such as smoke, blown off\n"
    "by breeze, such as mist, have been evaporated by the rising sun; what traces\n"
    "have I left behind me?Have I ever left any gossamer traces?I have come to the\n"
    "world, stark naked?It is not fair, why should I have made such a trip?You the\n"
    "wise, tell me, why should our days gone forever?\n");

    return 0;
}


