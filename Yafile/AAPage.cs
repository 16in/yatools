using System.Web;

namespace Yatool
{

    /// <summary>
    /// アスキーアートページ
    /// </summary>
    /// <remarks>
    /// アスキーアートファイルのページ単位管理クラスです。
    /// </remarks>
    public class AAPage
    {

        //-----------------------------
        // プロパティ
        //-----------------------------
        /// <summary>ページタイトル</summary>
        public string Title { get; set; } = string.Empty;

        /// <summary>ページ内容</summary>
        public string Text { get; set; } = string.Empty;

        /// <summary>参照解決済みページ内容</summary>
        public string DecodedText
        {
            get { return HttpUtility.HtmlDecode( Text ); }
        }


        //-----------------------------
        // 関数定義
        //-----------------------------
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <remarks>
        /// デフォルトコンストラクタです。
        /// </remarks>
        public AAPage( ) : this( string.Empty, string.Empty )
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="text">ページ内容テキスト</param>
        /// <remarks>
        /// ページ内容のみを初期化するコンストラクタです。
        /// </remarks>
        public AAPage( string text ) : this( string.Empty, text )
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="title">ページタイトル</param>
        /// <param name="text">ページ内容テキスト</param>
        /// <remarks>
        /// タイトルと内容を初期化するコンストラクタです。
        /// </remarks>
        public AAPage( string title, string text )
        {
            Title = title == null ? string.Empty : title;
            Text = text == null ? string.Empty : text;
        }

    }

}
