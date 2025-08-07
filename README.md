# ExifReader
exifreader is a minimal C++ program that reads a JPEG image and extracts the raw EXIF segment (APP1) if it exists.

Features  
- Detects if a JPEG file contains EXIF data.
- Locates and extracts the APP1 EXIF segment.
- Does not depend on external libraries.
- Prints the EXIF segment size.

  compilation:
  g++ -std=c++17 -o exif_reader exif_reader.cpp

  usage:
  ./exif_reader pathToImage.jpg
