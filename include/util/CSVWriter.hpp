#ifndef CSVWRITER_HPP
#define CSVWRITER_HPP

#include <string>
#include <vector>
#include <fstream>
using namespace std;

namespace util {

    class CSVWriter {
    public:
        CSVWriter() = default;
        explicit CSVWriter(const std::string& path) { open(path); }
        ~CSVWriter() { close(); }

        void open(const std::string& path);

        void write_header(const std::vector<std::string>& header);

        void write_row(const std::vector<std::string>& values);

        void close();

        bool is_open() const { 
            return ofs_.is_open(); 
        }

    private:
        std::ofstream ofs_;
        std::string path_;
        static std::string escape_cell(const std::string& cell);
    };

} 

#endif 