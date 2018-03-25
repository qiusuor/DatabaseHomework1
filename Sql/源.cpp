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
		cout << "������" << tableName << endl;

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
		cout << "��ѯ�ֶ� ��" << proerty << "   Ŀ��ֵ��" << value << endl;
		cout << "��ѯ����ǣ�" << endl;
		cout << "������" << tableName << endl;

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

		cout << "������Ŀ��ɾ����" << endl;
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
		cout<<"ɾ������Ϊ��"<< endl;
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
	sql.builtSqlTable(string("ѧ����Ϣ��"), std::vector<string>({ "�༶","�γ�", "��ʦ", "ѧ��","ѧ��","����","�ɼ�" }));
	sql.add(std::vector<string>({ "981101","���ݿ�","����", "98��","98110101","����", "99" }));
	sql.add(std::vector<string>({ "981101", "���ݿ�","����","98��","98110102","����","59" }));
	sql.add(std::vector<string>({ "981102", "�����","����","98��","98110202","��һһ","60" }));
	sql.add(std::vector<string>({ "981101","�����","����","98��","98110203","����","30" }));
	sql.add(std::vector<string>({ "981101", "���ݿ�","2233", "98��","98110203","��С��","10" }));
	sql.writeSql();
	sql.readSql();
	sql.printData();
	sql.search("�γ�", "�����");
	sql.del("�γ�", "�����");
	system("pause");
}