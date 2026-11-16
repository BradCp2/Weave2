/**
 * @file
 * @brief
 */

#ifndef WEAVE2_REPOSITORY_H
#define WEAVE2_REPOSITORY_H

#include <filesystem>

namespace weave::core
{
    class Repository
    {
        public:
        /**
         * @brief default constructor
         */
        Repository() = default;

        // Does specified path exist in repo?
        /**
         * @todo
         * @param path
         * @return
         */
        bool existsAt(const std::filesystem::path& path) const;
    };
} // core

#endif //WEAVE2_REPOSITORY_H
