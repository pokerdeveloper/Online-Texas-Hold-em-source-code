#ifndef MAJONG_HU_TYPE_H
#define MAJONG_HU_TYPE_H

#include <set>
#include <unordered_map>
#include <functional>
#include "mjalgorithm.h"

namespace algorithm {

enum HuType
{
    /**************88番*****************/
    kInvalid = 0,//无效类型
    kDaSiXi, // 大四喜
    kDaSanYuan, // 大三元
    kJiuBaoLianDeng, // 九宝连灯
    kSiGang, // 四杠
    kQiLianDui, // 七连对
    kBaiWanShi, // 百万石
    kTianHu, // 天胡
    kDiHu, // 地胡
    kRenHu, // 人胡

    /**************64番*****************/
    kXiaoSixi, // 小四喜
    kXiaoSanYuan,// 小三元
    kZiYiSe, // 字一色
    kSiAnKe, // 四暗刻
    kYiSeShuangLongHui, // 一色双龙会

    /**************48番*****************/
    kYiSeSiTongShun, // 一色四筒顺
    kYiSeSiJieGao, // 一色四节高

    /**************32番*****************/
    kYiSeSiBuGao, // 一色四步高
    kSanGang, // 三杠
    kHunYao9, // 混幺九

    /**************24番*****************/
    kQiDui, // 七对
    kQingYiSe, // 清一色
    kYiSeSanTongShun, // 一色三同顺
    kYiSeSanJieGao, // 一色三节高

    /**************16番*****************/
    kQingLong, // 清龙
    kYiSeSanBuGao, // 一色三步高
    kSanAnKe, // 三暗刻
    kTianTing, // 天听

    /**************12番*****************/
    kDaYu5, // 大于5
    kXiaoYu5, // 小于5
    kSanFengKe, // 三风刻

    /**************8番*****************/
    kMiaoShouHuiChun, // 妙手回春
    kHaiDiLaoYue, // 海底捞月
    kGangShangHua, // 杠上开花
    kQiangGangHu, // 抢杠胡
    
    /**************6番*****************/
    kPengPengHu, // 碰碰胡
    kHunYiSe, // 混一色
    kQuanQiuRen, // 全球人
    kShuangAnGang, // 双暗杠
    kShuangJianKe, // 双箭刻

    /**************4番*****************/
    kQuanDaiYao, // 全带幺
    kBuQiuRen, // 不求人
    kShuangMingGang, // 双明杠
    kHuJueZhang, // 胡绝张
    kZhiLi, // 直立

    /**************2番*****************/
    kJianKe, // 箭刻
    kMenQianQing, // 门前清
    kPingHu, // 平胡
    kSiGuiYi, // 四归一
    kShuangAnKe, // 双暗刻
    kYiAnGang, // 暗杠
    kDuanYao, // 断幺

    /**************1番*****************/
    kErWuBaJiang, // 二五八将
    kYaoJiuTou, // 幺九头
    kBaoTing, //报听
    kYiBanGao, // 一般高
    kLianLiu, // 连六
    kLaoShaoFu, // 老少副
    kYaoJiuKe, // 幺九刻
    kYiMingGang, // 一明杠
    kBianZhang, // 边张
    kKanZhang, // 坎张
    kDanDiaoJing, // 单吊将
    kZiMo, //自摸
};

// 牌型对应的番数
static const std::vector<int> VEC_FAN { 
    0,
    88, 88, 88, 88, 88, 88, 88, 88, 88,
    64, 64, 64, 64, 64,
    48, 48,
    32, 32, 32,
    24, 24, 24, 24,
    16, 16, 16, 16,
    12, 12, 12,
    8, 8, 8, 8,
    6, 6, 6, 6, 6,
    4, 4, 4, 4, 4,
    2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

enum HuAction
{
    Invalid           = 0x0, // 无效
    TianHu            = 0x1, // 天胡（88番）
    DiHu              = 0x2, // 地胡（88番）
    RenHu             = 0x4, // 人胡（88番）
    TianTing          = 0x8, // 天听（16番）
    MiaoShouHuiChun   = 0x10, // 妙手回春（8番）
    HaiDiLaoYue       = 0x20, // 海底捞月（8番）
    GangShangHua      = 0x40, // 杠上开花（8番）
    QiangGangHu       = 0x80, // 抢杠胡（8番）
};

struct MjCalcContext
{
    MjCalcContext()
    {
    }

    MjCalcContext(ICardPtr card, uint64_t action, bool zimo, bool baoting, bool juezhang, bool tianting) :
        hu_card(card),
        hu_action(action),
        is_zimo(zimo),
        is_baoting(baoting),
        is_juezhang(juezhang),
        is_tianting(tianting)
    {
    }

    MjCalcContext(const MjCalcContext &) = default;
    MjCalcContext(MjCalcContext &&) = default;
    MjCalcContext &operator=(const MjCalcContext &) = default;
    MjCalcContext &operator=(MjCalcContext &&) = default;

    ICardPtr hu_card;
    uint64_t hu_action;
    bool is_zimo;
    bool is_baoting;
    bool is_juezhang;
    bool is_tianting;
};

class MajongHuType
{
    using Card = uint16_t;

public:
    MajongHuType() = default;
    ~MajongHuType() = default;

    bool CheckHu(const std::vector<ICardPtr> &hand_cards, std::vector<OutDoorCards> &vec_door_cards);

    /**************88番*****************/
    // 大四喜
    bool DaSiXi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 大三元 
    bool DaSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 九宝连灯
    bool JiuBaoLianDeng(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四杠
    bool SiGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 七连对
    bool QiLianDui(const std::vector<OutDoorCards> &vec_door_cards, std::vector<OutDoorCards> &out) const;
    // 百万石
    bool BaiWanShi(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************64番*****************/
    // 小四喜
    bool XiaoSixi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 小三元
    bool XiaoSanYuan(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 字一色
    bool ZiYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四暗刻
    bool SiAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const;
    // 一色双龙会
    bool YiSeShuangLongHui(const std::vector<OutDoorCards> &vec_door_cards) const;

    /**************48番*****************/
    // 一色四筒顺
    bool YiSeSiTongShun(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        std::vector<OutDoorCards> &out) const;

    // 一色四节高
    bool YiSeSiJieGao(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        std::vector<OutDoorCards> &out) const;

    /**************32番*****************/
    // 一色四步高
    bool YiSeSiBuGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三杠
    bool SanGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 混幺九
    bool HunYao9(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************24番*****************/
    // 七对
    bool QiDui(const std::vector<OutDoorCards> &vec_door_cards, std::vector<OutDoorCards> &out) const;
    // 清一色
    bool QingYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三同顺
    bool YiSeSanTongShun(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三节高和三暗刻
    bool YiSeSanJieGaoAndSanAnKe(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        const MjCalcContext &ctx,
        std::vector<OutDoorCards> &out) const;
    // 一色三节高
    bool YiSeSanJieGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************16番*****************/
    // 清龙
    bool QingLong(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一色三步高
    bool YiSeSanBuGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三暗刻
    bool SanAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const;
    // 天听
    bool TianTing(HuAction hu_action) const;
    
    /**************12番*****************/
    // 大于5
    bool DaYu5(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 小于5
    bool XiaoYu5(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 三风刻
    bool SanFengKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************6番*****************/
    // 碰碰胡
    bool PengPengHu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 混一色
    bool HunYiSe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 全球人
    bool QuanQiuRen(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const;
    // 双暗杠
    bool ShuangAnGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双箭刻
    bool ShuangJianKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    
    /**************4番*****************/
    // 全带幺
    bool QuanDaiYao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 不求人
    bool BuQiuRen(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const;
    // 双明杠
    bool ShuangMingGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 胡绝张
    bool HuJueZhang(bool juezhang) const {return juezhang;}
    // 直立
    bool ZhiLi(const std::vector<OutDoorCards> &vec_door_cards, bool zimo, bool baoting) const;

    /**************2番*****************/
    // 箭刻
    bool JianKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 门前清
    bool MenQianQing(const std::vector<OutDoorCards> &vec_door_cards, bool zimo) const;
    // 平胡
    bool PingHu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 四归一
    bool SiGuiYi(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 双暗刻
    bool ShuangAnKe(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const;
    // 暗杠
    bool YiAnGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 断幺
    bool DuanYao(const std::vector<OutDoorCards> &vec_door_cards) const;

    /**************1番*****************/
    // 二五八将
    bool ErWuBaJiang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 幺九头
    bool YaoJiuTou(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一般高
    bool YiBanGao(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 连六
    bool LianLiu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 老少副
    bool LaoShaoFu(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 幺九刻
    bool YaoJiuKe(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 一明杠
    bool YiMingGang(const std::vector<OutDoorCards> &vec_door_cards) const;
    // 边张
    bool BianZhang(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const;
    // 坎张
    bool KanZhang(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const;
    // 单吊将
    bool DanDiaoJing(const std::vector<OutDoorCards> &vec_door_cards, const ICardPtr &c) const;
    // 暗刻的数量
    int AnKeCount(const std::vector<OutDoorCards> &vec_door_cards, const MjCalcContext &ctx) const;

private:
    int ShunZiCount(const std::set<int> &set) const;
    bool IsAnKe(const OutDoorCards &door_cards, const MjCalcContext &ctx) const;
    bool IsJianKe(const OutDoorCards &door_cards) const;
    bool IsFengKe(const OutDoorCards &door_cards) const;
    bool IsGang(const OutDoorCards &door_cards) const;
    bool IsTongShun(const OutDoorCards &door_cards1, const OutDoorCards &door_cards2) const;
    bool IsShunZi123(const OutDoorCards &door_cards) const;
    bool IsShunZi789(const OutDoorCards &door_cards) const;
    bool IsNotHandDoorCards(const OutDoorCards &door_cards) const;
    
    bool IsComposeTypeHu(const std::vector<OutDoorCards> &hand_door_cards,
        const std::vector<OutDoorCards> &door_cards, 
        std::vector<OutDoorCards> &out,
        std::function<bool (const std::vector<OutDoorCards>&)> func) const;

private:
    uint16_t Wan(Card c) const {return c & 0x000F;}
    uint16_t Feng(Card c) const {return c & 0x00F0;}
    uint16_t Zi(Card c) const {return c & 0x0F00;}
    uint16_t Hua(Card c) const {return c & 0xF000;}

    bool IsMyriao(const ICardPtr &c) const {return c->type() == MjCard::E_MYRIAD_TYPE;} //万
    bool IsWind(const ICardPtr &c) const {return c->type() == MjCard::E_WIND_TYPE;} //风
    bool IsWord(const ICardPtr &c) const {return c->type() == MjCard::E_WORD_TYPE;} //字
    bool IsFlower(const ICardPtr &c) const {return c->type() == MjCard::E_FLOWER_TYPE;} //花

private:
    MjAlgorithm algorithm_;
};

} //namespace algorithm

# endif //MAJONG_HU_TYPE_H