using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapMaker
{
    class TileInfo
    {
        private Image image = null;
        private UInt32 width = 0;
        private UInt32 height = 0;

        public TileInfo(Image sourceImage, Rectangle sourceRect, UInt32 width, UInt32 height)
        {
            this.image = null;
            this.width = 0u;
            this.height = 0u;

            MakeImage(sourceImage, sourceRect, width, height);
        }

        private void MakeImage(Image sourceImage, Rectangle sourceRect, UInt32 width, UInt32 height)
        {
            // TODO
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
