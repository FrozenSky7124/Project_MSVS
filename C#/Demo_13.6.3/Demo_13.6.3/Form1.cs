using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Demo_13._6._3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.button1.Text = "Click";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.toolStripProgressBar1.Minimum = 0;
            this.toolStripProgressBar1.Maximum = 1000000;
            this.toolStripProgressBar1.Step = 2;
            for (int i = 1; i <= 50000; i++)
            {
                this.toolStripProgressBar1.PerformStep();
                this.toolStripStatusLabel1.Text = "Value:" + Convert.ToString(this.toolStripProgressBar1.Value);
            }
        }
    }
}
