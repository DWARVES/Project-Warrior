
#ifndef DEF_GAMEPLAY_CHARACTER
#define DEF_GAMEPLAY_CHARACTER

#include <string>
#include "geometry/aabb.hpp"
#include "lua/script.hpp"

namespace gameplay
{
    /** @brief Reprensents a playable character. */
    class Character
    {
        public:
            /** @brief Initialize the character. Doesn't load anything.
             * @param path The path to the directory of this caracter.
             */
            Character(const std::string& path);
            Character() = delete;
            Character(const Character&) = delete;
            ~Character();

            /** @brief The colors possibles for a character. */
            enum Color : int {
                Red   = 0, /**< @brief The red theme of the character. */
                Blue  = 1, /**< @brief The blue theme of the character. */
                Green = 2, /**< @brief The green theme of the character. */
                White = 3, /**< @brief The white theme of the character. */
                Black = 4, /**< @brief The black theme of the character. */
                None  = 5, /**< @brief None means the default color of the character. */
            };

            /** @brief Load the preview, the name and the description. */
            bool preload();
            /** @brief Check if the character has been validated. */
            bool validated() const;
            /** @brief Returns the name of the character. */
            std::string name() const;
            /** @brief Returns the description of the character. */
            std::string desc() const;
            /** @brief Print the preview picture.
             * @param msize The max size of the picture.
             */
            void preview(const geometry::AABB& msize) const;
            /** @brief Print the big preview picture.
             * @param color The color of the character.
             * @param msize the max size of the picture.
             */
            void bigPreview(Color color, const geometry::AABB& msize);

        private:
            std::string m_namespace; /**< @brief The name of the namespace used by this character in gfx and audio. */
            std::string m_path;      /**< @brief The path to the directory of the character. */
            static size_t m_count;   /**< @brief The number of created characters. */

            lua::Script m_preview;   /**< @brief The preview.lua script. */
            std::string m_name;      /**< @brief The name of the character. */
            std::string m_desc;      /**< @brief The description of the character. */
            bool m_valid;            /**< @brief Has the character been validated. */

            /* Internal methods. */
            /** @brief Draw the "preview" texture with a maximum size. */
            void drawPrev(const geometry::AABB& msize) const;
    };
}

#endif

