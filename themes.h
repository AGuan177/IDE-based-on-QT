#ifndef THEMES_H
#define THEMES_H

#include <QMap>
#include <QString>

namespace Theme { // 命名空间开始

// 定义并初始化颜色映射
static QMap<QString, QString> Default = {
    {"o_minimap_bgc","#121e2c"},
    {"o_tree_bgc", "#121e2c"},
    {"o_output_bgc", "#121e2c"},
    {"o_minimap_fontc", "#ffffff"},
    {"o_tree_fontc", "#ffffff"},
    {"o_output_fontc", "#ffffff"},

    // 词法编辑器的背景与颜色
    {"l_color", "#46505c"},
    {"l_paper", "#121e2c"},
    {"l_comment", "#46505c"},
    {"l_quote", "#7b86b4"},
    {"l_keyword", "#849fb4"},
    {"l_number", "#f4d9db"},
    {"l_preprocessor", "#8abf89"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#000000"},
    {"s_select_bgc", "#ffffff"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#ffffff"},
    {"s_margin_bgc", "#121e2c" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#ff7f00"},
    {"s_match_bgc", "#add8e6"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#ff0000"},
    {"s_unmatch_bgc", "#ff0000"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#000080"}

};



} // 命名空间结束

#endif // THEMES_H
