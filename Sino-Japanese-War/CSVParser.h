#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "WarshipComponent.h"

using namespace std;

class CSVParser
{
public:
	vector<vector<std::string>> initWithCSV(string path)
	{
		ifstream  data(path);
		string line;
		vector<vector<std::string>> parsedCsv;
		// 跳过第一个
		getline(data, line);
		while (getline(data, line))
		{
			stringstream lineStream(line);
			string cell;
			cout << line << endl;
			vector<string> parsedRow;
			while (getline(lineStream, cell, ','))
			{
				parsedRow.push_back(cell);
			}
			parsedCsv.push_back(parsedRow);
		}
		return parsedCsv;
	}
};