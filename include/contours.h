#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

/**
 * Reads a vector of contours from a stream
 * 
 * @param input Stream from which image is read
 * @tparam coord_type Coordinate type of `Point`s in contours
 * @return Vector of contours from `input` 
 */
template <typename coord_type>
std::vector<std::vector<cv::Point_<coord_type>>> readContours(std::istream & input = std::cin)
{
    std::vector<std::vector<cv::Point_<coord_type>>> contours;
    for (std::string line; std::getline(input, line);) {
        std::istringstream line_stream(line);
        contours.push_back({});
        for (auto it = std::istream_iterator<coord_type>(line_stream); it != std::istream_iterator<coord_type>();) {
            coord_type x = *(it++);
            coord_type y = *(it++);
            contours.back().push_back({x, y});
        }
    }
    return contours;
}
