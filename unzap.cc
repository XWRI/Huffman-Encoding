#include "huffman.h"

int main(int argc, char* argv[]) {
    //  Checks if the number of input arguments is correct
    if (argc < 3) {
      std::cerr << "Usage: ./zap <inputfile> <zapfile>" << std::endl;
      exit(1);
    }
    Huffman hm;

    std::ifstream inputfile(argv[1]);
    //  Checks if the correct file was given in the command line
    if (!inputfile.is_open()) {
      std::cerr << "Error: cannot open input file " << argv[1]
                << "." << std::endl;
      exit(1);
    }

    std::ofstream outputfile(argv[2],
                             std::ofstream::binary | std::ofstream::trunc);

    hm.Decompress(inputfile, outputfile);

    std::cout << "Decompressed zap file " << argv[1] << " into output file "
              << argv[2] << std::endl;

    return 0;
}
