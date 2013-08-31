
#ifndef DEF_CORE_FAKEFS
#define DEF_CORE_FAKEFS

#include <string>
#include <vector>
#include <unordered_map>
#include <istream>
#include <ostream>

namespace core
{
    /* Useless liberator : just do nothing, for default */
    template <typename T> class UselessLiberator
    {
        public:
            void operator()(T value) const 
            {
                if(value) /* Avoid warnings */
                    return;
            }
    };

    /* Default pointer liberator, T must be a pointer */
    template <typename T> class PointerLiberator
    {
        public:
            void operator()(T value) const
            {
                if(value) delete value;
            }
    };

    /* T is the type of data stored
     * Liberator is a class which operator() free a T pointer
     * operator() must be const
     */
    template <typename T, typename Liberator = UselessLiberator<T>> class FakeFS
    {
        public:
            /* If todel is true, lb will be free'd with delete */
            FakeFS(const Liberator* lb = NULL, bool todel = false);
            FakeFS(const FakeFS&) = delete;
            ~FakeFS();

            /* This functions will discard all the content and put the instance as it was just created */
            void clear();

            /*******************************
             *   Namespace manipulation    *
             *******************************/

            /* Create a new namespace, return false if could not create
             * Can be a path : all the new path will be created
             */
            bool createNamespace(const std::string& name);
            void deleteNamespace(const std::string& name);
            bool existsNamespace(const std::string& name) const;

            /* Enter a namespace : everything done will be in this namespace */
            bool enterNamespace(const std::string& name);
            /* Return actual namespace */
            std::string actualNamespace() const;
            /* Lists namespaces in the current one */
            std::vector<std::string> listNamespaces() const;

            /*******************************
             *    Entities manipulation    *
             *******************************/

            /* Name can only be a simple name, not a path */
            /* Return false if name already exists */
            bool createEntity(const std::string& name, T value);
            void deleteEntity(const std::string& name);
            bool existsEntity(const std::string& name);
            /* Create a link named name to target
             * target can be a whole path and not only a simple name
             * return false if name already exists or of target dosn't exists
             */
            bool link(const std::string& name, const std::string& target);

            /* Return false if name does not exists
             * Last value is NOT free'd
             */
            bool setEntityValue(const std::string& name, T value);
            /* Return 0 if name does not exists, but can also return 0 if name value is 0
             * Use existsEntity method to be sure
             */
            T getEntityValue(const std::string& name);
            /* Lists entities in current namespace */
            std::vector<std::string> listEntities() const;

            /*******************************
             *    Input/Output streams     *
             *******************************/

            /* Save the state of the FakeFS to an ostream
             * Return false if something went wrong
             * The output has a human-readable json-inspired syntax
             * tabs is the number of 't' added at the beggining of each line
             * Saver is a class with an operator() converting T to std::string
             */
            template <typename Saver> bool save(std::ostream& os, const Saver& sav, unsigned int tabs = 0) const;
            /* Load a FakeFS struct from an istream
             * Return false if something went wrong
             * The previous content of the FakeFS instance will be discarded
             * The input needs to be formatted as save format its output
             * Loader is a class with an operator() converting std::string to T
             */
            template <typename Loader> bool load(const std::istream& path, const Loader& l);

        private:
            const Liberator* m_lb;
            bool m_todel;

            /* Entities */
            struct _entity {
                _entity *prev, *next;
                unsigned int count;
                T value;
            };
            _entity* m_first;
            void freeEntity(_entity* e);

            /* Abr */
            struct _abr {
                std::string name;
                _abr* parent;
                std::vector<_abr*> subs;
                std::unordered_map<std::string, _entity*> entities;
            };
            _abr* m_root;
            _abr* m_actual;
            void freeAbr(_abr* a);

            /* I/O */
            template <typename Saver> void save(std::ostream& os, unsigned int tabs, const _abr* const abr, const Saver& sav) const;
    };
}

/* Definitions */
#include "core/fakefs-def.hpp"

#endif

