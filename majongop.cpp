#include "majongop.h"
#include <memory.h>
#include "majong.pb.h"

namespace algorithm {

using XGameMJProto::DoorCardType;

MaJongOp::MaJongOp(const std::vector<ICardPtr> &hand_cards)
{
	memset(card_group_, 0, sizeof(CardGroup));
	for (auto &iter : hand_cards)
	{
		card_group_[iter->type()][iter->face()] += 1;
		card_group_[iter->type()][0] += 1;
	}
}

bool MaJongOp::CheckAnGang(std::vector<uint16_t> &apcards) const
{
	std::vector<uint16_t> vec;
	for (int i = 0; i < ROW; ++i)
	{
		if (card_group_[i][0] < 4) 
			continue;

		for (int j = 1; j < CLOU; ++j)
		{
			if (card_group_[i][j] == 4)
			{
				uint16_t card = 1U << (i * 4);
				card *= j;
				vec.push_back(card);
			}
		}
	}

	if (!vec.empty())
	{
		apcards.insert(apcards.end(), vec.begin(), vec.end());
		return true;
	}

	return false;
}

bool MaJongOp::CheckComposeKeZiHu(std::vector<OutDoorCards> &vec_door_cards)
{
	std::vector<int> vec{5, 2, 8, 1, 9, 3, 4, 6, 7};
	for (auto &it : vec)
	{
		if (AssignJiangComposeKeZiHu(vec_door_cards, it))
			return true;
	}
	return false;
}
bool MaJongOp::AssignJiangComposeKeZiHu(std::vector<OutDoorCards> &vec_door_cards, int face)
{
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (card_group_[i][0] == 0)
			continue;
		if (card_group_[i][face] >= 2)
		{
			card_group_[i][face] -= 2;
			card_group_[i][0] -= 2;
			is_hu = CheckComposeKeZi333(vec_door_cards);
			card_group_[i][face] += 2;
			card_group_[i][0] += 2;
			if (is_hu)
			{
				OutDoorCards door;
				auto c = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)face, 0);
				door.card_type = DoorCardType::JIANG;
				door.cards.push_back(c);
				door.cards.push_back(c);
				vec_door_cards.push_back(door);
				return true;
			}
		}
	}
	return is_hu;
}
bool MaJongOp::AssignJiangComposeShunZiHu(std::vector<OutDoorCards> &vec_door_cards, int face)
{
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (card_group_[i][0] == 0)
			continue;
		if (card_group_[i][face] >= 2)
		{
			card_group_[i][face] -= 2;
			card_group_[i][0] -= 2;
			is_hu = CheckComposeShunZi333(vec_door_cards);
			card_group_[i][face] += 2;
			card_group_[i][0] += 2;
			if (is_hu)
			{
				OutDoorCards door;
				auto c = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)face, 0);
				door.card_type = DoorCardType::JIANG;
				door.cards.push_back(c);
				door.cards.push_back(c);
				vec_door_cards.push_back(door);
				return true;
			}
		}
	}
	return is_hu;
}
bool MaJongOp::CheckComposeKeZi333(std::vector<OutDoorCards> &vec_door_cards)
{
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (card_group_[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (card_group_[i][j] == 0) continue;

			if (CheckComposeKeZi(card_group_, i, j))
			{
				card_group_[i][j] -= 3;
				card_group_[i][0] -= 3;
				is_hu = CheckComposeKeZi333(vec_door_cards);
				card_group_[i][j] += 3;
				card_group_[i][0] += 3;
				if (is_hu)
				{
					OutDoorCards door;
					door.card_type = DoorCardType::KE_ZI;
					auto c = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)j, 0);
					door.cards.push_back(c);
					door.cards.push_back(c);
					door.cards.push_back(c);
					vec_door_cards.push_back(door);
					return true;
				}
			}

			if (CheckComposeShuZi(card_group_, i, j))
			{
				card_group_[i][j] -= 1;
				card_group_[i][j + 1] -= 1;
				card_group_[i][j + 2] -= 1;
				card_group_[i][0] -= 3;
				is_hu = CheckComposeKeZi333(vec_door_cards);
				card_group_[i][j] += 1;
				card_group_[i][j + 1] += 1;
				card_group_[i][j + 2] += 1;
				card_group_[i][0] += 3;
				if (is_hu)
				{
					OutDoorCards door;
					door.card_type = DoorCardType::SHUN_ZI;
					auto c1 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)j, 0);
					auto c2 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)(j + 1), 0);
					auto c3 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)(j + 2), 0);
					door.cards.push_back(c1);
					door.cards.push_back(c2);
					door.cards.push_back(c3);
					vec_door_cards.push_back(door);
					return true;
				}
			}
		}
	}

	int card_num = 0;
	for (int i = 0; i < ROW; ++i)
		card_num += card_group_[i][0];

	return card_num == 0;
}
bool MaJongOp::CheckComposeShunZi333(std::vector<OutDoorCards> &vec_door_cards)
{
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (card_group_[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (card_group_[i][j] == 0) continue;

			if (CheckComposeShuZi(card_group_, i, j))
			{
				card_group_[i][j] -= 1;
				card_group_[i][j + 1] -= 1;
				card_group_[i][j + 2] -= 1;
				card_group_[i][0] -= 3;
				is_hu = CheckComposeShunZi333(vec_door_cards);
				card_group_[i][j] += 1;
				card_group_[i][j + 1] += 1;
				card_group_[i][j + 2] += 1;
				card_group_[i][0] += 3;
				if (is_hu)
				{
					OutDoorCards door;
					door.card_type = DoorCardType::SHUN_ZI;
					auto c1 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)j, 0);
					auto c2 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)(j + 1), 0);
					auto c3 = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)(j + 2), 0);
					door.cards.push_back(c1);
					door.cards.push_back(c2);
					door.cards.push_back(c3);
					vec_door_cards.push_back(door);
					return true;
				}
			}

			if (CheckComposeKeZi(card_group_, i, j))
			{
				card_group_[i][j] -= 3;
				card_group_[i][0] -= 3;
				is_hu = CheckComposeShunZi333(vec_door_cards);
				card_group_[i][j] += 3;
				card_group_[i][0] += 3;
				if (is_hu)
				{
					OutDoorCards door;
					door.card_type = DoorCardType::KE_ZI;
					auto c = std::make_shared<MjCard>((MjCard::EType)i, (MjCard::EFace)j, 0);
					door.cards.push_back(c);
					door.cards.push_back(c);
					door.cards.push_back(c);
					vec_door_cards.push_back(door);
					return true;
				}
			}

		}
	}

	int card_num = 0;
	for (int i = 0; i < ROW; ++i)
		card_num += card_group_[i][0];

	return card_num == 0;
}
} //namespace algorithm 