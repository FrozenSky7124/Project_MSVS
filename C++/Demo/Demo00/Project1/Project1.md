##1、猜价格游戏
  
编写C++程序完成以下功能：
  
（1）假定有一件商品，程序用随机数指定该商品的价格（1-1000的整数）；

（2）提示用户猜价格，并输入：若用户猜的价格比商品价格高或低，对用户作出相应的提示；

（3）直到猜对为止，并给出提示。

##获取更加精确的随机数

    LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);//初始化一个以微秒为单位的时间种子
    int _value=rand()%10;//产生一个随机数