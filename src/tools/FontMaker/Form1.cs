using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FontMaker
{
    public partial class Form1 : Form
    {
        private String textureName = null;
        private String fontDescriptorPath = null;
        private double scaleX = 1.0;
        private double scaleY = 1.0;
        private int sourceWidth = 0;
        private int sourceHeight = 0;
        private int targetWidth = 0;
        private int targetHeight = 0;
        private Bitmap sourceImage = null;
        private Point imagePos = new Point(0, 0);

        /* Forms */
        private AddChar addChar = null;
        private EditChar editChar = null;

        public Form1()
        {
            this.addChar = new AddChar(this);
            this.editChar = new EditChar(this);

            InitializeComponent();

            Reset();
        }

        private void SetImage(String name, PictureBox box)
        {
            button4.Text = Path.GetFileName(name);
            textureName = button4.Text;

            imagePos.X = imagePos.Y = 0;
            sourceImage = new Bitmap(name);
            scaleX = 1.0f;
            scaleY = 1.0f;

            sourceWidth = sourceImage.Width;
            sourceHeight = sourceImage.Height; 
            double ratio;

            if (sourceWidth > sourceHeight)
            {
                targetWidth = box.Width;
                ratio = (double)targetWidth / sourceWidth;
                targetHeight = (int)(ratio * sourceHeight);
            }
            else if (sourceWidth < sourceHeight)
            {
                targetHeight = box.Height;
                ratio = (double)targetHeight / sourceHeight;
                targetWidth = (int)(ratio * sourceWidth);
            }
            else
            {
                targetHeight = pictureBox1.Height;
                targetWidth = pictureBox1.Width;
            }                

            Bitmap tempBitmap = new Bitmap(box.Width, box.Height, PixelFormat.Format24bppRgb);

            tempBitmap.SetResolution(sourceImage.HorizontalResolution, sourceImage.VerticalResolution);
            Graphics bmGraphics = Graphics.FromImage(tempBitmap);


            bmGraphics.Clear(Color.Black);

            bmGraphics.InterpolationMode = InterpolationMode.NearestNeighbor;
            bmGraphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            bmGraphics.SmoothingMode = SmoothingMode.None;

            bmGraphics.DrawImage(sourceImage,
                                 new Rectangle(0, 0, targetWidth, targetHeight),
                                 new Rectangle(0, 0, sourceWidth, sourceHeight),
                                 GraphicsUnit.Pixel);

            bmGraphics.Dispose();
            box.Image = tempBitmap;

            this.editChar.UpdateImage(sourceImage);

            this.trackBar1.Enabled = true;
            this.trackBar1.Value = 1;
        }

        private void UpdateImage()
        {
            if (sourceImage == null)
                return;

            Bitmap tempBitmap = new Bitmap(pictureBox1.Width, pictureBox1.Height, PixelFormat.Format24bppRgb);

            tempBitmap.SetResolution(sourceImage.HorizontalResolution, sourceImage.VerticalResolution);
            Graphics bmGraphics = Graphics.FromImage(tempBitmap);

            bmGraphics.Clear(Color.Black);

            bmGraphics.InterpolationMode = InterpolationMode.NearestNeighbor;
            bmGraphics.PixelOffsetMode = PixelOffsetMode.Half;
            bmGraphics.SmoothingMode = SmoothingMode.None;

            bmGraphics.DrawImage(sourceImage, new Rectangle(0, 0, targetWidth, targetHeight),
                                 new Rectangle(imagePos.X, imagePos.Y, (int)(sourceWidth / scaleX), (int)(sourceHeight / scaleY)),
                                 GraphicsUnit.Pixel);

            bmGraphics.Dispose();
            pictureBox1.Image = tempBitmap;
        }

        private void ScaleImage(float scale)
        {
            if (sourceImage == null)
                return;

            if (scale <= 1.0f)
                scale = 1.0f;

            scaleX = scale;
            scaleY = scale;

            UpdateImage();
        }

        private void SetFontFile(String name)
        {
            fontDescriptorPath = name;
            listView1.Items.Clear();
            button2.Enabled = false;
            button3.Enabled = false;

            StreamReader reader = new StreamReader(name);
            string rawLine = null;
           
            List<String> lines = new List<String>();
            while ((rawLine = reader.ReadLine()) != null)
            {
                if ((rawLine.Length == 0) || (rawLine[0] == '#'))
                    continue;

                lines.Add(rawLine);
            }

            int step = 100/lines.Count;
            toolStripLoadingProgressBar.Visible = true;
            toolStripLoadingProgressBar.Value = 0;            
            foreach(string line in lines)
            {
                if ((line.Length == 0) ||  (line[0] == '#'))
                    continue;

                string [] v = line.Split(' ');
                if (v.Length == 2)
                {
                    if (v[0] == "Texture")
                    {
                        if (this.textureName != null)
                        {
                            if (v[1] != this.textureName)
                            {
                                MessageBox.Show("Texture does not match. (" + v[1] + ", " + this.textureName + ")", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            }
                        } 
                        else
                        {
                            SetImage(Path.GetDirectoryName(name) + Path.DirectorySeparatorChar + v[1], pictureBox1);
                        }                        
                    }
                }
                else if(v.Length == 5)
                {
                    String c = v[0];
                    UInt32 x = Convert.ToUInt32(v[1]);
                    UInt32 y = Convert.ToUInt32(v[2]);
                    UInt32 w = Convert.ToUInt32(v[3]);
                    UInt32 h = Convert.ToUInt32(v[4]);

                    ListViewItem item = new ListViewItem(c);
                    item.Checked = true;

                    item.SubItems.Add(new ListViewItem.ListViewSubItem(item, x.ToString()));
                    item.SubItems.Add(new ListViewItem.ListViewSubItem(item, y.ToString()));
                    item.SubItems.Add(new ListViewItem.ListViewSubItem(item, w.ToString()));
                    item.SubItems.Add(new ListViewItem.ListViewSubItem(item, h.ToString()));

                    listView1.Items.Add(item);
                }
                toolStripLoadingProgressBar.Value += step;         
            }
            reader.Close();
            toolStripLoadingProgressBar.Visible = false;
            
        }

        private void oPenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!CanCloseWithoutSaving())
            {
                SaveQuestion();
            }

            Reset();            

            DialogResult result = openFileDialog2.ShowDialog();
            if(result == DialogResult.OK)
            {                
                SetFontFile(openFileDialog2.FileName.ToString());
            }
        }

        private void OnPaint(object sender, PaintEventArgs e)
        {
            e.Graphics.PixelOffsetMode = PixelOffsetMode.Half;

            float _scaleX = ((float)targetWidth / sourceWidth) * (float)scaleX;
            float _scaleY = ((float)targetHeight / sourceHeight) * (float)scaleY;

            // Draw helper lines     
            if(targetWidth != 0 && sourceWidth != 0)
            {
                int _y = ((int)_scaleY);
                for (int y = 0; y < pictureBox1.Height; ++y)
                {
                    e.Graphics.DrawLine(new Pen(Color.FromArgb(50, 0, 0, 0)), new Point(0, _y), new Point(pictureBox1.Width, _y));
                    _y += ((int)_scaleY);
                }

                int _x = 0;
                for (int x = 0; x < pictureBox1.Width; ++x)
                {
                    e.Graphics.DrawLine(new Pen(Color.FromArgb(50, 0, 0, 0)), new Point(_x, 0), new Point(_x, pictureBox1.Height));
                    _x += ((int)_scaleX);
                }
            }
            
            // Draw glyphs
            foreach (ListViewItem item in listView1.Items)
            {
                if (item.Checked)
                {                    
                    int x = (int)((Convert.ToDouble(item.SubItems[1].Text)) * _scaleX);
                    int y = (int)((Convert.ToDouble(item.SubItems[2].Text)) * _scaleY);
                    int w = (int)((Convert.ToDouble(item.SubItems[3].Text)) * _scaleX);
                    int h = (int)((Convert.ToDouble(item.SubItems[4].Text)) * _scaleY);
                    e.Graphics.FillRectangle(new SolidBrush(item.Selected ? Color.FromArgb(180, 255, 180, 0) : Color.FromArgb(180, 38, 132, 211)), new Rectangle(x-(int)(imagePos.X * _scaleX), y-(int)(imagePos.Y * _scaleY), w, h));
                }
            }
        }

        private void replaceTextureToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ((listView1.Items.Count > 0 || sourceImage != null) && fontDescriptorPath != null)
            {
                SaveToFile(fontDescriptorPath);
            }
            else
            {
                MessageBox.Show("Nothing to save");
            }
        }

        private void listView1_MouseClick(object sender, MouseEventArgs e)
        {
    
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            float zoom = (float)trackBar1.Value;
            label2.Text = "Zoom (" + (zoom * 100.0f).ToString("0.0") + "%):";
            // TODO: Handle it with zooming!
            imagePos.X = 0; 
            imagePos.Y = 0;
            if(zoom > 0.0f)
            {
                hScrollBar1.Enabled = true;
                vScrollBar1.Enabled = true;
            }
            else
            {
                hScrollBar1.Enabled = false;
                vScrollBar1.Enabled = false;
            }

            ScaleImage(zoom);
        }

        private void hScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            imagePos.X = (int)((float)sourceWidth * ((float)hScrollBar1.Value / hScrollBar1.Maximum));
            UpdateImage();
        }

        private void vScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            imagePos.Y = (int)((float)sourceHeight * ((float)vScrollBar1.Value / vScrollBar1.Maximum));
            UpdateImage();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            pictureBox1.Refresh();

            if (listView1.SelectedItems.Count > 0)
            {
                button2.Enabled = true;
                button3.Enabled = true;
            }
            else
            {
                button2.Enabled = false;
                button3.Enabled = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!addChar.Visible)
            {
                addChar.Show();
            }
            else
            {
                addChar.BringToFront();
            }
        }

        public void NewCharacter(string character)
        {
            if(character.Length != 1)
            {
                throw new Exception("Please type any character to add.");
            }

            foreach (ListViewItem item in listView1.Items)
            {
                if (item.Text == character)
                {
                    throw new Exception("Duplicate");
                }
            }

            ListViewItem newItem = new ListViewItem(character);
            newItem.Checked = true;

            newItem.SubItems.Add(new ListViewItem.ListViewSubItem(newItem, "0"));
            newItem.SubItems.Add(new ListViewItem.ListViewSubItem(newItem, "0"));
            newItem.SubItems.Add(new ListViewItem.ListViewSubItem(newItem, "0"));
            newItem.SubItems.Add(new ListViewItem.ListViewSubItem(newItem, "0"));

            listView1.Items.Add(newItem);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if(listView1.SelectedItems[0] != null)
            {
                if(MessageBox.Show("Do you really want to remove this character?", "Question", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    listView1.Items.Remove(listView1.SelectedItems[0]);
                }
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                if (this.editChar.IsDisposed)
                {
                    this.editChar = new EditChar(this);
                    this.editChar.UpdateImage(sourceImage);
                }
                                
                ListViewItem item = listView1.SelectedItems[0];
                this.editChar.Setup(item, item.Text, new Rectangle(Convert.ToInt32(item.SubItems[1].Text), Convert.ToInt32(item.SubItems[2].Text), Convert.ToInt32(item.SubItems[3].Text), Convert.ToInt32(item.SubItems[4].Text)));
                this.editChar.Show();
            }
        }

        public void UpdateCharacter(ListViewItem item, String character, Rectangle rect)
        {
            item.Text = character;
            item.SubItems[1].Text = rect.Left.ToString();
            item.SubItems[2].Text = rect.Top.ToString();
            item.SubItems[3].Text = rect.Width.ToString();
            item.SubItems[4].Text = rect.Height.ToString();

            pictureBox1.Refresh();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                SetImage(openFileDialog1.FileName.ToString(), pictureBox1);
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!CanCloseWithoutSaving())
            {
                SaveQuestion();
            }

            Reset();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!CanCloseWithoutSaving())
            {
                SaveQuestion();
            }

            Reset();
        }

        private void SaveQuestion()
        {
            if(MessageBox.Show("Do you want to save your current work?", "Question", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                SaveFontAs();
            }               
        }

        private void SaveToFile(String fileName)
        {
            StreamWriter writer = new StreamWriter(fileName);
            if (textureName != null)
            {
                writer.Write("Texture " + textureName);
                writer.Write(writer.NewLine);
            }

            foreach (ListViewItem item in listView1.Items)
            {
                writer.Write(item.Text.ToString().ToCharArray()[0] + " " + item.SubItems[1].Text.ToString() + " " + item.SubItems[2].Text.ToString()
                    + " " + item.SubItems[3].Text.ToString() + " " + item.SubItems[4].Text.ToString() + writer.NewLine);
            }

            writer.Close();
            fontDescriptorPath = fileName;
        }

        private void SaveFontAs()
        {
            saveFileDialog1.FileName = fontDescriptorPath;
            if(saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                SaveToFile(saveFileDialog1.FileName.ToString());
            }
            else
            {
                if(MessageBox.Show("Are you sure do you want to skip saving? Unsaved changes will be lose.", "Question", MessageBoxButtons.YesNo) == DialogResult.No)
                {
                    SaveFontAs();
                }
            }
        }

        private bool CanCloseWithoutSaving()
        {
            return listView1.Items.Count == 0 && sourceImage == null;
        }

        private void Reset()
        {
            listView1.Items.Clear();
            fontDescriptorPath = null;
            textureName = null;
            scaleX = 1.0;
            scaleY = 1.0;
            sourceWidth = 0;
            sourceHeight = 0;
            targetWidth = 0;
            targetHeight = 0;
            sourceImage = null;
            imagePos = new Point(0, 0);
            pictureBox1.Image = null;
            pictureBox1.Refresh();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFontAs();
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            // TODO: Improve
            if((e.Control && e.KeyCode == Keys.S) && fontDescriptorPath != null)
            {
                MessageBox.Show("Saving");
                SaveToFile(fontDescriptorPath);
            }
        }

        private void listView1_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            pictureBox1.Refresh();
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.sourceImage != null)
            {
                double w = (((double)sourceWidth / scaleX) * ((double)e.X/targetWidth));
                double h = (((double)sourceHeight / scaleY)  * ((double)e.Y/targetHeight));
                
             
               this.toolStripStatusLabel1.Text = ("X: " + (imagePos.X+(int)w)+ " Y: " + (imagePos.Y+(int)h));
            }
            else
            {
                this.toolStripStatusLabel1.Text = "To see current pixel coordinates load texture";
            }
        }

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            if (listView1.Items.Count > 0)
            {
                float _scaleX = ((float)targetWidth / sourceWidth) * (float)scaleX;
                float _scaleY = ((float)targetHeight / sourceHeight) * (float)scaleY;

                double w = (((double)sourceWidth / scaleX) * ((double)e.X / targetWidth));
                double h = (((double)sourceHeight / scaleY) * ((double)e.Y / targetHeight));


                int x = (imagePos.X + (int)w);
                int y = (imagePos.Y + (int)h);
                Point mousePixelizedPosition = new Point(x, y);

                foreach (ListViewItem item in listView1.Items)
                {
                    Rectangle rect = new Rectangle(Convert.ToInt32(item.SubItems[1].Text.ToString()), Convert.ToInt32(item.SubItems[2].Text.ToString()), Convert.ToInt32(item.SubItems[3].Text.ToString()), Convert.ToInt32(item.SubItems[4].Text.ToString()));
                    if(rect.Contains(mousePixelizedPosition))
                    {
                        listView1.Focus();
                        item.Selected = true;
                        listView1.Refresh();
                        break;
                    }
                }
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!CanCloseWithoutSaving())
            {
                if (MessageBox.Show("Do you really want to close FontMaker? Unsaved changes won't be saved!", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No)
                {
                    e.Cancel = true;
                }
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }
    }
}
