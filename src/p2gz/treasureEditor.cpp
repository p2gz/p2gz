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

using namespace gz;

void TreasureEditor::init()
{
	treasures = static_cast<ListMenu*>(p2gz->menu->get_option("level/treasures")->get_sub_menu());
}

// Release treasures captured by breadbugs.
Game::Pellet* handle_breadbug(Game::EnemyBase* enemy, const char* config_name)
{
	Game::PanModokiBase::Obj* breadbug = static_cast<Game::PanModokiBase::Obj*>(enemy);
	for (int i = 0; i < breadbug->mHeldTreasureNum; i++) {
		if (strcmp(config_name, breadbug->mHeldTreasures[i]->getConfigName()) == 0) {
			Game::PelletInitArg initArg;
			Game::pelletMgr->makePelletInitArg(initArg, breadbug->mHeldTreasures[i]->getConfigName());
			Game::Pellet* pellet = breadbug->mHeldTreasures[i];
			GZEXPECT(pellet, "breadbug pellet bad");

			pellet->mMgr->setComeAlive(pellet);
			initArg.mState             = Game::PelBirthType_Normal;
			initArg.mDoSkipCreateModel = 1;
			pellet->init(&initArg);

			Vector3f pos = breadbug->mHomePosition;
			pos.y += 10.0f;
			pellet->setPosition(pos, false);

			breadbug->mHeldTreasureNum--;
			breadbug->mHeldTreasures[i] = nullptr;

			return pellet;
		}
	}

	return nullptr;
}

// Drop treasures carried by dweevils.
Game::Pellet* handle_dweevil(Game::EnemyBase* enemy, const char* config_name)
{
	Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
	if (id == Game::EnemyTypeID::EnemyID_BigTreasure) {
		Game::BigTreasure::Obj* titan = static_cast<Game::BigTreasure::Obj*>(enemy);
		if (strcmp(config_name, "elec") == 0) {
			titan->mTreasureHealth[0] = 0.0f;
			titan->updateTreasure();
			return titan->mTreasures[0];
		}
		if (strcmp(config_name, "fire") == 0) {
			titan->mTreasureHealth[1] = 0.0f;
			titan->updateTreasure();
			return titan->mTreasures[1];
		}
		if (strcmp(config_name, "gas") == 0) {
			titan->mTreasureHealth[2] = 0.0f;
			titan->updateTreasure();
			return titan->mTreasures[2];
		}
		if (strcmp(config_name, "water") == 0) {
			titan->mTreasureHealth[3] = 0.0f;
			titan->updateTreasure();
			return titan->mTreasures[3];
		}
		if (strcmp(config_name, "loozy") == 0) {
			titan->releaseItemLoozy();
			return titan->mLouie;
		}
	} else {
		Game::OtakaraBase::Obj* dweevil = static_cast<Game::OtakaraBase::Obj*>(enemy);
		if (dweevil->mTreasure && dweevil->mTreasure->getObjType() == OBJTYPE_Pellet) {
			Game::Pellet* treasure = static_cast<Game::Pellet*>(dweevil->mTreasure);
			if (strcmp(treasure->getConfigName(), config_name) == 0) {
				dweevil->fallTreasure(false);
				return treasure;
			}
		}
	}

	return nullptr;
}

Game::Creature* find_treasure(const char* config_name)
{
	GZEXPECT(config_name, "cannot find treasure without config name");

	// normal treasures
	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		if (treasure->isAlive() && strcmp(treasure->getConfigName(), config_name) == 0) {
			return treasure;
		}
	}

	// exploration kit upgrades
	Iterator<Game::PelletItem::Object> upgradeIterator(Game::PelletItem::mgr);
	CI_LOOP(upgradeIterator)
	{
		Game::PelletItem::Object* treasure = *upgradeIterator;
		if (treasure->isAlive() && strcmp(treasure->getConfigName(), config_name) == 0) {
			return treasure;
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

		// Any normal enemy with a treasure inside
		if (enemy->mPelletDropCode != 0) {
			Game::PelletInitArg arg;
			Game::pelletMgr->makePelletInitArg(arg, enemy->mPelletDropCode);
			if (strcmp(arg.mTextIdentifier, config_name) == 0) {
				return enemy;
			}
		}

		// Breadbugs
		Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
		if (id == Game::EnemyTypeID::EnemyID_PanModoki || id == Game::EnemyTypeID::EnemyID_OoPanModoki) {
			Game::PanModokiBase::Obj* breadbug = static_cast<Game::PanModokiBase::Obj*>(enemy);
			for (int i = 0; i < breadbug->mHeldTreasureNum; i++) {
				if (strcmp(config_name, breadbug->mHeldTreasures[i]->getConfigName()) == 0) {
					return enemy;
				}
			}
			continue;
		}
	}

	return nullptr;
}

// Release the treasure if it's held by an enemy or buried
Game::Pellet* release_treasure(Game::Creature* creature, const char* config_name)
{
	GZEXPECT(creature, "cannot release null creature");
	GZEXPECT(creature->isAlive(), "cannot release from dead creature");

	if (creature->isTeki()) {
		Game::EnemyBase* enemy             = static_cast<Game::EnemyBase*>(creature);
		Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
		if (id == Game::EnemyTypeID::EnemyID_PanModoki || id == Game::EnemyTypeID::EnemyID_OoPanModoki) {
			Game::Pellet* released = handle_breadbug(enemy, config_name);
			if (released) {
				return released;
			}
		}

		if (enemy->mPelletDropCode != 0) {
			Game::PelletInitArg arg;
			Game::pelletMgr->makePelletInitArg(arg, enemy->mPelletDropCode);
			if (strcmp(arg.mTextIdentifier, config_name) == 0) {
				enemy->throwupItem();
				Game::Pellet* pellet = enemy->mHeldPellet;
				Game::CreatureKillArg killArg(Game::CKILL_LeaveNoCarcass);
				enemy->kill(&killArg);
				return pellet;
			}
		}

		GZEXPECT(false, "can't release treasure from enemy id %d", id);
	} else {
		// release treasure if buried
		Iterator<Game::BaseItem> buriedIterator(Game::ItemTreasure::mgr);
		CI_LOOP(buriedIterator)
		{
			Game::ItemTreasure::Item* item = static_cast<Game::ItemTreasure::Item*>(*buriedIterator);
			// ItemTreasures still exist after releasing their pellets
			if (!item->mPellet) {
				continue;
			}

			if (strcmp(config_name, item->mPellet->getConfigName()) == 0) {
				item->mTotalLife = 0.0f;
				item->releasePellet();
				return static_cast<Game::Pellet*>(creature);
			}
		}

		// Release the treasure from dweevils if it's being grabbed
		GeneralMgrIterator<Game::EnemyBase> enemyIterator(Game::generalEnemyMgr);
		CI_LOOP(enemyIterator)
		{
			Game::EnemyBase* enemy = enemyIterator.getObject();
			if (!enemy->isAlive()) {
				continue;
			}

			Game::EnemyTypeID::EEnemyTypeID id = enemy->getEnemyTypeID();
			if (id == Game::EnemyTypeID::EnemyID_FireOtakara || id == Game::EnemyTypeID::EnemyID_WaterOtakara
			    || id == Game::EnemyTypeID::EnemyID_GasOtakara || id == Game::EnemyTypeID::EnemyID_ElecOtakara
			    || id == Game::EnemyTypeID::EnemyID_BigTreasure) {
				Game::Pellet* released_pellet = handle_dweevil(enemy, config_name);
				if (released_pellet) {
					return released_pellet;
				}
			}
		}

		return static_cast<Game::Pellet*>(creature);
	}
}

// Enable treasure editor.
void TreasureEditor::start_move(const char* treasure_name)
{
	// either the treasure itself or the creature holding it
	Game::Creature* creature = find_treasure(treasure_name);
	if (!creature || !creature->isAlive()) {
		active_treasure = spawn_treasure(treasure_name);
	} else {
		active_treasure = release_treasure(creature, treasure_name);
	}

	if (!active_treasure) {
		OSReport("couldn't find treasure %s to move\n", treasure_name);
		return;
	}

	enabled = true;
	p2gz->waypoint_viewer->toggle(true);
	p2gz->freecam->enable();
	p2gz->freecam->set_position(active_treasure->getPosition());
	initial_position = active_treasure->getPosition();
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
	// Rebuild from scratch so the list reflects the current area (no stale cross-area entries).
	clear_treasures();

	// Live treasures (uncollected / currently spawned), plus exploration-kit upgrades.
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

	// Add treasures that have already been collected by finding them in the generator cache.
	if (in_above_ground_play()) {
		FOREACH_NODE(Game::Generator, Game::generatorCache->getFirstGenerator(), gen)
		{
			if (!gen->mObject) {
				continue;
			}

			if (gen->mObject->mTypeID == 'pelt') {
				Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
				// only real treasures and exploration-kit upgrades, not number/other pellets
				if (gen_pellet->mPelType != Game::PelletList::PLK_Otakara && gen_pellet->mPelType != Game::PelletList::PLK_Item) {
					continue;
				}
				if (!gen_pellet->mGenParm) {
					continue;
				}
				Game::PelletConfig* cfg
				    = Game::PelletList::Mgr::mInstance->getConfig(gen_pellet->mPelType)->getPelletConfig(gen_pellet->mGenParm->mIndex);
				if (cfg) {
					add(cfg->mParams.mName.mData);
				}
			} else if (gen->mObject->mTypeID == 'teki') {
				// enemies that carry a treasure (e.g. an enemy the preset didn't spawn)
				Game::GenObjectEnemy* gen_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
				if (gen_enemy->mOtakaraItemCode.isNull()) {
					continue;
				}
				int kind                = gen_enemy->mOtakaraItemCode.getPelletKind();
				int index               = gen_enemy->mOtakaraItemCode.getPelletIndex();
				Game::PelletConfig* cfg = Game::PelletList::Mgr::mInstance->getConfig(kind)->getPelletConfig(index);
				if (cfg) {
					add(cfg->mParams.mName.mData);
				}
			}
		}
	}
}

// Add a submenu for the given live treasure.
void TreasureEditor::add(Game::Pellet* treasure)
{
	add(treasure->getConfigName());
}

// Add a submenu for the treasure with the given config name. Works without a live pellet so that
// preset-collected treasures (generator present, no live creature) can still be listed and edited.
void TreasureEditor::add(const char* treasure_name)
{
	for (int i = 0; i < treasures->options.len(); i++) {
		if (strcmp(treasures->options[i]->title, treasure_name) == 0) {
			return;
		}
	}

	JKRHeap* prev_heap = sys->mSysHeap->becomeCurrentHeap();

	// clang-format off
	ToggleMenuOption* collected_opt = new ToggleMenuOption(
		"collected", false, new CurriedDelegate1<TreasureEditor, const char*, bool>(this, &set_collected, treasure_name));

	OpenSubMenuOption* treasure_opt = new OpenSubMenuOption(treasure_name, (new ListMenu(
	    new BoundDelegate2<TreasureEditor, const char*, ToggleMenuOption*>(this, &sync_treasure_option, treasure_name, collected_opt)))
		->push(new PerformActionMenuOption("move", new BoundDelegate1<TreasureEditor, const char*>(this, &start_move, treasure_name)))
		->push(collected_opt));

	treasure_opt->on_focus = new BoundDelegate1<TreasureEditor, const char*>(this, &focus_treasure, treasure_name);

	treasures->push(treasure_opt);
	// clang-format on

	prev_heap->becomeCurrentHeap();
}

void TreasureEditor::focus_treasure(const char* treasure_name)
{
	Game::Creature* treasure = find_treasure(treasure_name);

	// Move camera to treasure if we found it
	if (treasure) {
		Game::PlayCamera* camera = Game::cameraMgr->mCameraObjList[p2gz->navi_tools->active_navi()->getNaviID()];
		if (camera) {
			treasure->mLod.setFlag(AILOD_IsVisibleBoth);
			camera->mGoalPosition = treasure->getPosition();
		}
	}
}

void TreasureEditor::sync_treasure_option(const char* treasure_name, ToggleMenuOption* treasure_collected_opt)
{
	Game::Creature* treasure = find_treasure(treasure_name);
	treasure_collected_opt->set_selection(!treasure || !treasure->isAlive());
}

Game::Pellet* birth_pellet(Game::PelletConfig* cfg, const char* config_name, int kind)
{
	Game::PelletInitArg arg;
	arg.mDontCheckCollected = true;
	arg.mTextIdentifier     = const_cast<char*>(config_name);
	arg.mPelletType         = kind;
	arg.mPelletIndex        = cfg->mParams.mIndex;
	arg.mPelView            = nullptr;
	return Game::pelletMgr->birth(&arg);
}

Game::Pellet* TreasureEditor::spawn_treasure(const char* config_name)
{
	// Find the treasure config so we can look up its index
	Game::PelletConfig* cfg      = nullptr;
	Game::PelletList::cKind kind = Game::PelletList::PLK_Otakara;
	for (int i = (int)Game::PelletList::PLK_Otakara; i < (int)Game::PelletList::PLK_Item; i++) {
		Game::PelletConfigList* config_list = Game::PelletList::Mgr::mInstance->getConfigList(static_cast<Game::PelletList::cKind>(i));
		cfg                                 = config_list->getPelletConfig(const_cast<char*>(config_name));
		if (cfg) {
			kind = static_cast<Game::PelletList::cKind>(i);
			break;
		}
	}
	GZEXPECT(cfg, "no pellet config found for %s", config_name);

	if (in_above_ground_play()) {
		FOREACH_NODE(Game::Generator, Game::generatorCache->getFirstGenerator(), gen)
		{
			if (gen->mObject->mTypeID == 'pelt') {
				Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
				int treasure_id             = gen_pellet->mGenParm->mIndex;
				int kind                    = gen_pellet->mPelType;
				const char* treasure_name
				    = Game::PelletList::Mgr::mInstance->getConfig(kind)->getPelletConfig(treasure_id)->mParams.mName.mData;
				if ((!gen->mCreature || !gen->mCreature->isAlive()) && strcmp(treasure_name, config_name) == 0) {
					gen->generate();
					GZEXPECT(gen->mCreature, "treasure generator failed");
					return static_cast<Game::Pellet*>(gen->mCreature);
				}
			} else if (gen->mObject->mTypeID == 'teki') {
				Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
				if (gen_obj_enemy->mOtakaraItemCode.getPelletKind() == kind
				    && gen_obj_enemy->mOtakaraItemCode.getPelletIndex() == cfg->mParams.mIndex) {
					// Respawning the enemy is difficult because many of them are handled differently after
					// dying or when not spawned. This is left as an improvement for the future.
					// Instead we just spawn the treasure where the enemy would be.
					Game::Pellet* pellet = birth_pellet(cfg, config_name, kind);
					pellet->setPosition(gen->mPosition, false);
					return pellet;
				}
			}
		}
		GZEXPECT(false, "no suitable pellet or teki generator found for treasure %s", config_name);
	} else if (in_cave_play()) {
		Game::Pellet* pellet = birth_pellet(cfg, config_name, kind);

		// We don't know which spawn point the treasure spawned at, so just place it on the active captain.
		Game::Navi* navi = p2gz->navi_tools->active_navi();
		f32 y            = Game::mapMgr->getMinY(navi->mPosition) + (pellet->getCylinderHeight() * 0.5f);
		Vector3f pos     = Vector3f(navi->mPosition.x, y, navi->mPosition.z);
		pellet->setPosition(pos, false);
		return pellet;
	} else {
		GZEXPECT(false, "must be in cave or AG to spawn treasure");
	}
}

// Toggle whether the given treasure is collected. Disables move and sets collected.
void TreasureEditor::set_collected(const char* treasure_name, bool collected)
{
	Game::Creature* creature = find_treasure(treasure_name);
	if (collected) {
		if (!creature || !creature->isAlive()) {
			return;
		}

		Game::CreatureKillArg arg(Game::CKILL_DontCountAsDeath);
		creature->kill(&arg);
	} else {
		if (creature && creature->isAlive()) {
			return;
		}
		spawn_treasure(treasure_name);
		focus_treasure(treasure_name);
	}
}

// Clear all treasures on transit.
void TreasureEditor::clear_treasures()
{
	if (treasures) {
		treasures->clear();
	}
}
