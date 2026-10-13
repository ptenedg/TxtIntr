#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <getopt.h>
#include <cstdlib>
#include <cstring>

void usage(const char* progname) {
    std::cout << "Usage: " << progname << " -o <operation> <operand1> [operand2]\n\n"
              << "Calculator (variant 13 - logarithmic).\n\n"
              << "Options:\n"
              << "  -o, --operation <op>   Operation to perform:\n"
              << "                           ln   - natural logarithm ln(x)\n"
              << "                           log  - logarithm log_y(x)\n"
              << "  -h, --help             Show this help message\n\n"
              << "Operands are positional arguments.\n"
              << "For negative operands, use '--' before them.\n\n"
              << "Examples:\n"
              << "  " << progname << " -o ln 2.71828\n"
              << "  " << progname << " --operation log 10 1000\n"
              << "  " << progname << " -o ln -- -5\n";
}

int main(int argc, char* argv[]) {
    std::string operation;
    std::vector<double> operands;

    static struct option long_options[] = {
        {"operation", required_argument, nullptr, 'o'},
        {"help",      no_argument,       nullptr, 'h'},
        {nullptr,     0,                 nullptr,  0 }
    };

    // Подавляем сообщения об ошибках getopt
    opterr = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, "+o:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'o':
                operation = optarg;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            case '?':
                // Неизвестная опция - возможно, это отрицательное число
                if (optind > 0 && optind <= argc) {
                    try {
                        size_t pos = 0;
                        double val = std::stod(argv[optind - 1], &pos);
                        if (pos == std::strlen(argv[optind - 1])) {
                            operands.push_back(val);
                        } else {
                            std::cerr << "Error: invalid operand '" << argv[optind - 1] << "'\n";
                            return 1;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error: unknown option or invalid operand '" 
                                  << argv[optind - 1] << "'\n";
                        usage(argv[0]);
                        return 1;
                    }
                }
                break;
            default:
                break;
        }
    }

    // Собираем оставшиеся позиционные операнды
    for (int i = optind; i < argc; ++i) {
        try {
            size_t pos = 0;
            double val = std::stod(argv[i], &pos);
            if (pos != std::strlen(argv[i])) {
                std::cerr << "Error: invalid operand '" << argv[i] << "'\n";
                return 1;
            }
            operands.push_back(val);
        } catch (const std::exception& e) {
            std::cerr << "Error: cannot parse operand '" << argv[i] << "'\n";
            return 1;
        }
    }

    if (operation.empty()) {
        std::cerr << "Error: operation is not specified.\n\n";
        usage(argv[0]);
        return 1;
    }

    double result = 0.0;

    if (operation == "ln") {
        if (operands.size() != 1) {
            std::cerr << "Error: operation 'ln' requires exactly 1 operand, got "
                      << operands.size() << "\n";
            return 1;
        }
        double x = operands[0];
        if (x <= 0.0) {
            std::cerr << "Error: ln(x) is defined only for x > 0 (got x = " << x << ")\n";
            return 1;
        }
        result = std::log(x);
        std::cout << "ln(" << x << ") = " << result << "\n";
    }
    else if (operation == "log") {
        if (operands.size() != 2) {
            std::cerr << "Error: operation 'log' requires exactly 2 operands (y and x), got "
                      << operands.size() << "\n";
            return 1;
        }
        double y = operands[0];
        double x = operands[1];
        if (y <= 0.0 || y == 1.0) {
            std::cerr << "Error: base y must be > 0 and != 1 (got y = " << y << ")\n";
            return 1;
        }
        if (x <= 0.0) {
            std::cerr << "Error: argument x must be > 0 (got x = " << x << ")\n";
            return 1;
        }
        result = std::log(x) / std::log(y);
        std::cout << "log_" << y << "(" << x << ") = " << result << "\n";
    }
    else {
        std::cerr << "Error: unknown operation '" << operation << "'\n";
        std::cerr << "Supported operations: ln, log\n";
        return 1;
    }

    return 0;
}
