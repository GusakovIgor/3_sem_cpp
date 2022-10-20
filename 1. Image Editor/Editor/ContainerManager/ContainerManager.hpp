#ifndef CONTAINER_MANAGER_HPP
#define CONTAINER_MANAGER_HPP

class ContainerManager
{

public:

	ContainerManager ();
	~ContainerManager ();

	void Execute (Command** command);

	void LoadImageToContainer   (Command* load_image_base);
	void SaveImageFromContainer (Command* save_image_base);

	void AddContainer    (Command* add_container_base);
	void DelContainer    (Command* del_container_base);
	void ListContainers  (Command* list_containers_base);
	void SwitchContainer (Command* swicth_container_base);

private:

	int active_container;

	vector <Container*> containers;

	Loader* loader;
	Saver*  saver;

	unordered_map <CommandType, void (ContainerManager::*) (Command* command)> operations;
};


#endif // CONTAINER_MANAGER_HPP