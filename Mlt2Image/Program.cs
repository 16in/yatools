using Yatool;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

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
            try
            {
                EntryPoint( args );
            }
            catch( System.Exception e )
            {
                new ThreadExceptionDialog( e ).ShowDialog( );
                var path = Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "error.txt" );
                File.WriteAllText( path, e.ToString( ) );
            }
        }

        /// <summary>
        /// エントリポイント
        /// </summary>
        /// <param name="args">コマンドライン引数</param>
        /// <remarks>
        /// プログラムのエントリポイントです。
        /// </remarks>
        public static void EntryPoint( string[ ] args )
        {
            /*---- オプション生成 ----*/
            string optionPath = Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "mlt2image.config.xml" );
            var option = Option.Read( optionPath );
            if( option == null )
            {
                option = new Option( );
                option.Save( optionPath );
            }
            bool isHelp = !option.Parse( args );
            if( isHelp || option.InputPaths.Count <= 0 )
            {
                PrintHelp( );
                return;
            }


            /*---- フォント生成 ----*/
            Font font = null;
            if( !string.IsNullOrWhiteSpace( option.FontName ) )
            {
                font = new Font( option.FontName,  option.FontSize, FontStyle.Regular, GraphicsUnit.Point );
            }


            /*---- ファイルをすべて表示 ----*/
            string outputRoot = option.OutputDirectory;
            string[ ] paths = option.InputPaths.ToArray( );
            foreach( var path in paths )
            {
                try
                {
                    /* 出力先生成 */
                    string output = string.Empty;
                    if( option.IsCreateSubDiretory )
                    {
                        output = Path.Combine( outputRoot == null ? Path.GetDirectoryName( path ) : outputRoot, Path.GetFileNameWithoutExtension( path ) );
                    }
                    else
                    {
                        output = outputRoot == null ? Path.GetDirectoryName( path ) : outputRoot;
                    }
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
                catch( System.Exception e )
                {
                    System.Console.Error.WriteLine( "出力に失敗しました。\n[{0}]", path );

                    new ThreadExceptionDialog( e ).ShowDialog( );
                    File.WriteAllText( Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "error.txt" ), e.ToString( ) );
                }
            }
        }

        /// <summary>
        /// ヘルプ表示
        /// </summary>
        /// <remarks>
        /// ヘルプを表示します。
        /// </remarks>
        public static void PrintHelp( )
        {
            var helpPath = Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "help.txt" );
            if( File.Exists( helpPath ) )
            {
                var help = File.ReadAllText( helpPath, System.Text.Encoding.UTF8 );
                System.Console.Error.WriteLine( help );
            }
        }

    }
}
