##2、计算 N 以内的所有素数

编写C++程序完成以下功能：

（1）提示用户输入N；

（2）计算出从2到N之间的所有素数；

（3）将结果保存在一个文本文件中

###Windows下，获得工作目录如下：

    #include  <direct.h>  
    #include  <stdio.h>  
    char   buffer[MAX_PATH];   
    getcwd(buffer, MAX_PATH);   
    printf( "The   current   directory   is:   %s ",   buffer);  

###获得exe当前目录：

    #include <windows.h>
    char chpath[MAX_PATH];
    GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
    std::cout<<chpath<<std::endl;
     