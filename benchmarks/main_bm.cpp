#include <benchmark/benchmark.h>

#include <random>
#include <thread>

#include "sequential_tree.h"
#include "parellel_tree.h"

static SequentialTree* arbol_datos = nullptr;
static ParallelTree* arbol_datos = nullptr;
static const int VALOR_MEDIO = 10;
static const int NUMERO_ELEMENTOS = 5;
static const int NUMERO_VECTORES = 50;

void inicializa() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> uni_dis(VALOR_MEDIO - 10, VALOR_MEDIO + 10);

  for(int i = 0; i < NUMERO_VECTORES; ++i) {
    std::vector<double> tmp(NUMERO_ELEMENTOS);
    for(int j = 0; j < NUMERO_ELEMENTOS; ++j) tmp[j] = uni_dis(gen);

    if(arbol_datos == nullptr)
      arbol_datos = new SequentialTree(tmp);
    else
      arbol_datos->insert(tmp);
  }
}

void finaliza() { delete arbol_datos; }

static void BM_secuencial(benchmark::State& state) {
  for(auto _ : state) {
    double res = arbol_datos->calculateMaxAverage();
    benchmark::DoNotOptimize(res);
  }
}

static void BM_parallel(benchmark::State& state) {
  for(auto _ : state) {
    double res = arbol_datos->calculateMaxAverage();
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(BM_secuencial)->UseRealTime()->Unit(benchmark::kMillisecond);
BENCHMARK(BM_parallel)->UseRealTime()->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
  inicializa();
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  finaliza();
}
