class Object{
public:

	string myName;
	string myLevel;

	Object *parent;
	Object *next;
	Object *previous;
	Object *child;

	Object();
	Object(const string& name, const string& level, Object* pointer, Object* pointerParent);

	Object* findChild(const string& name);
	void addChild(const string& name, const string& level);
	void removeChild(string& toRemove);
	void removeMe(string& toRemove);
	void printName(string& outputString);
};

