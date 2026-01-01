/*
 * @file mjalgorithm.h
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#ifndef MAJONG_MJ_ALGORITHM_H
#define MAJONG_MJ_ALGORITHM_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include "mjcard.h"
#include "majong.pb.h"

namespace algorithm {

class MjAlgorithm
{
	static const int32_t ROW = MjCard::E_TYPE_COUNT_;
	static const int32_t CLOU = MjCard::E_FACE_COUNT_;

	using Card = uint8_t;
	using CardGroup = Card[ROW][CLOU];

public:
	MjAlgorithm();
	~MjAlgorithm();

	void InputHandCard(const std::vector<ICardPtr> &hand_cards);
	void InputHandCard(const std::vector<uint16_t> &hand_cards);
	void OutPutDoorCards(std::vector<OutDoorCards> &vec_door_cards) const;
	bool CheckPingHu();
	bool CheckQiDuiHu();
	bool CheckShiSanYaoHu();

	void set_laizi_num(const int num) { laizi_num_ = num; }
	int laizi_num() const { return laizi_num_; }
	void set_laizi_card(const Card card) { laizi_card_ = card; }
	const Card &laizi_card() const { return laizi_card_; }

private:
	bool CheckJiangCanHu(CardGroup cards);
	bool CheckAssignJiangHu(CardGroup cards);
	bool CheckLaiZiJiangHu(CardGroup cards);
	bool CheckCompose333(CardGroup cards);
	bool QiDuiHu(CardGroup cards);
	bool ShiSanYaoHu(CardGroup cards);

public:
	bool CheckComposeShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (type != MjCard::E_MYRIAD_TYPE) return false;
		if (value > 7) return false;
		return cards[type][value + 1] > 0 && cards[type][value + 2];
	}

	bool CheckComposeKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		return cards[type][value] == 3;
	}

	//11%
	bool CheckComposeLaiZiKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() <= 0) return false;
		return cards[type][value] >= 2;
	}

	//12%
	bool CheckComposeLaiZiShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() == 0 || value >= 10) return false;
		return cards[type][value + 1] > 0;
	}

	//1%3
	bool CheckComposeMiddleLaiZiShuZi(const CardGroup cards, const int &type, const int &value) const
	{
		if (laizi_num() == 0 || value >= 9) return false;
		return cards[type][value + 2];
	}

	//1%%
	bool CheckComposeTwoLaiZiKeZi(const CardGroup cards, const int &type, const int &value) const
	{
		return laizi_num() >= 2 && cards[type][value] > 0;
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
	int laizi_num_ = 0;
	Card laizi_card_ = 0;
	CardGroup card_group_;

	struct DoorCards
	{
		XGameMJProto::DoorCardType card_type = XGameMJProto::INVALID;
		std::vector<Card> cards;
	};
	std::vector<std::shared_ptr<DoorCards>> door_group_;
	std::map<Card, std::vector<int>> hand_cards_info_;
};

} //namespace algorithm

#endif //MAJONG_MJ_ALGORITHM_H