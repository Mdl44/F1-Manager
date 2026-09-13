#include "TrackMap.h"
#include <fstream>
#include <cctype>
#include "Exceptions.h"

namespace {
    // Converts a circuit name to its tracks/ filename (lowercase, underscores).
    std::string slugify(const std::string& name) {
        std::string slug;
        bool last_was_underscore = false;
        for (const char c : name) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                slug += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                last_was_underscore = false;
            } else if (!last_was_underscore && !slug.empty()) {
                slug += '_';
                last_was_underscore = true;
            }
        }
        while (!slug.empty() && slug.back() == '_') {
            slug.pop_back();
        }
        return slug;
    }
}

TrackMap::TrackMap(const std::string& circuit_name) {
    const std::string path = "tracks/" + slugify(circuit_name) + ".txt";
    std::ifstream file(path);
    if (!file) {
        throw ConfigurationFileException(path);
    }

    int x, y;
    while (file >> x >> y) {
        points.push_back({x, y});
    }

    if (points.empty()) {
        throw ConfigurationFileException(path + ": no track points found");
    }
}

const std::vector<TrackPoint>& TrackMap::get_points() const {
    return points;
}
