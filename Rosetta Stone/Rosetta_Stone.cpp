#include "Rosetta_Stone.h"

Rosetta_Stone::Rosetta_Stone(const std::string& path) {
    in.open(path);
    labels = new std::list<std::pair<std::string, int>>;
    op_list = new std::list<OP*>;
    org = 0;
}

Rosetta_Stone::~Rosetta_Stone() {
    in.close();
}

void Rosetta_Stone::find_org() {
    in.seekg(0, std::ifstream::beg);
    std::string tmp;
    std::getline(in, tmp);
    size_t x = tmp.find(".org");
    if (x != std::string::npos) {
        tmp = tmp.substr(x+4, tmp.size());
        org = stoi(tmp, nullptr, 16);
    } else {
        org = 0;
        in.seekg(0, std::ifstream::beg);
    }
}

void Rosetta_Stone::find_labels() {
    std::stringstream ss;
    ss << in.rdbuf();
    std::string s;

    int t_org = org;
    size_t x;
    while (ss.good()) {
        ss >> s;
        x = s.find(':');
        if (x != std::string::npos) {
            std::pair<std::string, int> a(s.substr(0, x),t_org);
            labels->push_back(a);
        } else if (s.find('$') != std::string::npos) {
            t_org += 2;
        } else if (!is_reg(s)) {
            t_org++;
        }
    }
}

unsigned char Rosetta_Stone::reg(const std::string& s) {
    if (s == "X") {
        return 0;
    } else if (s == "Y") {
        return 1;
    } else if (s == "ACC") {
        return 2;
    }
    std::string a = "Unknown register " + s + "\n";
    throw std::invalid_argument(a);
}

int Rosetta_Stone::search_label(const std::string& s) {
    for (const auto& i: *labels) {
        if (i.first == s) {
            return i.second;
        }
    }
    std::string a = "Label doesnt exist " + s + "\n";
    throw std::invalid_argument(a);
}

bool Rosetta_Stone::is_label(std::string s) {
    s = s.substr(0, s.size() - 1);
    bool a = false;
    for (const auto& i: *labels) {
        if (i.first == s) {
            a = true;
            break;
        }
    }
    if (a) {
        return true;
    }
    return false;
}

bool Rosetta_Stone::is_reg(const std::string& s) {
    if (s.find('X') != std::string::npos ||
        s.find('Y') != std::string::npos ||
        s.find("ACC") != std::string::npos) {
        return true;
    }
    return false;
}

void Rosetta_Stone::parse() {
    find_org();
    find_labels();
    in.seekg(0, std::ifstream::beg);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string s;
    OP* op;
    while (ss.good()) {
        try {
            ss >> s;
            if (s == ".org") {
                ss >> s;
            } else {
                if (!is_label(s)) {
                    if (s == "NOP") {
                        op = new OP(0);
                    } else if (s == "CLR") {
                        ss >> s;
                        op = new OP((1 + reg(s)));
                    } else if (s == "OUT") {
                        ss >> s;
                        op = new OP((4 + reg(s)));
                    } else if (s == "CMP") {
                        ss >> s;
                        if (s.find('$') != std::string::npos) {
                            s = s.substr(1, s.size());
                            op = new OP(19,
                                        std::stoi(s, nullptr, 16),
                                        true);
                        } else {
                            op = new OP((16 + reg(s)));
                        }
                    } else if (s == "CALL") {
                        ss >> s;
                        op = new OP(20, search_label(s), true);
                    } else if (s == "STR") {
                        ss >> s;
                        op = new OP(21 + reg(s));
                    } else if (s == "JZ") {
                        ss >> s;
                        op = new OP(32, search_label(s), true);
                    } else if (s == "JNZ") {
                        ss >> s;
                        op = new OP(33, search_label(s), true);
                    } else if (s == "JN") {
                        ss >> s;
                        op = new OP(34, search_label(s), true);
                    } else if (s == "JL") {
                        ss >> s;
                        op = new OP(35, search_label(s), true);
                    } else if (s == "JG") {
                        ss >> s;
                        op = new OP(36, search_label(s), true);
                    } else if (s == "JE") {
                        ss >> s;
                        op = new OP(37, search_label(s), true);
                    } else if (s == "RET") {
                        op = new OP(38);
                    } else if (s == "HLT") {
                        op = new OP(39);
                    } else if (s == "ADD") {
                        ss >> s;
                        if (s.find('$') != std::string::npos) {
                            s = s.substr(1, s.size());
                            op = new OP(51,
                                        std::stoi(s, nullptr, 16),
                                        true);
                        } else if (is_reg(s)) {
                            op = new OP(48 + reg(s));
                        } else {
                            op = new OP(52, true,
                                        std::stoi(s, nullptr, 16));
                        }
                    } else if (s == "SUB") {
                        ss >> s;
                        if (s.find('$') != std::string::npos) {
                            s = s.substr(1, s.size());
                            op = new OP(67,
                                        std::stoi(s, nullptr, 16),
                                        true);
                        } else if (is_reg(s)) {
                            op = new OP(64 + reg(s));
                        } else {
                            op = new OP(68, true,
                                        std::stoi(s, nullptr, 16));
                        }
                    } else if (s == "LDR") {
                        ss >> s;
                        unsigned char x = reg(s);
                        ss >> s;
                        if (s.find('$') != std::string::npos) {
                            s = s.substr(1, s.size());
                            op = new OP(96 + x,
                                        std::stoi(s, nullptr, 16),
                                        true);
                        } else {
                            op = new OP(83 + x, true,
                                        std::stoi(s, nullptr, 16));
                        }
                    } else if (s == "MOV") {
                        ss >> s;
                        unsigned char x = reg(s);
                        ss >> s;
                        unsigned char y = reg(s);
                        if (y == 0) {
                            y = 1;
                        }
                        op = new OP(112 + 2 * x + y - 1);
                    } else if (s == "AND") {
                        ss >> s;
                        if (is_reg(s)) {
                            ss >> s;
                            op = new OP(131, true,
                                        std::stoi(s, nullptr, 16));
                        } else {
                            op = new OP(128 + reg(s));
                        }
                    } else if (s == "OR") {
                        ss >> s;
                        if (is_reg(s)) {
                            ss >> s;
                            op = new OP(135, true,
                                        std::stoi(s, nullptr, 16));
                        } else {
                            op = new OP(132 + reg(s));
                        }
                    } else if (s == "XOR") {
                        ss >> s;
                        if (is_reg(s)) {
                            ss >> s;
                            op = new OP(147, true,
                                        std::stoi(s, nullptr, 16));
                        } else {
                            op = new OP(144 + reg(s));
                        }
                    } else if (s == "NOT") {
                        ss >> s;
                        if (is_reg(s)) {
                            ss >> s;
                            op = new OP(151, true,
                                        std::stoi(s, nullptr, 16));
                        } else {
                            op = new OP(148 + reg(s));
                        }
                    } else {
                        std::string a = "Unknown token" + s + "\n";
                        throw std::invalid_argument(a);
                    }
                    op_list->push_back(op);
                }
            }
        } catch (std::invalid_argument& e) {
            throw e;
        }
    }
}

void Rosetta_Stone::print() const {
    std::cout<<"Org "<<org<<"\n";
    std::cout<<"Labels:\n";
    for (const auto& i: *labels) {
        std::cout<<"Lab: "<<i.first<<" org: "<<std::hex<<i.second<<std::endl;
    }
    std::cout<<"OPs:\n";
    for (auto i: *op_list) {
        std::cout<<(int)i->op<<"\n";
    }
}

void Rosetta_Stone::create_hex() {
    out.open("out.hex", std::ios::binary | std::ios::out);
    out.write((char *)&(org), 2);
    for (auto i: *op_list) {
        if (!i->need_val && !i->need_address) {
            out.write((char *)&(i->op), 1);
        } else if (i->need_val) {
            out.write((char *)&(i->op), 1);
            out.write((char *)&(i->val), 1);
        } else {
            out.write((char *)&(i->op), 1);
            out.write((char *)&(i->address), 2);
        }
    }
    out.close();
}

OP::OP(unsigned char op) : op(op) {}

OP::OP(unsigned char op, bool v, unsigned char val)
      : op(op), val(val), need_val(v) {}

OP::OP(unsigned char op, int add, bool a)
      : op(op), address(add), need_address(a) {}
