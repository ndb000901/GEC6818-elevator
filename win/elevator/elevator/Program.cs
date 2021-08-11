using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace elevator
{
    static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        /// 

        public static Form myForm = new Form();
        [STAThread]
        static void Main()
        {
            Application.SetHighDpiMode(HighDpiMode.SystemAware);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form());
            Application.Run(myForm);
            
            
        }
    }
}
