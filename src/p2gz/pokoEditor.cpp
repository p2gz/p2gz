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
	if (p2gz->menu->is_open() && p2gz->menu->get_active_layer() && p2gz->menu->get_active_layer()->title
	    && strcmp(p2gz->menu->get_active_layer()->title, "pokos") == 0) {
		open = true;
		if (Game::playData->mPokoCount >= 10000 && pokos->get_selected_val() < 10000) {
			Game::playData->mStoryFlags &= ~Game::STORY_DebtPaid;
		} else if (Game::playData->mPokoCount < 10000 && pokos->get_selected_val() >= 10000) {
			Game::playData->mStoryFlags |= Game::STORY_DebtPaid;
		}
		Game::playData->mPokoCount = pokos->get_selected_val();

		return;
	}

	pokos->set_selected_val(Game::playData->mPokoCount);
}
