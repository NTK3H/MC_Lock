# MC_Lock

# Info:
这是一款可以满足小型文件加密的程序\
它被设计的十分简答易用,相信很快便能上手。

# How to:
1.使用前需要使用genkey生成密匙文件或者输入密码加密或解密(选其一且二者可并存,后者优先级高)\
2.使用密码加解密时密码前面先输入':',否则程序便认为你输入的是一个密匙的目录\
例如 密码是 123456 则应输入 :123456\
3.在未输入的情况下直接回车则按照默认值([]内的内容)为准\
4.如果要加密一个文件可以在输入加密文件目录的时候直接将此文件拖进去\
5.当某个异常发生后(例如无法打开文件)那么程序会报告这个异常之后阻塞。此时按任意键即可退出\
6.关于程序在output文件夹下的预定义文件:\
password.pwd:	存放了生成的密匙\
locked.mclock:	存放了加密后的产物\
source.txt:		待加密的内容\
unlock.txt:		解密后的内容

# BUG List:
1. 加密文件过大可能会使程序停止执行(最大支持25MB)
