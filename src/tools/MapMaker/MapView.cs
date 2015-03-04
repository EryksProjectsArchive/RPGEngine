using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapMaker
{
    class MapView : UserControl
    {
        public float scale = 1.0f;
        private TileInfo[,] tiles = null;
        private int tileBasedWidth = 0;
        private int tileBasedHeight = 0;
        private int tileSize = 0;

        public void AddTile(int x, int y, TileInfo tile)
        {
            tiles[x, y] = tile;
        }

        public void RemoveTile(int x, int y)
        {
            tiles[x, y] = null;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //e.Graphics.PixelOffsetMode = PixelOffsetMode.Half;
            
            e.Graphics.FillRectangle(new SolidBrush(this.BackColor), new Rectangle(0, 0, this.Width, this.Height));
            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(50, 0, 0, 0), 1), new Rectangle(0, 0, this.Width-1, this.Height-1));

            int scaledTileSize = (int)((float)tileSize / this.scale);
            if(this.tiles != null)
            {
                Rectangle rectangle = new Rectangle(0, 0, scaledTileSize, scaledTileSize);
                for (int y = 0; y < tileBasedHeight; y++)
                {
                    for (int x = 0; x < tileBasedWidth; x++)
                    {
                        if(this.tiles[x, y] != null)
                        {
                            Image image = null;
                            if((image = this.tiles[x, y].GetImage()) != null)
                            {
                                e.Graphics.DrawImage(image, rectangle);
                            }
                        }
                        rectangle.X += scaledTileSize;                       
                    }
                    rectangle.Y += scaledTileSize;
                    rectangle.X = 0;
                }
            }
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // MapView
            // 
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.Name = "MapView";
            this.Size = new System.Drawing.Size(708, 559);
            this.ResumeLayout(false);            
        }

        private void Setup(int tileSize)
        {
            this.tileSize = tileSize;
            tileBasedWidth = this.Width / tileSize;
            tileBasedHeight = this.Height / tileSize;
            this.tiles = new TileInfo[tileBasedWidth, tileBasedHeight];
            for(int x = 0; x < tileBasedWidth; x++)
            {
                for(int y = 0; y < tileBasedHeight; y++)
                {
                    this.tiles[x, y] = null;
                }
            }
        }
    }
}
