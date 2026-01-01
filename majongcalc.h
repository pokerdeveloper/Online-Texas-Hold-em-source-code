#ifndef MAJONG_CALC_H
#define MAJONG_CALC_H

#include <set>
#include "majonghutype.h"

namespace algorithm {


class MajongCalc
{
public:
    MajongCalc();
    ~MajongCalc() = default;

    void SetContext(const MjCalcContext &context);
    bool CheckHu(const std::vector<ICardPtr> &hand_cards);
    int CalculateFan(uint64_t bit_type);
    uint64_t CalculateHuType(const std::vector<OutDoorCards> &vec_door_cards);
    uint64_t ExcludeHuType(uint64_t bit64) const;
    const std::vector<OutDoorCards> &HuDoorCards() const;
    void SetHuDoorCards(const std::vector<OutDoorCards> &vec);
    
private:
    MajongHuType mjhutype_;
    std::map<HuType, std::set<HuType>> map_exclude_list_;
    MjCalcContext context_;
    std::vector<OutDoorCards> hu_door_cards_;
};

} //namespace algorithm

# endif //MAJONG_CALC_H
