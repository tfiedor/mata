// example5.cc - parsing a NFA from file

#include <mata/nfa.hh>
#include <mata/inter-aut.hh>
#include <iostream>
#include <fstream>

using namespace Mata::Nfa;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Input file missing\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];

    std::fstream fs(filename, std::ios::in);
    if (!fs) {
        std::cerr << "Could not open file \'" << filename << "'\n";
        return EXIT_FAILURE;
    }

    Mata::Parser::Parsed parsed;
    Nfa aut;
    StringToSymbolMap stsm;
    try {
        parsed = Mata::Parser::parse_mf(fs, true);
        fs.close();

        if (parsed.size() != 1) {
            throw std::runtime_error(
                    "The number of sections in the input file is not 1\n");
        }
        if (parsed[0].type.find("NFA") == std::string::npos) {
            throw std::runtime_error("The type of input automaton is not NFA\n");
        }

        std::vector<Mata::IntermediateAut> inter_aut = Mata::IntermediateAut::parse_from_mf(parsed);

        if (inter_aut[0].is_nfa())
            construct(&aut, inter_aut[0]);
    }

    catch (const std::exception& ex) {
        fs.close();
        std::cerr << "libMATA error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }


    aut.print_to_DOT(std::cout);
    return EXIT_SUCCESS;
}