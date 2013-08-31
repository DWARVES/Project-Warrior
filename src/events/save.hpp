
#ifndef DEF_EVENTS_SAVE
#define DEF_EVENTS_SAVE

#include <string>
#include "core/fakefs.hpp"

namespace events
{
    /* Save is a singlebone : all the methods modifies an only global instance */
    class Save
    {
        public:
            typedef long int number_t;

            Save();
            Save(const Save&) = delete;
            ~Save();

            /* Init the ressources */
            static bool init();
            /* free all the ressource */
            static void quit();

            /* Manage namespaces (see core::FakeFS for more details)
             * Always return false if not initialized
             */
            bool createNamespace(const std::string& name);
            bool deleteNamespace(const std::string& name);
            bool enterNamespace(const std::string& name);
            bool existsNamespace(const std::string& name);

            /* Create a variable and set it to value
             * Does not works if name is already used
             */
            bool createVariable(const std::string& name, number_t value = 0);
            /* Delete a variable */
            void deleteVariable(const std::string& name);
            /* Set the name variable to nvalue
             * Return false if name does not exists
             */
            bool setVariable(const std::string& name, number_t nvalue);
            /* Return the value of the variable named name
             * Return 0 if name does not exists
             */
            number_t getVariable(const std::string& name);

            /* Save the variables to the at the name path
             * If the file already exists, it's erased
             * Return false if an error happened
             * Do not modifies the state
             */
            bool save(const std::string& name) const;
            /* Load the variables from a file
             * If the file can't be read, return false and the actual state is not modified
             * It first clears the complete state, deleting all the variables and namespaces before charging
             */
            bool load(const std::string& name);

        private:
            static core::FakeFS<Save::number_t>* m_fs;
    };
}

#endif

