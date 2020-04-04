using System.Drawing;
using System.Windows.Forms;

namespace Yatool
{

    /// <summary>
    /// 画像関係クラス
    /// </summary>
    /// <remarks>
    /// AA関係の画像関係を処理するためのクラスです。
    /// </remarks>
    public class Graphics
    {

        //-----------------------------
        // 関数定義
        //-----------------------------
        /// <summary>
        /// 標準フォント生成
        /// </summary>
        /// <returns>AA表示に合わせたデフォルトフォント。フォントが存在しない場合はnull</returns>
        /// <remarks>
        /// AA表示に合わせた標準フォントを生成して返します。
        /// </remarks>
        public static Font CreateDefaultFont( )
        {
            return new Font( "ＭＳ Ｐゴシック", 16, FontStyle.Regular, GraphicsUnit.Pixel );
        }


        //-----------------------------
        // ページ画像化関係
        //-----------------------------
        /// <summary>
        /// ページ画像化
        /// </summary>
        /// <param name="page">画像化するページデータ</param>
        /// <returns>画像化されたページデータ</returns>
        /// <remarks>
        /// AAページを画像化します。
        /// </remarks>
        public static Bitmap PageToBitmap( AAPage page )
        {
            return PageToBitmap( page, true, null, Color.Black, Color.White );
        }

        /// <summary>
        /// ページ画像化
        /// </summary>
        /// <param name="page">画像化するページデータ</param>
        /// <param name="textColor">描画文字色</param>
        /// <param name="backgroundColor">背景色</param>
        /// <returns>画像化されたページデータ</returns>
        /// <remarks>
        /// AAページを画像化します。
        /// </remarks>
        public static Bitmap PageToBitmap( AAPage page, Color textColor, Color backgroundColor )
        {
            return PageToBitmap( page, true, null, textColor, backgroundColor );
        }

        /// <summary>
        /// ページ画像化
        /// </summary>
        /// <param name="page">画像化するページデータ</param>
        /// <param name="font">文字列を描画するフォント。nullの場合は標準フォント</param>
        /// <param name="textColor">描画文字色</param>
        /// <param name="backgroundColor">背景色</param>
        /// <returns>画像化されたページデータ</returns>
        /// <remarks>
        /// AAページを画像化します。
        /// </remarks>
        public static Bitmap PageToBitmap( AAPage page, Font font, Color textColor, Color backgroundColor )
        {
            return PageToBitmap( page, true, font, textColor, backgroundColor );
        }

        /// <summary>
        /// ページ画像化
        /// </summary>
        /// <param name="page">画像化するページデータ</param>
        /// <param name="isDecode">HTML文字参照を解決するならtrue、それ以外はfalse</param>
        /// <param name="font">文字列を描画するフォント。nullの場合は標準フォント</param>
        /// <param name="textColor">描画文字色</param>
        /// <param name="backgroundColor">背景色</param>
        /// <returns>画像化されたページデータ</returns>
        /// <remarks>
        /// AAページを画像化します。
        /// </remarks>
        public static Bitmap PageToBitmap( AAPage page, bool isDecode, Font font, Color textColor, Color backgroundColor )
        {
            return PageToBitmap( (isDecode ? page.DecodedText : page.Text), font, textColor, backgroundColor );
        }

        /// <summary>
        /// ページ画像化
        /// </summary>
        /// <param name="text">画像化する文字列</param>
        /// <param name="font">文字列を描画するフォント。nullの場合は標準フォント</param>
        /// <param name="textColor">描画文字色</param>
        /// <param name="backgroundColor">背景色</param>
        /// <returns>画像化されたページデータ</returns>
        /// <remarks>
        /// AAページを画像化します。
        /// </remarks>
        public static Bitmap PageToBitmap( string text, Font font, Color textColor, Color backgroundColor )
        {
            /*---- フォントチェック ----*/
            if( font == null )
            {
                font = CreateDefaultFont( );
            }


            /*---- 描画範囲特定 ----*/
            var size = TextRenderer.MeasureText( text, font );


            /*---- Bitmap書き込み ----*/
            var image = new Bitmap( size.Width, size.Height );
            using( var graphics = System.Drawing.Graphics.FromImage( image ) )
            {
                using( var brush = new SolidBrush( backgroundColor ) )
                {
                    graphics.FillRectangle( brush, 0, 0, size.Width, size.Height );
                }
                TextRenderer.DrawText( graphics, text, font, Point.Empty, textColor, TextFormatFlags.NoPrefix );
            }

            return image;
        }

    }

}
