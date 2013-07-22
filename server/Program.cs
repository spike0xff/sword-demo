using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Server
{
    class Program
    {
        static void Main(string[] args)
        {
            SwordServer swordserver = new SwordServer();
            swordserver.Prefix = "http://172.16.2.13:12345/";
            swordserver.Start();
            System.Threading.Thread.Sleep(9999999);
            swordserver.Stop();
            swordserver = null;
        }
    }
}
