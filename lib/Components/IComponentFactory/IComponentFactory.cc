#include "IComponentFactory.h"

using map_type = IComponentFactory::map_type;

__declspec(dllexport) IComponent *  IComponentFactory::createInstance(std::string const& s)
{
    auto it = IComponentFactory::map->find(s);
    if(it == IComponentFactory::map->end())
        return nullptr;
    return it->second();
}