#ifndef CSVWRITER_HPP
#define CSVWRITER_HPP

#include <string>
#include <vector>
#include <fstream>

namespace util {

/*
 * Simple CSV writer utility.
 * - open(file_path) opens the file for writing (overwrites existing file).
 * - write_header(...) writes a header row.
 * - write_row(...) writes a single CSV row (vector of strings).
 * - close() closes the file.
 *
 * This class flushes after every row to make output visible immediately.
 */

class CSVWriter {
public:
    CSVWriter() = default;
    explicit CSVWriter(const std::string& path) { open(path); }
    ~CSVWriter() { close(); }

    // Open file for writing (overwrites). Creates parent directories if needed.
    void open(const std::string& path);

    // Write header row (calls write_row internally)
    void write_header(const std::vector<std::string>& header);

    // Write a data row (vector<string>)
    void write_row(const std::vector<std::string>& values);

    // Close file (safe to call multiple times)
    void close();

    // Convenience: check if file is open
    bool is_open() const { return ofs_.is_open(); }

private:
    std::ofstream ofs_;
    std::string path_;
    // Escape/quote CSV cell if it contains comma or quote or newline.
    static std::string escape_cell(const std::string& cell);
};

} // namespace util

#endif // CSVWRITER_HPP