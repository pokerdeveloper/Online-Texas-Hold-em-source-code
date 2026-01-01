#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include "mjcard.h"

namespace algorithm {

class MaJongOp
{
	static const int32_t ROW = MjCard::E_TYPE_COUNT_;
	static const int32_t CLOU = MjCard::E_FACE_COUNT_;

	using Card = uint8_t;
	using CardGroup = Card[ROW][CLOU];

public:
    MaJongOp(const std::vector<ICardPtr> &hand_cards);

	bool CheckComposeShuZiLeft(const ICardPtr &c) const
	{
        int type = c->type();
        int value = c->face();
		if (value <= 2) return false;
		return card_group_[type][value - 2] > 0 && card_group_[type][value - 1] > 0;
	}

	bool CheckComposeShuZiMeddle(const ICardPtr &c) const
	{
        int type = c->type();
        int value = c->face();
		if (value == 1 || value == 9) return false;
		return card_group_[type][value - 1] > 0 && card_group_[type][value + 1] > 0;
	}

    bool CheckComposeShuZiRigth(const ICardPtr &c) const
	{
        int type = c->type();
        int value = c->face();
		if (value >= 8) return false;
		return card_group_[type][value + 1] > 0 && card_group_[type][value + 2] > 0;
	}

	bool CheckComposeKeZi(const ICardPtr &c) const
	{
        int type = c->type();
        int value = c->face();
		return card_group_[type][value] >= 2;
	}

 	bool CheckComposeGang(const ICardPtr &c) const
	{
        int type = c->type();
        int value = c->face();
		return card_group_[type][value] == 3;
	} 

	bool CheckAnGang(std::vector<uint16_t> &apcards) const;
	bool CheckComposeKeZiHu(std::vector<OutDoorCards> &vec_door_cards);
	bool AssignJiangComposeKeZiHu(std::vector<OutDoorCards> &vec_door_cards, int face);
	bool AssignJiangComposeShunZiHu(std::vector<OutDoorCards> &vec_door_cards, int face);

private :
	bool CheckComposeKeZi333(std::vector<OutDoorCards> &vec_door_cards);
	bool CheckComposeShunZi333(std::vector<OutDoorCards> &vec_door_cards);
	bool CheckComposeKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[type][value] == 3;
	}
	bool CheckComposeShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (type != MjCard::E_MYRIAD_TYPE) return false;
		if (value > 7) return false;
		return cards[type][value + 1] > 0 && cards[type][value + 2];
	}

	Card recoverCard(const int &type, const int &value) const
	{
		return (type << 4) | (value & 0x0F);
	}

	int type(const Card c) const 
	{
		return (c & 0xF0) >> 4;
	}

	int value(const Card c) const
	{
		return c & 0x0F;
	}

private:
    CardGroup card_group_;
};


} //namespace algorithm 