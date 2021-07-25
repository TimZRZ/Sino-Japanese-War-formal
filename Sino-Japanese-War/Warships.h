#include "CSVParser.h"
#include "Types.h"
#include "WarshipComponent.h"
#include "CommanderModel.h"
#include <algorithm>
#include <map>

using namespace std;

class Warships
{
public:
	vector<WarshipComponent *> warshipStack[40][20];
	vector<WarshipComponent *> warships;
	int curIndex;
	WarshipComponent * curWarship;

	void loadWarships(Manager *manager) {

		std::map<string,CommanderModel *> commandersMap = loadCommanders();

		CSVParser *parser = new CSVParser();
		vector<vector<string>> data = parser->initWithCSV("../assets/csv/warships.csv");

		int level = 0;
		int x = 3;
		int y = 10;
		for (auto warshipDatumOrigin = data.begin(); warshipDatumOrigin != data.end(); ++warshipDatumOrigin) {
			vector<string> warshipDatum = *warshipDatumOrigin;
			auto& warship(manager->addEntity());
			warship.addComponenet<WarshipComponent>(0, 0, x, y, 1, (string("../assets/ship/") + warshipDatum.at(0) + string("-0.png")).c_str(), level);

			warship.getComponent<WarshipComponent>().data->commander = commandersMap[warshipDatum.at(4)];
			warship.getComponent<WarshipComponent>().data->id = warshipDatum.at(0);
			warship.getComponent<WarshipComponent>().data->name = warshipDatum.at(1);
			warship.getComponent<WarshipComponent>().data->type = stoi(warshipDatum.at(2));
			warship.getComponent<WarshipComponent>().data->nation = warshipDatum.at(3);
			warship.getComponent<WarshipComponent>().texture0 = TextureManager::loadTexture((string("../assets/ship/") + warshipDatum.at(0) + string("-0.png")).c_str());
			warship.getComponent<WarshipComponent>().texture1 = TextureManager::loadTexture((string("../assets/ship/") + warshipDatum.at(0) + string("-1.png")).c_str());

			warships.push_back(&warship.getComponent<WarshipComponent>());
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
			curWarship = warshipStack[x][y][curIndex];
		}
		else
		{
			curWarship->offClicked();
			curIndex = (curIndex == 0 or curIndex > warshipStack[x][y].size() - 1) ? warshipStack[x][y].size() - 1 : (curIndex - 1);
			warshipStack[x][y][curIndex]->onClicked();
			curWarship = warshipStack[x][y][curIndex];
		}
	}

	void choseWarship(int x, int y, string id)
	{
		if (curWarship)
		{
			curWarship->offClicked();
		}
		
		for (int i = 0; i < warshipStack[x][y].size(); i ++)
		{
			if (warshipStack[x][y][i]->data->id == id)
			{
				curIndex = i;
				warshipStack[x][y][curIndex]->onClicked();
				curWarship = warshipStack[x][y][curIndex];
				break;
			}
		}
	}

	void move(Vector2D warshipPos, Vector2D newPos, WarshipComponent* chosenWarship)
	{
		warshipStack[(int)newPos.x][(int)newPos.y].push_back(chosenWarship);
		warshipStack[(int)warshipPos.x][(int)warshipPos.y].erase(warshipStack[(int)warshipPos.x][(int)warshipPos.y].begin()+ curIndex);


		curIndex = warshipStack[(int)newPos.x][(int)newPos.y].size() - 1;

		// 重置isTop和panelLevel
		for (int i = 0; i < warshipStack[(int)warshipPos.x][(int)warshipPos.y].size(); i++)
		{
			warshipStack[(int)warshipPos.x][(int)warshipPos.y][i]->panelLevel = i;
			if (i == warshipStack[(int)warshipPos.x][(int)warshipPos.y].size() - 1)
			{
				warshipStack[(int)warshipPos.x][(int)warshipPos.y][i]->isTop = TRUE;
			}
			else
			{
				warshipStack[(int)warshipPos.x][(int)warshipPos.y][i]->isTop = FALSE;
			}
		}
;
		for (int i = 0; i < warshipStack[(int)newPos.x][(int)newPos.y].size(); i++)
		{
			warshipStack[(int)newPos.x][(int)newPos.y][i]->panelLevel = i;
			if (i == warshipStack[(int)newPos.x][(int)newPos.y].size() - 1)
			{
				warshipStack[(int)newPos.x][(int)newPos.y][i]->isTop = TRUE;
			}
			else
			{
				warshipStack[(int)newPos.x][(int)newPos.y][i]->isTop = FALSE;
			}
		}


		// 保证绘制层级
		// TODO: 排序算法有待优化
		for (int i = 0; i < warships.size(); i++)
		{
			for (int j = 0; j < warships.size(); j++)
			{
				if (warships[i]->panelLevel < warships[j]->panelLevel)
				{
					swap(warships[i], warships[j]);
				}
			}
		}
		/*
		cout << "-----------" << endl;
		for (WarshipComponent* s : warships)
		{
			cout << s->panelLevel << endl;
		}
		cout << "-----------" << endl;
		*/
	}

	void draw()
	{
		if (curWarship)
		{
			curWarship->drawSelectRegionTile();
		}

		for (WarshipComponent * warship : warships)
		{
			warship->draw();
		}
	}

	std::map<string, CommanderModel*> loadCommanders() {
		std::map<string, CommanderModel*> commandersMap;
		CSVParser *parser = new CSVParser();
		vector<vector<string>> data = parser->initWithCSV("../assets/csv/commander.csv");
		vector<CommanderModel*> commanders = vector<CommanderModel*>();
		for (auto commanderDatumOrigin = data.begin(); commanderDatumOrigin != data.end(); ++commanderDatumOrigin) {
			vector<string> commanderDatum = *commanderDatumOrigin;
			CommanderModel *commander = new CommanderModel();
			commander->id = commanderDatum.at(0);
			commander->name = commanderDatum.at(1);
			commander->nation = commanderDatum.at(2);
			commander->level = stoi(commanderDatum.at(3));
			commandersMap[commanderDatum.at(0)] = commander;
			cout << commanderDatum.at(0) << endl;
		}
		return commandersMap;
	}
};

/*
q8,廣甲,2,q
q9,超勇,2,q
q10,揚威,2,q
q11,平遠,2,q
q12,廣丙,2,q
q13,鎮中,3,q
q14,鎮南,3,q
q15,福龍,4,q
q16,左一,4,q
q17,左二,4,q
q18,左三,4,q
*/