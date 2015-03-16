using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Demo_12._1._2
{
    public partial class WinForm1 : Form
    {
        public WinForm1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            WinForm2 winform2 = new WinForm2();
            winform2.Show();
        }

    }
}
