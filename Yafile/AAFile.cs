using System.IO;
using System.Text;
using System.Collections.Generic;

namespace Yatool
{

    /// <summary>
    /// アスキーアートファイル
    /// </summary>
    /// <remarks>
    /// アスキーアート関係のファイル管理クラスです。
    /// </remarks>
    public class AAFile
    {

        //-----------------------------
        // 列挙体
        //-----------------------------
        /// <summary>
        /// AAファイル・タイプ
        /// </summary>
        /// <remarks>
        /// アスキーアート関係のファイルタイプです。
        /// </remarks>
        public enum FileType
        {
            /// <summary>プレーンテキスト</summary>
            PlaneText = 0x00,

            /// <summary>MLT</summary>
            MLT = 0x01,

            /// <summary>AST</summary>
            AST = 0x02,

            /// <summary>ASD</summary>
            ASD = 0x03,
        }


        //-----------------------------
        // プロパティ
        //-----------------------------
        /// <summary>ページ一覧</summary>
        public AAPage[ ] Pages
        {
            get { return m_Pages.ToArray( ); }
            set { m_Pages = new List<AAPage>( value ); }
        }


        //-----------------------------
        // コンストラクタ
        //-----------------------------
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <remarks>
        /// デフォルトコンストラクタです。
        /// </remarks>
        public AAFile( )
        {
        }


        //-----------------------------
        // ページ操作
        //-----------------------------
        /// <summary>
        /// ページ追加
        /// </summary>
        /// <param name="page">追加するページ</param>
        /// <remarks>
        /// ページを追加します。
        /// </remarks>
        public void AddPage( AAPage page )
        {
            m_Pages.Add( page );
        }

        /// <summary>
        /// ページ追加
        /// </summary>
        /// <param name="pages">追加するページ一覧</param>
        /// <exception cref="System.ArgumentNullException">pagesがnull</exception>
        /// <remarks>
        /// ページを追加します。
        /// </remarks>
        public void AddPages( IEnumerable<AAPage> pages )
        {
            m_Pages.AddRange( pages );
        }

        /// <summary>
        /// ページ挿入
        /// </summary>
        /// <param name="index">挿入するインデックス</param>
        /// <param name="page">挿入するページ</param>
        /// <exception cref="System.ArgumentOutOfRangeException">indexが範囲外であった</exception>
        /// <remarks>
        /// ページを挿入します。      \n
        /// indexが末尾を超えていた場合は失敗します。
        /// </remarks>
        public void InsertPage( int index, AAPage page )
        {
            m_Pages.Insert( index, page );
        }

        /// <summary>
        /// ページ挿入
        /// </summary>
        /// <param name="index">挿入するインデックス</param>
        /// <param name="pages">挿入するページ一覧</param>
        /// <exception cref="System.ArgumentNullException">pagesがnull</exception>
        /// <exception cref="System.ArgumentOutOfRangeException">indexが範囲外であった</exception>
        /// <remarks>
        /// ページを挿入します。      \n
        /// indexが末尾を超えていた場合は失敗します。
        /// </remarks>
        public void InsertPages( int index, IEnumerable<AAPage> pages )
        {
            m_Pages.InsertRange( index, pages );
        }

        /// <summary>
        /// ページ削除
        /// </summary>
        /// <param name="index">削除するインデックス</param>
        /// <exception cref="System.ArgumentOutOfRangeException">indexが範囲外であった</exception>
        /// <remarks>
        /// ページを削除します。
        /// </remarks>
        public void DeletePage( int index )
        {
            m_Pages.RemoveAt( index );
        }

        /// <summary>
        /// ページ削除
        /// </summary>
        /// <param name="page">削除するページ</param>
        /// <returns>削除に成功したらtrue、それ以外はfalse</returns>
        /// <remarks>
        /// ページを削除します。
        /// </remarks>
        public bool DeletePage( AAPage page )
        {
            return m_Pages.Remove( page );
        }

        /// <summary>
        /// 全ページ削除
        /// </summary>
        /// <remarks>
        /// すべてのページを削除します。
        /// </remarks>
        public void DeleteAll( )
        {
            m_Pages.Clear( );
        }


        //-----------------------------
        // ファイル操作
        //-----------------------------
        /// <summary>
        /// 書き出し
        /// </summary>
        /// <param name="type">ファイルタイプ</param>
        /// <param name="path">ファイルパス</param>
        /// <remarks>
        /// ファイルへ書き出します。
        /// </remarks>
        public void WriteFile( FileType type, string path )
        {
            using( var stream = new FileStream( path, FileMode.OpenOrCreate, FileAccess.Write ) )
            {
                WriteFile( type, stream );
            }
        }

        /// <summary>
        /// 書き出し
        /// </summary>
        /// <param name="type">ファイルタイプ</param>
        /// <param name="stream">書き出し先ストリーム</param>
        /// <param name="encoding">文字コード。nullの場合はshift-jis。失敗時はutf-8</param>
        /// <remarks>
        /// ファイルへ書き出します。
        /// </remarks>
        public void WriteFile( FileType type, Stream stream, Encoding encoding = null )
        {
            if( encoding == null )
            {
                try
                {
                    encoding = Encoding.GetEncoding( 932 );
                }
                catch( System.NotSupportedException )
                {
                    encoding = Encoding.UTF8;
                }
            }
            using( var writer = new StreamWriter( stream, encoding ) )
            {
                int pageNumber = 0;
                foreach( var page in m_Pages )
                {
                    if( type == FileType.MLT )
                    {
                        if( pageNumber == (m_Pages.Count - 1) )
                        {
                            writer.Write( page.Text.Replace( "\n", "\r\n" ) );
                        }
                        else
                        {
                            writer.Write( "{0}\r\n[SPLIT]\r\n", page.Text.Replace( "\n", "\r\n" ) );
                        }
                    }
                    else if( type == FileType.AST )
                    {
                        writer.Write( "[AA][{0}]\r\n{1}\r\n", page.Title, page.Text.Replace( "\n", "\r\n" ) );
                    }
                    else if( type == FileType.ASD )
                    {
                        writer.Write( "{1}\x2\x2{0}\r\n", page.Title, page.Text.Replace( "\n", "\x1\x1" ) );
                    }
                    else
                    {
                        writer.Write( "{0}", page.Text.Replace( "\n", "\r\n" ) );
                    }
                    pageNumber++;
                }
            }
        }

        /// <summary>
        /// ファイルタイプチェック
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <param name="encoding">文字コード。nullの場合はshift-jis。失敗時はutf-8</param>
        /// <returns>ファイルタイプ</returns>
        /// <remarks>
        /// 指定されたファイルのファイルタイプを取得します。
        /// </remarks>
        public static FileType CheckFileType( string path, Encoding encoding = null )
        {
            FileType type = FileType.PlaneText;
            if( encoding == null )
            {
                try
                {
                    encoding = Encoding.GetEncoding( 932 );
                }
                catch( System.NotSupportedException )
                {
                    encoding = Encoding.UTF8;
                }
            }
            using( var stream = new FileStream( path, FileMode.Open, FileAccess.Read ) )
            {
                type = CheckFileType( stream, encoding );
            }
            return type;
        }

        /// <summary>
        /// ファイルタイプチェック
        /// </summary>
        /// <param name="stream">ストリーム</param>
        /// <param name="encoding">文字コード。nullの場合はshift-jis。失敗時はutf-8</param>
        /// <returns>ファイルタイプ</returns>
        /// <remarks>
        /// 指定されたストリームのファイルタイプを取得します。
        /// </remarks>
        public static FileType CheckFileType( Stream stream, Encoding encoding = null )
        {
            FileType type = FileType.PlaneText;
            if( encoding == null )
            {
                try
                {
                    encoding = Encoding.GetEncoding( 932 );
                }
                catch( System.NotSupportedException )
                {
                    encoding = Encoding.UTF8;
                }
            }
            using( var reader = new StreamReader( stream, encoding ) )
            {
                do
                {
                    var line = reader.ReadLine( );
                    if( line == null )
                    {
                        break;
                    }
                    else if( line.IndexOf( "\x2\x2" ) >= 0 )
                    {
                        type = FileType.ASD;
                        break;
                    }
                    if( line.Length > 0 && line[ 0 ] == '[' )
                    {
                        if( line.Contains( "[SPLIT]" ) )
                        {
                            type = FileType.MLT;
                            break;
                        }
                        else if( line.StartsWith( "[AA]" ) )
                        {
                            type = FileType.AST;
                            break;
                        }
                    }
                } while( true );
            }
            return type;
        }

        /// <summary>
        /// 読み込み
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <returns>読み込まれたファイル</returns>
        /// <remarks>
        /// ファイルを読み込みます。
        /// </remarks>
        public static AAFile ReadFile( string path )
        {
            return ReadFile( path, null );
        }

        /// <summary>
        /// 読み込み
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <param name="encoding">文字コード。nullの場合はshift-jis。失敗時はutf-8</param>
        /// <returns>読み込まれたファイル</returns>
        /// <remarks>
        /// ファイルを読み込みます。
        /// </remarks>
        public static AAFile ReadFile( string path, Encoding encoding )
        {
            if( encoding == null )
            {
                try
                {
                    encoding = Encoding.GetEncoding( 932 );
                }
                catch( System.NotSupportedException )
                {
                    encoding = Encoding.UTF8;
                }
            }

            AAFile file = null;
            var fileType = CheckFileType( path, encoding );
            using( var stream = new FileStream( path, FileMode.Open, FileAccess.Read ) )
            {
                file = ReadFile( fileType, stream, encoding );
            }
            return file;
        }

        /// <summary>
        /// 読み込み
        /// </summary>
        /// <param name="fileType">ファイルタイプ</param>
        /// <param name="stream">ストリーム</param>
        /// <param name="encoding">エンコード</param>
        /// <returns>読み込まれたAAFile</returns>
        /// <remarks>
        /// ストリームからファイルを読み込みます。
        /// </remarks>
        public static AAFile ReadFile( FileType fileType, Stream stream, Encoding encoding )
        {
            if( encoding == null )
            {
                try
                {
                    encoding = Encoding.GetEncoding( 932 );
                }
                catch( System.NotSupportedException )
                {
                    encoding = Encoding.UTF8;
                }
            }

            AAFile file = null;
            if( fileType == FileType.PlaneText )
            {
                using( var reader = new StreamReader( stream, encoding ) )
                {
                    file = new AAFile( );
                    file.AddPage( new AAPage( reader.ReadToEnd( ).Replace( "\r\n", "\n" ) ) );
                }
            }
            else if( fileType == FileType.MLT )
            {
                file = ReadMltFile( stream, encoding );
            }
            else if( fileType == FileType.AST )
            {
                file = ReadAstFile( stream, encoding );
            }
            else if( fileType == FileType.ASD )
            {
                file = ReadAsdFile( stream, encoding );
            }
            return file;
        }

        /// <summary>
        /// MLTファイル読み込み
        /// </summary>
        /// <param name="stream">ストリーム</param>
        /// <param name="encoding">文字エンコード</param>
        /// <returns>読み込まれたファイル</returns>
        /// <remarks>
        /// ストリームからMLTファイルを読み込みます。
        /// </remarks>
        private static AAFile ReadMltFile( Stream stream, Encoding encoding )
        {
            AAFile file = null;
            using( var reader = new StreamReader( stream, encoding ) )
            {
                file = new AAFile( );
                string page = null;
                string line = null;
                while( (line = reader.ReadLine( )) != null )
                {
                    if( line.Equals( "[SPLIT]" ) )
                    {
                        file.AddPage( new AAPage( page ) );
                        page = null;
                    }
                    else if( page == null )
                    {
                        page = line;
                    }
                    else
                    {
                        page += "\n" + line;
                    }
                }
                if( page != null )
                {
                    file.AddPage( new AAPage( page ) );
                }
            }
            return file;
        }

        /// <summary>
        /// ASTファイル読み込み
        /// </summary>
        /// <param name="stream">ストリーム</param>
        /// <param name="encoding">文字エンコード</param>
        /// <returns>読み込まれたファイル</returns>
        /// <remarks>
        /// ストリームからASTファイルを読み込みます。
        /// </remarks>
        private static AAFile ReadAstFile( Stream stream, Encoding encoding )
        {
            AAFile file = null;
            using( var reader = new StreamReader( stream, encoding ) )
            {
                file = new AAFile( );
                string page = null, title = null;
                string line = null;
                while( (line = reader.ReadLine( )) != null )
                {
                    if( line.StartsWith( "[AA][" ) )
                    {
                        if( page != null && title != null )
                        {
                            file.AddPage( new AAPage( title, page == null ? string.Empty : page ) );
                        }

                        var titleEnd = line.IndexOf( ']', 5 );
                        title = line.Substring( 5, titleEnd - 5 );
                        page = null;
                    }
                    else if( page == null )
                    {
                        page = line;
                    }
                    else
                    {
                        page += "\n" + line;
                    }
                }
                if( page != null || title != null )
                {
                    file.AddPage( new AAPage( title, page == null ? string.Empty : page ) );
                }
            }
            return file;
        }

        /// <summary>
        /// ASDファイル読み込み
        /// </summary>
        /// <param name="stream">ストリーム</param>
        /// <param name="encoding">文字エンコード</param>
        /// <returns>読み込まれたファイル</returns>
        /// <remarks>
        /// ストリームからASDファイルを読み込みます。
        /// </remarks>
        private static AAFile ReadAsdFile( Stream stream, Encoding encoding )
        {
            AAFile file = null;
            using( var reader = new StreamReader( stream, encoding ) )
            {
                file = new AAFile( );
                string line = null;
                while( (line = reader.ReadLine( )) != null )
                {
                    var split = line.IndexOf( "\x2\x2" );
                    if( split >= 0 )
                    {
                        var page = line.Substring( 0, split );
                        var title = line.Substring( split + 2 );
                        file.AddPage( new AAPage( title, page.Replace( "\x1\x1", "\n" ) ) );
                    }
                }
            }
            return file;
        }


        //-----------------------------
        // 非公開メンバ
        //-----------------------------
        /// <summary>ページリスト</summary>
        protected List<AAPage> m_Pages = new List<AAPage>( );

    }

}
