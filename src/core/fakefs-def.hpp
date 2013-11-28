
#include "core/pathParser.hpp"
#include "core/logger.hpp"
#include <boost/regex.hpp>
#include <sstream>

namespace core
{
    template <typename T, typename L> FakeFS<T,L>::FakeFS(const L* lb, bool todel)
        : m_lb(lb), m_todel(todel), m_root(NULL), m_actual(NULL)
    {
        if(!m_lb)
            m_lb = new L;
        m_todel = true;

        m_root = createNode("", NULL);
        m_actual = m_root;
    }

    template <typename T, typename L> FakeFS<T,L>::~FakeFS()
    {
        freeAbr(m_root);
        if(m_todel)
            delete m_lb;
    }

    template <typename T, typename L> void FakeFS<T,L>::clear()
    {
        freeAbr(m_root);
        m_root = createNode("", NULL);
        m_actual = m_root;
    }

    template <typename T, typename L> bool FakeFS<T,L>::createNamespace(const std::string& name)
    {
        if(existsNamespace(name))
            return false;

        _node* act = NULL;
        std::vector<std::string> parts = path::parts(name);

        if(path::absolute(name))
            act = m_root;
        else
            act = m_actual;

        for(size_t i = 0; i < parts.size(); ++i) {
            auto it = std::find_if(act->subs.begin(), act->subs.end(), [&] (_node* a) { return a->name == parts[i]; } );
            if(it != act->subs.end())
                act = *it;
            else
                act = createNode(parts[i], act);
        }

        return true;
    }

    template <typename T, typename L> void FakeFS<T,L>::deleteNamespace(const std::string& name)
    {
        _node* abr;
        _node* labr;
        if(path::absolute(name))
            abr = m_root;
        else
            abr = m_actual;

        /* Find the directory to delete */
        std::vector<std::string> parts = path::parts(name);
        for(size_t i = 0; i < parts.size(); ++i) {
            labr = abr;
            auto it = std::find_if(abr->subs.begin(), abr->subs.end(), [&] (_node* a) { return a->name == parts[i]; } );
            /* Do not delete if not existant */
            if(it == abr->subs.end())
                return;
            abr = *it;
        }

        /* Deleting the namespace */
        freeAbr(abr);
        auto it = std::find_if(labr->subs.begin(), labr->subs.end(), [&] (_node* a) { return a == abr; } );
        labr->subs.erase(it);
    }

    template <typename T, typename L> bool FakeFS<T,L>::existsNamespace(const std::string& name) const
    {
        _node* abr;
        if(path::absolute(name))
            abr = m_root;
        else
            abr = m_actual;

        std::vector<std::string> parts = path::parts(name);
        for(size_t i = 0; i < parts.size(); ++i) {
            auto it = std::find_if(abr->subs.begin(), abr->subs.end(), [&] (_node* a) {return a->name == parts[i]; } );
            if(it == abr->subs.end())
                return false;
            abr = *it;
        }

        return true;
    }

    template <typename T, typename L> bool FakeFS<T,L>::enterNamespace(const std::string& name)
    {
        _node* abr;
        if(path::absolute(name))
            abr = m_root;
        else
            abr = m_actual;

        std::vector<std::string> parts = path::parts(name);
        for(size_t i = 0; i < parts.size(); ++i) {
            auto it = std::find_if(abr->subs.begin(), abr->subs.end(), [&] (_node* a) {return a->name == parts[i]; } );
            if(it == abr->subs.end()) {
                logger::logm(std::string("Tried to enter an unexistant namespace : ") + name, logger::WARNING);
                return false;
            }
            abr = *it;
        }

        m_actual = abr;
        return true;
    }

    template <typename T, typename L> std::string FakeFS<T,L>::actualNamespace() const
    {
        std::string actualName = "/";
        _node* actual = m_actual;

        while(actual->parent) {
            actualName.insert(0, actual->name);
            actualName.insert(0, 1, '/');
            actual = actual->parent;
        }

        return actualName;
    }

    template <typename T, typename L> bool FakeFS<T,L>::createEntity(const std::string& name, T value)
    {
        /* If name already used */
        if(existsEntity(name))
            return false;

        /* Add an entity to the list */
        _entity* ent = new _entity;
        ent->count = 1;
        ent->value = value;

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

    template <typename T, typename L> bool FakeFS<T,L>::existsEntity(const std::string& name) const
    {
        return m_actual->entities.find(name) != m_actual->entities.end();
    }

    template <typename T, typename L> bool FakeFS<T,L>::link(const std::string& name, const std::string& target)
    {
        if(existsEntity(name))
            return false;

        /* Parse target path */
        _node* abr;
        if(path::absolute(target))
            abr = m_root;
        else
            abr = m_actual;

        std::vector<std::string> parts = path::parts(target);
        for(size_t i = 0; i < (parts.size() - 1); ++i) {
            auto it = std::find_if(abr->subs.begin(), abr->subs.end(), [&] (_node* a) {return a->name == parts[i]; } );
            if(it == abr->subs.end()) {
                logger::logm(std::string("Tried to link an entity to an unexistant entity : ") + target, logger::WARNING);
                return false;
            }
            abr = *it;
        }

        /* Find the target entity */
        _entity* t = NULL;
        if(abr->entities.find(parts.back()) == abr->entities.end()) {
            logger::logm(std::string("Tried to link an entity to an unexistant entity : ") + target, logger::WARNING);
            return false;
        }
        else
            t = abr->entities[parts.back()];

        /* Link */
        ++t->count;
        m_actual->entities[name] = t;
        return true;
    }

    template <typename T, typename L> bool FakeFS<T,L>::setEntityValue(const std::string& name, T value)
    {
        if(!existsEntity(name)) {
            logger::logm(std::string("Tried to set an unexistant entity : ") + name, logger::WARNING);
            return false;
        }
        m_actual->entities[name]->value = value;
        return true;
    }

    template <typename T, typename L> T FakeFS<T,L>::getEntityValue(const std::string& name) const
    {
        if(!existsEntity(name))
            return static_cast<T>(0);
        return m_actual->entities[name]->value;
    }

    template <typename T, typename L> void FakeFS<T,L>::freeEntity(_entity* e)
    {
        e->count--;
        if(e->count != 0)
            return;

        (*m_lb)(e->value);
        delete e;
    }

    template <typename T, typename L> void FakeFS<T,L>::freeAbr(_node* n)
    {
        if(!n->dir) {
            delete n;
            return;
        }

        for(_node* e : n->subs) {
            freeAbr(e);
        }
        for(auto pair : n->entities) {
            freeEntity(pair.second);
        }
        delete n;
    }

    template <typename T, typename L> std::vector<std::string> FakeFS<T,L>::listNamespaces() const
    {
        std::vector<std::string> ret(m_actual->subs.size());

        for(size_t i = 0; i < ret.size(); ++i)
            ret[i] = m_actual->subs[i]->name;
        return ret;
    }

    template <typename T, typename L> std::vector<std::string> FakeFS<T,L>::listEntities() const
    {
        std::vector<std::string> ret;

        for(typename std::unordered_map<std::string, _entity*>::const_iterator it = m_actual->entities.begin(); it != m_actual->entities.end(); ++it) {
            ret.push_back(it->first);
        }
        return ret;
    }

    template <typename T, typename L> template <typename Saver> bool FakeFS<T,L>::save(std::ostream& os, const Saver& sav, unsigned int tabs) const
    {
        save(os, tabs, m_root, sav);
        return true;
    }

    template <typename T, typename L> template <typename Saver> void FakeFS<T,L>::save(std::ostream& os, unsigned int tabs, const _node* const abr, const Saver& sav) const
    {
        std::string tbs(tabs, '\t');

        /* Store entities */
        for(auto pair : abr->entities)
            os << tbs << "\"" << pair.first << "\" : \"" << sav(pair.second->value) << "\"" << std::endl;

        /* Store sub-namespaces */
        for(_node* a : abr->subs) {
            os << tbs << "\"" << a->name << "\" : {" << std::endl;
            save(os, tabs + 1, a, sav);
            os << tbs << "}" << std::endl;
        }
    }

    template <typename T, typename L> template <typename Loader> bool FakeFS<T,L>::load(std::istream& is, const Loader& l)
    {
        clear();
        load(is, l, m_root);
        m_actual = m_root;
        return true;
    }

    template <typename T, typename L> template <typename Loader> void FakeFS<T,L>::load(std::istream& is, const Loader& l, _node* to)
    {
        std::string line;
        boost::regex ent ("^\\s*\"(.+?)\"\\s*:\\s*\"(.+?)\".*$"); /* Regex for an entity : ^[spaces]"(key)"[spaces]:[spaces]"(value)" */
        boost::regex nsp ("^\\s*\"(.+?)\"\\s*:\\s*{.*$");         /* Regex for namespace : ^[spaces]"(key)"[spaces]:[spaces]{ */
        boost::regex cmt ("^\\s*//.*$");                          /* Regex for commentary : ^[spaces]// */
        boost::regex end ("^\\s*\\}.*$");                         /* Regex for the end of a namespace : ^[spaces]} */
        boost::regex empty ("^\\s*$");                            /* Regex for an empty line : ^[spaces]$ */
        boost::smatch results;

        /* Save actual pos */
        _node* lact = m_actual;
        /* Allow use of other functions like addEntity */
        m_actual = to;
        size_t lineCount = 0;

        while(getline(is, line)) {
            ++lineCount;
            /* Is the line an entity */
            if(boost::regex_match(line, results, ent)) {
                createEntity(results[1], l(results[2]));
            }
            /* Is the line a new namespace */
            else if(boost::regex_match(line, results, nsp)) {
                _node* n = createNode(results[1], to);
                load(is, l, n);
            }
            /* Is the line the en of a namespace */
            else if(boost::regex_match(line, end)) {
                m_actual = lact;
                return;
            }
            /* Is the line invalid */
            else if(!boost::regex_match(line, cmt)
                    && !boost::regex_match(line, empty)) {
                std::ostringstream oss;
                oss << "Invalid line #" << lineCount << " \"" << line << "\" while loading a FakeFS.";
                logger::logm(oss.str(), logger::WARNING);
            }
        }

        /* Restore m_actual to its last value */
        m_actual = lact;
    }

    template <typename T, typename L> typename FakeFS<T,L>::_node* FakeFS<T,L>::createNode(const std::string& name, _node* parent)
    {
        if(parent) {
            auto it = std::find_if(parent->subs.begin(), parent->subs.end(), [&] (_node* a) { return a->name == name; } );
            if(it != parent->subs.end())
                return NULL;
        }

        _node* ret  = new _node;
        ret->name   = name;
        ret->dir    = true;
        ret->parent = parent;
        if(parent)
            parent->subs.push_back(ret);
        return ret;
    }
}

