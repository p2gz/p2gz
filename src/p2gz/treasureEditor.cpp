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

// min size of the largest (contiguous) free block we require before spawning a treasure
static const u32 MIN_FREE_HEAP_FOR_SPAWN = 0x4000; // i.e. 16KB

// true if the current heap has room for a treasure model
static bool heap_has_room_for_spawn()
{
	JKRHeap* heap = JKRHeap::getCurrentHeap();
	if (!heap) {
		// default to spawning, just in case
		return true;
	}
	return heap->getFreeSize() >= MIN_FREE_HEAP_FOR_SPAWN;
}

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
		// most likely the treasure couldn't be spawned because the heap is too low
		if (!heap_has_room_for_spawn()) {
			p2gz->show_callout("No Memory To Spawn Treasure");
		}
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

// no generator in caves => record each treasure's position on spawn so we can respawn it
void TreasureEditor::update()
{
	if (!in_cave_play()) {
		return;
	}

	Iterator<Game::PelletOtakara::Object> treasureIterator(Game::PelletOtakara::mgr);
	CI_LOOP(treasureIterator)
	{
		Game::PelletOtakara::Object* treasure = *treasureIterator;
		if (treasure->isAlive()) {
			record_spawn_position(treasure->getConfigName(), treasure->getPosition());
		}
	}

	Iterator<Game::PelletItem::Object> upgradeIterator(Game::PelletItem::mgr);
	CI_LOOP(upgradeIterator)
	{
		Game::PelletItem::Object* treasure = *upgradeIterator;
		if (treasure->isAlive()) {
			record_spawn_position(treasure->getConfigName(), treasure->getPosition());
		}
	}
}

void TreasureEditor::record_spawn_position(const char* config_name, const Vector3f& position)
{
	for (size_t i = 0; i < spawn_positions.len(); i++) {
		if (strcmp(spawn_positions[i].name, config_name) == 0) {
			// already recorded
			return;
		}
	}

	TreasureSpawn entry;
	entry.name     = config_name;
	entry.position = position;
	spawn_positions.push(entry);
}

bool TreasureEditor::get_spawn_position(const char* config_name, Vector3f& out)
{
	for (size_t i = 0; i < spawn_positions.len(); i++) {
		if (strcmp(spawn_positions[i].name, config_name) == 0) {
			out = spawn_positions[i].position;
			return true;
		}
	}
	return false;
}

void TreasureEditor::sync()
{
	// clear menu to avoid stale copies (but not treasure positions)
	if (treasures) {
		treasures->clear();
	}

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

	// add each cave treasure to the spawn list when available
	if (in_cave_play()) {
		for (size_t i = 0; i < spawn_positions.len(); i++) {
			add(spawn_positions[i].name);
		}
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
	// delay attaching delegate, since it needs a pointer back to the option
	ToggleMenuOption* collected_opt = new ToggleMenuOption("collected", false, nullptr);
	collected_opt->set_on_selected(
		new CurriedDelegate2<TreasureEditor, const char*, ToggleMenuOption*, bool>(this, &set_collected, treasure_name, collected_opt));

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

	// work out where to point the camera
	// - a spawned treasure uses its pellet position
	// - a collected/despawned treasure uses its generator position (where it will respawn)
	Vector3f target;
	bool have_target = false;
	if (treasure) {
		treasure->mLod.setFlag(AILOD_IsVisibleBoth);
		target      = treasure->getPosition();
		have_target = true;
	} else if (in_above_ground_play()) {
		FOREACH_NODE(Game::Generator, Game::generatorCache->getFirstGenerator(), gen)
		{
			if (!gen->mObject) {
				continue;
			}
			Game::PelletConfig* cfg = nullptr;
			if (gen->mObject->mTypeID == 'pelt') {
				Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
				if (gen_pellet->mGenParm) {
					cfg = Game::PelletList::Mgr::mInstance->getConfig(gen_pellet->mPelType)->getPelletConfig(gen_pellet->mGenParm->mIndex);
				}
			} else if (gen->mObject->mTypeID == 'teki') {
				Game::GenObjectEnemy* gen_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
				if (!gen_enemy->mOtakaraItemCode.isNull()) {
					cfg = Game::PelletList::Mgr::mInstance->getConfig(gen_enemy->mOtakaraItemCode.getPelletKind())
					          ->getPelletConfig(gen_enemy->mOtakaraItemCode.getPelletIndex());
				}
			}
			if (cfg && strcmp(cfg->mParams.mName.mData, treasure_name) == 0) {
				target      = gen->mPosition;
				have_target = true;
				break;
			}
		}
	}

	if (!have_target) {
		return;
	}

	Game::PlayCamera* camera = Game::cameraMgr->mCameraObjList[p2gz->navi_tools->active_navi()->getNaviID()];
	if (camera) {
		camera->mGoalPosition = target;
	}
}

void TreasureEditor::sync_treasure_option(const char* treasure_name, ToggleMenuOption* treasure_collected_opt)
{
	Game::Creature* treasure = find_treasure(treasure_name);
	treasure_collected_opt->set_selection(!treasure || !treasure->isAlive());
}

Game::Pellet* birth_pellet(Game::PelletConfig* cfg, const char* config_name, int kind)
{
	if (!heap_has_room_for_spawn()) {
		// indicate why a treasure isn't spawning for when we go insane later trying to trace it
		OSReport("couldn't spawn treasure %s: heap too low (largest free block %u bytes)\n", config_name,
		         JKRHeap::getCurrentHeap() ? JKRHeap::getCurrentHeap()->getFreeSize() : 0);
		return nullptr;
	}

	Game::PelletInitArg arg;
	arg.mDontCheckCollected = true;
	arg.mTextIdentifier     = const_cast<char*>(config_name);
	arg.mPelletType         = kind;
	arg.mPelletIndex        = cfg->mParams.mIndex;
	arg.mPelView            = nullptr;

	Game::Pellet* spawned_treasure = p2gz->treasure_editor->search_loaded_treasure_index(cfg->mParams.mIndex);

	// Creating a model takes up memory, so only do it if we absolutely have to.
	if (spawned_treasure) {
		arg.mDoSkipCreateModel = 1;
		spawned_treasure->mMgr->setComeAlive(spawned_treasure);
		spawned_treasure->init(&arg);
	} else {
		arg.mDoSkipCreateModel = 0;
		spawned_treasure       = Game::pelletMgr->birth(&arg);
	}
	return spawned_treasure;
}

// place pellet so it doesn't clip into the ground
static void place_pellet_on_floor(Game::Pellet* pellet, Vector3f pos)
{
	pos.y = Game::mapMgr->getMinY(pos) + pellet->getCylinderHeight() * 0.5f;
	pellet->setPosition(pos, false);
}

Game::Pellet* TreasureEditor::spawn_treasure(const char* config_name)
{
	// Find the treasure config so we can look up its index
	Game::PelletConfig* cfg      = nullptr;
	Game::PelletList::cKind kind = Game::PelletList::PLK_Otakara;
	for (int i = (int)Game::PelletList::PLK_Otakara; i <= (int)Game::PelletList::PLK_Item; i++) {
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
			if (!gen->mObject) {
				continue;
			}
			if (gen->mObject->mTypeID == 'pelt') {
				Game::GenPellet* gen_pellet = static_cast<Game::GenPellet*>(gen->mObject);
				int treasure_id             = gen_pellet->mGenParm->mIndex;
				int kind                    = gen_pellet->mPelType;
				const char* treasure_name
				    = Game::PelletList::Mgr::mInstance->getConfig(kind)->getPelletConfig(treasure_id)->mParams.mName.mData;
				if ((!gen->mCreature || !gen->mCreature->isAlive()) && strcmp(treasure_name, config_name) == 0) {
					Game::Pellet* pellet = birth_pellet(cfg, config_name, kind);
					if (!pellet) {
						return nullptr;
					}
					place_pellet_on_floor(pellet, gen->mPosition + gen->mOffset);
					gen->mCreature     = pellet;
					pellet->mGenerator = gen;
					return pellet;
				}
			} else if (gen->mObject->mTypeID == 'teki') {
				Game::GenObjectEnemy* gen_obj_enemy = static_cast<Game::GenObjectEnemy*>(gen->mObject);
				if (gen_obj_enemy->mOtakaraItemCode.getPelletKind() == kind
				    && gen_obj_enemy->mOtakaraItemCode.getPelletIndex() == cfg->mParams.mIndex) {
					// Respawning the enemy is difficult because many of them are handled differently after
					// dying or when not spawned. This is left as an improvement for the future.
					// Instead we just spawn the treasure where the enemy would be.
					Game::Pellet* pellet = birth_pellet(cfg, config_name, kind);
					if (!pellet) {
						return nullptr;
					}
					place_pellet_on_floor(pellet, gen->mPosition + gen->mOffset);
					return pellet;
				}
			}
		}
		GZEXPECT(false, "no suitable pellet or teki generator found for treasure %s", config_name);
	} else if (in_cave_play()) {
		Game::Pellet* pellet = birth_pellet(cfg, config_name, kind);
		if (!pellet) {
			return nullptr;
		}

		// respawn at the treasure's recorded floor spawn spot
		// (default to current captain's position if we didn't get a spawn spot, such as for treasures in enemies)
		Vector3f pos;
		if (!get_spawn_position(config_name, pos)) {
			Game::Navi* navi = p2gz->navi_tools->active_navi();
			f32 y            = Game::mapMgr->getMinY(navi->mPosition) + (pellet->getCylinderHeight() * 0.5f);
			pos              = Vector3f(navi->mPosition.x, y, navi->mPosition.z);
		}
		pellet->setPosition(pos, false);
		return pellet;
	} else {
		GZEXPECT(false, "must be in cave or AG to spawn treasure");
	}
}

// clear the buried-treasure holder (ItemTreasure::Item) for this treasure, if it has one
static void release_buried_holder(const char* config_name)
{
	Iterator<Game::BaseItem> buriedIterator(Game::ItemTreasure::mgr);
	CI_LOOP(buriedIterator)
	{
		Game::ItemTreasure::Item* item = static_cast<Game::ItemTreasure::Item*>(*buriedIterator);
		// skip dead pellets
		if (!item->isAlive() || !item->mPellet || !item->mPellet->isAlive()) {
			continue;
		}
		if (strcmp(config_name, item->mPellet->getConfigName()) == 0) {
			item->mTotalLife = 0.0f;
			item->releasePellet();
		}
	}
}

// Toggle whether the given treasure is collected. Disables move and sets collected.
void TreasureEditor::set_collected(const char* treasure_name, ToggleMenuOption* collected_opt, bool collected)
{
	Game::Creature* creature = find_treasure(treasure_name);
	if (collected) {
		// kill the pellet
		if (!creature || !creature->isAlive()) {
			return;
		}

		// if it's buried, dig it up first so the holder dies with it
		release_buried_holder(treasure_name);

		Game::PelletKillArg arg;
		arg.mFlags    = Game::CKILL_DontCountAsDeath;
		arg.mDoRevive = false;
		creature->kill(&arg);
	} else {
		// spawn a new pellet
		// this leaks due to the model, so just perma show "collected" when the heap is too low so we don't crash
		if (creature && creature->isAlive()) {
			return;
		}
		if (!heap_has_room_for_spawn()) {
			OSReport("couldn't un-collect treasure %s: heap too low to spawn\n", treasure_name);
			// let the user know why nothing appeared
			p2gz->show_callout("No Memory To Spawn Treasure");
			if (collected_opt) {
				// keep toggle in sync with the actual treasure state
				collected_opt->set_selection(true);
			}
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
	// drop remembered spawn spots from the level we're leaving
	spawn_positions.clear();
}

void TreasureEditor::add_loaded_treasure(Game::Pellet* treasure)
{
	for (int i = 0; i < MAX_LOADED_TREASURES; i++) {
		if (loaded_treasures[i] == treasure) {
			return;
		}

		if (loaded_treasures[i] == nullptr) {
			loaded_treasures[i] = treasure;
			return;
		}
	}
}

Game::Pellet* TreasureEditor::search_loaded_treasure_index(int index_to_search)
{
	for (int i = 0; i < MAX_LOADED_TREASURES; i++) {
		if (loaded_treasures[i] && (loaded_treasures[i]->getConfigIndex() == index_to_search)) {
			return loaded_treasures[i];
		}
	}
	return nullptr;
}

void TreasureEditor::clear_loaded_treasure_array()
{
	for (int i = 0; i < MAX_LOADED_TREASURES; i++) {
		loaded_treasures[i] = nullptr;
	}
}

// Debug function. Displays the loaded treasure array.
void TreasureEditor::printout_array()
{
	for (int i = 0; i < MAX_LOADED_TREASURES; i++) {
		if (loaded_treasures[i]) {
			OSReport("Entry number %d has index %d and name %s \n", i, loaded_treasures[i]->getConfigIndex(),
			         loaded_treasures[i]->getConfigName());
		} else {
			OSReport("Entry number %d is empty \n", i);
		}
	}
}
