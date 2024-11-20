#include <benchmark/benchmark.h>

#include <random>
#include <thread>

#include "sequential_tree.h"
#include "parellel_tree.h"

// Variables para almacenar los árboles secuenciales y paralelos
static SequentialTree* arbol_secuencial = nullptr;
static ParallelTree* arbol_parallel = nullptr;

static const int VALOR_MEDIO = 10;
static const int NUMERO_ELEMENTOS = 5;
static const int NUMERO_VECTORES = 50;

void inicializa() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> uni_dis(VALOR_MEDIO - 10, VALOR_MEDIO + 10);

  // Inicializamos ambos árboles
  for(int i = 0; i < NUMERO_VECTORES; ++i) {
    std::vector<double> tmp(NUMERO_ELEMENTOS);
    for(int j = 0; j < NUMERO_ELEMENTOS; ++j) tmp[j] = uni_dis(gen);

    // Insertamos el vector en el árbol secuencial
    if(arbol_secuencial == nullptr)
      arbol_secuencial = new SequentialTree(tmp);
    else
      arbol_secuencial->insert(tmp);

    // Insertamos el vector en el árbol paralelo
    if(arbol_parallel == nullptr)
      arbol_parallel = new ParallelTree(tmp);
    else
      arbol_parallel->insert(tmp);
  }
}

void finaliza() { 
  delete arbol_secuencial; 
  delete arbol_parallel; 
}

static void BM_secuencial(benchmark::State& state) {
  for(auto _ : state) {
    // Usamos el árbol secuencial
    double res = arbol_secuencial->calculateMaxAverage();
    benchmark::DoNotOptimize(res);
  }
}

static void BM_parallel(benchmark::State& state) {
  for(auto _ : state) {
    // Usamos el árbol paralelo
    double res = arbol_parallel->calculateMaxAverage();
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
