#ifndef NODECREATOR_H
#define NODECREATOR_H

#include "Node.h"
#include <list>

class NodeCreator
{
public:
    static NodeCreator& Instance(){ static NodeCreator creator; return creator; };

    ~NodeCreator()
    {
        for(auto& i: m_nodesBase)
        {
            delete i;
        }
    }

    template<typename T, typename... TArgs>
    T* Create(TArgs&&... args){
        T* node(new T(std::forward<TArgs>(args)...));
        m_nodesBase.push_back(node);
        return node;
    }

private:
    NodeCreator(){}

    std::list<Expression*> m_nodesBase;
};

#endif // NODECREATOR_H
