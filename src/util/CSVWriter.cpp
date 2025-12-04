#include "util/CSVWriter.hpp"
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

namespace util {

void CSVWriter::open(const string& path) {
    // Close an existing file if previously open
    if (ofs_.is_open()) ofs_.close();

    // Ensure parent directory exists
    try {
        fs::path p(path);
        if (p.has_parent_path()) {
            fs::create_directories(p.parent_path());
        }
    } catch (...) {
        // ignore directory creation errors here; the ofstream open will fail if necessary
    }

    // Open file for truncation (overwrite)
    ofs_.open(path, std::ios::out | std::ios::trunc);
    path_ = path;
}

string CSVWriter::escape_cell(const string& cell) {
    bool need_quote = false;
    for (char c : cell) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') { need_quote = true; break; }
    }
    if (!need_quote) return cell;

    // Escape double quotes by doubling them
    string out;
    out.reserve(cell.size() + 4);
    out.push_back('"');
    for (char c : cell) {
        if (c == '"') out.append("\"\"");
        else out.push_back(c);
    }
    out.push_back('"');
    return out;
}

void CSVWriter::write_row(const vector<string>& values) {
    if (!ofs_.is_open()) return; // silently return if file not open; you could throw instead
    // Build the line
    ostringstream ss;
    for (size_t i = 0; i < values.size(); ++i) {
        ss << escape_cell(values[i]);
        if (i + 1 < values.size()) ss << ',';
    }
    ss << '\n';
    ofs_ << ss.str();
    ofs_.flush(); // ensure disk visibility immediately
}

void CSVWriter::write_header(const vector<string>& header) {
    write_row(header);
}

void CSVWriter::close() {
    if (ofs_.is_open()) {
        ofs_.flush();
        ofs_.close();
    }
}

} // namespace util