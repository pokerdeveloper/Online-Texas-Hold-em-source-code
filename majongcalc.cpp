#include "majongcalc.h"
#include <iostream>
#include <sstream>
#include <bitset>

namespace algorithm {

static const int BIT_NUMBER_64 = 64;

static const std::vector<std::string> DEBUT_HU_TYPE { 
    /**************88番*****************/
    "无效",
	"大四喜",
    "大三元",
    "九宝连灯",
    "四杠",
    "七连对",
    "百万石",
    "天胡",
    "地胡",
    "人胡",
	
    /**************64番*****************/
    "小四喜",
    "小三元",
    "字一色",
    "四暗刻",
    "一色双龙会",
	
    /**************48番*****************/
	"一色四筒顺",
    "一色四节高",
	
    /**************32番*****************/
    "一色四步高",
    "三杠",
    "混幺九",
	
    /**************24番*****************/
    "七对",
    "清一色",
    "一色三同顺",
    "一色三节高",
	
    /**************16番*****************/
    "清龙",
    "一色三步高",
    "三暗刻",
    "天听",
	
    /**************12番*****************/
    "大于5",
    "小于5",
    "三风刻",
	
    /**************8番*****************/
    "妙手回春",
    "海底捞月",
    "杠上开花",
    "抢杠胡",
    
    /**************6番*****************/
    "碰碰胡",
    "混一色",
    "全球人",
    "双暗杠",
    "双箭刻",
	
    /**************4番*****************/
    "全带幺",
    "不求人",
    "双明杠",
    "胡绝张",
    "直立",
	
    /**************2番*****************/
    "箭刻",
    "门前清",
    "平胡",
    "四归一",
    "双暗刻",
    "暗杠",
    "断幺",
	
    /**************1番*****************/
    "二五八将",
    "幺九头",
    "报听",
    "一般高",
    "连六",
    "老少副",
    "幺九刻",
    "一明杠",
    "边张",
    "坎张",
    "单吊将",
    "自摸"
};

MajongCalc::MajongCalc() :
    mjhutype_(),
    map_exclude_list_(),
    context_()
{
    map_exclude_list_[kDaSiXi].insert({kXiaoSixi, kSanFengKe, kPengPengHu, kYaoJiuKe});
    map_exclude_list_[kDaSanYuan].insert({kXiaoSanYuan, kJianKe, kShuangJianKe, kYaoJiuKe});
    map_exclude_list_[kJiuBaoLianDeng].insert({kQingYiSe, kBuQiuRen, kMenQianQing, kYaoJiuKe});
    map_exclude_list_[kSiGang].insert({kSanGang, kPengPengHu, kShuangAnGang, kYiAnGang, kShuangMingGang, kYiMingGang, kDanDiaoJing});
    map_exclude_list_[kQiLianDui].insert({kQingYiSe, kBuQiuRen, kDanDiaoJing, kMenQianQing, kQiDui, kLianLiu, kYiBanGao});
    map_exclude_list_[kTianHu].insert({kDanDiaoJing, kKanZhang, kBianZhang});
    map_exclude_list_[kBaiWanShi].insert({kQingYiSe});
    map_exclude_list_[kXiaoSanYuan].insert({kJianKe, kShuangJianKe, kYaoJiuKe});
    map_exclude_list_[kZiYiSe].insert({kPengPengHu, kHunYao9, kQuanDaiYao, kYaoJiuKe});
    map_exclude_list_[kSiAnKe].insert({kMenQianQing, kPengPengHu, kSanAnKe, kShuangAnKe, kBuQiuRen});
    map_exclude_list_[kYiSeShuangLongHui].insert({kPingHu, kQiDui, kQingYiSe, kYiBanGao, kLaoShaoFu});
    map_exclude_list_[kYiSeSiTongShun].insert({kYiSeSanJieGao, kYiBanGao, kSiGuiYi, kYiSeSanTongShun});
    map_exclude_list_[kYiSeSiJieGao].insert({kYiSeSanTongShun, kYiSeSanJieGao, kPengPengHu});
    map_exclude_list_[kYiSeSiBuGao].insert({kYiSeSanBuGao, kLaoShaoFu, kLianLiu});
    map_exclude_list_[kSanGang].insert({kShuangAnGang, kShuangMingGang, kYiAnGang, kYiMingGang});
    map_exclude_list_[kHunYao9].insert({kPengPengHu, kYaoJiuKe, kQuanDaiYao});
    map_exclude_list_[kQiDui].insert({kMenQianQing, kBuQiuRen, kDanDiaoJing});
    map_exclude_list_[kYiSeSanTongShun].insert({kYiSeSanJieGao, kYiBanGao});
    map_exclude_list_[kYiSeSanJieGao].insert({kYiSeSanTongShun});
    map_exclude_list_[kQingLong].insert({kLianLiu, kLaoShaoFu});
    map_exclude_list_[kYiSeSanBuGao].insert({kShuangAnKe});
    map_exclude_list_[kTianTing].insert({kZhiLi, kBaoTing});
    map_exclude_list_[kSanFengKe].insert({kYaoJiuKe});
    map_exclude_list_[kMiaoShouHuiChun].insert({kZiMo});
    map_exclude_list_[kGangShangHua].insert({kZiMo});
    map_exclude_list_[kQiangGangHu].insert({kHuJueZhang});
    map_exclude_list_[kQuanQiuRen].insert({kDanDiaoJing});
    map_exclude_list_[kShuangAnGang].insert({kShuangAnKe, kYiAnGang});
    map_exclude_list_[kShuangJianKe].insert({kJianKe});
    map_exclude_list_[kBuQiuRen].insert({kMenQianQing, kZiMo});
    map_exclude_list_[kShuangMingGang].insert({kYiMingGang});
    map_exclude_list_[kHuJueZhang].insert({kQiangGangHu});
    map_exclude_list_[kZhiLi].insert({kBaoTing, kMenQianQing});
    map_exclude_list_[kJianKe].insert({kYaoJiuKe});
}

void MajongCalc::SetContext(const MjCalcContext &ctx)
{
    context_ = ctx;
}

bool MajongCalc::CheckHu(const std::vector<ICardPtr> &hand_cards)
{
    std::vector<OutDoorCards> vec_out;
    auto ret = mjhutype_.CheckHu(hand_cards, vec_out);
    SetHuDoorCards(vec_out);
    return ret;
}

int MajongCalc::CalculateFan(uint64_t bit_type)
{    
    std::bitset<BIT_NUMBER_64> bs(bit_type);

    std::string debug;
    int num = 0;
    for (int i = 0; i < BIT_NUMBER_64; ++i)
    {
        if (bs.test(i))
        {
            num += VEC_FAN[i];
            debug.append(DEBUT_HU_TYPE[i] + ",");
        }
    }
    std::cout << "{" << debug << "}" << std::endl;
    return num;
}

uint64_t MajongCalc::CalculateHuType(const std::vector<OutDoorCards> &vec)
{
    std::vector<OutDoorCards> vec_door_cards;
    vec_door_cards.assign(vec.begin(), vec.end());
    vec_door_cards.insert(vec_door_cards.end(), hu_door_cards_.begin(), hu_door_cards_.end());

    std::bitset<BIT_NUMBER_64>bs;

    if (context_.hu_action & HuAction::TianHu)
        bs.set(kTianHu);
    else if (context_.hu_action & HuAction::DiHu)
        bs.set(kDiHu);
    else if (context_.hu_action & HuAction::RenHu)
        bs.set(kRenHu);
    
    if (context_.hu_action & HuAction::MiaoShouHuiChun)
        bs.set(kMiaoShouHuiChun);
    if (context_.hu_action & HuAction::GangShangHua)
        bs.set(kGangShangHua);
    if (context_.hu_action & HuAction::QiangGangHu)
        bs.set(kQiangGangHu);
    if (context_.hu_action & HuAction::HaiDiLaoYue)
        bs.set(kHaiDiLaoYue);

    if (context_.is_tianting)
    {
        bs.set(kTianTing);
    }
    else if (context_.is_baoting)
    {
        bs.set(kBaoTing);
    }

    if (context_.is_zimo)
    {
        bs.set(kZiMo);
    }
    if (mjhutype_.HuJueZhang(context_.is_juezhang))
    {
        bs.set(kHuJueZhang);
    }

    if (mjhutype_.JiuBaoLianDeng(hu_door_cards_))
        bs.set(kJiuBaoLianDeng);
    // 风刻
    else if (mjhutype_.DaSiXi(vec_door_cards))
        bs.set(kDaSiXi);
    else if (mjhutype_.XiaoSixi(vec_door_cards))
        bs.set(kXiaoSixi);
    else if (mjhutype_.SanFengKe(vec_door_cards))
        bs.set(kSanFengKe);
    
    // 箭刻
    if (mjhutype_.DaSanYuan(vec_door_cards))
        bs.set(kDaSanYuan);
    else if (mjhutype_.XiaoSanYuan(vec_door_cards))
        bs.set(kXiaoSanYuan);
    else if (mjhutype_.ShuangJianKe(vec_door_cards))
        bs.set(kShuangJianKe);
    else if (mjhutype_.JianKe(vec_door_cards))
        bs.set(kJianKe);

    // 杠
    if (mjhutype_.SiGang(vec_door_cards))
        bs.set(kSiGang);
    else if (mjhutype_.SanGang(vec_door_cards))
        bs.set(kSanGang);
    else
    {
        if (mjhutype_.ShuangAnGang(vec_door_cards))
            bs.set(kShuangAnGang);
        else if (mjhutype_.YiAnGang(vec_door_cards))
            bs.set(kYiAnGang);

        if (mjhutype_.ShuangMingGang(vec_door_cards))
            bs.set(kShuangMingGang);
        else if (mjhutype_.YiMingGang(vec_door_cards))
            bs.set(kYiMingGang);
    }

    std::vector<OutDoorCards> copy_door_cards;

    // 七对 同顺 节高 步高
    if (mjhutype_.QiLianDui(hu_door_cards_, copy_door_cards))
    {
        bs.set(kQiLianDui);
        vec_door_cards.assign(copy_door_cards.begin(), copy_door_cards.end());
    }
    else if (mjhutype_.YiSeSiTongShun(hu_door_cards_, vec, copy_door_cards))
    {
        bs.set(kYiSeSiTongShun);
        vec_door_cards.assign(vec.begin(), vec.end());
        vec_door_cards.insert(vec_door_cards.end(), copy_door_cards.begin(), copy_door_cards.end());
    }
    else if (mjhutype_.YiSeSiJieGao(hu_door_cards_, vec, copy_door_cards))
    {
        bs.set(kYiSeSiJieGao);
        vec_door_cards.assign(vec.begin(), vec.end());
        vec_door_cards.insert(vec_door_cards.end(), copy_door_cards.begin(), copy_door_cards.end());
    }
    else if (mjhutype_.YiSeSiBuGao(vec_door_cards))
        bs.set(kYiSeSiBuGao);
    else if (mjhutype_.QiDui(hu_door_cards_, copy_door_cards))
    {
        bs.set(kQiDui);
        vec_door_cards.assign(copy_door_cards.begin(), copy_door_cards.end());
    }
    else if (mjhutype_.YiSeSanJieGaoAndSanAnKe(hu_door_cards_, vec, context_, copy_door_cards))
    {
        bs.set(kYiSeSanJieGao);
        vec_door_cards.assign(vec.begin(), vec.end());
        vec_door_cards.insert(vec_door_cards.end(), copy_door_cards.begin(), copy_door_cards.end());
    }
    else if (mjhutype_.YiSeSanTongShun(vec_door_cards))
        bs.set(kYiSeSanTongShun);
    else if (mjhutype_.YiSeSanJieGao(vec_door_cards))
        bs.set(kYiSeSanJieGao);
    else if (mjhutype_.YiSeSanBuGao(vec_door_cards))
        bs.set(kYiSeSanBuGao);
    else if (mjhutype_.YiBanGao(vec_door_cards))
        bs.set(kYiBanGao);

    // 暗刻
    if (mjhutype_.SiAnKe(vec_door_cards, context_))
        bs.set(kSiAnKe);
    else if (mjhutype_.SanAnKe(vec_door_cards, context_))
        bs.set(kSanAnKe);
    else if (mjhutype_.ShuangAnKe(vec_door_cards, context_))
        bs.set(kShuangAnKe);

    if (mjhutype_.PingHu(vec_door_cards))
        bs.set(kPingHu);
    else if (mjhutype_.PengPengHu(vec_door_cards))
        bs.set(kPengPengHu);

    // 幺
    if (mjhutype_.HunYao9(vec_door_cards))
        bs.set(kHunYao9);
    else if (mjhutype_.QuanDaiYao(vec_door_cards))
        bs.set(kQuanDaiYao);
    else if (mjhutype_.DuanYao(vec_door_cards))
        bs.set(kDuanYao);
    else if (mjhutype_.YaoJiuTou(vec_door_cards))
        bs.set(kYaoJiuTou);
    else if (mjhutype_.YaoJiuKe(vec_door_cards))
        bs.set(kYaoJiuKe);

    // 求人
    if (mjhutype_.QuanQiuRen(vec_door_cards, context_.is_zimo))
        bs.set(kQuanQiuRen);
    else if (mjhutype_.BuQiuRen(vec_door_cards, context_.is_zimo))
        bs.set(kBuQiuRen);
    else if (mjhutype_.MenQianQing(vec_door_cards, context_.is_zimo))
        bs.set(kMenQianQing);

    if (mjhutype_.BaiWanShi(vec_door_cards))
        bs.set(kBaiWanShi);
    if (mjhutype_.QingLong(vec_door_cards))
        bs.set(kQingLong);

    // 大于小于5
    if (mjhutype_.XiaoYu5(vec_door_cards))
        bs.set(kXiaoYu5);
    else if (mjhutype_.DaYu5(vec_door_cards))
        bs.set(kDaYu5);

    if (mjhutype_.ZhiLi(vec_door_cards, context_.is_zimo, context_.is_baoting))
        bs.set(kZhiLi);
    if (mjhutype_.SiGuiYi(hu_door_cards_))
        bs.set(kSiGuiYi);
    if (mjhutype_.ErWuBaJiang(vec_door_cards))
        bs.set(kErWuBaJiang);
    if (mjhutype_.LianLiu(vec_door_cards))
        bs.set(kLianLiu);
    if (mjhutype_.LaoShaoFu(vec_door_cards))
        bs.set(kLaoShaoFu);

    /*************单吊/边张/坎张*************/
    if (mjhutype_.BianZhang(vec_door_cards, context_.hu_card))
        bs.set(kBianZhang);
    else if (mjhutype_.KanZhang(vec_door_cards, context_.hu_card))
        bs.set(kKanZhang);
    else if (mjhutype_.DanDiaoJing(vec_door_cards, context_.hu_card))
        bs.set(kDanDiaoJing);

    // 一色
    if (mjhutype_.YiSeShuangLongHui(vec_door_cards))
        bs.set(kYiSeShuangLongHui);
    else if (mjhutype_.ZiYiSe(vec_door_cards))
        bs.set(kZiYiSe);
    else if (mjhutype_.QingYiSe(vec_door_cards))
        bs.set(kQingYiSe);
    else if (mjhutype_.HunYiSe(vec_door_cards))
        bs.set(kHunYiSe);

    return ExcludeHuType(bs.to_ullong());
}

uint64_t MajongCalc::ExcludeHuType(uint64_t bit64) const
{
    std::bitset<BIT_NUMBER_64> bs(bit64);
    for (int i = 0; i < BIT_NUMBER_64; ++i)
    {
        if (!bs.test(i))
            continue;

        auto it = map_exclude_list_.find((HuType)i);
        if (it == map_exclude_list_.end())
            continue;

        for (auto iter : it->second)
            bs.set(iter, false);
    }
    return bs.to_ullong();
}

const std::vector<OutDoorCards> &MajongCalc::HuDoorCards() const
{
    return hu_door_cards_;
}

void MajongCalc::SetHuDoorCards(const std::vector<OutDoorCards> &vec)
{
    hu_door_cards_.assign(vec.begin(), vec.end());
}

} //namespace algorithm