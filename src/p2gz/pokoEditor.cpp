#include <p2gz/PokoEditor.h>
#include <p2gz/p2gz.h>
#include <Game/Entities/ItemOnyon.h>
#include <Game/gamePlayData.h>

using namespace gz;

void PokoEditor::init()
{
	pokos = static_cast<DecimalInputOption*>(p2gz->menu->get_option("items/pokos"));
}

void PokoEditor::update()
{
	if (p2gz->menu->is_active_menu("pokos")) {
		pokos->set_selected_val(Game::playData->mPokoCount);
		return;
	}

	int previous               = Game::playData->mPokoCount;
	Game::playData->mPokoCount = pokos->get_selected_val();

	// Clear flags above current repay level to reenable percent cutscenes.
	if (pokos->get_selected_val() / 1000 < previous / 1000) {
		// Game::playData->mDebtProgressFlags.clear() doesn't work, but this does!
		for (int i = 0; i < 16; i++) {
			int byte = i >> 3;
			Game::playData->getDebtProgressFlags(1 - byte) &= ~(1 << (i - (byte << 3)));
		}
		Game::playData->experienceRepayLevelFirstClear();
	}

	// Set flags up to current repay level to avoid percent cutscenes.
	if (previous / 1000 < pokos->get_selected_val() / 1000) {
		Game::playData->experienceRepayLevelFirstClear();
	}

	if (previous >= 10000 && pokos->get_selected_val() < 10000) {
		Game::playData->mStoryFlags &= ~Game::STORY_DebtPaid;
	} else if (previous < 10000 && pokos->get_selected_val() >= 10000) {
		Game::playData->mStoryFlags |= Game::STORY_DebtPaid;
	}
}
