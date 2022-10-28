#ifndef CONTAINER_MANAGER_HPP
#define CONTAINER_MANAGER_HPP


class ContainerManager;

typedef void (ContainerManager::*ContainerManagerCmdPtr) (Command* command);

class ContainerManager
{

public:

    ContainerManager ();
    ~ContainerManager ();

    void Execute (Command* command);

    std::string ActiveContainerName ();

private:

    int active_container;

    vector <Container*> containers;

    Loader* loader;
    Saver*  saver;

    void LoadImageToContainer   (Command* load_image_base);
    void SaveImageFromContainer (Command* save_image_base);

    void AddContainer    (Command* add_container_base);
    void DelContainer    (Command* del_container_base);
    void ListContainers  (Command* list_containers_base);
    void SwitchContainer (Command* swicth_container_base);

    const unordered_map <CommandType, ContainerManagerCmdPtr> operations =
    {
        {CommandType::cmd_load, &ContainerManager::LoadImageToContainer},
        {CommandType::cmd_save, &ContainerManager::SaveImageFromContainer},

        {CommandType::cmd_add_container,    &ContainerManager::AddContainer},
        {CommandType::cmd_del_container,    &ContainerManager::DelContainer},
        {CommandType::cmd_list_containers,  &ContainerManager::ListContainers},
        {CommandType::cmd_switch_container, &ContainerManager::SwitchContainer}
    };
};


#endif // CONTAINER_MANAGER_HPP