#include <imageio.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

namespace utils {

cv::Mat read_image(std::istream & input, bool untie_input)
{
    if (untie_input)
        input.tie(nullptr);
    input >> std::noskipws;
    std::vector<char> buf;
    size_t newline_count = 0;
    size_t prefix_size = 0;
    bool reading_prefix = true;
    size_t bit_per_pixel = 8;
    size_t height;
    size_t width;
    size_t second_newline_idx;

    char t;
    while ((reading_prefix || (buf.size() - prefix_size) * 8 < height * width * bit_per_pixel) && input >> t) {
        if (reading_prefix)
            prefix_size++;
        if (t == '\n') {
            newline_count++;
            if (newline_count == 2) {
                second_newline_idx = buf.size();
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
                else {
                    std::stringstream ss(std::string(buf.begin() + second_newline_idx, buf.end()));
                    size_t t;
                    ss >> t;
                    bit_per_pixel = std::ceil(std::log2(t));
                }
                reading_prefix = false;
            }
        }
        buf.push_back(t);
    }

    if (buf.size() == 0) {
        return cv::Mat();
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
#ifdef _WIN32
    setmode(fileno(stdout), O_BINARY);
#endif
    std::vector<unsigned char> output_buffer;
    std::string output_format;
    if (binary) {
        output_format = ".pbm";
    }
    else {
        switch (image.channels()) {
        case 1:
            output_format = ".pgm";
            break;

        case 3:
            output_format = ".ppm";
            break;

        default:
            throw std::domain_error(std::to_string(image.channels()) + " channels is not supported by netpbm");
        }
    }
    if (image.depth() > 3) {
        std::cout << "Warning: provided image's depth is 32 or 64 bit, which is not supported by netpbm. Depth will be reduced.";
    }
    cv::imencode(output_format, image, output_buffer);
    output.write(reinterpret_cast<const char *>(output_buffer.data()), output_buffer.size());
}

} // namespace utils
