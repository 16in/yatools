using Yatool;
using System.IO;

namespace Mlt2Html
{

    /// <summary>
    /// MLT2HTML
    /// </summary>
    /// <remarks>
    /// MLT2HTMLのエントリーポイントです。
    /// </remarks>
    public class Program
    {

        /// <summary>
        /// エントリポイント
        /// </summary>
        /// <param name="args">コマンドライン引数</param>
        /// <remarks>
        /// エントリポイントです。
        /// </remarks>
        static void Main( string[ ] args )
        {
            /*---- オプション生成 ----*/
            string optionPath = Path.Combine( System.AppDomain.CurrentDomain.BaseDirectory, "mlt2html.config.xml" );
            var option = Option.Read( optionPath );
            if( option == null )
            {
                option = new Option( );
                option.Save( optionPath );
            }
        }

    }

}
