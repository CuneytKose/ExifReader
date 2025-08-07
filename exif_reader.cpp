#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool find_exif_segment(const std::string& filename, std::vector<unsigned char>& exif_data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    unsigned char marker[2];
    file.read(reinterpret_cast<char*>(marker), 2);
    if (marker[0] != 0xFF || marker[1] != 0xD8) {
        std::cerr << "Not a JPEG file.\n";
        return false;
    }

    while (file.read(reinterpret_cast<char*>(marker), 2)) {
        if (marker[0] != 0xFF) break;

        unsigned short size;
        file.read(reinterpret_cast<char*>(&size), 2);
        size = ((size >> 8) & 0xFF) | ((size & 0xFF) << 8); 
        if (marker[1] == 0xE1) { // APP1 segment (EXIF)
            std::vector<unsigned char> buffer(size - 2);
            file.read(reinterpret_cast<char*>(buffer.data()), size - 2);
            if (std::string(buffer.begin(), buffer.begin() + 6) == "Exif\0\0") {
                exif_data = buffer;
                return true;
            }
        } else {
            file.seekg(size - 2, std::ios::cur);
        }
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image.jpg>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::vector<unsigned char> exif_data;

    if (find_exif_segment(filename, exif_data)) {
        std::cout << "Found EXIF segment with " << exif_data.size() << " bytes.\n";
    } else {
        std::cout << "No EXIF data found.\n";
    }

    return 0;
}
