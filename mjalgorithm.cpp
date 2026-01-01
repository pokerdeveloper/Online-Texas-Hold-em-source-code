/*
 * @file mjalgorithm.cpp
 * @author YangSongxi
 * @mail ysx-cpp@gmail.com
 * @github https://github.com/ysx-cpp
 * @date Oct 08 2020
 */
#include "mjalgorithm.h"
#include <memory.h>
#include <assert.h>
#include <complex>
#include <iostream>

namespace algorithm {

using XGameMJProto::DoorCardType;

MjAlgorithm::MjAlgorithm()
{
}

MjAlgorithm::~MjAlgorithm()
{
}

void MjAlgorithm::InputHandCard(const std::vector<ICardPtr> &hand_cards)
{
	hand_cards_info_.clear();
	memset(card_group_, 0, sizeof(CardGroup));
	door_group_.clear();

	for (auto &iter : hand_cards)
	{
		if (iter->type() == type(laizi_card_) && iter->face() == value(laizi_card_))
			continue;

		card_group_[iter->type()][iter->face()] += 1;
		card_group_[iter->type()][0] += 1;

		auto c = recoverCard(iter->type(), iter->face());
		hand_cards_info_[c].push_back(iter->index());
	}
}

void MjAlgorithm::InputHandCard(const std::vector<uint16_t> &hand_cards)
{
	std::vector<ICardPtr> cards;
	for (auto &it : hand_cards)
	{
		MjCard::EType type = MjCard::E_TYPE_COUNT_;
		MjCard::EFace face = MjCard::E_FACE_COUNT_;
		if (it & 0x000f)
		{
			type = MjCard::E_MYRIAD_TYPE;
			uint16_t v = it & 0x000f;
			face = static_cast<MjCard::EFace>(v);
		}
		else if (it & 0x00f0)
		{
			type = MjCard::E_WIND_TYPE;
			uint16_t v = it & 0x00f0;
			face = static_cast<MjCard::EFace>(v >> 4);
		}
		else if (it & 0x0f00)
		{
			type = MjCard::E_WORD_TYPE;
			uint16_t v = it & 0x0f00;
			face = static_cast<MjCard::EFace>(v >> 8);
		}	
		else if (it & 0xf000)
		{
			type = MjCard::E_FLOWER_TYPE;
			uint16_t v = it & 0xf000;
			face = static_cast<MjCard::EFace>(v >> 12);
		}
		else
			assert(false);

		ICardPtr mjc = std::make_shared<MjCard>(type, face, 0);
		cards.push_back(mjc);
		//std::cout << mjc->type() << "|" << mjc->face() << std::endl;
	}
	InputHandCard(cards);
}

void MjAlgorithm::OutPutDoorCards(std::vector<OutDoorCards> &vec_door_cards) const
{
	for (auto &it : door_group_)
	{
		OutDoorCards door;
		for (auto &iter : it->cards)
		{
			// if (hand_cards_info_.find(iter) != hand_cards_info_.end() && !hand_cards_info_[iter].empty())
			// {
			// 	int index = hand_cards_info_[iter].back();
			// 	door_ptr->m_vSelfCard.push_back(index);
			// 	hand_cards_info_[iter].pop_back();
			// }

			MjCard::EType type = (MjCard::EType)this->type(iter);
			MjCard::EFace face = (MjCard::EFace)this->value(iter);
			ICardPtr mjc = std::make_shared<MjCard>(type, face, 0);
			door.cards.push_back(mjc);

			std::cout << mjc->type() << "|" << mjc->face() << std::endl;
		}
		door.card_type = it->card_type;
		vec_door_cards.push_back(door);
		std::cout << "door_type:" << (int)door.card_type << std::endl;
	}
}

bool MjAlgorithm::CheckPingHu()
{
	return CheckJiangCanHu(card_group_);
}

bool MjAlgorithm::CheckQiDuiHu()
{
	return QiDuiHu(card_group_);
}

bool MjAlgorithm::CheckShiSanYaoHu()
{
	return ShiSanYaoHu(card_group_);
}

bool MjAlgorithm::CheckJiangCanHu(CardGroup cards)
{
	if (CheckAssignJiangHu(cards))
	{
		return true;
	}

	if (laizi_num() >= 2 && CheckLaiZiJiangHu(cards))
	{
		return true;
	}

	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			//if (cards[i][j] > 2) continue;
			if (cards[i][j] >= 2)
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::JIANG;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (cards[i][j] >= 1 && laizi_num() >= 1)
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::JIANG;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}
	return is_hu;
}

bool MjAlgorithm::CheckAssignJiangHu(CardGroup cards)
{
	std::vector<int> vec {5, 2, 8, 1, 9};
	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (auto &j : vec)
		{
			if (cards[i][j] >= 2)
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::JIANG;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (cards[i][j] >= 1 && laizi_num() >= 1)
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::JIANG;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}
	return is_hu;
}

bool MjAlgorithm::CheckLaiZiJiangHu(CardGroup cards)
{
	bool is_hu = false;

	laizi_num_ -= 2;
	is_hu = CheckCompose333(cards);
	laizi_num_ += 2;

	if (is_hu)
	{
		auto door_card = std::make_shared<DoorCards>();
		door_card->card_type = DoorCardType::JIANG;
		door_card->cards.push_back(laizi_card_);
		door_card->cards.push_back(laizi_card_);
		door_group_.push_back(door_card);
		return true;
	}
	return is_hu;
}

bool MjAlgorithm::CheckCompose333(CardGroup cards)
{
	if (laizi_num() != 0 && laizi_num() % 3 == 0 && cards[0][0] + cards[1][0] == 0)
	{
		auto door_card = std::make_shared<DoorCards>();
		for (int i = 0; i < laizi_num(); ++i)
			door_card->cards.push_back(laizi_card_);

		door_group_.push_back(door_card);
		return true;
	}

	bool is_hu = false;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (cards[i][j] == 0) continue;

			if (CheckComposeShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 1] -= 1;
				cards[i][j + 2] -= 1;
				cards[i][0] -= 3;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 1] += 1;
				cards[i][j + 2] += 1;
				cards[i][0] += 3;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::SHUN_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 1));
					door_card->cards.push_back(recoverCard(i, j + 2));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeKeZi(cards, i, j))
			{
				cards[i][j] -= 3;
				cards[i][0] -= 3;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 3;
				cards[i][0] += 3;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::KE_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeLaiZiShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 1] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 1] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::SHUN_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 1));
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeLaiZiKeZi(cards, i, j))
			{
				cards[i][j] -= 2;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 2;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::KE_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeMiddleLaiZiShuZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][j + 2] -= 1;
				cards[i][0] -= 2;
				laizi_num_ -= 1;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][j + 2] += 1;
				cards[i][0] += 2;
				laizi_num_ += 1;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::SHUN_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(recoverCard(i, j + 2));
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
					return true;
				}
			}

			if (CheckComposeTwoLaiZiKeZi(cards, i, j))
			{
				cards[i][j] -= 1;
				cards[i][0] -= 1;
				laizi_num_ -= 2;
				is_hu = CheckCompose333(cards);
				cards[i][j] += 1;
				cards[i][0] += 1;
				laizi_num_ += 2;
				if (is_hu)
				{
					auto door_card = std::make_shared<DoorCards>();
					door_card->card_type = DoorCardType::KE_ZI;
					door_card->cards.push_back(recoverCard(i, j));
					door_card->cards.push_back(laizi_card_);
					door_card->cards.push_back(laizi_card_);
					door_group_.push_back(door_card);
					return true;
				}
			}
		}
	}

	int card_num = 0;
	for (int i = 0; i < ROW; ++i)
		card_num += cards[i][0];

	return laizi_num_ % 3 == 0 && card_num == 0;
}

bool MjAlgorithm::QiDuiHu(CardGroup cards)
{
	int single_cnt = 0;
	int all_card = 0;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		all_card += cards[i][0];
		if (cards[i][0] == 1)
		{
			++single_cnt;
			continue;
		}
		for (unsigned j = 1; j < CLOU; j++)
		{
			if (cards[i][j] == 0) continue;
			if (cards[i][j] & 1) //�������������߶�
			{
				single_cnt++;
			}
		}
	}
	//1.����+���Ʋ�����14���Ƽ����ܺ��߶�
	//2.���������ݲ����ں����������ܺ��߶�
	if ((all_card + laizi_num_ != 14) || single_cnt != laizi_num_)
	{
		return false;
	}

	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (unsigned j = 1; j < CLOU; j++)
		{
			int num = cards[i][j];
			while (num >= 2)
			{
				auto door_card = std::make_shared<DoorCards>();
				door_card->card_type = DoorCardType::JIANG;
				door_card->cards.push_back(recoverCard(i, j));
				door_card->cards.push_back(recoverCard(i, j));
				door_group_.push_back(door_card);
				num -= 2;
			}
		}
	}
	return true;
}

bool MjAlgorithm::ShiSanYaoHu(CardGroup cards)
{
	int jiang_num = 0;
	int count = 0;
	for (int i = 0; i < ROW; ++i)
	{
		if (cards[i][0] == 0) continue;
		for (int j = 1; j < CLOU; ++j)
		{
			if (i < 3 && j != 1 && j != 9) //�� Ͳ ��
			{
				return false;
			}

			if (jiang_num == 0 && cards[i][j] == 2)
			{
				jiang_num = 1;
			}
			if (jiang_num == 1 && cards[i][j] != 1)
			{
				return false;
			}
			else
			{
				++count;
			}
		}
	}
	return count + laizi_num_ + jiang_num * 2 == 14;
}

} //namespace algorithm
