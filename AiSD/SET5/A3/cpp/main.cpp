#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "RandomStreamGen.h"
#include "HashFunctions.h"
#include "HyperLogLog.h"
#include "HyperLogLogPlus/HyperLogLogPlus.h"

static std::string timestampStr() {
  using namespace std::chrono;
  auto now = system_clock::now();
  std::time_t t = system_clock::to_time_t(now);
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
  return oss.str();
}

struct Config {
  int streams = 20;
  int streamSize = 20000;
  int maxStrLen = 30;
  int b = 10;
  uint32_t seed = 42;
  std::vector<double> fractions{0.05, 0.10, 0.20, 0.50, 1.0};
};

static Config parseArgs(int argc, char** argv) {
  Config cfg;
  for (int i = 1; i < argc; i++) {
    std::string a = argv[i];
    auto need = [&](int k) { return i + k < argc; };

    if (a == "--streams" && need(1)) cfg.streams = std::stoi(argv[++i]);
    else if (a == "--n" && need(1)) cfg.streamSize = std::stoi(argv[++i]);
    else if (a == "--maxlen" && need(1)) cfg.maxStrLen = std::stoi(argv[++i]);
    else if (a == "--b" && need(1)) cfg.b = std::stoi(argv[++i]);
    else if (a == "--seed" && need(1)) cfg.seed = (uint32_t)std::stoul(argv[++i]);
  }
  return cfg;
}

int main(int argc, char** argv) {
  Config cfg = parseArgs(argc, argv);

  std::string outPath = "/Users/igorkravchenko/Documents/GitHub/HSE/AiSD/SET5/A3/results/hll_" + timestampStr() + ".csv";
  std::ofstream out(outPath);
  if (!out) {
    std::cerr << "Cannot open output file: " << outPath << "\n";
    return 1;
  }

  out << "stream_id,step,processed,true_unique,estimate,algo,b_or_p,m,seed\n";

  for (int s = 0; s < cfg.streams; s++) {
    uint32_t streamSeed = cfg.seed + (uint32_t)(s * 101u);

    RandomStreamGen gen(streamSeed, cfg.maxStrLen);
    auto stream = gen.generate((size_t)cfg.streamSize);
    auto checkpoints = RandomStreamGen::checkpoints((size_t)cfg.streamSize, cfg.fractions);
    size_t nextCpIdx = 0;

    std::unordered_set<std::string> exact;
    exact.reserve((size_t)cfg.streamSize);

    HyperLogLog hll((uint8_t)cfg.b);
    hll::HyperLogLogPlus hllp(cfg.b, 25);

    for (size_t i = 0; i < stream.size(); i++) {
      const std::string& x = stream[i];
      exact.insert(x);

      uint64_t h32 = hashfn::murmur3_64(x, 0x9747b28cU);
      hllp.add(h32);

      uint32_t h64 = hashfn::murmur3_32(x, 0x9747b28cU);
      hll.addHash(h64);

      size_t processed = i + 1;
      if (nextCpIdx < checkpoints.size() && processed == checkpoints[nextCpIdx]) {
        size_t tru = exact.size();

        double est_hll = hll.estimate();
        out << s << ","
            << nextCpIdx << ","
            << processed << ","
            << tru << ","
            << std::fixed << std::setprecision(6) << est_hll << ","
            << "hll" << ","
            << cfg.b << ","
            << (1u << cfg.b) << ","
            << streamSeed
            << "\n";

        double est_hllpp = hllp.estimate();
        out << s << ","
            << nextCpIdx << ","
            << processed << ","
            << tru << ","
            << std::fixed << std::setprecision(6) << est_hllpp << ","
            << "hllpp" << ","
            << cfg.b << ","
            << (1u << cfg.b) << ","
            << streamSeed
            << "\n";

        nextCpIdx++;
      }
    }
  }

  std::cout << "Saved: " << outPath << "\n";
  return 0;
}