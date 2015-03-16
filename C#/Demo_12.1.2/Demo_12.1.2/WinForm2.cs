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
    public partial class WinForm2 : Form
    {
        public WinForm2()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void WinForm2_Click(object sender, EventArgs e)
        {
            this.Hide();
            MessageBox.Show("单击窗体！");
        }
    }
}
