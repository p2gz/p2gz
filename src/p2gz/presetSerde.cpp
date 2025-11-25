#include <p2gz/Preset.h>

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

void read_piki_container(Stream& stream, Game::PikiContainer& container)
{
	for (u32 color = 0; color < 6; color++) {
		for (u32 stage = 0; stage < 3; stage++) {
			int amount                       = stream.readInt();
			container.getCount(color, stage) = amount;
		}
	}
}

void write_piki_container(Stream& output, Game::PikiContainer& container)
{
	for (u32 color = 0; color < 6; color++) {
		for (u32 stage = 0; stage < 3; stage++) {
			int amount = container.getCount(color, stage);
			output.writeInt(amount);
		}
	}
}

void Preset::read_file(const char* filename)
{
	char preset_file_path[256];
	sprintf(preset_file_path, "presets/%s", filename);
	OSReport("Loading preset %s\n", preset_file_path);

	void* preset_file
	    = JKRDvdRipper::loadToMainRAM(preset_file_path, nullptr, Switch_0, 0, nullptr, JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
	GZEXPECT(preset_file, "preset file %s not found", filename);

	RamStream preset_stream(preset_file, -1);
	preset_stream.setMode(STREAM_MODE_TEXT, 1);

	read(preset_stream);
	origin = PO_File;

	delete[] preset_file;
}

void Preset::read(Stream& input)
{
	name = input.readString(nullptr, 0);

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

void PresetPreview::read(const char* filename_)
{
	filename = filename_;

	char preset_file_path[256];
	sprintf(preset_file_path, "presets/%s", filename);
	OSReport("Loading preset preview from %s\n", preset_file_path);

	void* preset_file
	    = JKRDvdRipper::loadToMainRAM(preset_file_path, nullptr, Switch_0, 0, nullptr, JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
	GZEXPECT(preset_file, "preset file %s not found", filename);

	RamStream preset_stream(preset_file, -1);
	preset_stream.setMode(STREAM_MODE_TEXT, 1);

	name                     = preset_stream.readString(nullptr, 0);
	const char* category_str = preset_stream.readString(nullptr, 0);
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
	output.writeFloat(position.x);
	output.writeFloat(position.y);
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
	output.textWriteTab(2);
	output.writeInt(amount);
	output.writeInt(get_stage());
	output.writeInt(get_kind());

	if (amount > 0) {
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
	output.writeInt(spawn_override);
	if (spawn_override == PSO_SpawnAndMove) {
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
	output.writeFloat(gen_pos.x);
	output.writeFloat(gen_pos.y);
	output.writeFloat(gen_pos.z);
	output.writeInt(spawn_override);
}
