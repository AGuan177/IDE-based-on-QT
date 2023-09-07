#ifndef THEMES_H
#define THEMES_H

#include <QMap>
#include <QString>

namespace Theme { // 命名空间开始

//0默认米色
static QMap<QString, QString> Default = {

    {"o_mainwindow_bgc", "#F0E68C"},       // 最底层整个窗口的背景色，稍微不同于其他背景色
    {"o_minimap_bgc", "#F5F5DC"},         // 小地图的背景色
    {"o_tree_bgc", "#F5F5DC"},            // 树状图的背景色
    {"o_output_bgc", "#F5F5DC"},          // 调试窗的背景色
    {"o_minimap_fontc", "#000000"},       // 小地图的字体色
    {"o_tree_fontc", "#000000"},          // 树状图的字体色
    {"o_output_fontc", "#000000"},        // 调试窗的字体色

    // 词法编辑器的背景与颜色
    {"l_color", "#000000"},               // 词法编辑器的字体色
    {"l_paper", "#F5F5DC"},               // 词法编辑器的背景色
    {"l_comment", "#696969"},             // 词法编辑器注释的字体色
    {"l_quote", "#2E8B57"},               // 词法编辑器引号
    {"l_keyword", "#0000FF"},             // 词法编辑器关键字的字体色
    {"l_number", "#A52A2A"},              // 词法编辑器数字的字体色
    {"l_operator", "#000000"},            // 词法编辑器操作符的字体色
    {"l_preprocessor", "#800000"},        // 词法编辑器头文件的字体色

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#FFFFFF"},        // 选中的文字的字体色
    {"s_select_bgc", "#2E8B57"},          // 选中的文字的背景色

    // 行号栏的字体与背景
    {"s_margin_fontc", "#000000"},        // 行号栏的字体色
    {"s_margin_bgc", "#F5F5DC"},          // 行号栏的背景色

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#FFFFFF"},         // 匹配成功的括号的字体色
    {"s_match_bgc", "#2E8B57"},           // 匹配成功的括号的背景色

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#FFFFFF"},       // 匹配失败的括号的字体色
    {"s_unmatch_bgc", "#FF0000"},         // 匹配失败的括号的背景色

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#F5F5DC"}          // 折叠栏的背景色
};

//1白色
static QMap<QString, QString> White = {

    {"o_mainwindow_bgc", "#ECECEC"}, // 最底层整个窗口的背景色，稍微不同于其他白色，使用淡灰色
    {"o_minimap_bgc","#f5f5f5"},
    {"o_tree_bgc", "#f5f5f5"},
    {"o_output_bgc", "#f5f5f5"},
    {"o_minimap_fontc", "#333333"},
    {"o_tree_fontc", "#333333"},
    {"o_output_fontc", "#333333"},

    // 词法编辑器的背景与颜色
    {"l_color", "#000000"},
    {"l_paper", "#f5f5f5"},
    {"l_comment", "#a9a9a9"},
    {"l_quote", "#bb0066"},
    {"l_keyword", "#006699"},
    {"l_number", "#993366"},
    {"l_operator", "#333333"},            // 词法编辑器操作符的字体色
    {"l_preprocessor", "#339933"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#ffffff"},
    {"s_select_bgc", "#3399ff"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#777777"},
    {"s_margin_bgc", "#f5f5f5" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#008800"},
    {"s_match_bgc", "#e6ffcc"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#cc0000"},
    {"s_unmatch_bgc", "#ffcccc"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#e6e6fa"}

};

//2蓝色
static QMap<QString, QString> Blue = {

    {"o_mainwindow_bgc", "#87CEFA"}, // 最底层整个窗口的背景色，稍微不同于其他蓝色，使用淡蓝色
    {"o_minimap_bgc","#ADD8E6"},
    {"o_tree_bgc", "#ADD8E6"},
    {"o_output_bgc", "#ADD8E6"},
    {"o_minimap_fontc", "#00008B"},
    {"o_tree_fontc", "#00008B"},
    {"o_output_fontc", "#00008B"},

    // 词法编辑器的背景与颜色
    {"l_color", "#00008B"},
    {"l_paper", "#ADD8E6"},
    {"l_comment", "#1E90FF"},
    {"l_quote", "#4169E1"},
    {"l_keyword", "#0000CD"},
    {"l_operator", "#333333"},            // 词法编辑器操作符的字体色
    {"l_number", "#483D8B"},
    {"l_preprocessor", "#00BFFF"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#F0F8FF"},
    {"s_select_bgc", "#6495ED"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#191970"},
    {"s_margin_bgc", "#ADD8E6" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#7B68EE"},
    {"s_match_bgc", "#87CEFA"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#000080"},
    {"s_unmatch_bgc", "#B0C4DE"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#87CEEB"}
};

//3绿色
static QMap<QString, QString> Green = {

    {"o_mainwindow_bgc", "#7CFC00"}, // 最底层整个窗口的背景色，稍微不同于其他绿色，使用淡绿色
    {"o_minimap_bgc","#90EE90"},
    {"o_tree_bgc", "#90EE90"},
    {"o_output_bgc", "#90EE90"},
    {"o_minimap_fontc", "#008000"},
    {"o_tree_fontc", "#008000"},
    {"o_output_fontc", "#008000"},

    // 词法编辑器的背景与颜色
    {"l_color", "#008000"},
    {"l_paper", "#90EE90"},
    {"l_comment", "#32CD32"},
    {"l_quote", "#3CB371"},
    {"l_keyword", "#2E8B57"},
    {"l_operator", "#000000"},            // 词法编辑器操作符的字体色
    {"l_number", "#006400"},
    {"l_preprocessor", "#9ACD32"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#F0FFF0"},
    {"s_select_bgc", "#00FF7F"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#228B22"},
    {"s_margin_bgc", "#90EE90" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#6B8E23"},
    {"s_match_bgc", "#98FB98"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#556B2F"},
    {"s_unmatch_bgc", "#ADFF2F"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#7CFC00"}
};

//4紫色
static QMap<QString, QString> Purple = {

    {"o_mainwindow_bgc", "#BA55D3"}, // 最底层整个窗口的背景色，稍微不同于其他紫色，使用深紫色
    {"o_minimap_bgc","#D8BFD8"},
    {"o_tree_bgc", "#D8BFD8"},
    {"o_output_bgc", "#D8BFD8"},
    {"o_minimap_fontc", "#800080"},
    {"o_tree_fontc", "#800080"},
    {"o_output_fontc", "#800080"},

    // 词法编辑器的背景与颜色
    {"l_color", "#800080"},
    {"l_paper", "#D8BFD8"},
    {"l_comment", "#9370DB"},
    {"l_quote", "#BA55D3"},
    {"l_keyword", "#9400D3"},
    {"l_operator", "#006400"},            // 词法编辑器操作符的字体色
    {"l_number", "#8A2BE2"},
    {"l_preprocessor", "#9932CC"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#E6E6FA"},
    {"s_select_bgc", "#DDA0DD"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#4B0082"},
    {"s_margin_bgc", "#D8BFD8" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#EE82EE"},
    {"s_match_bgc", "#DA70D6"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#FF00FF"},
    {"s_unmatch_bgc", "#FFC0CB"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#D8BFD8"}
};

//5粉色
static QMap<QString, QString> Pink = {

    {"o_mainwindow_bgc", "#FF69B4"}, // 最底层整个窗口的背景色，稍微不同于其他粉色，使用深粉色
    {"o_minimap_bgc","#FFB6C1"},
    {"o_tree_bgc", "#FFB6C1"},
    {"o_output_bgc", "#FFB6C1"},
    {"o_minimap_fontc", "#DB7093"},
    {"o_tree_fontc", "#DB7093"},
    {"o_output_fontc", "#DB7093"},

    // 词法编辑器的背景与颜色
    {"l_color", "#DB7093"},
    {"l_paper", "#FFB6C1"},
    {"l_comment", "#FF69B4"},
    {"l_quote", "#FF1493"},
    {"l_keyword", "#C71585"},
    {"l_operator", "#800000"},            // 词法编辑器操作符的字体色
    {"l_number", "#DB7093"},
    {"l_preprocessor", "#FF69B4"},

    // 火花的背景与颜色

    // 设置选中文本的字体与背景
    {"s_select_fontc", "#FFF0F5"},
    {"s_select_bgc", "#FFC0CB"},

    // 行号栏的字体与背景
    {"s_margin_fontc", "#DB7093"},
    {"s_margin_bgc", "#FFB6C1" },

    // 设置匹配的括号的字体与背景
    {"s_match_fontc", "#FF1493"},
    {"s_match_bgc", "#FFB6C1"},

    // 设置未匹配的括号的字体与背景
    {"s_unmatch_fontc", "#C71585"},
    {"s_unmatch_bgc", "#FFC0CB"},

    // 设置折叠区域的背景色
    {"s_folding_bgc", "#FFB6C1"}
};

//6深色
static QMap<QString, QString> Dark = {
    {"o_mainwindow_bgc", "#2C3E50"},
    {"o_minimap_bgc", "#34495E"},
    {"o_tree_bgc", "#34495E"},
    {"o_output_bgc", "#34495E"},
    {"o_minimap_fontc", "#ECF0F1"},
    {"o_tree_fontc", "#ECF0F1"},
    {"o_output_fontc", "#ECF0F1"},
    {"l_color", "#ECF0F1"},
    {"l_paper", "#2C3E50"},
    {"l_comment", "#95A5A6"},
    {"l_quote", "#27AE60"},
    {"l_keyword", "#3498DB"},
    {"l_number", "#E74C3C"},
    {"l_operator", "#ECF0F1"},
    {"l_preprocessor", "#E67E22"},
    {"s_select_fontc", "#FFFFFF"},
    {"s_select_bgc", "#27AE60"},
    {"s_margin_fontc", "#ECF0F1"},
    {"s_margin_bgc", "#34495E"},
    {"s_match_fontc", "#FFFFFF"},
    {"s_match_bgc", "#27AE60"},
    {"s_unmatch_fontc", "#FFFFFF"},
    {"s_unmatch_bgc", "#FF0000"},
    {"s_folding_bgc", "#34495E"}
};

//7马卡龙
static QMap<QString, QString> Macaron = {
    {"o_mainwindow_bgc", "#FFE4E1"},
    {"o_minimap_bgc", "#F0E68C"},
    {"o_tree_bgc", "#FFDAB9"},
    {"o_output_bgc", "#FFC0CB"},
    {"o_minimap_fontc", "#333333"},
    {"o_tree_fontc", "#333333"},
    {"o_output_fontc", "#333333"},
    {"l_color", "#000000"},
    {"l_paper", "#F5F5DC"},
    {"l_comment", "#87CEEB"},
    {"l_quote", "#FF69B4"},
    {"l_keyword", "#6495ED"},
    {"l_number", "#9932CC"},
    {"l_operator", "#000000"},
    {"l_preprocessor", "#800000"},
    {"s_select_fontc", "#FFFFFF"},
    {"s_select_bgc", "#FF69B4"},
    {"s_margin_fontc", "#000000"},
    {"s_margin_bgc", "#F5F5DC"},
    {"s_match_fontc", "#FFFFFF"},
    {"s_match_bgc", "#FF69B4"},
    {"s_unmatch_fontc", "#FFFFFF"},
    {"s_unmatch_bgc", "#FF0000"},
    {"s_folding_bgc", "#F5F5DC"}
};

//8莫兰迪
static QMap<QString, QString> Morandi = {
    {"o_mainwindow_bgc", "#EEE8AA"},
    {"o_minimap_bgc", "#C0C080"},
    {"o_tree_bgc", "#C0C080"},
    {"o_output_bgc", "#C0C080"},
    {"o_minimap_fontc", "#333333"},
    {"o_tree_fontc", "#333333"},
    {"o_output_fontc", "#333333"},
    {"l_color", "#000000"},
    {"l_paper", "#F5F5DC"},
    {"l_comment", "#B0C4DE"},
    {"l_quote", "#D8BFD8"},
    {"l_keyword", "#778899"},
    {"l_number", "#9370DB"},
    {"l_operator", "#000000"},
    {"l_preprocessor", "#800000"},
    {"s_select_fontc", "#FFFFFF"},
    {"s_select_bgc", "#D8BFD8"},
    {"s_margin_fontc", "#000000"},
    {"s_margin_bgc", "#F5F5DC"},
    {"s_match_fontc", "#FFFFFF"},
    {"s_match_bgc", "#D8BFD8"},
    {"s_unmatch_fontc", "#FFFFFF"},
    {"s_unmatch_bgc", "#FF0000"},
    {"s_folding_bgc", "#F5F5DC"}
};


} // 命名空间结束

#endif // THEMES_H
