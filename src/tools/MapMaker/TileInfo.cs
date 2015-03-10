using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapMaker
{
    class TileInfo
    {
        private Bitmap image = null;
        private UInt32 width = 0;
        private UInt32 height = 0;

        public TileInfo(Image sourceImage, Rectangle sourceRect, UInt32 width, UInt32 height)
        {
            this.image = null;
            this.width = width;
            this.height = height;

            MakeImage(sourceImage, sourceRect, width, height);
        }

        private void MakeImage(Image sourceImage, Rectangle sourceRect, UInt32 width, UInt32 height)
        {
            if(image != null)
            {
                image.Dispose();
            }
            image = new Bitmap((int)width, (int)height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            image.SetResolution(sourceImage.HorizontalResolution, sourceImage.VerticalResolution);

            Graphics graphics = Graphics.FromImage(image);

            graphics.InterpolationMode = InterpolationMode.NearestNeighbor;

            graphics.Clear(Color.Black);
            graphics.DrawImage(sourceImage, new Rectangle(0, 0, (int)width, (int)height), sourceRect, GraphicsUnit.Pixel);
            graphics.Dispose();
        }

        public Image GetImage()
        {
            return image;
        }

        public Point GetImageSize()
        {            
            return new Point((int)this.width, (int)this.height);
        }
    }
}
