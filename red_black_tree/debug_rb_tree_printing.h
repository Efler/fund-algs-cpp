#ifndef RED_BLACK_TREE_DEBUG_RB_TREE_PRINTING_H
#define RED_BLACK_TREE_DEBUG_RB_TREE_PRINTING_H

////TODO: ------------------- FOR DEBUG ONLY! -------------------

#include <functional>
#include <iostream>
#include <vector>


enum class debug_color{
    red,
    black
};

template <typename tkey, typename tvalue>
struct debug_rb_node final{
    debug_rb_node() = default;
    debug_rb_node(const tkey& k, const tvalue& v, debug_rb_node* l, debug_rb_node* r, debug_color c):
            key(k), value(v), left(l), right(r), node_color(c){}

    tkey key;
    tvalue value;
    debug_rb_node* left;
    debug_rb_node* right;
    debug_color node_color;

    virtual ~debug_rb_node() = default;
};

template <typename tkey, typename tvalue>
void debug_rb_tree_printing(void* root) {
    auto node_concrete = reinterpret_cast<debug_rb_node<tkey, tvalue>*>(root);

    if(node_concrete == nullptr){
        std::cout << "~~~ DEBUG RB_TREE PRINTING ~~~" << std::endl;
        std::cout << "empty" << std::endl;
        std::cout << "~~~ DEBUG RB_TREE PRINTING ~~~" << std::endl;
        return;
    }

#define MAX_(x, y) ((x) > (y) ? (x) : (y))
#define MIN_(x, y) ((x) < (y) ? (x) : (y))

    std::cout << "~~~ DEBUG RB_TREE PRINTING ~~~" << std::endl;

    static std::string ch_hor = "-", ch_ver = "|", ch_ddia = "/", ch_rddia = "\\", ch_udia = "\\", ch_ver_hor = "|-", ch_udia_hor = "\\-", ch_ddia_hor = "/-", ch_ver_spa = "| ";
    auto RepStr = [](std::string const & s, size_t cnt) {
        if (ptrdiff_t(cnt) < 0)
            throw std::runtime_error("RepStr: Bad value " + std::to_string(ptrdiff_t(cnt)) + "!");
        std::string r;
        for (size_t i = 0; i < cnt; ++i)
            r += s;
        return r;
    };
    std::function<std::tuple<std::vector<std::string>, size_t, size_t>(debug_rb_node<tkey, tvalue>* node_concrete, bool)> Rec;
    Rec = [&RepStr, &Rec](debug_rb_node<tkey, tvalue>* node_concrete, bool left){
        std::vector<std::string> lines;
        if (!node_concrete)
            return std::make_tuple(lines, size_t(0), size_t(0));
        auto sval = std::to_string(node_concrete->key);
        if(node_concrete->node_color == debug_color::red) sval += "[R]";
        else sval += "[B]";
        auto resl = Rec(node_concrete->left, true), resr = Rec(node_concrete->right, false);
        auto const & vl = std::get<0>(resl);
        auto const & vr = std::get<0>(resr);
        auto cl = std::get<1>(resl), cr = std::get<1>(resr), lss = std::get<2>(resl), rss = std::get<2>(resr);
        size_t lv = sval.size();
        size_t ls = !vl.empty() ? lss : size_t(0);
        size_t rs = !vr.empty() ? rss : size_t(0);
        size_t lis = ls == 0 ? lv / 2 : MAX_(int(lv / 2 + 1 - (ls - cl)), 0);
        size_t ris = rs == 0 ? (lv + 1) / 2 : MAX_(int((lv + 1) / 2 - cr), (lis > 0 ? 0 : 1));
        size_t dashls = (ls == 0 ? 0 : ls - cl - 1 + lis - lv / 2), dashrs = (rs == 0 ? 0 : cr + ris - (lv + 1) / 2);
        lines.push_back(
                (ls == 0 ? "" : (RepStr(" ", cl) + ch_ddia + RepStr(ch_hor, dashls))) +
                sval + (rs == 0 ? "" : (RepStr(ch_hor, dashrs) + ch_rddia + RepStr(" ", rs - cr - 1)))
        );
        if (lines.back().size() != ls + lis + ris + rs)
            throw std::runtime_error("Dump: First line wrong size, got " + std::to_string(lines.back().size()) + ", expected " + std::to_string(ls + lis + ris + rs));
        for (size_t i = 0; i < MAX_(vl.size(), vr.size()); ++i) {
            std::string sl = RepStr(" ", ls), sr = RepStr(" ", rs);
            if (i < vl.size()) sl = vl[i];
            if (i < vr.size()) sr = vr[i];
            sl += RepStr(" ", lis);
            sr = RepStr(" ", ris) + sr;
            lines.push_back(sl + sr);
        }
        return std::make_tuple(lines, (left || ls + lis == 0 || lv % 2 == 1) ? ls + lis : ls + lis - 1, ls + lis + ris + rs);
    };
    auto v = std::get<0>(Rec(node_concrete, true));
    for (const auto& i : v)
        std::cout << i << std::endl;

    std::cout << "~~~ DEBUG RB_TREE PRINTING ~~~" << std::endl;

#undef MAX_
#undef MIN_
}


////TODO: ------------------- FOR DEBUG ONLY! -------------------

#endif //RED_BLACK_TREE_DEBUG_RB_TREE_PRINTING_H