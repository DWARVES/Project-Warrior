
#ifndef DEF_CORE_FAKEFS
#define DEF_CORE_FAKEFS

#include <string>
#include <vector>
#include <unordered_map>

namespace core
{
    /* T is the type of data stored
     * Liberator is a class which operator() free a T pointer
     * operator() must be const
     */
    template <typename T, typename Liberator> class FakeFS
    {
        public:
            /* If todel is true, lb will be free'd with delete */
            FakeFS(const Liberator* lb, bool todel = false);
            ~FakeFS();

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

            /*******************************
             *    Entities manipulation    *
             *******************************/

            /* Name can only be a simple name, not a path */
            /* Return false if name already exists */
            bool createEntity(const std::string& name, T* value);
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
            bool setEntityValue(const std::string& name, T* value);
            /* Return NULL if name does not exists, but can also return NULL if name value is NULL
             * Use existsEntity method to be sure
             */
            T* getEntityValue(const std::string& name);


        private:
            const Liberator* m_lb;
            bool m_todel;

            /* Entities */
            struct _entity {
                _entity *prev, *next;
                unsigned int count;
                T* value;
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

            /* To prevent usage */
            FakeFS();
            FakeFS(const FakeFS&);
            FakeFS& operator=(const FakeFS&);
    };
}

/* Definitions */
#include "core/fakefs-def.hpp"

#endif

