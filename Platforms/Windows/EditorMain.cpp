#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

using namespace rapidjson;
using namespace std;

struct MyHandler: public BaseReaderHandler<UTF8<>, MyHandler> {
	bool Null() { cout << "Null()" << endl; return true; }
	bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
	bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
	bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
	bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
	bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
	bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
	bool String(const char* str, SizeType length, bool copy) {
		cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool StartObject() { 
		cout << "StartObject()" << endl; return true; 
	}
	bool Key(const char* str, SizeType length, bool copy) {
		cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
	bool StartArray() { cout << "StartArray()" << endl; return true; }
	bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }

	int val;
};

int main()
{
// 	char json[] = "{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
// 
// 	DataDocument doc;
// 	doc.LoadFromFile("large-file.json");
// 	doc.SaveToFile("large-file-copy.json");
// 	//doc.LoadFromFile("Assets/BasicAtlas.atlas.meta");
// 
// 	rapidjson::Document doc2;
// 	doc2.ParseInsitu(json); 
// 	rapidjson::StringBuffer buffer;
// 	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
// 	doc2.Accept(writer);
// // 	auto& r = doc["hello"];
// // 	auto x = doc.FindMember("abc");
// 	
// // 	MyHandler handler;
// // 	Reader reader;
// // // 	StringStream ss(json);
// // // 	reader.Parse(ss, handler);
// // 	GenericInsituStringStream<UTF8<>> ss(json);
// // 	reader.Parse<kParseInsituFlag>(ss, handler);


	INITIALIZE_O2;

	Editor::EditorApplication* app = mnew Editor::EditorApplication();
	app->Initialize();
	app->Launch();

	return 0;
}
