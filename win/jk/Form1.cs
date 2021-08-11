using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace jk
{
    public partial class Form1 : Form
    {
        public int isCreate = 0;

        public Form1()
        {
            InitializeComponent();
        }
        //int i = 1;
        public void update(Image img)
        {
            //if(i == 6)
            //{
            //    i = 1;
            //}
            //FileStream fs = new FileStream("F:\\jk\\" + i + ".jpg", FileMode.Open,
            //                                  FileAccess.Read);//获取图片文件流
            //Image img = Image.FromStream(fs); // 文件流转换成Image格式
            //pictureBox1.Image = img;   //给 图片框设置要显示的图片
            //i++;
            //fs.Close(); // 关闭流，释放图片资源
            pictureBox1.Image = img;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //update();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (isCreate == 0)
            {
                ThreadStart childref = new ThreadStart(listen);
                //Program.myForm.log.Append("In Main: Creating the Child thread");
                //Program.myForm.log.Append(Environment.NewLine);
                //Program.myForm.updateLog();
                Thread childThread = new Thread(childref);
                childThread.Start();
                isCreate = 1;
            }
        }

        public void listen()
        {
            Service.receiveMessage();
        }
    }
}
