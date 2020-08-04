#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

namespace utils {

/**
 * Reads an image from a stream
 * 
 * @param input Stream from which image is read
 * @return Image from `input` 
 */
cv::Mat read_image(std::istream & input = std::cin);

/**
 * Reads images from a stream until no characters are left or the stream fails
 * 
 * @param input Stream from which image is read
 * @return Vector of images from `input` 
 */
std::vector<cv::Mat> read_images();

/**
 * Write an image to a stream in pbm, pgm or pbm. 
 * If `binary` is true, image is written in pbm, 
 * otherwise format is decided based on `image.channels()`
 * 
 * @param image Image to write
 * @param input Stream to which image is written
 * @param binary Whether to write image as a binary one (pbm)
 */
void write_image(cv::Mat image, std::ostream & output = std::cout, bool binary = false);

} // namespace utils
