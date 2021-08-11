using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace elevator
{
    

    public partial class Form : System.Windows.Forms.Form
    {
        //public int[] btn_up = new int[8];
        //public int[] btn_down = new int[8];
        public static int isCreate = 0;

        public StringBuilder log = new StringBuilder();

        public void updateLog()
        {
            textBox_log.Text = log.ToString();
        }
        public Form()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
        public void show()
        {
            //StringBuilder up = new StringBuilder();
            //StringBuilder down = new StringBuilder();
            //int i;
            //for(i = 1;i < 8;i++)
            //{
            //    up.Append(btn_up[i] + ",");
            //    down.Append(btn_down[i] + ",");
            //}
            //textBox.Text = up.ToString() + "\n" + down.ToString();
        }

        private void btn_up1_Click(object sender, EventArgs e)
        {
            btn_up1.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up1);
            log.Append("点击了1楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            show();
        }

        private void btn_up2_Click(object sender, EventArgs e)
        {
            btn_up2.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up2);

            log.Append("点击了2楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();

        }

        private void btn_up3_Click(object sender, EventArgs e)
        {

            btn_up3.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up3);
            log.Append("点击了3楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_up4_Click(object sender, EventArgs e)
        {

            btn_up4.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up4);
            log.Append("点击了4楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_up5_Click(object sender, EventArgs e)
        {
            btn_up5.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up5);
            log.Append("点击了5楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_up6_Click(object sender, EventArgs e)
        {

            btn_up6.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_up6);
            log.Append("点击了6楼上行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_up7_Click(object sender, EventArgs e)
        {

        }

        private void btn_down7_Click(object sender, EventArgs e)
        {

            btn_down7.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down7);
            log.Append("点击了1楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down6_Click(object sender, EventArgs e)
        {
            btn_down6.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down6);
            log.Append("点击了6楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down5_Click(object sender, EventArgs e)
        {
            btn_down5.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down5);
            log.Append("点击了5楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down4_Click(object sender, EventArgs e)
        {
            btn_down4.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down4);
            log.Append("点击了4楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down3_Click(object sender, EventArgs e)
        {
            btn_down3.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down3);
            log.Append("点击了3楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down2_Click(object sender, EventArgs e)
        {
            btn_down2.BackColor = System.Drawing.Color.Yellow;
            Service.sendMessage(Message.add_down2);
            log.Append("点击了2楼下行按钮");
            log.Append(Environment.NewLine);
            this.updateLog();
        }

        private void btn_down1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            Service.sendMessage(this.textBox2.Text);
        }

        private void Form_Load(object sender, EventArgs e)
        {
            this.btn_down1.Enabled = false;
            this.btn_up7.Enabled = false;

            if (isCreate == 0)
            {
                ThreadStart childref = new ThreadStart(listenCmd);
                //Program.myForm.log.Append("In Main: Creating the Child thread");
                //Program.myForm.log.Append(Environment.NewLine);
                //Program.myForm.updateLog();
                Thread childThread = new Thread(childref);
                childThread.Start();
                isCreate = 1;
            }

        }

        public void listenCmd()
        {
            Service.receiveMessage();
            Program.myForm.log.Append("122233");
            //Program.myForm.btn_down2.BackColor = System.Drawing.Color.Red;

        }

        private void button3_Click(object sender, EventArgs e)
        {
            updateLog();
        }

        public void clearColor(string cmd)
        {
            if(cmd == Message.delete_up1)
            {
                Program.myForm.btn_up1.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_up2)
            {
                Program.myForm.btn_up2.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_up3)
            {
                Program.myForm.btn_up3.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_up4)
            {
                Program.myForm.btn_up4.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_up5)
            {
                Program.myForm.btn_up5.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_up6)
            {
                Program.myForm.btn_up6.BackColor = System.Drawing.Color.White;
            }
            else if(cmd == Message.delete_down2)
            {
                Program.myForm.btn_down2.BackColor = System.Drawing.Color.White;
            }
            else if (cmd == Message.delete_down3)
            {
                Program.myForm.btn_down3.BackColor = System.Drawing.Color.White;
            }
            else if (cmd == Message.delete_down4)
            {
                Program.myForm.btn_down4.BackColor = System.Drawing.Color.White;
            }
            else if (cmd == Message.delete_down5)
            {
                Program.myForm.btn_down5.BackColor = System.Drawing.Color.White;
            }
            else if (cmd == Message.delete_down6)
            {
                Program.myForm.btn_down6.BackColor = System.Drawing.Color.White;
            }
            else if (cmd == Message.delete_down7)
            {
                Program.myForm.btn_down7.BackColor = System.Drawing.Color.White;
            }
        }
    }
}
