
#ifndef DEF_GAMEPLAY_SAVE
#define DEF_GAMEPLAY_SAVE

#include <string>
#include "core/fakefs.hpp"

/** @brief Contains all elements directly used in game, as personnages or stages. */
namespace gameplay
{
    /** @brief A class used to save variables events, which are numbers.
     *
     * Save is a singlebone : all the instances modifies an only one global instance
     * It must be inited before using any instance.
     */
    class Save
    {
        public:
            /** @brief The type of variables stored. */
            typedef long int number_t;

            Save();
            Save(const Save&) = delete;
            ~Save();

            /** @brief Init the global instance, must be called only once. */
            static bool init();
            /** @brief Free the global instance. */
            static void quit();

            /** @brief Create a namespace, works as core::FakeFS::createNamespace. */
            bool createNamespace(const std::string& name);
            /** @brief Delete an existing namespace, works as core::FakeFS::deleteNamespace. */
            bool deleteNamespace(const std::string& name);
            /** @brief Set a new actual namespace, works as core::FakeFS::enterNamespace. */
            bool enterNamespace(const std::string& name);
            /** @brief Check the existence of a namespace. */
            bool existsNamespace(const std::string& name);
            /** @brief Get the absolute path of the actual namespace. */
            std::string actualNamespace();

            /** @brief Create a new variable, works as core::FakeFS<number_t>::createEntity. */
            bool createVariable(const std::string& name, number_t value = 0);
            /** @brief Delete a variable, works as core::FakeFS::deleteEntity. */
            void deleteVariable(const std::string& name);
            /** @brief Change the value of the variable.
             * @param name The plain name of the variable, path aren't accepted (as in core::FakeFS::setEntityValue).
             */
            bool setVariable(const std::string& name, number_t nvalue);
            /** @brief Check the existence of a variable.
             * @param name The plain name of the variable, path aren't allowed (as in core::FakeFS::existsEntity).
             */
            bool existsVariable(const std::string& name);
            /** @brief Get the value of the variable.
             * @param name The plain name of the variable, path aren't allowed.
             * @return Follow the same policy as core::FakeFS<number_t>::getEntityValue, will return 0 when the variable doesn't exists, but also when it's its real value.
             */
            number_t getVariable(const std::string& name);

            /** @brief Save the variables to a file.
             * @param path The path to the file to save to. Will erase the file if it already exists.
             * @return False if couldn't save. If path existed, it ay have already been erased.
             */
            bool save(const std::string& path) const;
            /** @brief Load the variables from a file.
             *
             * It will first open the file, then discard all the content in memory, and then load from the opened file.
             * So if the file can't be opened, the actual content won't be lost.
             * But if there is an error while reading, you may ends up with an empty Save class.
             * This function never changes the file content.
             * @param path The path to the file to load.
             * @return False if couldn't load the file. If that's the case, you may either have an empty Save class or an unchanged class, depending on where the error happened.
             * @todo Improve error report, enventually use exceptions.
             */
            bool load(const std::string& path);

        private:
            /** @brief The global structure storing the variable and their values. */
            static core::FakeFS<Save::number_t>* m_fs;
    };
}

#endif

