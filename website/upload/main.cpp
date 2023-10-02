#include <iostream>
using namespace std;
class phone
{
public:
	phone()
	{
		number = 1;
	}
	int number;
};
class person
{
public:
	person()
	{
		p = new phone;
		p_number = 2;
	}
	int p_number;
	phone* p;
};
int mian()
{
	person p1;
	cout << p1.p->number << endl;
	return 0;
}
------WebKitFormBoundaryNqM7Mussx19Ot56B--