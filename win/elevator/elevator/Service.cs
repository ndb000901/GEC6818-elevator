using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace elevator
{
    class Service
    {
        public static void sendMessage(string sendString)
        {
            Console.WriteLine("client start.......");
            //string sendString = null;//要发送的字符串
            byte[] sendData = null;//要发送的字节数组
            UdpClient client = null;

            IPAddress remoteIP = IPAddress.Parse("192.168.0.10");
            int remotePort = 11000;
            IPEndPoint remotePoint = new IPEndPoint(remoteIP, remotePort);//实例化一个远程端点

            while (true)
            {
                //sendString = Console.ReadLine();
                sendData = Encoding.Default.GetBytes(sendString);

                client = new UdpClient();
                client.Send(sendData, sendData.Length, remotePoint);//将数据发送到远程端点
                client.Close();//关闭连接
                return;
            }
        }

        public static void receiveMessage()
        {
            Console.WriteLine("server start.....");
            UdpClient client = null;
            string receiveString = null;
            byte[] receiveData = null;
            //实例化一个远程端点，IP和端口可以随意指定，等调用client.Receive(ref remotePoint)时会将该端点改成真正发送端端点
            IPEndPoint remotePoint = new IPEndPoint(IPAddress.Any, 0);

            //IPEndPoint remotePoint = new IPEndPoint(IPAddress.Parse("0.0.0.0"), 11000);

            while (true)
            {
                client = new UdpClient(10000);
                receiveData = client.Receive(ref remotePoint);//接收数据
                receiveString = Encoding.Default.GetString(receiveData);
                Program.myForm.log.Append(receiveString);
                Program.myForm.log.Append(Environment.NewLine);
                Program.myForm.clearColor(receiveString);

                Console.WriteLine(receiveString);
                client.Close();//关闭连接
            }
        }
    
    }
}
