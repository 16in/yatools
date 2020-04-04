using Yatool;
using System.IO;
using System.Drawing;

namespace Mlt2Image
{

    /// <summary>
    /// Mlt2Image
    /// </summary>
    /// <remarks>
    /// Mlt2Imageのエントリーポイントです。
    /// </remarks>
    public class Mlt2Image
    {
        
        /// <summary>
        /// エントリポイント
        /// </summary>
        /// <param name="args">コマンドライン引数</param>
        /// <remarks>
        /// プログラムのエントリポイントです。
        /// </remarks>
        public static void Main( string[ ] args )
        {
            /*---- オプション生成 ----*/
            string optionPath = Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "mlt2image.config.xml" );
            var option = Option.Read( optionPath );
            if( option == null )
            {
                option = new Option( );
                option.Save( optionPath );
            }


            /*---- フォント生成 ----*/
            Font font = null;
            if( !string.IsNullOrWhiteSpace( option.FontName ) )
            {
                font = new Font( option.FontName,  option.FontSize, FontStyle.Regular, GraphicsUnit.Point );
            }


            /*---- ファイルをすべて表示 ----*/
            string outputRoot = option.OutputDirectory;
            string[ ] paths = args;
            foreach( var path in paths )
            {
                try
                {
                    /* 出力先生成 */
                    string output = Path.Combine( outputRoot == null ? Path.GetDirectoryName( path ) : outputRoot, Path.GetFileNameWithoutExtension( path ) );
                    if( !Directory.Exists( output ) )
                    {
                        Directory.CreateDirectory( output );
                    }


                    /* ページごとに出力する */
                    var file = AAFile.ReadFile( path );
                    for( int count = 0; count < file.Pages.Length; count++ )
                    {
                        var page = file.Pages[ count ];
                        var bmp = Yatool.Graphics.PageToBitmap( page, font, option.TextColor, option.BackGroundColor );

                        string outPath = Path.Combine( output, string.Format( "{0}.{1}", count + 1, option.Extension ) );
                        bmp.Save( outPath, option.Format );
                    }
                }
                catch( System.Exception )
                {
                    System.Console.Error.WriteLine( "出力に失敗しました。\n[{0}]", path );
                }
            }
        }

    }
}
