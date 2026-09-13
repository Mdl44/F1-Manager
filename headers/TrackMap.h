#ifndef TRACKMAP_H
#define TRACKMAP_H
#include <string>
#include <vector>

struct TrackPoint {
    int x;
    int y;
};

// Loads a circuit's real track-outline points from tracks/*.txt.
class TrackMap {
    std::vector<TrackPoint> points;
public:
    explicit TrackMap(const std::string& circuit_name);
    [[nodiscard]] const std::vector<TrackPoint>& get_points() const;
};

#endif
