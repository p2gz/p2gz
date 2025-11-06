#include <p2gz/p2gz.h>
#include <p2gz/NaviTools.h>
#include <Game/Navi.h>

using namespace gz;

NaviTools::NaviTools()
{
	boing_mode_enabled = false;
}

void NaviTools::update()
{
	if (boing_mode_enabled) {
		if (p2gz->controller->getButtonDown() & Controller::PRESS_A) {
			active_navi()->mVelocity.add(Vector3f(0.0, 300.0, 0.0));
		}
	}
}

void NaviTools::sync()
{
	ListMenu* captain_menu       = static_cast<ListMenu*>(p2gz->menu->get_option("captain")->get_sub_menu());
	FloatRangeMenuOption* hp_opt = static_cast<FloatRangeMenuOption*>(captain_menu->get_option("health"));
	hp_opt->set_selection(active_navi()->mHealth);
}

void NaviTools::set_boing_mode(bool enabled)
{
	boing_mode_enabled = enabled;
}

void NaviTools::set_active_navi_hp(f32 hp)
{
	active_navi()->mHealth = hp;
}

void NaviTools::kill()
{
	p2gz->menu->close();
	active_navi()->addDamage(9999.0, true);
}
