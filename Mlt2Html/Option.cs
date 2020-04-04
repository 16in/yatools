using System;
using System.IO;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace Mlt2Html
{

    /// <summary>
    /// オプション
    /// </summary>
    /// <remarks>
    /// MLT2HTMLのオプション構造です。
    /// </remarks>
    [Serializable, XmlRoot( "Mlt2Html" )]
    public class Option
    {

        //-----------------------------
        // 公開フィールド
        //-----------------------------
        /// <summary>入力ファイル一覧</summary>
        [XmlIgnore]
        public List<string> InputPaths = new List<string>( );

        /// <summary>出力先ディレクトリ</summary>
        [XmlElement( "OutputDirectory" )]
        public string OutputPath = null;

        /// <summary>使用するテンプレート</summary>
        [XmlElement( "Template" )]
        public string Template = "${ApplicationDirectory}/template/";


        //-----------------------------
        // 静的関数
        //-----------------------------
        /// <summary>
        /// オプション読み込み
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <returns>読み込まれたオプション。読み込みに失敗した場合はnull</returns>
        /// <remarks>
        /// オプション設定ファイルを読み込みます。
        /// </remarks>
        public static Option Read( string path )
        {
            Option option = null;
            if( File.Exists( path ) )
            {
                try
                {
                    var serializer = new XmlSerializer( typeof( Option ) );
                    using( var stream = new FileStream( path, FileMode.Open, FileAccess.Read ) )
                    {
                        option = (Option)serializer.Deserialize( stream );
                    }
                }
                catch( Exception )
                {
                    option = null;
                }
            }

            return option;
        }


        //-----------------------------
        // 関数定義
        //-----------------------------
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <remarks>
        /// コンストラクタです。
        /// </remarks>
        public Option( )
        {
        }

        /// <summary>
        /// オプション保存
        /// </summary>
        /// <param name="path">保存先ファイルパス</param>
        /// <remarks>
        /// オプションファイルを保存します。
        /// </remarks>
        public void Save( string path )
        {
            var serializer = new XmlSerializer( typeof( Option ) );
            using( var stream = new FileStream( path, FileMode.Create, FileAccess.Write ) )
            {
                serializer.Serialize( stream, this );
            }
        }

    }
}
