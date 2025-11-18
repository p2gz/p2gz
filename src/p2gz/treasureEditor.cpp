#include <p2gz/TreasureEditor.h>
#include <p2gz/FreeCam.h>
#include <Game/Entities/BigTreasure.h>
#include <Game/Entities/ItemTreasure.h>
#include <Game/Entities/Nest.h>
#include <Game/Entities/OtakaraBase.h>
#include <Game/Entities/PanModokiBase.h>
#include <Game/Entities/PelletItem.h>
#include <Game/Entities/PelletOtakara.h>
#include <Game/generalEnemyMgr.h>
#include <Game/MapMgr.h>
#include <Game/pelletMgr.h>
#include <VsOtakaraName.h>
#include <p2gz/p2gz.h>

namespace gz {
void TreasureEditor::init()
{
	treasures = static_cast<ListMenu*>(p2gz->menu->get_option("level/treasures")->get_sub_menu());
}

// Find the Pellet pointer for the selected treasure.
void TreasureEditor::find_treasure()
{
	active_treasure = nullptr;

	// normal treasures
	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		if (strcmp(treasure->getConfigName(), treasures->cur_option()->title) == 0) {
			active_treasure = treasure;
		}
	}

	// exploration kit upgrades are different for some reason
	Iterator<Game::PelletItem::Object> upgradeIterator(Game::PelletItem::mgr);
	CI_LOOP(upgradeIterator)
	{
		Game::PelletItem::Object* treasure = *upgradeIterator;
		if (strcmp(treasure->getConfigName(), treasures->cur_option()->title) == 0) {
			active_treasure = treasure;
		}
	}

	// treasures held or captured by enemies
	GeneralMgrIterator<Game::EnemyBase> enemyIterator(Game::generalEnemyMgr);
	CI_LOOP(enemyIterator)
	{
		Game::EnemyBase* enemy = enemyIterator.getObject();
		if (!enemy->isAlive()) {
			continue;
		}

		handle_breadbug(enemy);
		handle_dweevil(enemy);

		if (enemy->mPelletDropCode != 0) {
			Game::PelletInitArg arg;
			Game::pelletMgr->makePelletInitArg(arg, enemy->mPelletDropCode);
			if (strcmp(arg.mTextIdentifier, treasures->cur_option()->title) == 0) {
				enemy->throwupItem();
				active_treasure = enemy->mHeldPellet;

				Game::CreatureKillArg killArg(Game::CKILL_LeaveNoCarcass);
				enemy->kill(&killArg);
			}
		}
	}

	GZASSERTLINE(active_treasure);

	// release treasure if buried
	Iterator<Game::BaseItem> buriedIterator(Game::ItemTreasure::mgr);
	CI_LOOP(buriedIterator)
	{
		Game::ItemTreasure::Item* item = static_cast<Game::ItemTreasure::Item*>(*buriedIterator);
		// ItemTreasures still exist after releasing their pellets
		if (!item->mPellet) {
			continue;
		}

		if (strcmp(active_treasure->getConfigName(), item->mPellet->getConfigName()) == 0) {
			item->mTotalLife = 0.0f;
			item->releasePellet();
		}
	}
}

// Release treasures captured by breadbugs.
void TreasureEditor::handle_breadbug(Game::EnemyBase* enemy)
{
	Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
	if (id != Game::EnemyTypeID::EnemyID_PanModoki && id != Game::EnemyTypeID::EnemyID_OoPanModoki) {
		return;
	}

	Game::PanModokiBase::Obj* breadbug = static_cast<Game::PanModokiBase::Obj*>(enemy);
	for (int i = 0; i < breadbug->mHeldTreasureNum; i++) {
		if (strcmp(breadbug->mHeldTreasures[i]->getConfigName(), treasures->cur_option()->title) == 0) {
			if (breadbug->mHeldTreasures[0]) {
				Game::PelletKillArg killArg;
				breadbug->mHeldTreasures[0]->kill(&killArg);
			}

			Game::PelletInitArg initArg;
			if (Game::pelletMgr->makePelletInitArg(initArg, breadbug->mHeldTreasures[i]->getConfigName())) {
				Game::Pellet* pellet = breadbug->mHeldTreasures[i];
				if (pellet) {
					pellet->mMgr->setComeAlive(pellet);
					initArg.mState             = Game::PelBirthType_Normal;
					initArg.mDoSkipCreateModel = 1;
					pellet->init(&initArg);

					Vector3f pos = breadbug->mHomePosition;
					pos.y += 10.0f;
					pellet->setPosition(pos, false);

					active_treasure = pellet;

					breadbug->mHeldTreasureNum--;
					breadbug->mHeldTreasures[i] = nullptr;
				}
			}
			break;
		}
	}
}

// Drop treasures carried by dweevils.
void TreasureEditor::handle_dweevil(Game::EnemyBase* enemy)
{
	Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
	if (id != Game::EnemyTypeID::EnemyID_FireOtakara && id != Game::EnemyTypeID::EnemyID_WaterOtakara
	    && id != Game::EnemyTypeID::EnemyID_GasOtakara && id != Game::EnemyTypeID::EnemyID_ElecOtakara
	    && id != Game::EnemyTypeID::EnemyID_BigTreasure) {
		return;
	}

	if (id == Game::EnemyTypeID::EnemyID_BigTreasure) {
		Game::BigTreasure::Obj* titan = static_cast<Game::BigTreasure::Obj*>(enemy);
		if (strcmp(treasures->cur_option()->title, "elec") == 0) {
			active_treasure           = titan->mTreasures[0];
			titan->mTreasureHealth[0] = 0.0f;
			titan->updateTreasure();
		} else if (strcmp(treasures->cur_option()->title, "fire") == 0) {
			active_treasure           = titan->mTreasures[1];
			titan->mTreasureHealth[1] = 0.0f;
			titan->updateTreasure();
		} else if (strcmp(treasures->cur_option()->title, "gas") == 0) {
			active_treasure           = titan->mTreasures[2];
			titan->mTreasureHealth[2] = 0.0f;
			titan->updateTreasure();
		} else if (strcmp(treasures->cur_option()->title, "water") == 0) {
			active_treasure           = titan->mTreasures[3];
			titan->mTreasureHealth[3] = 0.0f;
			titan->updateTreasure();
		} else if (strcmp(treasures->cur_option()->title, "loozy") == 0) {
			active_treasure = titan->mLouie;
			titan->releaseItemLoozy();
		}
		return;
	}

	Game::OtakaraBase::Obj* dweevil = static_cast<Game::OtakaraBase::Obj*>(enemy);
	if (dweevil->mTreasure && dweevil->mTreasure->getObjType() == OBJTYPE_Pellet) {
		Game::Pellet* treasure = static_cast<Game::Pellet*>(dweevil->mTreasure);
		if (strcmp(treasure->getConfigName(), treasures->cur_option()->title) == 0) {
			active_treasure = treasure;
			dweevil->fallTreasure(false);
		}
	}
}

// Enable treasure editor.
void TreasureEditor::enable()
{
	enabled = true;

	find_treasure();
	initial_position = active_treasure->getPosition();

	p2gz->waypoint_viewer->toggle(true);
	p2gz->freecam->enable();
	p2gz->freecam->set_position(active_treasure->getPosition());
}

// Snap the active treasure to the nearest waypoint.
void TreasureEditor::snap_to_nearest_waypoint()
{
	Game::WayPoint* nearest = nullptr;
	Iterator<Game::WayPoint> iterator(Game::mapMgr->mRouteMgr);
	CI_LOOP(iterator)
	{
		Game::WayPoint* wp = *iterator;
		if (!nearest) {
			nearest = wp;
			continue;
		}

		Vector3f treasurePos = active_treasure->getPosition();
		Vector3f currentPos  = wp->getPosition();
		Vector3f nearestPos  = nearest->getPosition();
		if (sqrDistanceXZ(treasurePos, currentPos) < sqrDistanceXZ(treasurePos, nearestPos)) {
			nearest = wp;
		}
	}

	GZASSERTLINE(nearest);
	Vector3f pos = Vector3f(nearest->getPosition().x, p2gz->freecam->get_position().y, nearest->getPosition().z);
	p2gz->freecam->set_position(pos);
}

void TreasureEditor::sync()
{
	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		add(treasure);
	}

	Iterator<Game::PelletItem::Object> upgradeIterator(Game::PelletItem::mgr);
	CI_LOOP(upgradeIterator)
	{
		Game::PelletItem::Object* treasure = *upgradeIterator;
		add(treasure);
	}
}

// Add a submenu for the given treasure.
void TreasureEditor::add(Game::Pellet* pellet)
{
	Game::PelletItem::Object* treasure = static_cast<Game::PelletItem::Object*>(pellet);

	for (int i = 0; i < treasures->options.len(); i++) {
		if (strcmp(treasures->options[i]->title, treasure->getConfigName()) == 0) {
			return;
		}
	}

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();
	// clang-format off
	treasures->push(new OpenSubMenuOption(treasure->getConfigName(), (new ListMenu())
		->push(new PerformActionMenuOption("move", new Delegate<TreasureEditor>(p2gz->treasure_editor, &TreasureEditor::enable)))
		->push(new ToggleMenuOption("collected", false, new Delegate1<TreasureEditor, bool>(p2gz->treasure_editor, &TreasureEditor::toggle_collected)))
	));
	prev_heap->becomeCurrentHeap();
	// clang-format on
}

// Toggle whether the given treasure is collected. Disables move and sets collected.
void TreasureEditor::set_collected(Game::Pellet* pellet, bool collected)
{
	Game::PelletItem::Object* treasure = static_cast<Game::PelletItem::Object*>(pellet);
	for (int i = 0; i < treasures->options.len(); i++) {
		if (strcmp(treasures->options[i]->title, treasure->getConfigName()) == 0) {
			ListMenu* menu                    = static_cast<ListMenu*>(treasures->get_option(treasure->getConfigName())->get_sub_menu());
			menu->get_option("move")->visible = !collected;
			static_cast<ToggleMenuOption*>(menu->get_option("collected"))->set_selection(collected);
			return;
		}
	}
	GZASSERTLINE(false);
}

// Clear all treasures on transit.
void TreasureEditor::clear_treasures()
{
	if (treasures) {
		treasures->clear();
	}
}
} // namespace gz
