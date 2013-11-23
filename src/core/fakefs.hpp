
#ifndef DEF_CORE_FAKEFS
#define DEF_CORE_FAKEFS

#include <string>
#include <vector>
#include <unordered_map>
#include <istream>
#include <ostream>

/** @brief The namespace for core methods.
 *
 * Include system related methods, containers and some I/O methods.
 */
namespace core
{
    /** @brief Default liberator for FakeFS : do nothing (for non pointer values). */
    template <typename T> class UselessLiberator
    {
        public:
            T operator()(T value) const 
            {
                return value; /* Avoid warnings */
            }
    };

    /** @brief Pointer liberator for FakeFS : T must have an overloaded delete operator. */
    template <typename T> class PointerLiberator
    {
        public:
            void operator()(T value) const
            {
                if(value) delete value;
            }
    };

    /** @brief An abr storage class, with linking capacities.
     * T is the type of data stored.
     * Liberator is a class which operator() (must be const) free a T pointer.
     */
    template <typename T, typename Liberator = UselessLiberator<T>> class FakeFS
    {
        public:
            /** @brief Only one constructor.
             * @param lb The liberator utility, if NULL a new one will be allocated by the FakeFS.
             * @param todel If true, lb will be freed on destruction of the FakeFS.
             * */
            FakeFS(const Liberator* lb = NULL, bool todel = false);
            FakeFS(const FakeFS&) = delete;
            ~FakeFS();

            /** @brief This functions will discard (and free) all the content and put the instance as it was just created */
            void clear();

            /*******************************
             *   Namespace manipulation    *
             *******************************/

            /** @brief Create a new namespace, return false if could not create
             * @param name A path to the new namespace. Will create the whole path if necessary.
             * @return True if the whole path could be created.
             */
            bool createNamespace(const std::string& name);
            /** @brief Delete a namespace
             * @param name The path to the namespace to be destroyed (will only destroy the last namespace of the path, for example, in gui/img/tmp, it will destroy only tmp).
             */
            void deleteNamespace(const std::string& name);
            /** @brief Check the existance of a namespace */
            bool existsNamespace(const std::string& name) const;

            /** @brief Enter a namespace : from now on, all the relatifs paths will be relatives to this one.
             * @return True if could enter in the namespace.
             */
            bool enterNamespace(const std::string& name);
            /** @brief Return the actual namespace */
            std::string actualNamespace() const;
            /** @brief Lists all namespaces in the current one */
            std::vector<std::string> listNamespaces() const;

            /*******************************
             *    Entities manipulation    *
             *******************************/

            /** @brief Create a new entity.
             * An entity is a value stored, associated to a name, and situated in a namespace.
             * On creation, the entity will be on the actual namespace.
             * @param name The name of the entity. Can't be a path, must only be the plain name.
             * @param value The first value of the entity.
             * @return True if the entity could be created.
             */
            bool createEntity(const std::string& name, T value);
            /** @brief Delete an entity.
             * @param name The name of the entity. Can't be a path, must only be the plain name.
             */
            void deleteEntity(const std::string& name);
            /** @brief Check the existence of an entity.
             * @param name The name of the entity. Can't be a path, must only be the plain name.
             */
            bool existsEntity(const std::string& name) const;
            /** @brief Create a link to an entity.
             * The link will appear as a mere entity, but changing its value will also change the value of the entity linked, and there won't be duplication of data.
             * @param name The name of the link, which can only be a plain name.
             * @param target The path to the entity to link to.
             * @return False if name already exists or of target dosn't exists
             */
            bool link(const std::string& name, const std::string& target);

            /** @brief Change an entity value.
             * Be careful : the previous value is NOT free'd
             * @param name The name of the entity. Can't be a path, must only be the plain name.
             * @param value the new value of the entity.
             * @return False if name does not exists.
             */
            bool setEntityValue(const std::string& name, T value);
            /** @brief Access an entity value.
             * @param name The name of the entity. Can't be a path, must only be the plain name.
             * @return 0 if name does not exists, but can also return 0 if name value is 0. you should use existsEntity method to be sure which case it is.
             */
            T getEntityValue(const std::string& name) const;
            /** @brief Lists entities in current namespace */
            std::vector<std::string> listEntities() const;

            /*******************************
             *    Input/Output streams     *
             *******************************/

            /** @brief Save the state of the FakeFS to an ostream
             * The output has a human-readable json-inspired syntax.
             * @param os The std::ostream to save to. Must be ready for writing.
             * @param tabs It is the number of tabulations added at the beggining of each lines.
             * @param sav It is a class with an operator() converting T to std::string.
             * @return False if something went wrong and couldn't save.
             */
            template <typename Saver> bool save(std::ostream& os, const Saver& sav, unsigned int tabs = 0) const;
            /** @brief Load a FakeFS struct from an istream.
             * The previous content of the FakeFS instance will be discarded and free'd.
             * The input needs to be formatted with a particular json-inspired syntax, the same save use to write the contents of the class.
             * @param is The std::istream to load from. Must be ready for reading.
             * @param l It is a class with an operator() converting std::string to T.
             * @return False if something went wrong and couldn't load. If that's the case, the previous content could either have been discarded, or still be valid.
             */
            template <typename Loader> bool load(std::istream& is, const Loader& l);

        private:
            const Liberator* m_lb; /**< @brief The liberator used to free the data. */
            bool m_todel;          /**< @brief Indicate if the liberator must be free'd on destruction. */

            /* Entities */
            /** @brief The internal structure used to represent an entity. */
            struct _entity {
                _entity *prev,      /**< @brief Previous entity in the double linked list of all entities. */
                        *next;      /**< @brief Next entity in the double linked list of all entities. */
                unsigned int count; /**< @brief Count of references to this entity (agment with links). */
                T value;            /**< @brief The value stored. */
            };
            _entity* m_first; /**< @brief The first entity of the double linked list of all entities. */
            /** @brief Will free the data of the entity if necessary, and update e->count. */
            void freeEntity(_entity* e);

            /* Abr */
            /** @brief The internal structure used to represent a namespace. */
            struct _abr {
                std::string name;                                   /**< @brief The name of the namespace. */
                _abr* parent;                                       /**< @brief The parent namespace, NULL if this is the root. */
                std::vector<_abr*> subs;                            /**< @brief A vector of all namespaces in this namespace. */
                std::unordered_map<std::string, _entity*> entities; /**< @brief A map of all entities associated to their names in this namespace. */
            };
            _abr* m_root;   /**< @brief A pointer to the root namespace, in which all nemaspace are contained. */
            _abr* m_actual; /**< @brief A pointer to the actual namespace. */
            /** @brief Free a namespace.
             * Will recursively free all namespaces and entities in a.
             */
            void freeAbr(_abr* a);

            /* I/O */
            /** \brief Recursively writes a namespace to an ostream.
             * @param os The std::ostream to save to. Must be ready for writing.
             * @param tabs It is the number of tabulations added at the beggining of each lines.
             * @param abr The namespace to save.
             * @param sav It is a class with an operator() converting T to std::string.
             */
            template <typename Saver> void save(std::ostream& os, unsigned int tabs, const _abr* const abr, const Saver& sav) const;
            /** @brief Load a namespace struct from an istream.
             * @param is The std::istream to load from. Must be ready for reading.
             * @param l It is a class with an operator() converting std::string to T.
             * @param to The namespace to load the data to. If it already has data, it may cause an undefined behaviour.
             */
            template <typename Loader> void load(std::istream& is, const Loader& l, _abr* to);
    };
}

/* Definitions */
#include "core/fakefs-def.hpp"

#endif

