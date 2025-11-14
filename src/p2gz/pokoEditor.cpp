#include <p2gz/PokoEditor.h>
#include <p2gz/p2gz.h>
#include <p2gz/HelperInlines.h>
#include <Game/Entities/ItemOnyon.h>
#include <Game/gamePlayData.h>

using namespace gz;

void PokoEditor::init()
{
	poko_menu = static_cast<DecimalInputOption*>(p2gz->menu->get_option("captain/pokos"));
}

void PokoEditor::sync()
{
	poko_menu->set_selected_val(get_pokos());
}

void PokoEditor::apply_cave_pokos()
{
	Game::playData->mPokoCount     = Game::playData->mPokoCount + Game::playData->mCavePokoCount;
	Game::playData->mCavePokoCount = 0;
	sync();
}

u32 PokoEditor::get_pokos()
{
	if (in_cave_play()) {
		return Game::playData->mPokoCount + Game::playData->mCavePokoCount;
	}
	return Game::playData->mPokoCount;
}

void PokoEditor::set_pokos(u32 pokos)
{
	int previous;
	if (in_cave_play()) {
		previous                       = Game::playData->mPokoCount + Game::playData->mCavePokoCount;
		Game::playData->mCavePokoCount = pokos - Game::playData->mPokoCount;
	} else {
		previous                   = Game::playData->mPokoCount;
		Game::playData->mPokoCount = pokos;
	}

	// Clear flags above current repay level to reenable percent cutscenes.
	if (pokos / 1000 < previous / 1000) {
		// Game::playData->mDebtProgressFlags.clear() doesn't work, but this does!
		for (int i = 0; i < 16; i++) {
			int byte = i >> 3;
			Game::playData->getDebtProgressFlags(1 - byte) &= ~(1 << (i - (byte << 3)));
		}
		Game::playData->experienceRepayLevelFirstClear();
	}

	// Set flags up to current repay level to avoid percent cutscenes.
	if (previous / 1000 < pokos / 1000) {
		Game::playData->experienceRepayLevelFirstClear();
	}

	if (previous >= 10000 && pokos < 10000) {
		Game::playData->mStoryFlags &= ~Game::STORY_DebtPaid;
	} else if (previous < 10000 && pokos >= 10000) {
		Game::playData->mStoryFlags |= Game::STORY_DebtPaid;
	}
}
