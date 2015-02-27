using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FontMaker
{
    public partial class EditChar : Form
    {
        private Form1 main = null;
        private Bitmap sourceImage = null;
        private Rectangle rect = new Rectangle(0, 0, 10, 10);
        private ListViewItem item = null;

        public EditChar(Form1 main)
        {
            this.main = main;
            this.item = null;
            InitializeComponent();
        }

        private void CropImage(Rectangle rect)
        {
            if(this.sourceImage != null)
            {
                PictureBox pb = this.pictureBox1;

                Bitmap tmp = new Bitmap(pb.Width, pb.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

                tmp.SetResolution(sourceImage.HorizontalResolution, sourceImage.VerticalResolution);

                Graphics graphics = Graphics.FromImage(tmp);

                graphics.InterpolationMode = InterpolationMode.NearestNeighbor;
                graphics.PixelOffsetMode = PixelOffsetMode.Half;
                graphics.SmoothingMode = SmoothingMode.None;
                graphics.Clear(Color.Black);

                graphics.DrawImage(sourceImage, new Rectangle(0, 0, pb.Width, pb.Height), this.rect, GraphicsUnit.Pixel);
                graphics.Dispose();

                pictureBox1.Image = tmp;
            }
        }

        public void Setup(ListViewItem item, String character, Rectangle rect)
        {
            this.textBox1.Text = character;

            this.textBoxX.Text = Convert.ToString(rect.Left);
            this.textBoxY.Text = Convert.ToString(rect.Top);
            this.textBoxWidth.Text = Convert.ToString(rect.Width);
            this.textBoxHeight.Text = Convert.ToString(rect.Height);
            this.item = item;
            this.rect = rect;
            CropImage(this.rect);
        }

        public void UpdateImage(Bitmap sourceImage)
        {
            pictureBox1.Image = sourceImage;
            this.sourceImage = sourceImage;
            CropImage(this.rect);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.main.UpdateCharacter(this.item, this.textBox1.Text, this.rect);
            this.Hide();
        }

        private void textBoxX_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int x = Convert.ToInt32(textBoxX.Text);
                this.rect.X = x;

                CropImage(this.rect);
            }
            catch (Exception exception)
            {
                MessageBox.Show("This value must be integer! ("+exception.Message.ToString()+")");
            }
        }

        private void textBoxY_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int y = Convert.ToInt32(textBoxY.Text);
                this.rect.Y = y;

                CropImage(this.rect);
            }
            catch (Exception exception)
            {
                MessageBox.Show("This value must be integer! ("+exception.Message.ToString()+")");
            }
        }

        private void textBoxWidth_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int width = Convert.ToInt32(textBoxWidth.Text);
                this.rect.Width = width;

                CropImage(this.rect);
            }
            catch (Exception exception)
            {
                MessageBox.Show("This value must be integer! ("+exception.Message.ToString()+")");
            }
        }

        private void textBoxHeight_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int height = Convert.ToInt32(textBoxHeight.Text);                
                this.rect.Height = height;

                CropImage(this.rect);
            }
            catch (Exception exception)
            {
                MessageBox.Show("This value must be integer! ("+exception.Message.ToString()+")");
            }
        }
    }
}
