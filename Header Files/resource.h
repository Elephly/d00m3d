#ifndef RESOURCE_H
#define RESOURCE_H

class Resource{
public:
	enum RESOURCE_TYPE{
		FUEL,
		ENERGY,
		PARTS,
		BACKUP,
		NONE
	};
	Resource(RESOURCE_TYPE, float value);
	~Resource();
	
	RESOURCE_TYPE getType();
	float getValue();
private:
	RESOURCE_TYPE myType;
	float myValue;
};

#endif //RESOURCE_H