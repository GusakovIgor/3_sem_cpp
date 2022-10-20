#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include <string>

using std::string;


class Container
{

public:

	Container (const string& init_name);
	~Container ();

	void SetImage (const string& new_image_name, Image* new_image);
	Image* GetImage ();

	string Name ();
	string ImageName ();

	friend std::ostream& operator << (std::ostream& stream, Container& container);

private:

	string name;
	string image_name;

	Image* image;
};






#endif // CONTAINER_HPP