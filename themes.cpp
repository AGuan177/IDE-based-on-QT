#include "Themes.h"

QMap<QString, QString> Tdefault;

// 实现初始化函数
void Themes::initDefault(){
    // 其他三块的背景与字体
    Tdefault["o_minimap_bgc"] = "#121e2c";
    Tdefault["o_tree_bgc"] = "#121e2c";
    Tdefault["o_output_bgc"] = "#121e2c";
    Tdefault["o_minimap_fontc"] = "#ffffff";
    Tdefault["o_tree_fontc"] = "#ffffff";
    Tdefault["o_output_fontc"] = "#ffffff";

    // 词法编辑器的背景与颜色
    Tdefault["l_color"] = "#f4d9db";
    Tdefault["l_paper"] = "#121e2c";
    Tdefault["l_comment"] = "#46505c";
    Tdefault["l_quote"] = "#7b86b4";
    Tdefault["l_keyword"] = "#849fb4";
    Tdefault["l_number"] = "";
    Tdefault["l_preprocessor"] = "#8abf89";

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    Tdefault["s_select_fontc"] = "#000000";
    Tdefault["s_select_bgc"] = "#ffffff";

    // 行号栏的字体与背景
    Tdefault["s_margin_fontc"] = "#ffffff";
    Tdefault["s_margin_bgc"] = "#121e2c" ;

    // 设置匹配的括号的字体与背景
    Tdefault["s_match_fontc"] = "#ff7f00";
    Tdefault["s_match_bgc"] = "#add8e6";

    // 设置未匹配的括号的字体与背景
    Tdefault["s_unmatch_fontc"] = "#ff0000";
    Tdefault["s_unmatch_bgc"] = "#ff0000";

    // 设置折叠区域的背景色
    Tdefault["s_folding_bgc"] = "#000080";
}
