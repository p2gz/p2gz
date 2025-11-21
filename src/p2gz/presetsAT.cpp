#include <p2gz/Preset.h>
#include <p2gz/PresetsAT.h>

using namespace gz;
using namespace Game;

/////////////////////////////////////////////////////////////
///////////////////////// TREASURES /////////////////////////
static Preset::TreasureGenSpawnOverride t_at_start[] = {
	// nothing collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_Spawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_Spawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Spawn),  /* goo */
	Preset::TreasureGenSpawnOverride(130, PSO_Spawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_Spawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn), /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Spawn), /* air brake */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_eb[] = {
	// AW: dice and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_Spawn),   /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_Spawn),   /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),  /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_Spawn),  /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_Ignore), /* dice -- TODO: ADD POSITION --*/
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),  /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Spawn),  /* air brake */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_aw1[] = {
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_Spawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_scx[] = {
	// VoR: scrap moved
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_Ignore), /* scrap -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_fc[] = {
	// VoR: scrap moved (more)
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_Ignore), /* scrap -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_scrap[] = {
	// VoR: scrap moved (more)
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_Spawn),     /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_Spawn),     /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_Spawn),     /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_Spawn),     /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn), /* scrap -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(142, PSO_Spawn),    /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_Spawn),    /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_vor2[] = {
	// VoR: all collected
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Spawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_smc[] = {
	// VoR: all collected
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// PP: girdle moved
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_Spawn),   /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_Spawn),   /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_Spawn),   /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_Ignore), /* girdle -- TODO: ADD POSITION-- */
	Preset::TreasureGenSpawnOverride(140, PSO_Spawn),  /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_Spawn),  /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_Spawn),  /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_sr[] = {
	// VoR: all collected
	// AW: dice, globe, strawberry, bulb collected, AB and goo moved
	// PP: all collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Spawn),     /* healing cask/milk lid */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_DontSpawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_DontSpawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_DontSpawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_DontSpawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_DontSpawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_DontSpawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_DontSpawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_bk[] = {
	// VoR: all collected
	// AW: dice, globe, strawberry, bulb collected, AB moved, goo moved (more), 'cask' moved
	// PP: all collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_Ignore),     /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_Ignore),    /* healing cask/milk lid -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(185, PSO_Ignore),    /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_DontSpawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_DontSpawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_DontSpawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_DontSpawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_DontSpawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_DontSpawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_DontSpawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_sh[] = {
	// VoR: all collected
	// AW: all collected
	// PP: all collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_DontSpawn),  /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask/milk lid -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(185, PSO_DontSpawn), /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_DontSpawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_DontSpawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_DontSpawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_DontSpawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_DontSpawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_DontSpawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_DontSpawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_Spawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn), /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_doomsday[] = {
	// VoR: all collected
	// AW: all collected
	// PP: all collected
	// WW: doomsday collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_DontSpawn),  /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask/milk lid -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(185, PSO_DontSpawn), /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_DontSpawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_DontSpawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_DontSpawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_DontSpawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_DontSpawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_DontSpawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_DontSpawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_Spawn),     /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_Spawn),     /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_Spawn),     /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_DontSpawn), /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_Spawn),    /* seed of greed */
};

static Preset::TreasureGenSpawnOverride t_at_post_enter_hoh[] = {
	// VoR: all collected
	// AW: all collected
	// PP: all collected
	// WW: all collected
	// VoR
	Preset::TreasureGenSpawnOverride(47, PSO_DontSpawn),  /* ursidae */
	Preset::TreasureGenSpawnOverride(62, PSO_DontSpawn),  /* ring */
	Preset::TreasureGenSpawnOverride(71, PSO_DontSpawn),  /* crown */
	Preset::TreasureGenSpawnOverride(73, PSO_DontSpawn),  /* watch */
	Preset::TreasureGenSpawnOverride(87, PSO_DontSpawn),  /* scrap */
	Preset::TreasureGenSpawnOverride(142, PSO_DontSpawn), /* battery */
	Preset::TreasureGenSpawnOverride(157, PSO_DontSpawn), /* SAT */

	// AW
	Preset::TreasureGenSpawnOverride(11, PSO_DontSpawn),  /* globe */
	Preset::TreasureGenSpawnOverride(42, PSO_DontSpawn),  /* ichigo */
	Preset::TreasureGenSpawnOverride(43, PSO_DontSpawn),  /* goo -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(130, PSO_DontSpawn), /* pilgrim bulb */
	Preset::TreasureGenSpawnOverride(155, PSO_DontSpawn), /* dice */
	Preset::TreasureGenSpawnOverride(173, PSO_DontSpawn), /* healing cask/milk lid -- TODO: ADD POSITION -- */
	Preset::TreasureGenSpawnOverride(185, PSO_DontSpawn), /* air brake -- TODO: ADD POSITION -- */

	// PP
	Preset::TreasureGenSpawnOverride(53, PSO_DontSpawn),  /* onion replica */
	Preset::TreasureGenSpawnOverride(72, PSO_DontSpawn),  /* mine */
	Preset::TreasureGenSpawnOverride(77, PSO_DontSpawn),  /* scourge/lightning bolt */
	Preset::TreasureGenSpawnOverride(118, PSO_DontSpawn), /* girdle */
	Preset::TreasureGenSpawnOverride(140, PSO_DontSpawn), /* optical illustration/battery */
	Preset::TreasureGenSpawnOverride(152, PSO_DontSpawn), /* shell */
	Preset::TreasureGenSpawnOverride(172, PSO_DontSpawn), /* gherkin gate/milk lid */

	// WW
	Preset::TreasureGenSpawnOverride(27, PSO_DontSpawn),  /* nut */
	Preset::TreasureGenSpawnOverride(45, PSO_DontSpawn),  /* mushroom */
	Preset::TreasureGenSpawnOverride(50, PSO_DontSpawn),  /* pinecone */
	Preset::TreasureGenSpawnOverride(76, PSO_DontSpawn),  /* doomsday apparatus */
	Preset::TreasureGenSpawnOverride(183, PSO_DontSpawn), /* seed of greed */
};

/////////////////////////////////////////////////////////////
////////////////////////// ENEMIES //////////////////////////
static Preset::EnemyGenSpawnOverride e_at_enter_wfg[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn), // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),   // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_Spawn),  // near gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_Spawn),    // after gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-899.46f, -20.0f, 1109.54f), PSO_Spawn),     // to HoB
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),  // flower pot
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),    // near bulb
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(13.00f, 15.0f, 211.00f), PSO_Spawn),     // 1-pellets (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(209.00f, 10.0f, -206.00f), PSO_Spawn),   // 5-pellet (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(14.00f, 15.0f, -263.00f), PSO_Spawn),    // 1-pellet (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-258.00f, 15.0f, -133.00f), PSO_Spawn),  // 1-pellets (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(-36.44f, 15.0f, -104.84f), PSO_Spawn),    // BO
};

static Preset::EnemyGenSpawnOverride e_at_enter_hob[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),      // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_DontSpawn), // near gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_DontSpawn),   // after gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-899.46f, -20.0f, 1109.54f), PSO_Spawn),        // to HoB
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),     // flower pot
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),       // near bulb
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(13.00f, 15.0f, 211.00f), PSO_Spawn),        // 1-pellets (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(209.00f, 10.0f, -206.00f), PSO_Spawn),      // 5-pellet (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(14.00f, 15.0f, -263.00f), PSO_Spawn),       // 1-pellet (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-258.00f, 15.0f, -133.00f), PSO_Spawn),     // 1-pellets (BO)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(-36.44f, 15.0f, -104.84f), PSO_Spawn),       // BO
};

static Preset::EnemyGenSpawnOverride e_at_day_5[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),      // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_DontSpawn), // near gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_DontSpawn),   // after gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-899.46f, -20.0f, 1109.54f), PSO_Spawn),        // to HoB
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),     // flower pot
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),       // near bulb
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1631.42f, -53.0f, 3351.95f), PSO_Spawn), // near AB (near bridge)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1388.69f, -53.0f, 3972.27f), PSO_Spawn), // near AB (far bridge)
};

static Preset::EnemyGenSpawnOverride e_at_enter_scx[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-1153.0f, 47.0f, 2231.0f), PSO_Spawn), // entry to hubcap
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(548.54f, 100.0f, 546.48f), PSO_Spawn),     // bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(300.91f, 100.0f, 361.60f), PSO_Spawn),     // to the side
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(340.79f, 15.0f, 868.56f), PSO_Spawn),   // bridge (near)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(145.54f, 18.87f, 618.03f), PSO_Spawn),  // bridge (far)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-244.22f, 100.0f, 326.78f),
	                              PSO_Spawn), // between bridge and ursidae
};

static Preset::EnemyGenSpawnOverride e_at_enter_fc[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-1153.0f, 47.0f, 2231.0f), PSO_DontSpawn), // entry to hubcap
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(548.54f, 100.0f, 546.48f), PSO_Spawn),         // bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(300.91f, 100.0f, 361.60f), PSO_Spawn),         // to the side
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(340.79f, 15.0f, 868.56f), PSO_Spawn),       // bridge (near)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(145.54f, 18.87f, 618.03f), PSO_Spawn),      // bridge (far)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-244.22f, 100.0f, 326.78f),
	                              PSO_Spawn), // between bridge and ursidae
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(302.18f, 100.0f, -177.91f), PSO_Spawn),        // to FC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Fkabuto, Vector3f(182.16f, 210.0f, -1559.79f), PSO_Spawn),       // crown
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-271.09f, 114.0f, -1387.32f), PSO_Spawn), // FC
};

static Preset::EnemyGenSpawnOverride e_at_vor_cleanup[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-1153.0f, 47.0f, 2231.0f), PSO_DontSpawn), // entry to hubcap
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(548.54f, 100.0f, 546.48f), PSO_DontSpawn),     // bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(300.91f, 100.0f, 361.60f), PSO_Spawn),         // to the side
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(340.79f, 15.0f, 868.56f), PSO_Spawn),       // bridge (near)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(145.54f, 18.87f, 618.03f), PSO_Spawn),      // bridge (far)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-244.22f, 100.0f, 326.78f),
	                              PSO_Spawn), // between bridge and ursidae
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(302.18f, 100.0f, -177.91f), PSO_DontSpawn),        // to FC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Fkabuto, Vector3f(182.16f, 210.0f, -1559.79f), PSO_DontSpawn),       // crown
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-271.09f, 114.0f, -1387.32f), PSO_DontSpawn), // FC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(-679.41f, 60.0f, -24.93f), PSO_Spawn),             // 2x fiery
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(-810.08f, 60.0f, -378.64f), PSO_Spawn),            // 1x fiery
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_FireChappy, Vector3f(-616.06f, 100.0f, -778.38f), PSO_Spawn),        // fiery
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-259.88f, 50.0f, 2476.48f), PSO_Spawn),           // 10-pellet
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(52.87f, 9.17f, 3055.31f), PSO_Spawn),   // 5-pellet (base)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-1342.39f, 4.2f, 2825.76f), PSO_Spawn), // 5-pellet (bags 1)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-1340.81f, 0.0f, 2606.40f), PSO_Spawn), // 5-pellet (bags 2)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-584.42f, 0.0f, 3160.87f), PSO_Spawn),  // 1-pellet (base)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-21.92f, 0.0f, 2870.73f), PSO_Spawn),   // 1-pellet (base)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-400.92f, 0.0f, 2646.80f), PSO_Spawn),  // 1-pellet (base)

};

static Preset::EnemyGenSpawnOverride e_at_enter_cos[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-950.0f, 6.25f, -150.0f), PSO_Spawn),       // near CoS
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-480.0f, 89.01f, -1115.0f), PSO_Spawn), // 1-pellets, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-666.0f, 89.01f, -800.0f), PSO_Spawn),  // 5-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-375.0f, 89.01f, -800.0f), PSO_Spawn),  // 10-pellet, YO
};

static Preset::EnemyGenSpawnOverride e_at_enter_gk[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-950.0f, 6.25f, -150.0f), PSO_DontSpawn),       // near CoS
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-480.0f, 89.01f, -1115.0f), PSO_DontSpawn), // 1-pellets, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-666.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 5-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-375.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 10-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-650.0f, 52.89f, -1800.0f), PSO_Spawn),        // near GK
};

static Preset::EnemyGenSpawnOverride e_at_enter_smc[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-950.0f, 6.25f, -150.0f), PSO_DontSpawn),       // near CoS
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-480.0f, 89.01f, -1115.0f), PSO_DontSpawn), // 1-pellets, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-666.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 5-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-375.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 10-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-650.0f, 52.89f, -1800.0f), PSO_Spawn),        // near GK
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(-400.0f, -57.0f, -2950.0f), PSO_Spawn),      // near girdle scales
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-900.0f, -57.0f, -3000.0f), PSO_Spawn),       // near girdle
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-1100.0f, -57.0f, -3200.0f), PSO_Spawn),      // near girdle
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(0.0f, -57.0f, -3000.0f), PSO_Spawn),    // near SmC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-25.0f, 0.0f, -75.0f), PSO_Spawn),              // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(275.0f, 16.48f, -275.0f), PSO_Spawn),           // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(1350.0f, -91.40f, 1450.0f), PSO_Spawn),      // near SR
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(1400.0f, -90.05f, 1825.0f), PSO_Spawn),      // near SR
};

static Preset::EnemyGenSpawnOverride e_at_enter_sr[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-950.0f, 6.25f, -150.0f), PSO_DontSpawn),       // near CoS
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-480.0f, 89.01f, -1115.0f), PSO_DontSpawn), // 1-pellets, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-666.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 5-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Pelplant, Vector3f(-375.0f, 89.01f, -800.0f), PSO_DontSpawn),  // 10-pellet, YO
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-650.0f, 52.89f, -1800.0f), PSO_DontSpawn),    // near GK
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(-400.0f, -57.0f, -2950.0f),
	                              PSO_DontSpawn),                                                                      // near girdle scales
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(0.0f, -57.0f, -3000.0f), PSO_DontSpawn), // near SmC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-25.0f, 0.0f, -75.0f), PSO_DontSpawn),           // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(275.0f, 16.48f, -275.0f), PSO_DontSpawn),        // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(1350.0f, -91.40f, 1450.0f), PSO_Spawn),       // near SR
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tadpole, Vector3f(1400.0f, -90.05f, 1825.0f), PSO_Spawn),       // near SR
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(375.0f, 7.24f, -1375.0f), PSO_Spawn),           // near bitters
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_UmiMushiBlind, Vector3f(950.0f, 57.0f, 475.0f), PSO_Spawn),     // mine
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Catfish, Vector3f(150.0f, -57.0f, 1950.0f), PSO_Spawn),         // near onion
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(400.0f, -57.0f, 1600.0f), PSO_Spawn),            // near shell
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(750.0f, -57.0f, 1350.0f), PSO_Spawn),            // near shell
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(550.0f, 17.69f, -1000.0f), PSO_Spawn),           // near bitters
};

static Preset::EnemyGenSpawnOverride e_at_enter_bk[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),      // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_DontSpawn), // near gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_Spawn),       // after gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-899.46f, -20.0f, 1109.54f), PSO_Spawn),        // to HoB
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),     // flower pot
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-347.22f, -20.0f, 4107.39f), PSO_Spawn),     // near SH
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),       // near bulb
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-383.96f, 30.0f, 575.85f), PSO_Spawn),         // near electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-71.49f, -20.0f, 1117.59f), PSO_Spawn),        // near WFG
};

static Preset::EnemyGenSpawnOverride e_at_enter_sh[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-179.69f, -70.0f, 2909.94f), PSO_Spawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Chappy, Vector3f(-486.16f, -70.0f, 3058.21f), PSO_Spawn),      // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Qurione, Vector3f(-544.00f, -38.0f, 1804.00f), PSO_DontSpawn), // near gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-672.00f, -20.0f, 1392.00f), PSO_DontSpawn),   // after gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-899.46f, -20.0f, 1109.54f), PSO_Spawn),        // to HoB
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-738.00f, -28.0f, 2354.00f), PSO_Spawn),     // flower pot
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wealthy, Vector3f(-347.22f, -20.0f, 4107.39f), PSO_DontSpawn), // near SH
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kogane, Vector3f(518.00f, -75.0f, 3719.00f), PSO_Spawn),       // near bulb
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-383.96f, 30.0f, 575.85f), PSO_DontSpawn),     // near electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-71.49f, -20.0f, 1117.59f), PSO_Spawn),        // near WFG
};

static Preset::EnemyGenSpawnOverride e_at_enter_coc[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_MiniHoudai, Vector3f(-1576.15f, 20.0f, 1320.49f), PSO_Spawn), // wandering
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-3232.67f, 0.0f, -123.87f), PSO_Spawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-2245.25, 50.0f, 676.88f), PSO_Spawn),     // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-2781.4f, -30.0f, 162.67f), PSO_Spawn),       // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-3007.17f, -30.0f, 574.86f), PSO_Spawn),    // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-1340.17f, 30.0f, 491.33f), PSO_Spawn),       // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-1310.87f, 30.0f, 346.84f), PSO_Spawn),        // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1218.46f, 23.95f, 766.49f), PSO_Spawn),    // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1220.70f, 22.19f, 679.39f), PSO_Spawn),    // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1500.08f, -10.0f, 1002.67f), PSO_Spawn),      // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-342.40f, 0.0f, 1391.13f), PSO_Spawn), // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-92.24f, 0.0f, 1681.89f), PSO_Spawn),  // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-145.68f, 0.0f, 1537.79f), PSO_Spawn),         // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-467.91f, 0.0f, 1477.60f), PSO_Spawn),         // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-241.84f, 0.0f, 1716.82f), PSO_Spawn),         // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-3538.90f, -30.0f, 1044.17f), PSO_Spawn),    // water gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-2976.20f, 0.0f, 2237.37f), PSO_Spawn),        // electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kabuto, Vector3f(-2172.05f, 30.0f, 2714.40f), PSO_Spawn),     // near pinecone
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-2992.96f, 0.0f, 1651.54f), PSO_Spawn),        // near berries
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wtank, Vector3f(-3244.42f, 0.0f, 2052.76f), PSO_Spawn),       // near berries
};

static Preset::EnemyGenSpawnOverride e_at_enter_dd[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_MiniHoudai, Vector3f(-1576.15f, 20.0f, 1320.49f), PSO_DontSpawn), // wandering
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-3232.67f, 0.0f, -123.87f), PSO_DontSpawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-2245.25, 50.0f, 676.88f), PSO_DontSpawn),     // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-2781.4f, -30.0f, 162.67f), PSO_Spawn),           // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-3007.17f, -30.0f, 574.86f), PSO_DontSpawn),    // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-1340.17f, 30.0f, 491.33f), PSO_DontSpawn),       // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-1310.87f, 30.0f, 346.84f), PSO_DontSpawn),        // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1218.46f, 23.95f, 766.49f), PSO_Spawn),        // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1220.70f, 22.19f, 679.39f), PSO_Spawn),        // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1500.08f, -10.0f, 1002.67f), PSO_Spawn),          // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-342.40f, 0.0f, 1391.13f), PSO_Spawn),     // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-92.24f, 0.0f, 1681.89f), PSO_Spawn),      // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-145.68f, 0.0f, 1537.79f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-467.91f, 0.0f, 1477.60f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-241.84f, 0.0f, 1716.82f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-3538.90f, -30.0f, 1044.17f), PSO_Spawn),        // water gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-2976.20f, 0.0f, 2237.37f), PSO_Spawn),            // electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kabuto, Vector3f(-2172.05f, 30.0f, 2714.40f), PSO_Spawn),         // near pinecone
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-2992.96f, 0.0f, 1651.54f), PSO_Spawn),            // near berries
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wtank, Vector3f(-3244.42f, 0.0f, 2052.76f), PSO_Spawn),           // near berries
};

static Preset::EnemyGenSpawnOverride e_at_enter_hoh[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_MiniHoudai, Vector3f(-1576.15f, 20.0f, 1320.49f), PSO_DontSpawn), // wandering
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-3232.67f, 0.0f, -123.87f), PSO_DontSpawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-2245.25, 50.0f, 676.88f), PSO_DontSpawn),     // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-2781.4f, -30.0f, 162.67f), PSO_DontSpawn),       // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-3007.17f, -30.0f, 574.86f), PSO_DontSpawn),    // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-1340.17f, 30.0f, 491.33f), PSO_DontSpawn),       // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-1310.87f, 30.0f, 346.84f), PSO_DontSpawn),        // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1218.46f, 23.95f, 766.49f), PSO_Spawn),        // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1220.70f, 22.19f, 679.39f), PSO_Spawn),        // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1500.08f, -10.0f, 1002.67f), PSO_Spawn),          // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-342.40f, 0.0f, 1391.13f), PSO_Spawn),     // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-92.24f, 0.0f, 1681.89f), PSO_Spawn),      // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-145.68f, 0.0f, 1537.79f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-467.91f, 0.0f, 1477.60f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-241.84f, 0.0f, 1716.82f), PSO_Spawn),             // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-3538.90f, -30.0f, 1044.17f), PSO_Spawn),        // water gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-2976.20f, 0.0f, 2237.37f), PSO_Spawn),            // electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kabuto, Vector3f(-2172.05f, 30.0f, 2714.40f), PSO_Spawn),         // near pinecone
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-2992.96f, 0.0f, 1651.54f), PSO_Spawn),            // near berries
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wtank, Vector3f(-3244.42f, 0.0f, 2052.76f), PSO_Spawn),           // near berries
};

static Preset::EnemyGenSpawnOverride e_at_post_enter_hoh[] = {
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_MiniHoudai, Vector3f(-1576.15f, 20.0f, 1320.49f), PSO_DontSpawn), // wandering
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-3232.67f, 0.0f, -123.87f), PSO_DontSpawn),    // near base
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kochappy, Vector3f(-2245.25, 50.0f, 676.88f), PSO_DontSpawn),     // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Sarai, Vector3f(-2781.4f, -30.0f, 162.67f), PSO_DontSpawn),       // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Rkabuto, Vector3f(-3007.17f, -30.0f, 574.86f), PSO_DontSpawn),    // near CoC
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Armor, Vector3f(-1340.17f, 30.0f, 491.33f), PSO_DontSpawn),       // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-1310.87f, 30.0f, 346.84f), PSO_DontSpawn),        // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1218.46f, 23.95f, 766.49f), PSO_DontSpawn),    // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_GasHiba, Vector3f(-1220.70f, 22.19f, 679.39f), PSO_DontSpawn),    // DD gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Frog, Vector3f(-1500.08f, -10.0f, 1002.67f), PSO_Spawn),          // near bridge
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-342.40f, 0.0f, 1391.13f), PSO_Spawn),     // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hanachirashi, Vector3f(-92.24f, 0.0f, 1681.89f), PSO_Spawn),      // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-145.68f, 0.0f, 1537.79f), PSO_DontSpawn),  // impact site (nut)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-467.91f, 0.0f, 1477.60f), PSO_Spawn),      // impact site (bump)
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Hana, Vector3f(-241.84f, 0.0f, 1716.82f), PSO_Spawn),      // impact site
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Jigumo, Vector3f(-3538.90f, -30.0f, 1044.17f), PSO_Spawn), // water gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tobi, Vector3f(-2976.20f, 0.0f, 2237.37f), PSO_Spawn),     // electric gate
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Kabuto, Vector3f(-2172.05f, 30.0f, 2714.40f), PSO_Spawn),  // near pinecone
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Tank, Vector3f(-2992.96f, 0.0f, 1651.54f), PSO_Spawn),     // near berries
	Preset::EnemyGenSpawnOverride(EnemyTypeID::EnemyID_Wtank, Vector3f(-3244.42f, 0.0f, 2052.76f), PSO_Spawn),    // near berries
};

/////////////////////////////////////////////////////////////
/////////////////////////// GATES ///////////////////////////
static const char* g_at_post_enter_ec[]  = { "EC gate" };
static const char* g_at_post_enter_wfg[] = { "EC gate", "black gate" };
static const char* g_at_post_enter_scx[] = { "EC gate", "black gate", "water gate" };
static const char* g_at_post_enter_smc[] = { "EC gate", "black gate", "water gate", "zirconium rotor gate" };
static const char* g_at_post_enter_bk[]  = { "EC gate", "black gate", "water gate", "zirconium rotor gate", "blue onion gate", "SH gate" };
static const char* g_at_post_enter_hoh[]
    = { "EC gate", "black gate", "water gate", "zirconium rotor gate", "blue onion gate", "SH gate", "bridge gate", "HoH gate" };

/////////////////////////////////////////////////////////////
/////////////////////////// BAGS ////////////////////////////
static const char* b_post_enter_ec[] = { "landing area bag (15)", "hubcap bag (35)" };
static const char* b_all[]           = { "landing area bag (15)", "hubcap bag (35)", "WFG bag (200)" };

/////////////////////////////////////////////////////////////
////////////////////////// BRIDGES //////////////////////////
static const char* br_at_post_aw1[]       = { "globe bridge" };
static const char* br_at_post_enter_scx[] = { "globe bridge", "water bridge" };
static const char* br_at_post_enter_fc[]  = { "globe bridge", "water bridge", "FC bridge" };
static const char* br_at_post_enter_sr[]  = { "globe bridge", "water bridge", "FC bridge", "bitter plant bridge", "SR bridge" };
static const char* br_at_post_enter_dd[] = { "globe bridge", "water bridge", "FC bridge", "bitter plant bridge", "SR bridge", "DD bridge" };

/////////////////////////////////////////////////////////////
////////////////////////// UPGRADES /////////////////////////
static OlimarData::ItemIndex ek_at_post_vor1[] = { OlimarData::ODII_SphericalAtlas };
static OlimarData::ItemIndex ek_at_post_wfg[]  = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_FiveManNapsack };
static OlimarData::ItemIndex ek_at_post_hob[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector };
static OlimarData::ItemIndex ek_at_post_aw1[] = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_FiveManNapsack,
	                                              OlimarData::ODII_PrototypeDetector, OlimarData::ODII_GeographicProjection };
static OlimarData::ItemIndex ek_at_post_scx[]
    = { OlimarData::ODII_SphericalAtlas, OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb };
static OlimarData::ItemIndex ek_at_post_fc5[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles };
static OlimarData::ItemIndex ek_at_post_fc8[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage };
static OlimarData::ItemIndex ek_at_post_gk[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage,   OlimarData::ODII_DreamMaterial };
static OlimarData::ItemIndex ek_at_post_smc[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage,   OlimarData::ODII_DreamMaterial,  OlimarData::ODII_ProfessionalNoisemaker };
static OlimarData::ItemIndex ek_at_post_sr[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage,   OlimarData::ODII_DreamMaterial,  OlimarData::ODII_ProfessionalNoisemaker,
	    OlimarData::ODII_AmplifiedAmplifier };
static OlimarData::ItemIndex ek_at_post_bk[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage,   OlimarData::ODII_DreamMaterial,  OlimarData::ODII_ProfessionalNoisemaker,
	    OlimarData::ODII_AmplifiedAmplifier,   OlimarData::ODII_ForgedCourage };
static OlimarData::ItemIndex ek_at_post_sh[]
    = { OlimarData::ODII_SphericalAtlas,       OlimarData::ODII_FiveManNapsack, OlimarData::ODII_PrototypeDetector,
	    OlimarData::ODII_GeographicProjection, OlimarData::ODII_StellarOrb,     OlimarData::ODII_BruteKnuckles,
	    OlimarData::ODII_RepugnantAppendage,   OlimarData::ODII_DreamMaterial,  OlimarData::ODII_ProfessionalNoisemaker,
	    OlimarData::ODII_AmplifiedAmplifier,   OlimarData::ODII_ForgedCourage,  OlimarData::ODII_JusticeAlloy };

void PresetMgr::init_at_presets()
{
	init_at_vor1_presets();
	init_at_aw1_presets();
	init_at_vor2_presets();
	init_at_pp_presets();
	init_at_aw2_presets();
	init_at_ww_presets();
}

void PresetMgr::init_at_vor1_presets()
{
	// presets.push((new Preset("Day 1", AT))->set_day(1));
	// presets.push((new Preset("Day 2", AT))
	//                  ->set_onion_pikmin(Leaf, Red, 0)
	//                  ->set_day(2)
	//                  ->set_cutscene_flags(NUM_DAY_2_DEMO_FLAGS, (DemoFlags[NUM_DAY_2_DEMO_FLAGS]) { DAY_2_DEMO_FLAGS }));
	// presets.push((new Preset("Day 3", AT))
	//                  ->set_onion_pikmin(Leaf, Red, 9)
	//                  ->set_day(3)
	//                  ->set_cutscene_flags(NUM_DAY_3_DEMO_FLAGS, (DemoFlags[NUM_DAY_3_DEMO_FLAGS]) { DAY_3_DEMO_FLAGS }));
	// presets.push((new Preset("Day 4", AT))
	//                  ->set_onion_pikmin(Leaf, Red, 13)
	//                  ->set_day(4)
	//                  ->set_cutscene_flags(NUM_DAY_4_DEMO_FLAGS, (DemoFlags[NUM_DAY_4_DEMO_FLAGS]) { DAY_4_DEMO_FLAGS }));
	presets.push((new Preset("EC1 (1st visit)", AT))
	                 ->set_pikmin(Flower, Red, 46)
	                 ->set_pikmin(Leaf, Red, 6)
	                 ->set_time(10.25f)
	                 ->set_day(4)
	                 ->set_cutscene_flags(NUM_EC1_DEMO_FLAGS, (DemoFlags[NUM_EC1_DEMO_FLAGS]) { EC1_DEMO_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_EC_CAVE_FLAGS, (CaveIndex[NUM_EC_CAVE_FLAGS]) { EC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_ec), g_at_post_enter_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_enter_ec), b_post_enter_ec));
	presets.push((new Preset("EC2 (1st visit)", AT))
	                 ->set_pikmin(Flower, Red, 46)
	                 ->set_pikmin(Leaf, Red, 6)
	                 ->set_time(10.25f)
	                 ->set_day(4)
	                 ->set_cutscene_flags(NUM_EC2_DEMO_FLAGS, (DemoFlags[NUM_EC2_DEMO_FLAGS]) { EC2_DEMO_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_EC_CAVE_FLAGS, (CaveIndex[NUM_EC_CAVE_FLAGS]) { EC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_ec), g_at_post_enter_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_enter_ec), b_post_enter_ec));
	presets.push((new Preset("EC1-2 (2nd visit)", AT))
	                 ->set_pikmin(Flower, Red, 35)
	                 ->set_pikmin(Leaf, Red, 5)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_time(10.75f)
	                 ->set_day(4)
	                 ->set_cutscene_flags(NUM_EC_VISIT_2_DEMO_FLAGS, (DemoFlags[NUM_EC_VISIT_2_DEMO_FLAGS]) { EC_VISIT_2_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_VOR1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_VOR1_EK_FLAGS]) { POST_VOR1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_EC_CAVE_FLAGS, (CaveIndex[NUM_EC_CAVE_FLAGS]) { EC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_ec), g_at_post_enter_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_enter_ec), b_post_enter_ec));
}

void PresetMgr::init_at_aw1_presets()
{
	presets.push((new Preset("enter WFG", AT))
	                 ->set_onion_pikmin(Flower, Red, 28)
	                 ->set_onion_pikmin(Leaf, Red, 4)
	                 ->set_onion_pikmin(Leaf, Purple, 20)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_vor1), ek_at_post_vor1)
	                 ->set_cutscene_flags(NUM_AW1_DEMO_FLAGS, (DemoFlags[NUM_AW1_DEMO_FLAGS]) { AW1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_VOR1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_VOR1_EK_FLAGS]) { POST_VOR1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_EC_CAVE_FLAGS, (CaveIndex[NUM_EC_CAVE_FLAGS]) { EC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_ec), g_at_post_enter_ec)
	                 ->set_bags_flattened(ARRAY_SIZE(b_post_enter_ec), b_post_enter_ec)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_wfg), e_at_enter_wfg)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_start), t_at_start)
	                 ->set_enter_kind(PEK_FirstEnter));
	presets.push((new Preset("WFG1", AT))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(9.25f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_vor1), ek_at_post_vor1)
	                 ->set_cutscene_flags(NUM_AW1_DEMO_FLAGS, (DemoFlags[NUM_AW1_DEMO_FLAGS]) { AW1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_VOR1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_VOR1_EK_FLAGS]) { POST_VOR1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hob), e_at_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_start), t_at_start)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("WFG2-3", AT))
	                 ->set_pikmin(Flower, Red, 52)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_time(9.25f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_vor1), ek_at_post_vor1)
	                 ->set_cutscene_flags(NUM_POST_WFG1_DEMO_FLAGS, (DemoFlags[NUM_POST_WFG1_DEMO_FLAGS]) { POST_WFG1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_VOR1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_VOR1_EK_FLAGS]) { POST_VOR1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hob), e_at_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_start), t_at_start)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("WFG4-5", AT))
	                 ->set_pikmin(Flower, Red, 38)
	                 ->set_pikmin(Flower, Purple, 19)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(9.25f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_vor1), ek_at_post_vor1)
	                 ->set_cutscene_flags(NUM_POST_WFG3_DEMO_FLAGS, (DemoFlags[NUM_POST_WFG3_DEMO_FLAGS]) { POST_WFG3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_VOR1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_VOR1_EK_FLAGS]) { POST_VOR1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hob), e_at_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_start), t_at_start)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter HoB", AT))
	                 ->set_pikmin(Flower, Red, 38)
	                 ->set_pikmin(Flower, Purple, 19)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_time(9.25f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_wfg), ek_at_post_wfg)
	                 ->set_cutscene_flags(NUM_POST_WFG3_DEMO_FLAGS, (DemoFlags[NUM_POST_WFG3_DEMO_FLAGS]) { POST_WFG3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_WFG_EK_FLAGS]) { POST_WFG_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_WFG_CAVE_FLAGS, (CaveIndex[NUM_WFG_CAVE_FLAGS]) { WFG_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hob), e_at_enter_hob)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_start), t_at_start)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("HoB1-2", AT))
	        ->set_pikmin(Flower, Red, 36)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Flower, White, 15)
	        ->set_onion_pikmin(Flower, Red, 1)
	        ->set_onion_pikmin(Leaf, Blue, 20)
	        ->set_time(11.0f)
	        ->set_day(5)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_wfg), ek_at_post_wfg)
	        ->set_cutscene_flags(NUM_POST_ENTER_HOB_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_HOB_DEMO_FLAGS]) { POST_ENTER_HOB_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_WFG_EK_FLAGS]) { POST_WFG_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_day_5), e_at_day_5)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_eb), t_at_post_eb)
	        ->set_sprays(true, 1, false, 0)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("HoB3", AT))
	        ->set_pikmin(Flower, Red, 26)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Leaf, Purple, 10)
	        ->set_pikmin(Flower, White, 15)
	        ->set_onion_pikmin(Flower, Red, 1)
	        ->set_onion_pikmin(Leaf, Blue, 20)
	        ->set_time(11.0f)
	        ->set_day(5)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_wfg), ek_at_post_wfg)
	        ->set_cutscene_flags(NUM_POST_ENTER_HOB_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_HOB_DEMO_FLAGS]) { POST_ENTER_HOB_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_WFG_EK_FLAGS]) { POST_WFG_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_day_5), e_at_day_5)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_eb), t_at_post_eb)
	        ->set_sprays(true, 1, false, 0)
	        ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoB4", AT))
	                 ->set_pikmin(Flower, Red, 26)
	                 ->set_pikmin(Flower, Purple, 19)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_onion_pikmin(Leaf, Blue, 20)
	                 ->set_time(11.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_wfg), ek_at_post_wfg)
	                 ->set_cutscene_flags(NUM_POST_HOB3_DEMO_FLAGS, (DemoFlags[NUM_POST_HOB3_DEMO_FLAGS]) { POST_HOB3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_WFG_EK_FLAGS]) { POST_WFG_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_day_5), e_at_day_5)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_eb), t_at_post_eb)
	                 ->set_sprays(true, 1, false, 0)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoB5", AT))
	                 ->set_pikmin(Flower, Red, 21)
	                 ->set_pikmin(Flower, Purple, 34)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_onion_pikmin(Leaf, Blue, 20)
	                 ->set_time(11.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_wfg), ek_at_post_wfg)
	                 ->set_cutscene_flags(NUM_POST_HOB3_DEMO_FLAGS, (DemoFlags[NUM_POST_HOB3_DEMO_FLAGS]) { POST_HOB3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_WFG_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_WFG_EK_FLAGS]) { POST_WFG_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_day_5), e_at_day_5)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_eb), t_at_post_eb)
	                 ->set_sprays(true, 1, false, 0)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("day 5 cleanup", AT))
	                 ->set_pikmin(Flower, Red, 21)
	                 ->set_pikmin(Flower, Purple, 34)
	                 ->set_pikmin(Flower, White, 15)
	                 ->set_onion_pikmin(Flower, Red, 1)
	                 ->set_onion_pikmin(Leaf, Blue, 20)
	                 ->set_time(11.0f)
	                 ->set_day(5)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_hob), ek_at_post_hob)
	                 ->set_cutscene_flags(NUM_POST_HOB3_DEMO_FLAGS, (DemoFlags[NUM_POST_HOB3_DEMO_FLAGS]) { POST_HOB3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_HOB_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_HOB_EK_FLAGS]) { POST_HOB_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_day_5), e_at_day_5)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_eb), t_at_post_eb)
	                 ->set_sprays(true, 1, false, 0)
	                 ->set_enter_kind(PEK_FromCave));
}

void PresetMgr::init_at_vor2_presets()
{
	presets.push((new Preset("enter SCx", AT))
	                 ->set_onion_pikmin(Flower, Red, 22)
	                 ->set_onion_pikmin(Flower, Purple, 34)
	                 ->set_onion_pikmin(Flower, White, 15)
	                 ->set_onion_pikmin(Flower, Blue, 65)
	                 ->set_day(6)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_aw1), ek_at_post_aw1)
	                 ->set_cutscene_flags(NUM_POST_AW1_DEMO_FLAGS, (DemoFlags[NUM_POST_AW1_DEMO_FLAGS]) { POST_AW1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_AW1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_AW1_EK_FLAGS]) { POST_AW1_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOB_CAVE_FLAGS, (CaveIndex[NUM_HOB_CAVE_FLAGS]) { HOB_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_wfg), g_at_post_enter_wfg)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_aw1), br_at_post_aw1)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_scx), e_at_enter_scx)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_aw1), t_at_post_aw1)
	                 ->set_sprays(true, 7, true, 1)
	                 ->set_enter_kind(PEK_FromMap));
	presets.push(
	    (new Preset("SCx1-3", AT))
	        ->set_pikmin(Flower, Red, 15)
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Flower, White, 15)
	        ->set_pikmin(Flower, Blue, 50)
	        ->set_onion_pikmin(Flower, Red, 7)
	        ->set_onion_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_day(6)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_aw1), ek_at_post_aw1)
	        ->set_cutscene_flags(NUM_POST_ENTER_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SCX_DEMO_FLAGS]) { POST_ENTER_SCX_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_AW1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_AW1_EK_FLAGS]) { POST_AW1_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_scx), br_at_post_enter_scx)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_fc), e_at_enter_fc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_scx), t_at_post_enter_scx)
	        ->set_sprays(true, 6, true, 1)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SCx4", AT))
	        ->set_pikmin(Flower, Red, 15)
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Flower, White, 15)
	        ->set_pikmin(Leaf, White, 5)
	        ->set_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Flower, Red, 7)
	        ->set_onion_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_day(6)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_aw1), ek_at_post_aw1)
	        ->set_cutscene_flags(NUM_POST_ENTER_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SCX_DEMO_FLAGS]) { POST_ENTER_SCX_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_AW1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_AW1_EK_FLAGS]) { POST_AW1_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_scx), br_at_post_enter_scx)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_fc), e_at_enter_fc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_scx), t_at_post_enter_scx)
	        ->set_sprays(true, 6, true, 1)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SCx5-8", AT))
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Flower, White, 35)
	        ->set_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Flower, Red, 7)
	        ->set_onion_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_day(6)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_aw1), ek_at_post_aw1)
	        ->set_cutscene_flags(NUM_POST_ENTER_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SCX_DEMO_FLAGS]) { POST_ENTER_SCX_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_AW1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_AW1_EK_FLAGS]) { POST_AW1_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_scx), br_at_post_enter_scx)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_fc), e_at_enter_fc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_scx), t_at_post_enter_scx)
	        ->set_sprays(true, 6, true, 1)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SCx9", AT))
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Leaf, Purple, 15)
	        ->set_pikmin(Flower, White, 35)
	        ->set_pikmin(Flower, Blue, 30)
	        ->set_onion_pikmin(Flower, Red, 7)
	        ->set_onion_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_day(6)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_aw1), ek_at_post_aw1)
	        ->set_cutscene_flags(NUM_POST_ENTER_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SCX_DEMO_FLAGS]) { POST_ENTER_SCX_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_AW1_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_AW1_EK_FLAGS]) { POST_AW1_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_scx), br_at_post_enter_scx)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_fc), e_at_enter_fc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_scx), t_at_post_enter_scx)
	        ->set_sprays(true, 6, true, 1)
	        ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter FC", AT))
	                 ->set_pikmin(Flower, Purple, 30)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 30)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 19)
	                 ->set_onion_pikmin(Flower, Blue, 15)
	                 ->set_day(6)
	                 ->set_time(8.5f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_scx), ek_at_post_scx)
	                 ->set_cutscene_flags(NUM_POST_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_SCX_DEMO_FLAGS]) { POST_SCX_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SCX_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SCX_EK_FLAGS]) { POST_SCX_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_SCX_CAVE_FLAGS, (CaveIndex[NUM_SCX_CAVE_FLAGS]) { SCX_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_scx), br_at_post_enter_scx)
	                 ->set_plug_destroyed(true)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_fc), e_at_enter_fc)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_scx), t_at_post_enter_scx)
	                 ->set_sprays(true, 6, true, 0)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC1", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_scx), ek_at_post_scx)
	                 ->set_cutscene_flags(NUM_POST_SCX_DEMO_FLAGS, (DemoFlags[NUM_POST_SCX_DEMO_FLAGS]) { POST_SCX_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SCX_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SCX_EK_FLAGS]) { POST_SCX_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 0)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC2-3", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_scx), ek_at_post_scx)
	                 ->set_cutscene_flags(NUM_POST_FC1_DEMO_FLAGS, (DemoFlags[NUM_POST_FC1_DEMO_FLAGS]) { POST_FC1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SCX_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SCX_EK_FLAGS]) { POST_SCX_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 12)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC4", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_pikmin(Leaf, Bulbmin, 10)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_scx), ek_at_post_scx)
	                 ->set_cutscene_flags(NUM_POST_FC1_DEMO_FLAGS, (DemoFlags[NUM_POST_FC1_DEMO_FLAGS]) { POST_FC1_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SCX_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SCX_EK_FLAGS]) { POST_SCX_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 12)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC5", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Leaf, Purple, 5)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_pikmin(Leaf, Bulbmin, 5)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_scx), ek_at_post_scx)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SCX_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SCX_EK_FLAGS]) { POST_SCX_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 11)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC6", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_fc5), ek_at_post_fc5)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC5_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC5_EK_FLAGS]) { POST_FC5_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 11)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC7", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_pikmin(Leaf, Bulbmin, 10)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_fc5), ek_at_post_fc5)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC5_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC5_EK_FLAGS]) { POST_FC5_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 5, true, 11)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("FC8", AT))
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Leaf, Purple, 10)
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Leaf, White, 5)
	                 ->set_pikmin(Flower, Blue, 25)
	                 ->set_pikmin(Leaf, Bulbmin, 5)
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 29)
	                 ->set_onion_pikmin(Flower, Blue, 20)
	                 ->set_day(6)
	                 ->set_time(10.0f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_fc5), ek_at_post_fc5)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC5_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC5_EK_FLAGS]) { POST_FC5_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_fc), t_at_post_enter_fc)
	                 ->set_sprays(true, 9, true, 11)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("VoR cleanup", AT))
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 59)
	                 ->set_onion_pikmin(Flower, White, 40)
	                 ->set_onion_pikmin(Flower, Blue, 45)
	                 ->set_day(7)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_plug_destroyed(true)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_vor_cleanup), e_at_vor_cleanup)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_scrap), t_at_post_scrap)
	                 ->set_sprays(true, 8, true, 10)
	                 ->set_enter_kind(PEK_FromMap));
}

void PresetMgr::init_at_pp_presets()
{
	presets.push((new Preset("enter CoS", AT))
	                 ->set_onion_pikmin(Flower, Red, 7)
	                 ->set_onion_pikmin(Flower, Purple, 59)
	                 ->set_onion_pikmin(Flower, White, 40)
	                 ->set_onion_pikmin(Flower, Blue, 45)
	                 ->set_onion_pikmin(Leaf, Blue, 27)
	                 ->set_day(8)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	                 ->set_cutscene_flags(NUM_POST_FC3_DEMO_FLAGS, (DemoFlags[NUM_POST_FC3_DEMO_FLAGS]) { POST_FC3_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_FC_CAVE_FLAGS, (CaveIndex[NUM_FC_CAVE_FLAGS]) { FC_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_cos), e_at_enter_cos)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	                 ->set_sprays(true, 8, true, 9)
	                 ->set_enter_kind(PEK_FirstEnter));
	presets.push(
	    (new Preset("CoS1-2", AT))
	        ->set_pikmin(Flower, Red, 5)
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Leaf, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_COS_CAVE_FLAGS, (CaveIndex[NUM_COS_CAVE_FLAGS]) { COS_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_gk), e_at_enter_gk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 8, true, 9)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("CoS3-5", AT))
	        ->set_pikmin(Flower, Red, 5)
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_FC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_FC_EK_FLAGS]) { POST_FC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_COS_CAVE_FLAGS, (CaveIndex[NUM_COS_CAVE_FLAGS]) { COS_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_gk), e_at_enter_gk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 8, true, 9)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("enter GK", AT))
	        ->set_pikmin(Flower, Red, 5)
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(8.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_COS_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_COS_EK_FLAGS]) { POST_COS_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_COS_CAVE_FLAGS, (CaveIndex[NUM_COS_CAVE_FLAGS]) { COS_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_gk), e_at_enter_gk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 9, true, 8)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("GK1-5", AT))
	        ->set_pikmin(Flower, Red, 5)
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(9.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_COS_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_COS_EK_FLAGS]) { POST_COS_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_GK_CAVE_FLAGS, (CaveIndex[NUM_GK_CAVE_FLAGS]) { GK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_smc), e_at_enter_smc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 9, true, 8)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("GK6", AT))
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Leaf, Purple, 5)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(9.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_fc8), ek_at_post_fc8)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_COS_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_COS_EK_FLAGS]) { POST_COS_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_GK_CAVE_FLAGS, (CaveIndex[NUM_GK_CAVE_FLAGS]) { GK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_smc), e_at_enter_smc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 9, true, 7)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("enter SmC", AT))
	        ->set_pikmin(Flower, Purple, 31)
	        ->set_pikmin(Leaf, Purple, 5)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 28)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 27)
	        ->set_day(8)
	        ->set_time(9.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_gk), ek_at_post_gk)
	        ->set_cutscene_flags(NUM_POST_ENTER_COS_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COS_DEMO_FLAGS]) { POST_ENTER_COS_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_GK_EK_FLAGS]) { POST_GK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_GK_CAVE_FLAGS, (CaveIndex[NUM_GK_CAVE_FLAGS]) { GK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_scx), g_at_post_enter_scx)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_smc), e_at_enter_smc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_vor2), t_at_post_vor2)
	        ->set_sprays(true, 9, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SmC1-2", AT))
	        ->set_pikmin(Flower, Blue, 66)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 59)
	        ->set_onion_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 6)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_day(8)
	        ->set_time(12.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_gk), ek_at_post_gk)
	        ->set_cutscene_flags(NUM_POST_ENTER_SMC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SMC_DEMO_FLAGS]) { POST_ENTER_SMC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_GK_EK_FLAGS]) { POST_GK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sr), e_at_enter_sr)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_smc), t_at_post_enter_smc)
	        ->set_sprays(true, 8, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SmC3", AT))
	        ->set_pikmin(Flower, Blue, 66)
	        ->set_pikmin(Leaf, Bulbmin, 10)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 59)
	        ->set_onion_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 6)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_day(8)
	        ->set_time(12.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_gk), ek_at_post_gk)
	        ->set_cutscene_flags(NUM_POST_ENTER_SMC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SMC_DEMO_FLAGS]) { POST_ENTER_SMC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_GK_EK_FLAGS]) { POST_GK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sr), e_at_enter_sr)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_smc), t_at_post_enter_smc)
	        ->set_sprays(true, 7, true, 5)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SmC4", AT))
	        ->set_pikmin(Flower, Blue, 66)
	        ->set_pikmin(Leaf, Bulbmin, 20)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 59)
	        ->set_onion_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 6)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_day(8)
	        ->set_time(12.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_gk), ek_at_post_gk)
	        ->set_cutscene_flags(NUM_POST_ENTER_SMC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SMC_DEMO_FLAGS]) { POST_ENTER_SMC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_GK_EK_FLAGS]) { POST_GK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sr), e_at_enter_sr)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_smc), t_at_post_enter_smc)
	        ->set_sprays(true, 6, true, 4)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SmC5", AT))
	        ->set_pikmin(Flower, Blue, 66)
	        ->set_pikmin(Leaf, Bulbmin, 30)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 59)
	        ->set_onion_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 6)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_day(8)
	        ->set_time(12.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_gk), ek_at_post_gk)
	        ->set_cutscene_flags(NUM_POST_ENTER_SMC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SMC_DEMO_FLAGS]) { POST_ENTER_SMC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_GK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_GK_EK_FLAGS]) { POST_GK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sr), e_at_enter_sr)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_smc), t_at_post_enter_smc)
	        ->set_sprays(true, 5, true, 3)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("enter SR", AT))
	        ->set_pikmin(Flower, Blue, 56)
	        ->set_pikmin(Leaf, Purple, 10)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 59)
	        ->set_onion_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 6)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_day(8)
	        ->set_time(12.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_smc), ek_at_post_smc)
	        ->set_cutscene_flags(NUM_POST_ENTER_SMC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SMC_DEMO_FLAGS]) { POST_ENTER_SMC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SMC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SMC_EK_FLAGS]) { POST_SMC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_fc), br_at_post_enter_fc)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_plug_destroyed(true)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sr), e_at_enter_sr)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_smc), t_at_post_enter_smc)
	        ->set_sprays(true, 5, true, 3)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SR1-4", AT))
	        ->set_pikmin(Flower, Blue, 33)
	        ->set_pikmin(Flower, White, 32)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 40)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, White, 8)
	        ->set_onion_pikmin(Flower, Blue, 29)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(8)
	        ->set_time(16.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_smc), ek_at_post_smc)
	        ->set_cutscene_flags(NUM_POST_ENTER_SR_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SR_DEMO_FLAGS]) { POST_ENTER_SR_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SMC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SMC_EK_FLAGS]) { POST_SMC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_bk), e_at_enter_bk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sr), t_at_post_enter_sr)
	        ->set_sprays(true, 5, true, 3)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SR5", AT))
	        ->set_pikmin(Flower, Blue, 31)
	        ->set_pikmin(Flower, White, 32)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Leaf, Yellow, 18)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 40)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, White, 8)
	        ->set_onion_pikmin(Flower, Blue, 29)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(8)
	        ->set_time(16.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_smc), ek_at_post_smc)
	        ->set_cutscene_flags(NUM_POST_ENTER_SR_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SR_DEMO_FLAGS]) { POST_ENTER_SR_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SMC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SMC_EK_FLAGS]) { POST_SMC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_bk), e_at_enter_bk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sr), t_at_post_enter_sr)
	        ->set_sprays(true, 4, true, 2)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SR6", AT))
	        ->set_pikmin(Flower, Blue, 26)
	        ->set_pikmin(Flower, White, 32)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Leaf, Purple, 5)
	        ->set_pikmin(Leaf, Yellow, 18)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 40)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, White, 8)
	        ->set_onion_pikmin(Flower, Blue, 29)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(8)
	        ->set_time(16.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_smc), ek_at_post_smc)
	        ->set_cutscene_flags(NUM_POST_ENTER_SR_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SR_DEMO_FLAGS]) { POST_ENTER_SR_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SMC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SMC_EK_FLAGS]) { POST_SMC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_bk), e_at_enter_bk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sr), t_at_post_enter_sr)
	        ->set_sprays(true, 4, true, 1)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SR7", AT))
	        ->set_pikmin(Flower, Blue, 26)
	        ->set_pikmin(Flower, White, 32)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Leaf, Purple, 5)
	        ->set_pikmin(Leaf, Yellow, 18)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 40)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, White, 8)
	        ->set_onion_pikmin(Flower, Blue, 29)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 24)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(8)
	        ->set_time(16.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_smc), ek_at_post_smc)
	        ->set_cutscene_flags(NUM_POST_ENTER_SR_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SR_DEMO_FLAGS]) { POST_ENTER_SR_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SMC_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SMC_EK_FLAGS]) { POST_SMC_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_bk), e_at_enter_bk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sr), t_at_post_enter_sr)
	        ->set_sprays(true, 3, true, 5)
	        ->set_enter_kind(PEK_FromCave));
}

void PresetMgr::init_at_aw2_presets()
{
	presets.push(
	    (new Preset("enter BK", AT))
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 64)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 55)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 42)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sr), ek_at_post_sr)
	        ->set_cutscene_flags(NUM_POST_ENTER_SR_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_SR_DEMO_FLAGS]) { POST_ENTER_SR_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SR_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SR_EK_FLAGS]) { POST_SR_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_SMC_CAVE_FLAGS, (CaveIndex[NUM_SMC_CAVE_FLAGS]) { SMC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_smc), g_at_post_enter_smc)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_bk), e_at_enter_bk)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sr), t_at_post_enter_sr)
	        ->set_sprays(true, 6, true, 6)
	        ->set_enter_kind(PEK_FromMap));
	presets.push(
	    (new Preset("BK1-4", AT))
	        ->set_pikmin(Flower, Yellow, 30)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 20)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 44)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 55)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 12)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sr), ek_at_post_sr)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SR_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SR_EK_FLAGS]) { POST_SR_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sh), e_at_enter_sh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_bk), t_at_post_enter_bk)
	        ->set_sprays(true, 7, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("BK5-6", AT))
	        ->set_pikmin(Flower, Yellow, 20)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 20)
	        ->set_pikmin(Leaf, Purple, 10)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 44)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 55)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 12)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sr), ek_at_post_sr)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SR_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SR_EK_FLAGS]) { POST_SR_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sh), e_at_enter_sh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_bk), t_at_post_enter_bk)
	        ->set_sprays(true, 7, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("BK7", AT))
	        ->set_pikmin(Flower, Yellow, 15)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 20)
	        ->set_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 44)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 55)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 12)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sr), ek_at_post_sr)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SR_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SR_EK_FLAGS]) { POST_SR_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sh), e_at_enter_sh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_bk), t_at_post_enter_bk)
	        ->set_sprays(true, 7, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("enter SH", AT))
	        ->set_pikmin(Flower, Yellow, 15)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 35)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 44)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 55)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 12)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.0f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_bk), ek_at_post_bk)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_sh), e_at_enter_sh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_bk), t_at_post_enter_bk)
	        ->set_sprays(true, 7, true, 5)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SH1-3", AT))
	        ->set_pikmin(Flower, Blue, 40)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 60)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_bk), ek_at_post_bk)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_coc), e_at_enter_coc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sh), t_at_post_enter_sh)
	        ->set_sprays(true, 7, true, 5)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("SH4-7", AT))
	        ->set_pikmin(Flower, Blue, 35)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 19)
	        ->set_pikmin(Leaf, Purple, 5)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 60)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_bk), ek_at_post_bk)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_BK_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_BK_EK_FLAGS]) { POST_BK_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_coc), e_at_enter_coc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sh), t_at_post_enter_sh)
	        ->set_sprays(true, 7, true, 5)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("purple farm", AT))
	        ->set_pikmin(Flower, Blue, 35)
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 24)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 60)
	        ->set_onion_pikmin(Leaf, Purple, 15)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(9)
	        ->set_time(10.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_coc), e_at_enter_coc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sh), t_at_post_enter_sh)
	        ->set_sprays(true, 8, true, 8)
	        ->set_enter_kind(PEK_FromCave));
}

void PresetMgr::init_at_ww_presets()
{
	presets.push(
	    (new Preset("enter CoC", AT))
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 84)
	        ->set_onion_pikmin(Leaf, Purple, 20)
	        ->set_onion_pikmin(Flower, White, 40)
	        ->set_onion_pikmin(Flower, Blue, 45)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(10)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_BK_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_BK_DEMO_FLAGS]) { POST_ENTER_BK_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_BK_CAVE_FLAGS, (CaveIndex[NUM_BK_CAVE_FLAGS]) { BK_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_coc), e_at_enter_coc)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_sh), t_at_post_enter_sh)
	        ->set_sprays(true, 8, true, 8)
	        ->set_enter_kind(PEK_FirstEnter));
	presets.push(
	    (new Preset("CoC1-10", AT))
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 29)
	        ->set_pikmin(Leaf, Purple, 1)
	        ->set_pikmin(Flower, Blue, 30)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 55)
	        ->set_onion_pikmin(Leaf, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(10)
	        ->set_time(10.25f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_COC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COC_DEMO_FLAGS]) { POST_ENTER_COC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_COC_CAVE_FLAGS, (CaveIndex[NUM_COC_CAVE_FLAGS]) { COC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_dd), e_at_enter_dd)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_doomsday), t_at_post_doomsday)
	        ->set_sprays(true, 7, true, 8)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("enter DD", AT))
	        ->set_pikmin(Flower, White, 40)
	        ->set_pikmin(Flower, Purple, 29)
	        ->set_pikmin(Leaf, Purple, 1)
	        ->set_pikmin(Flower, Blue, 30)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 55)
	        ->set_onion_pikmin(Leaf, Purple, 19)
	        ->set_onion_pikmin(Flower, Blue, 15)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Flower, Yellow, 27)
	        ->set_onion_pikmin(Leaf, Yellow, 25)
	        ->set_day(10)
	        ->set_time(10.25f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_COC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COC_DEMO_FLAGS]) { POST_ENTER_COC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_COC_CAVE_FLAGS, (CaveIndex[NUM_COC_CAVE_FLAGS]) { COC_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_sr), br_at_post_enter_sr)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_dd), e_at_enter_dd)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_doomsday), t_at_post_doomsday)
	        ->set_sprays(true, 5, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("DD1-9", AT))
	        ->set_pikmin(Flower, White, 30)
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Flower, Blue, 20)
	        ->set_pikmin(Flower, Yellow, 35)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 69)
	        ->set_onion_pikmin(Leaf, Purple, 20)
	        ->set_onion_pikmin(Flower, White, 10)
	        ->set_onion_pikmin(Flower, Blue, 25)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Leaf, Yellow, 17)
	        ->set_day(10)
	        ->set_time(11.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_COC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COC_DEMO_FLAGS]) { POST_ENTER_COC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_DD_CAVE_FLAGS, (CaveIndex[NUM_DD_CAVE_FLAGS]) { DD_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hoh), e_at_enter_hoh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_doomsday), t_at_post_doomsday)
	        ->set_sprays(true, 5, true, 6)
	        ->set_enter_kind(PEK_FromCave));
	presets.push(
	    (new Preset("DD10-14", AT))
	        ->set_pikmin(Flower, White, 30)
	        ->set_pikmin(Flower, Purple, 15)
	        ->set_pikmin(Flower, Blue, 20)
	        ->set_pikmin(Flower, Yellow, 35)
	        ->set_onion_pikmin(Flower, Red, 2)
	        ->set_onion_pikmin(Flower, Purple, 69)
	        ->set_onion_pikmin(Leaf, Purple, 20)
	        ->set_onion_pikmin(Flower, White, 10)
	        ->set_onion_pikmin(Flower, Blue, 25)
	        ->set_onion_pikmin(Leaf, Blue, 1)
	        ->set_onion_pikmin(Leaf, Yellow, 17)
	        ->set_day(10)
	        ->set_time(11.5f)
	        ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	        ->set_cutscene_flags(NUM_POST_ENTER_COC_DEMO_FLAGS, (DemoFlags[NUM_POST_ENTER_COC_DEMO_FLAGS]) { POST_ENTER_COC_DEMO_FLAGS })
	        ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	        ->set_cave_cutscene_flags(NUM_DD_CAVE_FLAGS, (CaveIndex[NUM_DD_CAVE_FLAGS]) { DD_CAVE_FLAGS })
	        ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	        ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	        ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	        ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hoh), e_at_enter_hoh)
	        ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_doomsday), t_at_post_doomsday)
	        ->set_sprays(true, 7, true, 7)
	        ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("enter HoH", AT))
	                 ->set_pikmin(Flower, White, 30)
	                 ->set_pikmin(Flower, Purple, 15)
	                 ->set_pikmin(Flower, Blue, 20)
	                 ->set_pikmin(Flower, Yellow, 35)
	                 ->set_onion_pikmin(Flower, Red, 2)
	                 ->set_onion_pikmin(Flower, Purple, 69)
	                 ->set_onion_pikmin(Leaf, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 10)
	                 ->set_onion_pikmin(Flower, Blue, 25)
	                 ->set_onion_pikmin(Leaf, Blue, 1)
	                 ->set_onion_pikmin(Leaf, Yellow, 17)
	                 ->set_day(10)
	                 ->set_time(11.5f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	                 ->set_cutscene_flags(NUM_POST_DD_DEMO_FLAGS, (DemoFlags[NUM_POST_DD_DEMO_FLAGS]) { POST_DD_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_DD_CAVE_FLAGS, (CaveIndex[NUM_DD_CAVE_FLAGS]) { DD_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_bk), g_at_post_enter_bk)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_enter_hoh), e_at_enter_hoh)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_doomsday), t_at_post_doomsday)
	                 ->set_sprays(true, 11, true, 14)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoH1-5", AT))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 30)
	                 ->set_onion_pikmin(Flower, Red, 2)
	                 ->set_onion_pikmin(Flower, Purple, 64)
	                 ->set_onion_pikmin(Leaf, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 5)
	                 ->set_onion_pikmin(Flower, Blue, 16)
	                 ->set_onion_pikmin(Leaf, Blue, 1)
	                 ->set_onion_pikmin(Flower, Yellow, 9)
	                 ->set_onion_pikmin(Leaf, Yellow, 12)
	                 ->set_day(10)
	                 ->set_time(15.5f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	                 ->set_cutscene_flags(NUM_POST_DD_DEMO_FLAGS, (DemoFlags[NUM_POST_DD_DEMO_FLAGS]) { POST_DD_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOH_CAVE_FLAGS, (CaveIndex[NUM_HOH_CAVE_FLAGS]) { HOH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_hoh), g_at_post_enter_hoh)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_post_enter_hoh), e_at_post_enter_hoh)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_hoh), t_at_post_enter_hoh)
	                 ->set_sprays(true, 8, true, 11)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoH6-8", AT))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 30)
	                 ->set_onion_pikmin(Flower, Red, 2)
	                 ->set_onion_pikmin(Flower, Purple, 64)
	                 ->set_onion_pikmin(Leaf, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 5)
	                 ->set_onion_pikmin(Flower, Blue, 16)
	                 ->set_onion_pikmin(Leaf, Blue, 1)
	                 ->set_onion_pikmin(Flower, Yellow, 9)
	                 ->set_onion_pikmin(Leaf, Yellow, 12)
	                 ->set_day(10)
	                 ->set_time(15.5f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	                 ->set_cutscene_flags(NUM_POST_DD_DEMO_FLAGS, (DemoFlags[NUM_POST_DD_DEMO_FLAGS]) { POST_DD_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOH_CAVE_FLAGS, (CaveIndex[NUM_HOH_CAVE_FLAGS]) { HOH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_hoh), g_at_post_enter_hoh)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_post_enter_hoh), e_at_post_enter_hoh)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_hoh), t_at_post_enter_hoh)
	                 ->set_sprays(true, 9, true, 9)
	                 ->set_enter_kind(PEK_FromCave));
	presets.push((new Preset("HoH9-15", AT))
	                 ->set_pikmin(Flower, White, 35)
	                 ->set_pikmin(Flower, Purple, 20)
	                 ->set_pikmin(Flower, Blue, 30)
	                 ->set_pikmin(Leaf, Bulbmin, 10)
	                 ->set_onion_pikmin(Flower, Red, 2)
	                 ->set_onion_pikmin(Flower, Purple, 64)
	                 ->set_onion_pikmin(Leaf, Purple, 20)
	                 ->set_onion_pikmin(Flower, White, 5)
	                 ->set_onion_pikmin(Flower, Blue, 16)
	                 ->set_onion_pikmin(Leaf, Blue, 1)
	                 ->set_onion_pikmin(Flower, Yellow, 9)
	                 ->set_onion_pikmin(Leaf, Yellow, 12)
	                 ->set_day(10)
	                 ->set_time(15.5f)
	                 ->set_upgrades(ARRAY_SIZE(ek_at_post_sh), ek_at_post_sh)
	                 ->set_cutscene_flags(NUM_POST_DD_DEMO_FLAGS, (DemoFlags[NUM_POST_DD_DEMO_FLAGS]) { POST_DD_DEMO_FLAGS })
	                 ->set_ek_cutscene_flags(NUM_POST_SH_EK_FLAGS, (OlimarData::ItemIndex[NUM_POST_SH_EK_FLAGS]) { POST_SH_EK_FLAGS })
	                 ->set_cave_cutscene_flags(NUM_HOH_CAVE_FLAGS, (CaveIndex[NUM_HOH_CAVE_FLAGS]) { HOH_CAVE_FLAGS })
	                 ->set_destroyed_gates(ARRAY_SIZE(g_at_post_enter_hoh), g_at_post_enter_hoh)
	                 ->set_finished_bridges(ARRAY_SIZE(br_at_post_enter_dd), br_at_post_enter_dd)
	                 ->set_bags_flattened(ARRAY_SIZE(b_all), b_all)
	                 ->set_enemy_spawn_overrides(ARRAY_SIZE(e_at_post_enter_hoh), e_at_post_enter_hoh)
	                 ->set_treasure_spawn_overrides(ARRAY_SIZE(t_at_post_enter_hoh), t_at_post_enter_hoh)
	                 ->set_sprays(true, 7, true, 7)
	                 ->set_enter_kind(PEK_FromCave));
}
