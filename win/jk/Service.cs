using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace jk
{
    class Service
    {
        public static void receiveMessage()
        {
            Console.WriteLine("server start.....");
            UdpClient client = null;
            //string receiveString = null;
            byte[] receiveData = null;
            //实例化一个远程端点，IP和端口可以随意指定，等调用client.Receive(ref remotePoint)时会将该端点改成真正发送端端点
            IPEndPoint remotePoint = new IPEndPoint(IPAddress.Any, 0);

            //IPEndPoint remotePoint = new IPEndPoint(IPAddress.Parse("0.0.0.0"), 11000);

            while (true)
            {
                client = new UdpClient(30000);
                receiveData = client.Receive(ref remotePoint);//接收数据
                //receiveString = Encoding.Default.GetString(receiveData);

                MemoryStream ms = new MemoryStream(receiveData);
                ms.Position = 0;
                Image img = Image.FromStream(ms);
                Program.myForm.update(img);
                ms.Close();
                //FileStream fileStream = new FileStream(receiveData,0, receiveData.Length);

                //fileStream.Read(fileBytes, 0, byteLength);
                //Console.WriteLine(receiveString);
                
                client.Close();//关闭连接
            }
        }

    
}
}
