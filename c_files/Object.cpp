#include "Object.hpp"
Object::Object() {

	myName	="";
	myLevel   ="";

	next	=NULL;
	previous=NULL;
	child	=NULL;

}

Object::Object(const string& name, const string& level, Object* pointer) {
	myName	=name;
	myLevel   =level;

	next	=NULL;
	previous=pointer;
	child	=NULL;
}

Object* Object::findChild(const string& name){
	for (Object* tmp = child; tmp != NULL; tmp = tmp->next) {
		if (tmp->myName == name) { return tmp; }
	}
	return NULL;
}

void Object::addChild(const string& name, const string& level){
	if (child==NULL) {
		child = new Object(name, level, 0);
	} else {
		Object* tmp = child;
		while (tmp->next!=NULL) {
			tmp = tmp->next;
		}
		tmp->next = new Object(name, level, tmp);
	}
}

void Object::removeChild(){
	Object* tmp = child;
	for (; tmp != NULL; tmp = tmp->next) {
		tmp->removeChild();
		delete tmp->previous;
	}
	delete tmp;
	child = NULL;
}

void Object::printName(){
	Object* tmp = child;
	cout << this->myLevel << this->myName << endl;
        for (; tmp != NULL; tmp = tmp->next) {
                tmp->printName();
        }
}
