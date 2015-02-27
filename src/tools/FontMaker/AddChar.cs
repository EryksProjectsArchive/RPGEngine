using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FontMaker
{
    public partial class AddChar : Form
    {
        private Form1 main;
        public AddChar(Form1 main)
        {
            this.main = main;
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                this.main.NewCharacter(this.textBox1.Text);
                this.textBox1.Text = "";
                this.Hide();
            } 
            catch(Exception exception)
            {
                MessageBox.Show(exception.Message.ToString());
            }
        }

        private void AddChar_KeyDown(object sender, KeyEventArgs e)
        {
            if (!this.Visible)
                return;

            if(e.KeyCode == Keys.Enter)
            {
                button1_Click(this, null);
                e.Handled = true;
            }
        }        
    }
}
