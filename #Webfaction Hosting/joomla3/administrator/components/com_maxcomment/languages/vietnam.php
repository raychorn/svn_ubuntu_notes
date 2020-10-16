﻿<?php
/*********************************************
* mXcomment - Component                      *
* Copyright (C) 2007-2008 by Bernard Gilly   *
* --------- All Rights Reserved ------------ *
* Homepage   : www.visualclinic.fr           *
* Version    : 1.0.7                         *
* License    : Creative Commons              *
*********************************************/

// ensure this file is being included by a parent file
defined( '_VALID_MOS' ) or die( 'Direct Access to this location is not allowed.' );

DEFINE("_MXC_CPL_CONFIG","Cấu hình");
DEFINE("_MXC_CPL_ADM_COMMENTS","Ý kiến người biên soạn");
DEFINE("_MXC_CPL_USER_COMMENTS","Ý kiến người xem");
DEFINE("_MXC_CPL_FAVOURED","Ưa thích");
DEFINE("_MXC_CPL_EDIT_CSS_FILE","Chỉnh tập tin CSS");
DEFINE("_MXC_CPL_EDIT_LANGUAGE_FILE","Chỉnh ngôn ngữ");
DEFINE("_MXC_CPL_BAD_WORDS","Quản lý các từ có nghĩa xấu");
DEFINE("_MXC_CPL_SUPPORT_WEBSITE","Website hỗ trợ");
DEFINE("_MXC_CPL_ABOUT","Giới thiệu");
DEFINE("_MXC_MSG_IMPORT_SUCCESS","Hoàn tất chuyển dữ liệu từ AkoComment Tweaked Special Edition!");
DEFINE("_MXC_MSG_IMPORT_ERROR","Có lỗi khi chuyển dữ liệu từ AkoComment Tweaked Special Edition!");
DEFINE("_MXC_CONTROLPANEL","Bảng điều khiển");
DEFINE("_MXC_COMMENTS","Ý kiến");
DEFINE("_MXC_COMMENT","Ý kiến");
DEFINE("_MXC_EDIT","Sửa");
DEFINE("_MXC_FILTER","Bộ lọc");
DEFINE("_MXC_AUTHOR","Tác giả");
DEFINE("_MXC_AUTHORARTICLE","Tác giả bài viết");
DEFINE("_MXC_DATE","Ngày");
DEFINE("_MXC_IP","IP");
DEFINE("_MXC_CONTENT_ITEM","Mục nội dung");
DEFINE("_MXC_PUBLISHED","Giới thiệu");
DEFINE("_MXC_CLOSE","Đóng");
DEFINE("_MXC_CANCEL","Thôi");
DEFINE("_MXC_SAVE","Lưu");
DEFINE("_MXC_NEW","Ý kiến mới");
DEFINE("_MXC_NONE","Chưa");
DEFINE("_MXC_RATING","Bình chọn");
DEFINE("_MXC_NO_RATING","Chưa được bình chọn");
DEFINE("_MXC_LEVEL_RATING","Cấp độ bình chọn");
DEFINE("_MXC_ORDERING","Thứ tự");
DEFINE("_MXC_TITLE","Tựa đề");
DEFINE("_MXC_ADD","Thêm");
DEFINE("_MXC_DETAILS","Chi tiết");
DEFINE("_MXC_PREVIEW_ARTICLE","Xem trước chủ đề");
DEFINE("_MXC_ITEM_DELETED","Mục đã xóa");
DEFINE("_MXC_ITEM_SAVED","Mục đã được lưu");
DEFINE("_MXC_LANGUAGE_SAVED","Ngôn ngữ đã được lưu");
DEFINE("_MXC_FILE_NOT_WRITEABLE","Không thể ghi lên tập tin!");
DEFINE("_MXC_FILE_MUST_BE_WRITEABLE","Tập tin cần phải được phép ghi để lưu thay đổi của bạn.");
DEFINE("_MXC_WARNING","Cảnh báo...");
DEFINE("_MXC_EDIT_LANGUAGE","Sửa tập tin ngôn ngữ");
DEFINE("_MXC_EDIT_CSS","Sửa tập tin CSS");
DEFINE("_MXC_CSS_SAVED","Tập tin CSS đã được lưu");
DEFINE("_MXC_EDITORSCOMMENTS","Ý kiến người biên soạn");
DEFINE("_MXC_USERSCOMMENTS","Ý kiến người xem");
DEFINE("_MXC_EDITORSRATING","Người biên soạn bình chọn");
DEFINE("_MXC_USERSRATING","Thành viên bình chọn");
DEFINE("_MXC_FAVOURED","Ưa thích");
DEFINE("_MXC_FAVORITES","Ưa thích");
DEFINE("_MXC_BADWORDS","Từ xấu");
DEFINE("_MXC_BADWORD","Từ xấu");
DEFINE("_MXC_SETTINGS_SAVED","Cấu hình đã được lưu");
DEFINE("_MXC_NUM_CHARCARTERS","Ký tự còn lại:");
DEFINE("_MXC_RESET","Xóa");
DEFINE("_MXC_RESET_FAVOURED_COUNT","Khởi tạo lại số mục ưa thích");
DEFINE("_MXC_COUNTER_RESETED","Số mục ưa thích đã được khởi tạo");
DEFINE("_MXC_MOSTPOPULAR","Nhiều người xem nhất");
DEFINE("_MXC_LASTCOMMENTS","Ý kiến mới nhất");
DEFINE("_MXC_MOSTFAVOURED","Được ưa thích nhiều");
DEFINE("_MXC_CURRENTSETTINGS","Cấu hình hiện tại");
DEFINE("_MXC_EXPLANATION","Giải thích");
DEFINE("_MXC_GENERAL","Tổng quát");
DEFINE("_MXC_MAINOPERATINGMODE","Chế độ hoạt động chính");
DEFINE("_MXC_MANUAL","manual");
DEFINE("_MXC_SEMIAUTOMATIC","semi-automatic");
DEFINE("_MXC_AUTOMATIC","automatic");
DEFINE("_MXC_EXPL_MANUAL","Ở chế độ manual, với lệnh <font color='green'>{mxc}</font>, bạn có thể cho phép người dùng được viết ý kiến cho chủ đề. Hoặc bạn có thể đóng ý kiến lại bằng cách thêm lệnh <font color='green'>{mxc::closed}</font>.");
DEFINE("_MXC_SECTIONSAVAILABLE","Section có thể chọn");
DEFINE("_MXC_EXPL_AUTOMATIC","Với chế độ automatic hoặc semi-automatic, bạn có thể cho phép viết ý kiến chỉ các section đã được chọn tại đây. Bạn có thể đóng một mục bằng lệnh <font color='green'>{mxc::closed}</font>.");
DEFINE("_MXC_COMMENTFORM","Mẫu thể hiện");
DEFINE("_MXC_EXPL_COMMENTFORM","Chọn nơi để hiện ý kiến.");
DEFINE("_MXC_OPENINSAMEWINDOW","Hiện trong cùng cửa sổ");
DEFINE("_MXC_OPENINPOPUPWINDOW","Hiện trong cửa sổ popup");
DEFINE("_MXC_WIDTH_POPUP","Chiều rộng cửa sổ popup");
DEFINE("_MXC_HEIGHT_POPUP","Chiều cao cửa sổ popup");
DEFINE("_MXC_EXPL_WIDTH_POPUP","Điền vào chiều rộng nếu bạn chọn hiện trong cửa sổ popup");
DEFINE("_MXC_EXPL_HEIGHT_POPUP","Điền vào chiều cao nếu bạn chọn hiện trong cửa sổ popup");
DEFINE("_MXC_AUTOPUBLICHCOMMENTS","Tự động đăng tải ý kiến");
DEFINE("_MXC_EXPL_AUTOPUBLICHCOMMENTS","Bạn có muốn tự động đăng tải ý kiến?");
DEFINE("_MXC_ANONYMOUSCOMMENTS","Kể cả các ý kiến vô danh");
DEFINE("_MXC_EXPL_ANONYMOUSCOMMENTS","Bạn có cho phép người dùng chưa đăng ký được thêm ý kiến?");
DEFINE("_MXC_REPORTCOMMENT","Báo cáo về ý kiến");
DEFINE("_MXC_EXPL_REPORTCOMMENT","Người dùng có thể gởi báo cáo về một ý kiến cho quản trị viên hay không?");
DEFINE("_MXC_REPLYCOMMENT","Trả lời");
DEFINE("_MXC_EXPL_REPLYCOMMENT","Cho phép dùng chức năng trả lời một ý kiến?");
DEFINE("_MXC_SHOWNAMEORUSERNAME","Hiện tên thật hay tên đăng nhập?");
DEFINE("_MXC_NAME","Tên");
DEFINE("_MXC_USERNAME","Tên đăng nhập");
DEFINE("_MXC_EXPL_SHOWNAMEORUSERNAME","Chỉ dành cho thành viên");
DEFINE("_MXC_USEMAXCOMMENTONARCHIVES","Dùng mXcomment cho mục Archive");
DEFINE("_MXC_TEMPLATE","Mẫu thể hiện");
DEFINE("_MXC_COMMENTS_SORTING","Sắp xếp ý kiến");
DEFINE("_MXC_NEW_ENTRIES_FIRST","Mục mới hiện trước");
DEFINE("_MXC_NEW_ENTRIES_LAST","Mục mới hiện sau");
DEFINE("_MXC_EXPL_COMMENTS_SORTING","Sắp xếp ý kiến mới");
DEFINE("_MXC_AUTOLIMIT_NUM_COMMENTS_PER_ARTICLE","Tự động hạn chế số ý kiến cho một mục");
DEFINE("_MXC_UNLIMITED","Không hạn chế");
DEFINE("_MXC_DISABLED_ADD_FORM","Không cho phép nhập ý kiến khi đã đến giới hạn cho phép");
DEFINE("_MXC_COMMENTPERPAGE","Số ý kiến trên 1 trang");
DEFINE("_MXC_EXPL_COMMENTPERPAGE","Số ý kiến trên 1 trang được thể hiện");
DEFINE("_MXC_EXPL_CHOOSE_TEMPLATE","Chọn mẫu có sẵn để sử dụng");
DEFINE("_MXC_SHOWRSSFEED","Hiện RSS Feed");
DEFINE("_MXC_DATEFORMAT","Dạng thể hiện ngày tháng");
DEFINE("_MXC_EXPL_DATEFORMAT","Định dạng thể hiện ngày tháng theo hàm <strong><em>strftime</em></strong> (ví dụ : <strong><font color='green'>%d-%m-%Y %H:%M </font></strong>)");
DEFINE("_MXC_FEATURES","Chức năng");
DEFINE("_MXC_POPULAR","Phổ biến");
DEFINE("_MXC_SHOWICONPOPULAR","Hiện biểu tượng khi đạt số lần hits/views");
DEFINE("_MXC_LIMITFORSHOWICONPOPULAR","Không thể hiện biểu tượng nếu chưa được");
DEFINE("_MXC_HITS_VIEWS","Hits/Views");
DEFINE("_MXC_RATING_2","Bình chọn");
DEFINE("_MXC_USERS","Người dùng");
DEFINE("_MXC_REGISTERED_ONLY","Chỉ dành cho thành viên");
DEFINE("_MXC_ALL_USERS","Tất cả người dùng");
DEFINE("_MXC_EXPL_WHO_ADD_FAVOURITE","Chọn người có thể thêm mục ưa thích");
DEFINE("_MXC_NUMBEROFFAVOURITES","Số ưa thích");
DEFINE("_MXC_EXPL_AFTER_VOTING_FAVOURITE","Có bao nhiêu mục ưa thích được hiện sau khi bình chọn?");
DEFINE("_MXC_MENUS_FOR_FAVOURED","Tạo menu cho mục ưa thích");
DEFINE("_MXC_HOWCREATEMENU_1","Cách tạo menu &laquo; ưa thích của người dùng &raquo; ?");
DEFINE("_MXC_HOWCREATEMENU_2","Cách tạo menu &laquo; ưa thích của tôi &raquo; cho thành viên?");
DEFINE("_MXC_POSTING","Gởi ý kiến");
DEFINE("_MXC_MAXCOMMENTLENGTH","Chiều dài ý kiến tối đa (ký tự)");
DEFINE("_MXC_BLANKFORUNLIMITED","Để trống để không hạn chế");
DEFINE("_MXC_WRAPWORDLONGERTHAN","Xuống hàng nếu các từ dài hơn (ký tự)");
DEFINE("_MXC_EXPL_WRAPWORDLONGER","Điền số ký tự tối đa cho một từ. Địa chỉ URL không bị ảnh hưởng.");
DEFINE("_MXC_BBCODESUPPORT","Hỗ trợ BB Code");
DEFINE("_MXC_EXPL_BBCODESUPPORT","Cho phép dùng BB Codes trong ý kiến ?");
DEFINE("_MXC_SMILIESSUPPORT","Hỗ trợ biểu tượng mặt cười");
DEFINE("_MXC_EXPL_SMILIESSUPPORT","Cho phép dùng biểu tượng mặt cười trong ý kiến ?");
DEFINE("_MXC_PICTURESUPPORT","Hỗ trợ hình ảnh");
DEFINE("_MXC_EXPL_PICTURESUPPORT","Hỗ trợ hình ảnh trong ý kiến ?");
DEFINE("_MXC_MAX_WIDTH_PICTURESUPPORT","Chiều rộng tối đa");
DEFINE("_MXC_EXPL_WIDTH_PICTURESUPPORT","Chiều rộng tối đa của hình (pixels)");
DEFINE("_MXC_SHOWCHECKBOXFORCONTACT","Hiện hộp đánh dấu để liên lạc");
DEFINE("_MXC_EXPL_SHOWCHECKBOXFORCONTACT","Hiện &laquo; Báo cho tôi biết khi có ý kiến liên quan &raquo;");
DEFINE("_MXC_SECURITY","Bảo mật");
DEFINE("_MXC_NOTIFYADMIN","Báo cho Admin");
DEFINE("_MXC_EXPL_NOTIFYADMIN","Báo email cho Admin?");
DEFINE("_MXC_ADMINEMAIL","Email của Admin");
DEFINE("_MXC_EXPL_ADMINEMAIL","Địa chỉ để báo ý kiến?");
DEFINE("_MXC_FLOODPROTECTION","Chống gởi ý kiến liên tục");
DEFINE("_MXC_EXPL_FLOODPROTECTION","Thời gian cho phép giữa các lần gởi (giây)");
DEFINE("_MXC_BADWORDSFILTER","Lọc từ có nghĩa xấu");
DEFINE("_MXC_EXPL_BADWORDSFILTER","Áp dụng bộ lọc từ, thay thế từ có nghĩa xấu trong ý kiến bằng các ký tự *****");
DEFINE("_MXC_INTEGRATION","Tích hợp");
DEFINE("_MXC_SECURITYIMAGE","Security Images");
DEFINE("_MXC_USESECURITYIMAGE","Dùng Security Images");
DEFINE("_MXC_EXPL_USESECURITYIMAGE","Bảo mật bằng Security Images (phiên bản từ 4.1 trở lên)");
DEFINE("_MXC_SPAMPREVENTION","Ngăn chặn SPAM");
DEFINE("_MXC_ASKIMET","Dịch vụ Akismet - Phát hiện SPAM ");
DEFINE("_MXC_WORDPRESSKEYAPI","Mã API tại WordPress [API key]");
DEFINE("_MXC_EXPL_WORDPRESSKEYAPI","Bạn có thể lấy mã API bằng cách <a href='http://wordpress.com/signup/' target='_blank'>đăng ký thành viên tại WordPress.com</a>.");
DEFINE("_MXC_BLOGURL","Blog URL");
DEFINE("_MXC_EXPL_BLOGURL","Nếu để trống thì biến \$mosConfig_live_site sẽ được dùng.");
DEFINE("_MXC_EXPL_USEASKIMET","Chặn SPAM với Akismet. Xem <a href='http://akismet.com/' target='_blank'>www.akismet.com</a> để biết thêm chi tiết.");
DEFINE("_MXC_DELETEALLSPAM","Xóa tất cả SPAM");
DEFINE("_MXC_FILTERAKISMET","Filtre Akismet");
DEFINE("_MXC_COMUNITYBUILDER","Community Builder");
DEFINE("_MXC_CBAUTORLINK","Tác giả của mục");
DEFINE("_MXC_EXPL_CBAUTORLINK","Liên kết tác giả đề mục đến hồ sơ của Community Builder");
DEFINE("_MXC_CBAUTORCOMMENTLINK","Tác giả của ý kiến");
DEFINE("_MXC_EXPL_CBAUTORCOMMENTLINK","Liên kết tác giả ý kiến đến hồ sơ của Community Builder");
DEFINE("_MXC_SHOWAVATARCBPROFILE","Thể hiện hình trong hồ sơ CB");
DEFINE("_MXC_EXPL_SHOWAVATARCBPROFILE","Hiện hình trong hồ sơ CB cho ý kiến ?");
DEFINE("_MXC_MAXAVATARWIDTH","Chiều rộng tối đa của hình");
DEFINE("_MXC_VISUALRECOMMEND","Visual Recommend");
DEFINE("_MXC_USEVISUALRECOMMENDFORMAILFRIEND","Dùng Visual Recommend để gởi email cho bạn bè");
DEFINE("_MXC_EXPL_USEVISUALRECOMMENDFORMAILFRIEND","Thay thế chức năng gởi email cho bạn bè của Joomla bằng Visual Recommend. Bạn phải bỏ chọn chức năng này trong Joomla.");
DEFINE("_MXC_PIXELS","Pixels");
DEFINE("_MXC_LOADINGELEMENTS","Tùy chọn thể hiện ý kiến");
DEFINE("_MXC_SHOWDATECREATED","Hiện ngày tạo");
DEFINE("_MXC_SHOWDATEMODIFIED","Hiện ngày cập nhật");
DEFINE("_MXC_SECTION","Section");
DEFINE("_MXC_CATEGORY","Category");
DEFINE("_MXC_KEYWORDS","Từ khóa");
DEFINE("_MXC_QUOTETHIS","Đưa mục này lên website");
DEFINE("_MXC_PRINT","In");
DEFINE("_MXC_SENDBYEMAIL","Gởi bằng email");
DEFINE("_MXC_DELICIOUS","Lưu vào del.icio.us");
DEFINE("_MXC_RELATEDARTICLES","Chủ đề liên quan");
DEFINE("_MXC_READMORE","Đọc tiếp");
DEFINE("_MXC_LINKORIMAGE","(liên kết hoặc hình)");
DEFINE("_MXC_LINK","Liên kết");
DEFINE("_MXC_IMAGE","Hình");
DEFINE("_MXC_YES","Đồng ý");
DEFINE("_MXC_NO","Không");
DEFINE("_MXC_SHOW_STATUT","Trạng thái của ý kiến");
DEFINE("_MXC_EXPL_SHOW_STATUT","Cho biết ý kiến là của khách hay của thành viên");
DEFINE("_MXC_LABEL","Nhãn");
DEFINE("_MXC_MORECOMMENTS","Ý kiến khác...");
DEFINE("_MXC_ADDYOURCOMMENT","Thêm ý kiến");
DEFINE("_MXC_REPLYTOTHISCOMMENT","Trả lời ý kiến này...");
DEFINE("_MXC_SEEALLREPLIES","Xem tất cả %s trả lời");
DEFINE("_MXC_REPLIES","trả lời");
DEFINE("_MXC_REPORTTHISCOMMENT","Thông báo ý kiến này cho người quản trị");
DEFINE("_MXC_NOCOMMENT","Chưa có ý kiến");
DEFINE("_MXC_RSSFEED","Ý kiến qua RSS");
DEFINE("_MXC_QUOTETHISARTICLEONYOURSITE","Hiện chủ đề này trên trang web của bạn");
DEFINE("_MXC_CREATELINKTOWARDSTHISARTICLE","Để đặt liên kết của mục này lên website của bạn,<br />hãy chép và dán đoạn văn bản bên dưới vào trang web.");
DEFINE("_MXC_PREVIEWQUOTE","Xem trước:");
DEFINE("_MXC_GOBACKITEM","Quay lại đề mục");
DEFINE("_MXC_RSS_LASTCOMMENTS","ý kiến cuối cùng");
DEFINE("_MXC_RSS_COMMENTON","Ngày có ý kiến");
DEFINE("_MXC_RSS_VIEWCOMMENT","hiện ý kiến");
DEFINE("_MXC_RSS_WRITTENBY","Viết bởi");
DEFINE("_MXC_COMMENTS_ARE_CLOSED","Chủ đề này đã đóng lại. Không thể thêm ý kiến.");
DEFINE("_MXC_GOHOME","Ði tới trang chủ");
DEFINE("_MXC_YOURFAVOURED","Chủ đề được ưa thích");
DEFINE("_MXC_YOURFAVOUREDUSER","Chủ đề ưa thích của riêng bạn");
DEFINE("_MXC_FAVOUREDUSERMUSTLOGIN","Chỉ các thành viên mới chọn được ý kiến ưa thích.<br />Vui lòng đăng nhập hay đăng ký.");
DEFINE("_MXC_ADDFAVOURED","Thêm vào ưa thích");
DEFINE("_MXC_RECOMMENDTHISARTICLE","Thích chủ đề này");
DEFINE("_MXC_YOUHAVEFAVOUREDTHISARTICLE","Bạn đã chọn cho chủ đề này rồi");
DEFINE("_MXC_THANKFAVOURED","Cám ơn bạn đã ưa thích chủ đề này");
DEFINE("_MXC_FAVOUREDREMOVE","Gỡ bỏ");
DEFINE("_MXC_NOFAVOURED","Chưa có mục ưa thích nào");
DEFINE("_MXC_WHATYOUWANT","Bạn muốn...");
DEFINE("_MXC_FAVOUREDONLYREGISTERED","Chỉ có thành viên mới chọn được mục ưa thích. Vui lòng đăng nhập hay đăng ký.");
DEFINE("_MXC_REPORT","Báo cáo");
DEFINE("_MXC_REPORTACOMMENT","Báo có ý kiến");
DEFINE("_MXC_REPORTINTRO","Cám on bạn đã dành thời gian để báo cho người quản trị về ý kiến bên dưới.");
DEFINE("_MXC_REPORTINTRO2","Xin vui lòng điền vào báo cáo ngắn dưới đây và chọn nút Gởi để xử lý.");
DEFINE("_MXC_REASON_REPORT","Lý do bạn báo ý kiến");
DEFINE("_MXC_COMMENTINQUESTION","Ý kiến cần quan tâm");
DEFINE("_MXC_THANKS4UREPORT","Cám ơn bạn, bản báo cáo đã được gởi tới người quản trị.");
DEFINE("_MXC_ERRORONSENDREPORT","Hệ thống gặp lỗi, bản báo cáo không gởi được tới người quản trị.");
DEFINE("_MXC_BUTTON_SUBMIT","Gởi");
DEFINE("_MXC_REPORTONCOMMENT","Thông báo về ý kiến");
DEFINE("_MXC_REPORTADMINEMAIL","Người dùng đã thông báo về ý kiến sau:");
DEFINE("_MXC_FORMREPORTVALIDATE","Xin cho biết lý do bạn thông báo về ý kiến này.");
DEFINE("_MXC_ENTERNAME","Tên");
DEFINE('_MXC_ENTERMAIL', 'E-mail');
DEFINE("_MXC_FORMVALIDATECOMMENT","Vui lòng gởi ít nhất là một ý kiến cho chủ đề này!");
DEFINE("_MXC_FORMVALIDATENAME","Vui lòng điền tên của bạn");
DEFINE("_MXC_FORMVALIDATEMAIL","Vui lòng điền địa chỉ email của bạn");
DEFINE("_MXC_FORMVALIDATETITLE","Vui lòng điền tựa đề");
DEFINE("_MXC_ARTICLE","Đề mục");
DEFINE("_MXC_COMMENT_AN_ARTICLE","Góp ý cho mục");
DEFINE("_MXC_COMMENTONLYREGISTERED","Xin vui lòng đăng ký hay đăng nhập để gởi ý kiến.");
DEFINE("_MXC_NOTIFY_ME_FOLLOW_UP","Nhắc nhở tôi khi có ý kiến mới");
DEFINE("_MXC_ADMINMAILSUBJECT","Có ý kiến mới ở mục %s");
DEFINE("_MXC_ADMINMAIL","Chào Admin,<br /><br />Người dùng đã gởi ý kiến mới:<br />");
DEFINE("_MXC_ADMINMAILFOOTER","Thư này được tạo tự động chỉ với mục đích thông tin. Xin đừng trả lời.<br />");
DEFINE("_MXC_USERSUBSCRIBEMAIL","Xin chào %s,<br /><br />Người dùng đã gởi ý kiến mới:<br />");
DEFINE("_MXC_SECURITYCODE_WRONG","Mã kiểm tra nhập không đúng! Vui lòng nhập lại...");
DEFINE("_MXC_COMMENT_SAVED","Cám ơn bạn. Ý kiến của bạn đã được lưu! Ý kiến sẽ bị xóa nếu không có liên quan gì tới mục này.");
DEFINE("_MXC_REGISTERED","Thành viên");
DEFINE("_MXC_GUEST","Khác");
DEFINE("_MXC_DISPLAY_X_RELATEDITEM","Hiện X mục liên quan");
DEFINE("_MXC_NO_RELATEDITEM","Không có đề mục liên quan.");
DEFINE("_MXC_SHOWICONNEW","Hiện biểu tượng Mới!");
DEFINE("_MXC_EXPL_SHOWICONNEW","Hiện biểu tượng Mới! ngay sau ngày tạo");
DEFINE("_MXC_DAYS_NEW","Số ngày để biết một mục là <b>còn mới</b>");
DEFINE("_MXC_VOTE","bình chọn");
DEFINE("_MXC_VOTES","bình chọn");
DEFINE("_MXC_RELATED_ARTICLE_TO_THIS_ARTICLE","Các mục liên quan tới mục này");
DEFINE("_MXC_SPAM","SPAM");
DEFINE("_MXC_POTENTIALSPAM","Có thể là SPAM");
DEFINE("_MXC_NOTSPAM","Không phải SPAM");
DEFINE("_MXC_FEEDBACKTOAKISMET","Nếu bạn cho rằng ý kiến này không phải SPAM: hãy phản hồi cho Akismet");
DEFINE("_MXC_MSGFEEDBACKTOAKISMET","Ý kiến này đã được gởi tới Akismet");
DEFINE("_MXC_SPAMALERT","Ý kiến này được <a href='http://akismet.com' target='_blank'>Akismet.com</a> cho biết có thể là một tin quấy rối [SPAM]. Vì vậy, tin này sẽ được người quản trị xem xét lại trước khi đăng.");
DEFINE("_MXC_WAITING","Đợi");
DEFINE("_MXC_SECONDS","giây");
DEFINE("_MXC_ALL","Tất cả");
DEFINE("_MXC_QUOTE","Quote");
DEFINE("_MXC_COMMENTCLOSED","Mục này đã đóng, không thể thêm ý kiến.");
DEFINE("_MXC_OPACITYEFFECTONIMAGE","Hiệu ứng trong suốt cho hình");
DEFINE("_MXC_EXPL_OPACITYEFFECTONIMAGE","Thể hiện biểu tượng hoặc hình với hiệu ứng trong suốt khi rê chuột");
DEFINE("_MXC_OPACITYEFFECTPERCENT","Giá trị trong suốt");
DEFINE("_MXC_EXPL_OPACITYEFFECTPERCENT","Giá trị cho hiệu ứng trong suốt");

DEFINE("_MXC_TITLE_CONFIRM_UNSUBSCRIBE","Xác nhận về việc hủy bỏ nhận thông báo");
DEFINE("_MXC_CONFIRM_UNSUBSCRIBE","Yêu cầu hủy bỏ việc nhận thông báo ý kiến của bạn đã được thực thi.");
DEFINE("_MXC_UNSUBSCRIBE_TO_COMMENT","Thông báo này được gởi đến bạn vì bạn có yêu cầu theo dõi ý kiến của mục này. Bạn có thể ngưng nhận thông báo bằng cách nhấn vào liên kết dưới đây:");
DEFINE("_MXC_WRITEFIRSTCOMMENT","Bạn là người đầu tiên cho ý kiến");
DEFINE("_MXC_SHOW_IP","Hiện IP của người cho ý kiến");

// added in 1.0.3
DEFINE("_MXC_SHOW_FAVOUREDCOUNTER","Hiện số mục ưa thích");
DEFINE("_MXC_DISPLAY_TITLE_FIELD","Hiện tựa đề");
DEFINE("_MXC_EXPL_DISPLAY_TITLE_FIELD","Cho phép hiện tựa đề. Sẽ không thể hiện nếu không cho.");
DEFINE("_MXC_DISPLAY_EMAIL_FIELD","Hiện email");
DEFINE("_MXC_EXPL_DISPLAY_EMAIL_FIELD","Cho phép hiện email. Nếu không sẽ không thể hiện và không dùng các chức năng có liên quan đến email.");
DEFINE("_MXC_GRAVATAR","Gravatar");
DEFINE("_MXC_SHOW_GRAVATAR_USER","Hiện Gravatar người dùng");
DEFINE("_MXC_EXPL_SHOW_GRAVATAR_USER","Hiện <a href='http://site.gravatar.com' target='_blank'>Gravatar</a> người dùng (nhận diện hình toàn cầu). Phải cho phép <b>Hiện email</b> trong phần <b>Gởi ý kiến</b>.");
DEFINE("_MXC_REPLACE_CB_AVATAR","Thay thế hình trong CB");
DEFINE("_MXC_EXPL_REPLACE_CB_AVATAR_BY_GRAVATAR_USER","Thay thế hình của CB bằng Gravatar người dùng");
DEFINE("_MXC_CHOOSE_DEFAULT_GRAVATAR","Chọn hình Gravatar mặc định");
DEFINE("_MXC_WARNING_CONFIG","<font color='red'>Cảnh báo : kiểm tra lại cấu hình <b>mXcomment</b> của bạn !</font>");
DEFINE("_MXC_LANGUAGE","Ngôn ngữ");
DEFINE("_MXC_COMMENT_LANGUAGE","Ngôn ngữ cho ý kiến:");

//  added in 1.0.5
DEFINE("_MXC_MATHGUARD","Mathguard");
DEFINE("_MXC_MATHGUARD_URL","<a href='http://www.codegravity.com/projects/mathguard/' target='_blank'>Mathguard</a>");
DEFINE("_MXC_MATHGUARD_SECURITY_QUESTION","Để tránh SPAM, vui lòng nhập vào ô tính bên dưới (Thành viên không cần phải nhập):");
DEFINE("_MXC_BLOCKIPADDRESSES","Khóa địa chỉ IP");
DEFINE("_MXC_CPL_BLOCK_IP","IP bị khóa");
DEFINE("_MXC_CPANEL","Home");

// added in 1.0.6
DEFINE("_MXC_SEE_ALL_VARS_TPL","Xem tất cả các biến có thể dùng được trong mẫu thể hiện");

// added in 1.0.8
DEFINE("_MXC_DISPLAY_URL_FIELD","Hiện URL");
DEFINE("_MXC_URL","URL");
?>
