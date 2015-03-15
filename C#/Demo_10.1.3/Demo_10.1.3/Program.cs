using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo_10._1._3 //显式接口成员实现
{
    interface ImyInterface1
    {
        int Add();
    }
    interface ImyInterface2
    {
        int Add();
    }
    class myClass:ImyInterface1,ImyInterface2
    {
        int ImyInterface1.Add()
        {
            int x = 3;
            int y = 5;
            return x + y;
        }
        int ImyInterface2.Add()
        {
            int x = 3;
            int y = 5;
            int z = 7;
            return x + y + z;
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            myClass myclass = new myClass();
            ImyInterface1 imyinterface1 = myclass;
            ImyInterface2 imyinterface2 = myclass;
            Console.WriteLine(imyinterface1.Add());
            Console.WriteLine(imyinterface2.Add());
        }
    }
}
