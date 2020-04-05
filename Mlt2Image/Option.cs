using System;
using System.IO;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;

namespace Mlt2Image
{

    /// <summary>
    /// オプション
    /// </summary>
    /// <remarks>
    /// Mlt2Imageのオプション設定です。
    /// </remarks>
    [Serializable, XmlRoot( "Mlt2Image" )]
    public class Option
    {

        //-----------------------------
        // プロパティ
        //-----------------------------
        /// <summary>背景色</summary>
        [XmlIgnore]
        public Color BackGroundColor { get; set; }

        /// <summary>文字色</summary>
        [XmlIgnore]
        public Color TextColor { get; set; }

        /// <summary>フォーマット</summary>
        [XmlIgnore]
        public ImageFormat Format { get; set; }

        /// <summary>出力拡張子</summary>
        [XmlIgnore]
        public string Extension { get; set; }


        //-----------------------------
        // 公開フィールド
        //-----------------------------
        /// <summary>入力ファイル一覧</summary>
        [XmlIgnore]
        public List<string> InputPaths = new List<string>( );

        /// <summary>出力先ディレクトリ</summary>
        [XmlElement( "OutputDirectory" )]
        public string OutputDirectory = null;

        /// <summary>出力フォーマット</summary>
        [XmlElement( "OutputFormat" )]
        public string OutputFormat = "png";

        /// <summary>背景色</summary>
        /// <remarks>#RRGGBB,#RRGGBBAA,背景色文字列の何れかの形式が有効です。</remarks>
        [XmlElement( "BackGroundColor" )]
        public string BackGroundColorText = "#ffffff";

        /// <summary>文字色</summary>
        /// <remarks>#RRGGBB,#RRGGBBAA,背景色文字列の何れかの形式が有効です。</remarks>
        [XmlElement( "TextColor" )]
        public string TextColorText = "#000000";

        /// <summary>フォント名</summary>
        [XmlElement( "FontName" )]
        public string FontName = "ＭＳ Ｐゴシック";

        /// <summary>フォントサイズ</summary>
        /// <remarks>フォントのポイント数です。</remarks>
        [XmlElement( "FontSize" )]
        public float FontSize = 12.0f;

        /// <summary>サブディレクトリ生成フラグ</summary>
        [XmlElement( "IsCreateSubDiretory" )]
        public bool IsCreateSubDiretory = true;


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

            // 読み取れていたら初期設定
            if( option != null )
            {
                option.Setup( );
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
            Setup( );
        }

        /// <summary>
        /// 設定反映
        /// </summary>
        /// <remarks>
        /// オプション設定を反映します。
        /// </remarks>
        public void Setup( )
        {
            // カラー変換
            try
            {
                BackGroundColor = ColorTranslator.FromHtml( BackGroundColorText );
            }
            catch( Exception )
            {
                BackGroundColor = Color.White;
            }
            try
            {
                TextColor = ColorTranslator.FromHtml( TextColorText );
            }
            catch( Exception )
            {
                BackGroundColor = Color.Black;
            }

            // フォーマット
            var format = OutputFormat == null ? "png" : OutputFormat.Trim( ).ToLower( );
            if( format.Equals( "png" ) )
            {
                Format = ImageFormat.Png;
                Extension = "png";
            }
            else if( format.Equals( "jpeg" ) || format.Equals( "jpg" ) )
            {
                Format = ImageFormat.Jpeg;
                Extension = "jpg";
            }
            else if( format.Equals( "bmp" ) )
            {
                Format = ImageFormat.Bmp;
                Extension = "bmp";
            }
            else if( format.Equals( "gif" ) )
            {
                Format = ImageFormat.Gif;
                Extension = "gif";
            }
            else if( format.Equals( "tiff" ) )
            {
                Format = ImageFormat.Tiff;
                Extension = "tiff";
            }
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

        /// <summary>
        /// パース
        /// </summary>
        /// <param name="options">オプションリスト</param>
        /// <returns>ヘルプ表示を行う場合はfalse、それ以外はtrue</returns>
        /// <remarks>
        /// オプション一覧をパースします。
        /// </remarks>
        public bool Parse( string[] options )
        {
            bool isHelp = false;
            try
            {
                for( int i = 0; i < options.Length; i++ )
                {
                    var op = options[ i ];
                    if( !string.IsNullOrWhiteSpace( op ) )
                    {
                        if( op[ 0 ] != '-' )
                        {
                            InputPaths.Add( op );
                        }
                        else
                        {
                            op = op.ToLower( );
                            switch( op )
                            {
                            case "-o": case "--output": OutputDirectory = options[ ++i ]; break;
                            case "-f": case "--format": OutputFormat = options[ ++i ]; break;
                            case "-b": case "--bgcolor": BackGroundColorText = options[ ++i ]; break;
                            case "-t": case "--textcolor": TextColorText = options[ ++i ]; break;
                            case "-s": case "--subdirectoy": IsCreateSubDiretory = true; break;
                            case "-ds": case "--disable-subdirectoy": IsCreateSubDiretory = false; break;
                            case "-fn": case "--font-name": FontName = options[ ++i ]; break;
                            case "-fs": case "--font-size": FontSize = float.Parse( options[ ++i ] ); break;
                            case "-h": case "--help": isHelp = true; break;
                            }
                        }
                    }
                }
                Setup( );
            }
            catch( Exception )
            {
                isHelp = true;
            }
            return !isHelp;
        }

    }

}
