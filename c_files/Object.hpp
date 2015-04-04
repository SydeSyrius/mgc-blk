class Object{
public:

	string myName;
	string myLevel;

	Object *next;
	Object *previous;
	Object *child;

	Object();
	Object(const string& name, const string& level, Object* pointer);

	Object* findChild(const string& name);
	void addChild(const string& name, const string& level);
	void removeChild();
	void printName();
};

