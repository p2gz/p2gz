#include <p2gz/p2gz.h>
#include <p2gz/gzmenu.h>
#include <p2gz/EKEditor.h>
#include <p2gz/BoundDelegate.h>

using namespace gz;
using namespace Game;

void EKEditor::init()
{
	upgrades_menu = static_cast<ListMenu*>(p2gz->menu->get_option("captain/upgrades")->get_sub_menu());
	GZASSERTLINE(upgrades_menu);

	// clang-format off
    upgrades_menu
        ->push(new ToggleMenuOption(
            "pluckaphone", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_ProfessionalNoisemaker)))
        ->push(new ToggleMenuOption(
            "mega tweeter", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_AmplifiedAmplifier)))
        ->push(new ToggleMenuOption(
            "napsack", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_FiveManNapsack)))
        ->push(new ToggleMenuOption(
            "treasure gauge", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_PrototypeDetector)))
        ->push(new ToggleMenuOption(
            "stellar orb", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_StellarOrb)))
        ->push(new ToggleMenuOption(
            "brute knuckles", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_BruteKnuckles)))
        ->push(new ToggleMenuOption(
            "rush boots", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_RepugnantAppendage)))
        ->push(new ToggleMenuOption(
            "scorch guard", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_ForgedCourage)))
        ->push(new ToggleMenuOption(
            "anti-electrifier", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_DreamMaterial)))
        ->push(new ToggleMenuOption(
            "metal suit Z", false,
            new CurriedDelegate1<EKEditor, OlimarData::ItemIndex, bool>(this, &set_upgrade, OlimarData::ODII_JusticeAlloy)));
	// clang-format on
}

void EKEditor::set_menu_opt(OlimarData::ItemIndex item, const char* opt_name)
{
	ToggleMenuOption* opt = static_cast<ToggleMenuOption*>(upgrades_menu->get_option(opt_name));
	GZASSERTLINE(opt);
	opt->set_selection(playData->mOlimarData->hasItem(item));
}

void EKEditor::check_upgrades()
{
	if (!playData) {
		return;
	}

	set_menu_opt(OlimarData::ODII_ProfessionalNoisemaker, "pluckaphone");
	set_menu_opt(OlimarData::ODII_AmplifiedAmplifier, "mega tweeter");
	set_menu_opt(OlimarData::ODII_FiveManNapsack, "napsack");
	set_menu_opt(OlimarData::ODII_PrototypeDetector, "treasure gauge");
	set_menu_opt(OlimarData::ODII_StellarOrb, "stellar orb");
	set_menu_opt(OlimarData::ODII_BruteKnuckles, "brute knuckles");
	set_menu_opt(OlimarData::ODII_RepugnantAppendage, "rush boots");
	set_menu_opt(OlimarData::ODII_ForgedCourage, "scorch guard");
	set_menu_opt(OlimarData::ODII_DreamMaterial, "anti-electrifier");
	set_menu_opt(OlimarData::ODII_JusticeAlloy, "metal suit Z");
}

BitFlag<u16> EKEditor::get_upgrades_bitfield()
{
	BitFlag<u16> bitfield;
	for (size_t i = Game::OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM; i < Game::OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM; i++) {
		const u16 mask = 1 << i;
		if (playData->mOlimarData->hasItem(i)) {
			bitfield.set(mask);
		}
	}
	return bitfield;
}

void EKEditor::set_upgrade(OlimarData::ItemIndex item, bool enabled)
{
	if (enabled) {
		playData->mOlimarData->getItem(item);

		// Avoids the treasure gauge doing its animation
		if (item == OlimarData::ODII_PrototypeDetector) {
			Game::playData->setDemoFlag(Game::DEMO_RADAR_ENABLED);
		}
	} else {
		bool validItem = item >= OlimarData::ODII_BruteKnuckles && item <= OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM;
		GZASSERTLINE(validItem);
		int data_idx = item < 8 ? 0 : 1;
		playData->mOlimarData->mFlags[data_idx] &= ~(1 << (item - (data_idx * 8)));

		if (item == OlimarData::ODII_PrototypeDetector) {
			Game::playData->mDemoFlags.resetFlag(Game::DEMO_RADAR_ENABLED);
		}
	}
}

void EKEditor::reset_all()
{
	for (int item = OlimarData::ODII_FIRST_EXPLORATION_KIT_ITEM; item < OlimarData::ODII_LAST_EXPLORATION_KIT_ITEM; item++) {
		set_upgrade(static_cast<OlimarData::ItemIndex>(item), false);
	}
}
