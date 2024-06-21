
#include <algorithm>
#include <boost/program_options.hpp>
#include <cstddef>
#include <fstream>
#include <random>
#include <sstream>
#include "cipher.h"

struct TaskGenerator
{
    TaskGenerator(
        std::size_t l1,
        std::size_t l2,
        std::size_t n,
        std::string input,
        std::string leak
    )
        : l1_(std::max(std::size_t(1), l1))
        , l2_(std::max(std::size_t(1), l2))
        , n_(std::max(std::size_t(1), n))
        , input_(input)
        , leak_(leak)
    {
    }

    int run();

    std::uint64_t gen(std::size_t l);

    std::size_t n_;
    std::size_t l1_;
    std::size_t l2_;

    std::string leak_;
    std::string input_;
};

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    po::options_description desc;

    desc.add_options()
    (
        "help",
        "Показать формат параметров генерации"
    )
    (
        "n",
        po::value<std::size_t>()->default_value(
            30
        ),
        "Количество вариантов"
    )
    (
        "l1",
        po::value<std::size_t>()->default_value(
            24
        ),
        "Длина регистра R1"
    )
    (
        "l2",
        po::value<std::size_t>()->default_value(
           32
        ),
        "Длина регистра R2"
    )
    (
        "input",
        po::value<std::string>(),
        "Файл с текстом для генерации задания"
    )
    (
        "leak",
        po::value<std::string>()->default_value(
            "7ry cR4@cK m3 tRy h@cK m3"
        ),
        "Leak plain - утекший открытый текст"
    )

    ;

    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, desc), options);

    if (options.count("help") or not options.count("input")) {
        std::cout << desc << std::endl;
        return 0;
    }

    const auto l1 = options["l1"].as<std::size_t>();
    const auto l2 = options["l2"].as<std::size_t>();
    const auto n = options["n"].as<std::size_t>();
    const auto leak = options["leak"].as<std::string>();
    const auto input = options["input"].as<std::string>();

    return TaskGenerator(l1, l2, n, input, leak).run();
}


int TaskGenerator::run()
{
    using namespace cipher;

    std::ifstream inpFile(input_);

    if (not inpFile.is_open()) {
        std::cerr
            << "Не удалось открыть файл с текстом " << input_
        ;
        return 1;
    }

    std::string text;
    std::string buffer;
    std::string leakPlain;
    std::string leakCipher;
    std::stringstream answerCsv;
    std::stringstream taskCsv;

    while (std::getline(inpFile, buffer)) {
        text += buffer;
    }

    std::size_t plainLen = std::min<std::size_t>(
        text.length() / n_,
        500
    );

    inpFile.close();

    answerCsv
        << "i, k1, k2, plain, plain hex\n"
    ;

    taskCsv
        << "i, r1, r2, l1, l2, cipher hex, leak plain hex, leak cipher hex\n"
    ;

    for (std::size_t i = 0; i < n_; ++i) {

        auto r1 = gen(l1_);
        auto r2 = gen(l2_);

        auto k1 = gen(l1_);
        auto k2 = gen(l2_);

        Cipher cip(r1, r2, l1_, l2_);


        std::string plain = text.substr(i * plainLen, plainLen);

        answerCsv
            << std::dec << i + 1 << ','
            << "0x" << std::hex << k1 << ','
            << "0x" << std::hex << k2 << ','
            << plain << ','
            << Cipher::toHex({plain.begin(), plain.end()}) << '\n'
        ;

        taskCsv
            << std::dec << i + 1 << ','
            << "0x" << std::hex << r1 << ','
            << "0x" << std::hex << r2 << ','
            << std::dec << l1_ << ','
            << std::dec << l2_ << ','
            << Cipher::toHex(cip.Enc({plain.begin(), plain.end()}, k1, k2)) << ','
            << Cipher::toHex({leak_.begin(), leak_.end()}) << ','
            << Cipher::toHex(cip.Enc({leak_.begin(), leak_.end()}, k1, k2)) << '\n'
        ;
    }

    std::ofstream answerFile("answer.csv");
    std::ofstream taskFile("task.csv");

    if (not answerFile.is_open()) {
        std::cerr
            << "Не удалось открыть файл answer.csv для записи\n"
            << "answer.csv content:\n"
            << answerCsv.str()
            << std::endl
        ;
        return 1;
    }

    answerFile << answerCsv.str();

    if (not taskFile.is_open()) {
        std::cerr
            << "Не удалось открыть файл task.csv для записи\n"
            << "task.csv content:\n"
            << taskCsv.str()
            << std::endl
        ;
        return 1;
    }

    taskFile << taskCsv.str();

    std::cerr << "Успешно сегенировано " << n_ << " варинатов\n";

    return 0;
}


std::uint64_t TaskGenerator::gen(std::size_t l)
{
    static std::random_device d;
    std::uniform_int_distribution<std::uint64_t> u(1, (1ull << l) - 1);
    return u(d);
}


