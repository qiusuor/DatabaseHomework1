#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

using namespace std;

typedef vector<string> record_type;
class mysql {
protected:
	void fillHelp(string &content, string&fileContent, int expectedSize) {
		for (int i = 0; i < expectedSize; i++)
			if (i < content.size())
				fileContent += content[i];
			else
				fileContent += " ";
	}
	void fillHelp(vector<string> &content, string&fileContent, int expectedSize) {
		for (int i = 0; i < content.size(); i++) {
			fillHelp(content[i], fileContent, expectedSize);
		}
	}
	void fillHelp(vector<vector<string>> &content, string&fileContent, int expectedSize) {
		for (int i = 0; i < content.size(); i++) {
			fillHelp(content[i], fileContent, expectedSize);
		}
	}
	string num2str(int i)
	{
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string sqlFileName;
private:
public:
	mysql(string sqlFileName) {
		this->sqlFileName = sqlFileName;
	}
	string tableName;
	int itemNum;
	int colNum;
	vector<string> headers;
	map<string, int> index;
	vector<vector<string>> contents;

	void readSql();
	void writeSql();
	void builtSqlTable(string& tableName, vector<string> &headers);
	void add(vector<string> &item) {
		contents.push_back(item);
		itemNum++;
		writeSql();
	}
	void printData() {
		cout << "表名：" << tableName << endl;

		for (auto i : headers)
			cout << i << "\t";
		cout << "\n";
		for (auto j : contents) {
			for (auto i : j)
				cout << i << "\t";
			cout << "\n";
		}
	}
	void search(string proerty, string value) {
		readSql();
		cout << "查询字段 ：" << proerty << "   目标值：" << value << endl;
		cout << "查询结果是：" << endl;
		cout << "表名：" << tableName << endl;

		for (auto i : headers)
			cout << i << "\t";
		cout << endl;
		for (int i = 0; i < itemNum; i++) {
			if (contents[i][index[proerty]]==value) {
				for (auto j : contents[i])
					cout << j << "\t";
					cout << "\n";
			}
		}
	}
	void del(string proerty, string value) {
		readSql();

		cout << "以下条目已删除：" << endl;
		vector<vector<string>> tt;
		for (int i = 0; i < itemNum; i++) {
			if (contents[i][index[proerty]] != value) {
				tt.push_back(contents[i]);
			}
			else {
				for (auto j : contents[i])
					cout << j << "\t";
				cout << "\n";
			}
		}
		contents.swap(tt);
		cout<<"删除后结果为："<< endl;
		printData();
		writeSql();
	}
};

void mysql::readSql() {
	headers.clear();
	index.clear();
	contents.clear();
	ifstream inFile(sqlFileName);
	inFile >> tableName;
	inFile >> colNum;
	
	for (int i = 0; i < colNum; i++) {
		string t;
		inFile >> t;
		headers.push_back(t);
		index[t] = i;
	}
	inFile.seekg(256+16+colNum*256 ,ios::beg);
	
	inFile >> itemNum;
	for (int i = 0; i < itemNum; i++) {
		vector<string> col;
		for (int j = 0; j < colNum; j++) {
			string t;
			inFile >> t;
			col.push_back(t);
		}
		contents.push_back(col);
	}
	inFile.close();
}
void mysql::writeSql() {
	string fileContent;
	fillHelp(tableName, fileContent, 256);
	fillHelp(num2str(headers.size()), fileContent, 16);
	fillHelp(headers, fileContent, 256);
	fillHelp(num2str(itemNum), fileContent, 16);
	fillHelp(contents, fileContent, 256);
	ofstream outFile(sqlFileName, ios::binary);
	//cout << fileContent;
	outFile << fileContent;
	outFile.close();
}
void mysql::builtSqlTable(string& tableName, vector<string> &headers) {
	this->tableName = tableName;
	this->itemNum = 0;
	this->headers = headers;
	contents.clear();
	writeSql();
//	writeSql(sqlFileName);
}

int main(void) {
	mysql sql("sql.txt");
	sql.builtSqlTable(string("学生信息表"), std::vector<string>({ "班级","课程", "教师", "学期","学号","姓名","成绩" }));
	sql.add(std::vector<string>({ "981101","数据库","李四", "98秋","98110101","张三", "99" }));
	sql.add(std::vector<string>({ "981101", "数据库","李四","98秋","98110102","张四","59" }));
	sql.add(std::vector<string>({ "981102", "计算机","李四","98秋","98110202","王一一","60" }));
	sql.add(std::vector<string>({ "981101","计算机","李四","98秋","98110203","王物","30" }));
	sql.add(std::vector<string>({ "981101", "数据库","2233", "98秋","98110203","王小博","10" }));
	sql.writeSql();
	sql.readSql();
	sql.printData();
	sql.search("课程", "计算机");
	sql.del("课程", "计算机");
	system("pause");
}