#include <imageio.h>

namespace utils {

cv::Mat read_image(std::istream & input)
{
    input >> std::noskipws;
    std::vector<char> buf;
    size_t newline_count = 0;
    size_t prefix_size = 0;
    bool reading_prefix = true;
    size_t bit_per_pixel = 8;
    size_t height;
    size_t width;

    while (!input.eof() && (reading_prefix || (buf.size() - prefix_size) * 8 < height * width * bit_per_pixel)) {
        char t;
        input >> t;
        if (reading_prefix)
            prefix_size++;
        if (t == '\n') {
            newline_count++;
            if (newline_count == 2) {
                std::stringstream ss(std::string(buf.begin() + 3, buf.end()));
                ss >> height >> width;
                if (buf[1] == '4') {
                    bit_per_pixel = 1;
                    reading_prefix = false;
                }
            }
            if (newline_count == 3 && buf[1] != '4') {
                if (buf[1] == '6') {
                    bit_per_pixel = 24;
                }
                reading_prefix = false;
            }
        }
        buf.push_back(t);
    }

    return cv::imdecode(buf, cv::IMREAD_UNCHANGED);
}

std::vector<cv::Mat> read_images()
{
    std::vector<cv::Mat> result;
    while (!std::cin.eof()) {
        cv::Mat image = read_image();
        if (!image.data) {
            break;
        }
        result.push_back(image);
    }
    return result;
}

void write_image(cv::Mat image, std::ostream & output, bool binary)
{
    std::vector<unsigned char> output_buffer;
    std::string output_format;
    if (binary) {
        output_format = "pbm";
    }
    else {
        switch (image.channels()) {
        case 1:
            output_format = "pgm";
            break;

        case 3:
            output_format = "ppm";
            break;

        default:
            throw std::domain_error(std::to_string(image.channels()) + " channels is not supported by netpbm");
        }
    }
    cv::imencode(output_format, image, output_buffer);
    for (unsigned char c : output_buffer) {
        output << c;
    }
}

} // namespace utils
