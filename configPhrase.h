#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cassert>
#include <map>
#include <list>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



struct Level
{
    Level() : parent(NULL), depth(0) {}
    Level(Level* p) : parent(p), depth(0) {}

    using value_map_t = std::map<std::string, std::string>;
    using section_map_t = std::map<std::string, Level>;
    using values_t = std::list<value_map_t::const_iterator>;
    using sections_t = std::list<section_map_t::const_iterator>;
    value_map_t values;
    section_map_t sections;
    values_t ordered_values;
    sections_t ordered_sections;
    Level* parent;
    size_t depth;

    const std::string& operator[](const std::string& name) { return values[name]; }
    Level& operator()(const std::string& name) { return sections[name]; }
};

class ConfigParser
{
public:
    ConfigParser(const char* fn);
    ConfigParser(std::istream& f) : sstream(&f), ln_(0) { parse(top_); }
    ConfigParser();
    Level& top() { return top_; }
    void dump(std::ostream& s) { dump(s, top(), ""); }
    //½ûÖ¹¿½±´¹¹Ôì
    ConfigParser(const ConfigParser &) = delete;  
    ConfigParser &operator=(const ConfigParser &) = delete;
private:
    void dump(std::ostream& s, const Level& l, const std::string& sname);
    void parse(Level& l);
    void parseSLine(std::string& sname, size_t& depth);
    void errMsg(const char* s);

private:
    Level top_;
    std::ifstream fstream;
    std::istream* sstream;
    std::string line_;
    size_t ln_;
};

inline void ConfigParser::errMsg(const char* s)
{
    std::ostringstream os;
    os << s << " on line #" << ln_;
    throw std::runtime_error(os.str());
}

inline std::string trim(const std::string& s)
{
    char p[] = " \t\r\n";
    long sp = 0;
    long ep = s.length() - 1;
    for (; sp <= ep; ++sp)
        if (!strchr(p, s[sp])) break;
    for (; ep >= 0; --ep)
        if (!strchr(p, s[ep])) break;
    return s.substr(sp, ep - sp + 1);
}

inline std::string value(const std::string& s)
{
    std::string c = "#;";
    std::string v = s;
    size_t ep = std::string::npos;
    for (size_t i = 0; i < c.size(); i++)
    {
        ep = s.find(c[i]);
        if (ep != std::string::npos)
            break;
    }
    if (ep != std::string::npos)
        v = s.substr(0, ep);
    return v;
}

inline ConfigParser::ConfigParser(const char* fn) : fstream(fn), sstream(&fstream), ln_(0)
{
    if (!fstream)
        throw std::runtime_error(std::string("failed to open file: ") + fn);

    parse(top_);
}

ConfigParser::ConfigParser() 
    :ln_(0)
{

}

inline void ConfigParser::parseSLine(std::string& sname, size_t& depth)
{
    depth = 0;
    for (; depth < line_.length(); ++depth)
        if (line_[depth] != '[') break;

    sname = line_.substr(depth, line_.length() - 2 * depth);
}

inline void ConfigParser::parse(Level& l)
{
    while (std::getline(*sstream, line_)) {
        //skip utf8 BOM in windows
        if (ln_++ == 0 && line_.size() > 3) {
            if (static_cast<unsigned char>(line_[0]) == 0xEF &&
                static_cast<unsigned char>(line_[1]) == 0xBB &&
                static_cast<unsigned char>(line_[2]) == 0xBF)
            {
                for (int i = 0; i < 3; i++)
                {
                    line_.erase(line_.begin());
                }
            }
        }

        if (line_[0] == '#' || line_[0] == ';') continue;
        line_ = trim(line_);
        if (line_.empty()) continue;
        if (line_[0] == '[') {
            size_t depth;
            std::string sname;
            parseSLine(sname, depth);
            Level* lp = NULL;
            Level* parent = &l;
            if (depth > l.depth + 1)
                errMsg("section with wrong depth");
            if (l.depth == depth - 1)
                lp = &l.sections[sname];
            else {
                lp = l.parent;
                auto nDepth = l.depth - depth;
                for (auto i = 0; i < nDepth; ++i) lp = lp->parent;
                parent = lp;
                lp = &lp->sections[sname];
            }
            if (lp->depth != 0)
                errMsg("duplicate section name on the same level");
            if (!lp->parent) {
                lp->depth = depth;
                lp->parent = parent;
            }
            parent->ordered_sections.push_back(parent->sections.find(sname));
            parse(*lp);
        }
        else {
            auto nPos = line_.find('=');
            if (nPos == std::string::npos)
                errMsg("no '=' found");
            auto v = value(trim(line_.substr(nPos + 1, line_.length() - nPos - 1)));
            auto res = l.values.insert(std::make_pair(trim(line_.substr(0, nPos)), v));
            if (!res.second)
                errMsg("duplicated key found");
            l.ordered_values.push_back(res.first);
        }
    }
}

inline void ConfigParser::dump(std::ostream& s, const Level& l, const std::string& sname)
{
    if (!sname.empty()) s << '\n';
    for (size_t i = 0; i < l.depth; ++i) s << '[';
    if (!sname.empty()) s << sname;
    for (size_t i = 0; i < l.depth; ++i) s << ']';
    if (!sname.empty()) s << std::endl;
    for (auto it = l.ordered_values.begin(); it != l.ordered_values.end(); ++it)
        s << (*it)->first << '=' << (*it)->second << std::endl;
    for (auto it = l.ordered_sections.begin(); it != l.ordered_sections.end(); ++it) {
        assert((*it)->second.depth == l.depth + 1);
        dump(s, (*it)->second, (*it)->first);
    }
}

#endif // CONFIG_HPP

