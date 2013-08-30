
#include "core/pathParser.hpp"
#include "core/logger.hpp"

namespace core
{
    template <typename T, typename L> FakeFS<T,L>::FakeFS(const L* lb, bool todel)
        : m_lb(lb), m_todel(todel), m_first(NULL), m_root(NULL), m_actual(NULL)
    {
        m_root = new _abr;
        m_root->parent = NULL;
        m_root->name.clear();
        m_actual = m_root;
    }

    template <typename T, typename L> FakeFS<T,L>::~FakeFS()
    {
        freeAbr(m_root);
        if(m_todel)
            delete m_lb;
    }

    template <typename T, typename L> bool FakeFS<T,L>::createNamespace(const std::string& name)
    {
        if(existsNamespace(name))
            return false;

        _abr* act; /* Local actual used */
        std::vector<std::string> parts = path::parts(name);
        size_t from; /* First idx in parts to use */

        if(path::absolute(name)) {
            std::string path("/");
            for(size_t i = 0; i < parts.size(); ++i) {
                path += parts[i];
                path += '/';
                if(!existsNamespace(path)) {
                    from = i;
                    break;
                }
            }
        }
        else {
            from = 0;
            act = m_actual;
        }

        size_t depth = parts.size() - from; /* Number of directories to create */
        for(size_t i = 0; i < depth; ++i) {
            _abr* node = new _abr;
            node->name = parts[from + i];
            node->parent = act;
            act->subs.push_back(node);
            act = node;
        }

        return true;
    }

    template <typename T, typename L> void FakeFS<T,L>::deleteNamespace(const std::string& name)
    {
        if(!existsNamespace(name))
            return;

        _abr* root;
        if(path::absolute(name))
            root = m_root;
        else
            root = m_actual;

        /* Find the directory to delete */
        std::vector<std::string> parts = path::parts(name);
        for(size_t i = 0; i < parts.size(); ++i) {
            bool found = false;
            for(size_t j = 0; j < root->subs.size(); ++j) {
                if(root->subs[j]->name == parts[i]) {
                    found = true;
                    root = root->subs[j];
                    break;
                }
            }

            if(!found) {
                return;
            }
        }

        /* Deleting the namespace */
        freeAbr(root);
    }

    template <typename T, typename L> bool FakeFS<T,L>::existsNamespace(const std::string& name) const
    {
        _abr* root;
        if(path::absolute(name))
            root = m_root;
        else
            root = m_actual;
        
        std::vector<std::string> parts = path::parts(name);
        size_t i;
        for(i = 0; i < parts.size(); ++i) {
            bool found = false;
            for(size_t j = 0; j < root->subs.size(); ++j) {
                if(root->subs[j]->name == parts[i]) {
                    found = true;
                    root = root->subs[j];
                    break;
                }
            }

            if(!found)
                break;
        }

        return i == parts.size();
    }

    template <typename T, typename L> bool FakeFS<T,L>::enterNamespace(const std::string& name)
    {
        if(name == "..") {
            if(m_actual->parent) {
                m_actual = m_actual->parent;
                return true;
            }
            else {
                logger::log("Tried to enter root's parent namespace.", logger::WARNING);
                return false;
            }
        }

        if(!existsNamespace(name)) {
            logger::log(std::string("Tried to enter an unexistant namespace : ") + name, logger::WARNING);
            return false;
        }

        _abr* root;
        if(path::absolute(name))
            root = m_root;
        else
            root = m_actual;

        std::vector<std::string> parts = path::parts(name);
        for(size_t i = 0; i < parts.size(); ++i) {
            for(size_t j = 0; j < root->subs.size(); ++j) {
                if(root->subs[j]->name == parts[i]) {
                    root = root->subs[j];
                    break;
                }
            }
        }

        m_actual = root;
        return true;
    }

    template <typename T, typename L> std::string FakeFS<T,L>::actualNamespace() const
    {
        std::string actualName = "/";
        _abr* actual = m_actual;

        while(actual->parent) {
            actualName.insert(0, actual->name);
            actualName.insert(0, 1, '/');
            actual = actual->parent;
        }

        return actualName;
    }

    template <typename T, typename L> bool FakeFS<T,L>::createEntity(const std::string& name, T* value)
    {
        /* If name already used */
        if(existsEntity(name))
            return false;

        /* Add an entity to the list */
        _entity* ent = new _entity;
        ent->prev = NULL;
        ent->next = m_first;
        ent->count = 1;
        ent->value = value;
        m_first = ent;

        /* Put it in the abr */
        m_actual->entities[name] = ent;
        return true;
    }

    template <typename T, typename L> void FakeFS<T,L>::deleteEntity(const std::string& name)
    {
        /* Find the entity */
        typename std::unordered_map<std::string, _entity*>::iterator it = m_actual->entities.find(name);
        if(it == m_actual->entities.end())
            return;

        /* Free it and remove it from the abr */
        freeEntity(m_actual->entities[name]);
        m_actual->entities.erase(it);
    }

    template <typename T, typename L> bool FakeFS<T,L>::existsEntity(const std::string& name)
    {
        return m_actual->entities.find(name) != m_actual->entities.end();
    }

    template <typename T, typename L> bool FakeFS<T,L>::link(const std::string& name, const std::string& target)
    {
        if(existsEntity(name))
            return false;

        /* Parse target path */
        _abr* root;
        if(path::absolute(target))
            root = m_root;
        else
            root = m_actual;

        std::vector<std::string> parts = path::parts(target);
        for(size_t i = 0; i < (parts.size() - 1); ++i) {
            bool found = false;
            for(size_t j = 0; j < root->subs.size(); ++j) {
                if(root->subs[j]->name == parts[i]) {
                    found = true;
                    root = root->subs[j];
                    break;
                }
            }
            if(!found) {
                logger::log(std::string("Tried to link an entity to an unexistant entity : ") + target, logger::WARNING);
                return false;
            }
        }

        /* Find the target entity */
        size_t idx = parts.size() - 1;
        _entity* t = NULL;
        if(root->entities.find(parts[idx]) == root->entities.end()) {
            logger::log(std::string("Tried to link an entity to an unexistant entity : ") + target, logger::WARNING);
            return false;
        }
        else
            t = root->entities[parts[idx]];

        /* Link */
        ++t->count;
        m_actual->entities[name] = t;
        return true;
    }

    template <typename T, typename L> bool FakeFS<T,L>::setEntityValue(const std::string& name, T* value)
    {
        if(!existsEntity(name)) {
            logger::log(std::string("Tried to set an unexistant entity : ") + name, logger::WARNING);
            return false;
        }
        m_actual->entities[name]->value = value;
        return true;
    }

    template <typename T, typename L> T* FakeFS<T,L>::getEntityValue(const std::string& name)
    {
        if(!existsEntity(name))
            return NULL;
        return m_actual->entities[name]->value;
    }
    
    template <typename T, typename L> void FakeFS<T,L>::freeEntity(_entity* e)
    {
        e->count--;
        if(e->count != 0)
            return;

        if(e == m_first)
            m_first = e->next;
        if(e->prev)
            e->prev->next = e->next;
        if(e->next)
            e->next->prev = e->prev;
        (*m_lb)(e->value);
        delete e;
    }

    template <typename T, typename L> void FakeFS<T,L>::freeAbr(_abr* a)
    {
        for(size_t i = 0; i < a->subs.size(); ++i) {
            freeAbr(a->subs[i]);
        }
        for(typename std::unordered_map<std::string, _entity*>::iterator it = a->entities.begin(); it != a->entities.end(); ++it) {
            freeEntity(it->second);
        }
        delete a;
    }
}

