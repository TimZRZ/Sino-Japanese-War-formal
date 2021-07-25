#include "Types.h"
#include "WarshipModel.h"

using namespace std;

class WarshipInfoPanel
{
public:
	WarshipModel *curData;

	void loadWarshipInfoPanel(Manager *manager) {
		auto& warshipInfo(manager->addEntity());
		// warshipInfo.addComponenet<WarshipModel>(0, 0, 1, curData);
		warshipInfo.addGroup(groupWarshipInfo);
	}
};