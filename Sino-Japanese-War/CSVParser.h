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
		// ������һ��
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