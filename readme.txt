测试步骤：
1.基本输出测试
  WLOGD/WLOGI/WLOGW/WLOGE/WLOGD
  WLOGPD/WLOGPI/WLOGPW/WLOGPE/WLOGPF

2.日志过滤测试
  (1)显示日志
    touch /tmp/wlog/testwlog/showlog
  (2)禁止日志
    touch /tmp/wlog/testwlog/nolog
  (3)禁止某一个tag的日志
    rm    /tmp/wlog/testwlog/nolog
    touch /tmp/wlog/testwlog/nolog-wlog
  (4)显示某一个tag的日志
    rm    /tmp/wlog/testwlog/showlog
    touch /tmp/wlog/testwlog/showlog-zhanghw
    rm    /tmp/wlog/testwlog/showlog-zhanghw
    touch /tmp/wlog/testwlog/showlog-jack

3.单条日志溢出测试
  一次性输出大于1024的日志，看是否截断。

4.日志文件溢出测试
  输出大于上限的日志，观察日志文件是否刷新为溢出部分。



