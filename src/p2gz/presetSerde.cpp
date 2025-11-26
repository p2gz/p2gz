#include <p2gz/Preset.h>
#include <p2gz/Utility.h>
#include <p2gz/p2gz.h>

using namespace gz;

#define READ_LIST(stream)          \
	list_count = stream.readInt(); \
	for (u32 i = 0; i < list_count; i++)

#define READ_LIST_T(stream, Type, dest)    \
	list_count = stream.readInt();         \
	dest.expandCapacityTo(list_count);     \
	for (u32 i = 0; i < list_count; i++) { \
		Type container;                    \
		container.read(stream);            \
		dest.push(container);              \
	}

#define READ_BITFLAG(stream, bitfield)        \
	bitfield.clear();                         \
	READ_LIST(stream)                         \
	{                                         \
		const int bit_idx = stream.readInt(); \
		const u32 bit     = 1 << bit_idx;     \
		bitfield.set(bit);                    \
	}

static const char* UPGRADE_NAMES[Game::OlimarData::ODII_COUNT] = {
	"ODII_BruteKnuckles",
	"ODII_DreamMaterial",
	"ODII_AmplifiedAmplifier",
	"ODII_ProfessionalNoisemaker",
	"ODII_StellarOrb",
	"ODII_JusticeAlloy",
	"ODII_ForgedCourage",
	"ODII_RepugnantAppendage",
	"ODII_PrototypeDetector",
	"ODII_FiveManNapsack",
	"ODII_SphericalAtlas",
	"ODII_GeographicProjection",
	"ODII_TheKey",
};

static const char* DEMO_FLAG_NAMES[Game::DEMO_FLAG_COUNT] = {
	"DEMO_Pluck_First_Pikmin",
	"DEMO_Discover_Treasure",
	"", // DEMO_NULL_2
	"DEMO_Find_Cave_Deeper_Hole",
	"DEMO_Find_Cave_Geyser",
	"DEMO_First_Cave_Enter",
	"DEMO_First_Cave_Return",
	"DEMO_Max_Pikmin_On_Field",
	"DEMO_First_Gate_Down",
	"DEMO_Find_Spiderwort_Mold",
	"DEMO_First_Spicy_Use",
	"DEMO_First_Bitter_Use",
	"DEMO_Eat_White_Pikmin",
	"DEMO_Purples_In_Ship",
	"DEMO_Whites_In_Ship",
	"DEMO_Reds_In_Onion",
	"DEMO_Find_Spicy_Drop",
	"DEMO_Find_Bitter_Drop",
	"DEMO_First_Globe_Day_End",
	"", // DEMO_UNUSED_Camera_Demo
	"DEMO_First_Nectar_Use",
	"DEMO_First_Sunset_Warning",
	"DEMO_President_Start",
	"DEMO_Find_Titan_Dweevil",
	"DEMO_Find_Loozy_Treasure",
	"DEMO_White_Candypop",
	"DEMO_Purple_Candypop",
	"DEMO_First_Spicy_Berry",
	"DEMO_First_Bitter_Berry",
	"DEMO_First_Spicy_Spray_Made",
	"DEMO_First_Bitter_Spray_Made",
	"DEMO_Day_One_Start",
	"DEMO_Enter_Awakening_Wood",
	"DEMO_Enter_Perplexing_Pool",
	"DEMO_Enter_Wistful_Wild",
	"DEMO_Meet_Red_Pikmin",
	"DEMO_Louie_Finds_Red_Onion",
	"DEMO_Unlock_Captain_Switch",
	"DEMO_First_Use_Louie",
	"DEMO_Reunite_Captains",
	"DEMO_First_Corpse_In_Cave",
	"", // DEMO_UNUSED_Detector
	"DEMO_Find_Yellow_Onion",
	"DEMO_Find_Blue_Onion",
	"DEMO_Discover_Bulbmin",
	"DEMO_Whites_Digging",
	"DEMO_Reds_Purples_Tutorial",
	"DEMO_Pikmin_In_Danger_Fire",
	"DEMO_You_Appear_Lost",
	"DEMO_First_Number_Pellet",
	"DEMO_Waterwraith_Appears",
	"DEMO_Pikmin_In_Danger_Water",
	"DEMO_Pikmin_In_Danger_Poison",
	"", // DEMO_UNK_53
	"DEMO_RADAR_ENABLED",
	"DEMO_SPICY_ENABLED",
	"DEMO_BITTER_ENABLED",
};

static const char* CAVE_NAMES[] = {
	"Above ground", "EC", "SCx", "FC", "HoB", "WFG", "BK", "SH", "CoS", "GK", "SR", "SmC", "CoC", "HoH", "DD",
};

struct TreasureNameMap {
	const int treasure_id;
	const char* name;
};

static const TreasureNameMap AG_TREASURE_NAMES[] = {
	{ 47, "Fossilized Ursidae" },    { 62, "Pink Menace" },         { 71, "Unspeakable Wonder" }, { 73, "Temporal Mechanism" },
	{ 87, "Utter Scrap" },           { 142, "Courage Reactor" },    { 157, "Spiny Alien Treat" },

	{ 11, "Geographic Projection" }, { 42, "Sunseed Berry" },       { 44, "Decorative Goo" },     { 130, "Pilgrim Bulb" },
	{ 155, "Chance Totem" },         { 173, "Healing Cask" },       { 185, "Air Brake" },

	{ 53, "Onion Replica" },         { 72, "Aquatic Mine" },        { 77, "Impediment Scourge" }, { 118, "Massage Girdle" },
	{ 140, "Optical Illustration" }, { 152, "Fortified Delicacy" }, { 172, "Gherkin Gate" },

	{ 27, "Armored Nut" },           { 45, "Anti-hiccup Fungus" },  { 50, "Conifer Spire" },      { 76, "Doomsday Apparatus" },
	{ 183, "Seed of Greed" },
};

void read_piki_container(Stream& stream, Game::PikiContainer& container)
{
	for (u32 color = 0; color < 6; color++) {
		for (u32 stage = 0; stage < 3; stage++) {
			int amount                       = stream.readInt();
			container.getCount(color, stage) = amount;
		}
	}
}

void write_piki_container(Stream& output, const char* comment, Game::PikiContainer& container)
{
	output.textWriteText("   "); // for aligning with the comment
	for (u32 color = 0; color < 6; color++) {
		for (u32 stage = 0; stage < 3; stage++) {
			int amount = container.getCount(color, stage);
			output.writeInt(amount);

			// align output to comments in text mode by adding spaces for shorter numbers
			if (amount < 10) {
				output.textWriteText(" ");
			}
		}
	}
	output.textWriteText("\t# %s\n", comment);
}

int num_set_bits(BitFlag<u16>& bitfield)
{
	int num_set = 0;
	for (u32 i = 0; i < 16; i++) {
		if (bitfield.isSet(1 << i)) {
			num_set += 1;
		}
	}
	return num_set;
}

void write_bitfield(BitFlag<u16>& bitfield, Stream& output, const char* comment, const char* item_names[])
{
	const int num_set = num_set_bits(bitfield);
	output.writeInt(num_set);
	output.textWriteText("\t# %s\n", comment);
	for (u32 i = 0; i < 16; i++) {
		if (bitfield.isSet(1 << i)) {
			output.textWriteTab(1);
			output.writeInt(i);
			output.textWriteText("\t# %s\n", item_names[i]);
		}
	}
}

template <typename T>
void write_vec(gz::Vec<T>& vec, Stream& output, const char* comment)
{
	output.writeInt(vec.len());
	output.textWriteText("\t# %s\n", comment);
	FOREACH_VEC(vec)
	{
		output.textWriteTab(1);
		vec[i].write(output);
		output.textWriteText("\n");
	}
}

void Preset::read_file(const char* filename)
{
	char preset_file_path[256];
	sprintf(preset_file_path, "presets/%s", filename);
	OSReport("Loading preset %s\n", preset_file_path);

	void* preset_file = JKRDvdRipper::loadToMainRAM(preset_file_path, nullptr, Switch_0, 0, sys->mSysHeap, JKRDvdRipper::ALLOC_DIR_BOTTOM,
	                                                0, nullptr, nullptr);
	GZEXPECT(preset_file, "preset file %s not found", filename);

	RamStream preset_stream(preset_file, -1);
	preset_stream.setMode(STREAM_MODE_TEXT, 1);

	read(preset_stream);
	origin = PO_File;

	delete[] preset_file;
}

void Preset::read(Stream& input)
{
	char* name_buf = input.readString(nullptr, 0);
	for (char* c = name_buf; *c != '\0'; c++) {
		if (*c == '_') {
			*c = ' ';
		}
	}
	name = name_buf;

	char category_str[16];
	input.readString(category_str, sizeof(category_str));
	if (strcmp(category_str, "PoD") == 0) {
		category = PoD;
	} else if (strcmp(category_str, "AT") == 0) {
		category = AT;
	} else {
		category = General;
	}

	read_piki_container(input, squad);
	read_piki_container(input, onion_pikis);

	int list_count; // for macros

	READ_LIST(input)
	{
		Sprout sprout;
		sprout.read(input);
		sprouts.push(sprout);
	}

	num_bitters      = input.readInt();
	bitters_unlocked = input.readInt() > 0;
	num_spicies      = input.readInt();
	spicies_unlocked = input.readInt() > 0;
	time             = input.readFloat();
	day              = input.readInt();
	pokos            = input.readInt();
	if (pokos == -1) {
		pokos       = 0;
		apply_pokos = false;
	} else {
		apply_pokos = true;
	}

	enter_kind = static_cast<EnterAreaKind>(input.readInt());

	READ_BITFLAG(input, upgrades);

	cutscenes.clear();
	READ_LIST(input)
	{
		int demo_idx = input.readInt();
		GZASSERTLINE(demo_idx < 64);
		cutscenes.set_cutscene_played(static_cast<Game::DemoFlags>(demo_idx));
	}

	READ_BITFLAG(input, ek_cutscenes);
	READ_BITFLAG(input, cave_cutscenes);

	READ_LIST_T(input, StructureOverride, destroyed_gates);
	READ_LIST_T(input, StructureOverride, finished_bridges);
	READ_LIST_T(input, StructureOverride, bags_flattened);

	plug_destroyed = input.readInt() > 0;

	READ_LIST_T(input, EnemyGenSpawnOverride, enemy_spawn_overrides);
	READ_LIST_T(input, TreasureGenSpawnOverride, treasure_spawn_overrides);
}

void Preset::write(Stream& output)
{
	char* name_buf = new char[strlen(name)];
	strcpy(name_buf, name);
	for (char* c = name_buf; *c != '\0'; c++) {
		if (*c == ' ') {
			*c = '_';
		}
	}
	output.writeString(name_buf);
	output.textWriteText("\t# preset name (USE UNDERSCORES INSTEAD OF SPACES)\n");
	delete[] name_buf;

	char* category_name;
	switch (category) {
	case PoD:
		category_name = "PoD";
		break;
	case AT:
		category_name = "AT";
		break;
	case General:
		category_name = "General";
		break;
	default:
		GZASSERTLINE(false);
	}
	output.writeString(category_name);
	output.textWriteText("\t# category\n");

	// Helpful comment for pikmin numbers, otherwise they're very hard to read
	output.textWriteText("#  bl bb bf rl rb rf yl yb yf pl pb pf wl wb wf cl cb cf\n");
	write_piki_container(output, "pikmin", squad);
	write_piki_container(output, "onions", onion_pikis);

	output.writeInt(sprouts.len());
	output.textWriteText("\t# num sprouts\n");
	FOREACH_VEC(sprouts)
	{
		output.textWriteTab(1);
		sprouts[i].write(output);
		output.textWriteText("\n");
	}

	output.writeInt(num_bitters);
	output.writeInt(bitters_unlocked ? 1 : 0);
	output.textWriteText("\t# bitters, bitters unlocked\n");
	output.writeInt(num_spicies);
	output.writeInt(spicies_unlocked ? 1 : 0);
	output.textWriteText("\t# spicies, spicies unlocked\n");

	output.writeFloat(time);
	output.textWriteText("\t# time of day\n");
	output.writeInt(day);
	output.textWriteText("\t# day number\n");

	if (apply_pokos) {
		output.writeInt(pokos);
	} else {
		output.writeInt(-1);
	}
	output.textWriteText("\t# pokos (-1 if not applied)\n");

	output.writeInt(static_cast<int>(enter_kind));
	output.textWriteText("\t# enter kind (FromCave = 0, FromMap = 1, FirstEnter = 2)\n");

	write_bitfield(upgrades, output, "num upgrades", UPGRADE_NAMES);

	int num_demo_flags = 0;
	for (u32 i = 0; i < Game::DEMO_FLAG_COUNT; i++) {
		if (cutscenes.cutscene_played(static_cast<Game::DemoFlags>(i))) {
			num_demo_flags += 1;
		}
	}
	output.writeInt(num_demo_flags);
	output.textWriteText("\t# num demo flags\n");
	for (u32 i = 0; i < Game::DEMO_FLAG_COUNT; i++) {
		if (cutscenes.cutscene_played(static_cast<Game::DemoFlags>(i))) {
			output.textWriteTab(1);
			output.writeInt(i);
			output.textWriteText("\t# %s\n", DEMO_FLAG_NAMES[i]);
		}
	}

	write_bitfield(ek_cutscenes, output, "num EK cutscenes", UPGRADE_NAMES);
	write_bitfield(cave_cutscenes, output, "num cave cutscene flags", CAVE_NAMES);

	output.writeInt(destroyed_gates.len());
	output.textWriteText("\t# num destroyed gates\n");
	FOREACH_VEC(destroyed_gates)
	{
		StructureOverride& oride = destroyed_gates[i];
		output.textWriteTab(1);
		oride.write(output);
		const char* gate_name = p2gz->structure_editor->find_gate_name(oride.position, oride.area);
		if (gate_name) {
			output.textWriteText("\t# %s\n", gate_name);
		} else {
			output.textWriteText("\n");
		}
	}

	output.writeInt(finished_bridges.len());
	output.textWriteText("\t# num finished bridges\n");
	FOREACH_VEC(finished_bridges)
	{
		StructureOverride& oride = finished_bridges[i];
		output.textWriteTab(1);
		oride.write(output);
		const char* bridge_name = p2gz->structure_editor->find_bridge_name(oride.position, oride.area);
		if (bridge_name) {
			output.textWriteText("\t# %s\n", bridge_name);
		} else {
			output.textWriteText("\n");
		}
	}

	output.writeInt(bags_flattened.len());
	output.textWriteText("\t# num flattened bags\n");
	FOREACH_VEC(bags_flattened)
	{
		StructureOverride& oride = bags_flattened[i];
		output.textWriteTab(1);
		oride.write(output);
		const char* bag_name = p2gz->structure_editor->find_bag_name(oride.position, oride.area);
		if (bag_name) {
			output.textWriteText("\t# %s\n", bag_name);
		} else {
			output.textWriteText("\n");
		}
	}

	output.writeInt(plug_destroyed ? 1 : 0);
	output.textWriteText("\t# plugs destroyed\n");

	write_vec(enemy_spawn_overrides, output, "num enemy spawns");

	output.writeInt(treasure_spawn_overrides.len());
	output.textWriteText("\t# num treasure spawn overrides\n");
	FOREACH_VEC(treasure_spawn_overrides)
	{
		output.textWriteTab(1);
		treasure_spawn_overrides[i].write(output);

		if (output.mMode == STREAM_MODE_TEXT) {
			const char* treasure_name = nullptr;
			for (u32 j = 0; j < ARRAY_SIZE(AG_TREASURE_NAMES); j++) {
				if (AG_TREASURE_NAMES[j].treasure_id == treasure_spawn_overrides[i].id) {
					treasure_name = AG_TREASURE_NAMES[j].name;
					break;
				}
			}
			if (treasure_name) {
				output.textWriteText("\t# %s", treasure_name);
			}
			output.textWriteText("\n");
		}
	}
}

void PresetPreview::read(const char* filename_)
{
	filename = filename_;

	char preset_file_path[256];
	sprintf(preset_file_path, "presets/%s", filename);

	void* preset_file = JKRDvdRipper::loadToMainRAM(preset_file_path, nullptr, Switch_0, 0, sys->mSysHeap, JKRDvdRipper::ALLOC_DIR_BOTTOM,
	                                                0, nullptr, nullptr);
	GZEXPECT(preset_file, "preset file %s not found", filename);

	RamStream preset_stream(preset_file, -1);
	preset_stream.setMode(STREAM_MODE_TEXT, 1);

	char* name_buf = preset_stream.readString(nullptr, 0);
	for (char* c = name_buf; *c != '\0'; c++) {
		if (*c == '_') {
			*c = ' ';
		}
	}
	name = name_buf;

	char category_str[16];
	preset_stream.readString(category_str, sizeof(category_str));
	if (strcmp(category_str, "PoD") == 0) {
		category = PoD;
	} else if (strcmp(category_str, "AT") == 0) {
		category = AT;
	} else {
		category = General;
	}

	read_piki_container(preset_stream, squad);
	read_piki_container(preset_stream, onion_pikis);

	if (strcmp(category_str, "PoD") == 0) {
		category = PoD;
	} else if (strcmp(category_str, "AT") == 0) {
		category = AT;
	} else {
		category = General;
	}

	delete[] preset_file;
}

void Preset::StructureOverride::read(Stream& input)
{
	area     = input.readInt();
	f32 x    = input.readFloat();
	f32 z    = input.readFloat();
	position = Vector2f(x, z);
	data     = input.readInt();
}

void Preset::StructureOverride::write(Stream& output)
{
	output.writeInt(area);
	output.textWriteTab(1);
	output.writeFloat(position.x);
	output.writeFloat(position.y);
	output.textWriteTab(1);
	output.writeInt(data);
}

void Preset::Sprout::read(Stream& input)
{
	amount = input.readInt();

	int stage      = input.readInt();
	int kind       = input.readInt();
	stage_and_kind = ((static_cast<u8>(stage) & 0x0F) << 4) | (static_cast<u8>(kind) & 0x0F);

	if (amount == 0) {
		pos.x = input.readFloat();
		pos.y = input.readFloat();
		pos.z = input.readFloat();
	}
}

void Preset::Sprout::write(Stream& output)
{
	output.writeInt(amount);
	output.textWriteTab(1);
	output.writeInt(get_stage());
	output.textWriteTab(1);
	output.writeInt(get_kind());

	if (amount > 0) {
		output.textWriteTab(1);
		output.writeFloat(pos.x);
		output.writeFloat(pos.y);
		output.writeFloat(pos.z);
	}
}

void Preset::TreasureGenSpawnOverride::read(Stream& input)
{
	id             = input.readInt();
	spawn_override = static_cast<GenSpawnOverride>(input.readInt());
	if (spawn_override == PSO_SpawnAndMove) {
		position_override.x = input.readFloat();
		position_override.y = input.readFloat();
		position_override.z = input.readFloat();
	}
}

void Preset::TreasureGenSpawnOverride::write(Stream& output)
{
	output.writeInt(id);
	output.textWriteTab(1);
	output.writeInt(spawn_override);
	if (spawn_override == PSO_SpawnAndMove) {
		output.textWriteTab(1);
		output.writeFloat(position_override.x);
		output.writeFloat(position_override.y);
		output.writeFloat(position_override.z);
	}
}

void Preset::EnemyGenSpawnOverride::read(Stream& input)
{
	enemy_id       = static_cast<Game::EnemyTypeID::EEnemyTypeID>(input.readInt());
	gen_pos.x      = input.readFloat();
	gen_pos.y      = input.readFloat();
	gen_pos.z      = input.readFloat();
	spawn_override = static_cast<GenSpawnOverride>(input.readInt());
}

void Preset::EnemyGenSpawnOverride::write(Stream& output)
{
	output.writeInt(enemy_id);
	output.textWriteTab(1);
	output.writeFloat(gen_pos.x);
	output.writeFloat(gen_pos.y);
	output.writeFloat(gen_pos.z);
	output.textWriteTab(1);
	output.writeInt(spawn_override);
}
