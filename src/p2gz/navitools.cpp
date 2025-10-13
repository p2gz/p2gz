#include <p2gz/NaviTools.h>
#include <Game/Navi.h>

using namespace gz;

NaviTools::NaviTools()
{
}

void NaviTools::jump()
{
	active_navi()->mVelocity.add(Vector3f(0.0, 1000.0, 0.0));
}

void NaviTools::kill()
{
	active_navi()->addDamage(9999.0, true);
}
