#include "CSVParser.h"
#include "Types.h"
#include "WarshipComponent.h"

using namespace std;

class Warships
{
public:
	vector<WarshipComponent *> warshipStack[40][20];
	int curIndex;

	void loadWarships(Manager *manager) {
		CSVParser *parser = new CSVParser();
		vector<vector<string>> data = parser->initWithCSV("../assets/csv/warships.csv");

		int level = 0;
		int x = 10;
		int y = 10;
		for (auto warshipDatumOrigin = data.begin(); warshipDatumOrigin != data.end(); ++warshipDatumOrigin) {
			vector<string> warshipDatum = *warshipDatumOrigin;
			auto& warship(manager->addEntity());
			warship.addComponenet<WarshipComponent>(0, 0, x, y, 1, (string("../assets/ship/") + warshipDatum.at(0) + string("-0.png")).c_str(), level);

			/*
			switch (stoi(warshipDatum.at(2))) {
			case 1:
				warship.getComponent<WarshipComponent>().data = new IroncladWarship();
				break;
			case 2:
				warship.getComponent<WarshipComponent>().data = new Cruiser();
				break;
			case 3:
				warship.getComponent<WarshipComponent>().data = new Gunboat();
				break;
			case 4:
				warship.getComponent<WarshipComponent>().data = new TorpedoCruiser();
				break;
			}*/

			warship.getComponent<WarshipComponent>().data->id = warshipDatum.at(0);
			warship.getComponent<WarshipComponent>().data->name = warshipDatum.at(1);
			warship.getComponent<WarshipComponent>().data->type = stoi(warshipDatum.at(2));
			warship.getComponent<WarshipComponent>().data->nation = warshipDatum.at(3);
			cout << (string("../assets/ship/") + warshipDatum.at(0) + string("-0.png")).c_str() << endl;
			warship.getComponent<WarshipComponent>().texture0 = TextureManager::loadTexture((string("../assets/ship/") + warshipDatum.at(0) + string("-0.png")).c_str());
			warship.getComponent<WarshipComponent>().texture1 = TextureManager::loadTexture((string("../assets/ship/") + warshipDatum.at(0) + string("-1.png")).c_str());
			warshipStack[x][y].push_back(&warship.getComponent<WarshipComponent>());
			warship.addGroup(groupWarships);

			level++;
		}
		warshipStack[x][y].back()->isTop = TRUE;
		curIndex = -1;
	}

	void checkNext(int x, int y)
	{
		if (curIndex < 0)
		{
			curIndex = warshipStack[x][y].size() - 1;
			warshipStack[x][y][curIndex]->onClicked();
		}
		else
		{
			warshipStack[x][y][curIndex]->offClicked();
			curIndex = (curIndex == 0) ? warshipStack[x][y].size() - 1 : (curIndex - 1);
			warshipStack[x][y][curIndex]->onClicked();
		}
	}

	void choseWarship(int x, int y, string id)
	{
		for (int i = 0; i < warshipStack[x][y].size(); i ++) {
			if (warshipStack[x][y][i]->data->id == id)
			{
				if (curIndex >= 0)
				{
					warshipStack[x][y][curIndex]->offClicked();
				}
				curIndex = i;
				warshipStack[x][y][curIndex]->onClicked();
				break;
			}
		}
	}
};