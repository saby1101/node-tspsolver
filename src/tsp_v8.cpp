#include <nan.h>
#include <vector>

#include "tsp.h"
#include "tsp_v8.h"

class TspWorker : public Nan::AsyncWorker {
  const bool roundtrip;
  const uint32_t N;
  const uint32_t T;
  const double lambda;
  const uint32_t reheatInterval;

  std::vector<std::vector<double>> costMatrix;

  tsp::path_type path;

public:
  TspWorker(v8::Local<v8::Array> costMatrixArr, bool roundtrip, uint32_t N,
            uint32_t T, double lambda, uint32_t reheatInterval,
            Nan::Callback *onRound, Nan::Callback *callback)
      : AsyncWorker(callback), roundtrip(roundtrip), N(N), T(T), lambda(lambda),
        reheatInterval(reheatInterval) {

    auto size = costMatrixArr.As<v8::Array>()->Length();
    costMatrix =
        std::vector<std::vector<double>>(size, std::vector<double>(size, 0));

    for (uint32_t i = 0; i < size; i++) {
      for (uint32_t j = 0; j < size; j++) {
        auto row = Nan::Get(costMatrixArr, i).ToLocalChecked().As<v8::Array>();
        auto costLocal = Nan::Get(row, j).ToLocalChecked();
        auto cost = Nan::To<double>(costLocal).FromJust();
        costMatrix[i][j] = cost;
      }
    }
  }

  void Execute() {
    path = tsp::solveTsp(N, T, lambda, reheatInterval, nullptr, costMatrix,
                         roundtrip);
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;
    auto pathArr = Nan::New<v8::Array>();
    for (uint32_t i = 0; i < path.size(); i++) {
      Nan::Set(pathArr, i, Nan::New<v8::Integer>(path[i]));
    }
    v8::Local<v8::Value> argv[] = {Nan::Null(), pathArr};

    Nan::Call(*callback, 2, argv);
  }
};

NAN_METHOD(SolveTsp) {
  auto costMatrixArr = info[0].As<v8::Array>();
  auto roundtrip = Nan::To<bool>(info[1]).FromJust();
  auto opts = info[2].As<v8::Object>();

  auto N = Nan::To<uint32_t>(
               Nan::Get(opts, Nan::New("N").ToLocalChecked()).ToLocalChecked())
               .FromJust();
  auto T = Nan::To<uint32_t>(
               Nan::Get(opts, Nan::New("T").ToLocalChecked()).ToLocalChecked())
               .FromJust();
  auto lambda =
      Nan::To<double>(
          Nan::Get(opts, Nan::New("lambda").ToLocalChecked()).ToLocalChecked())
          .FromJust();
  auto reheatInterval =
      Nan::To<double>(
          Nan::Get(opts, Nan::New("reheatInterval").ToLocalChecked())
              .ToLocalChecked())
          .FromJust();

  auto *callback =
      new Nan::Callback(Nan::To<v8::Function>(info[3]).ToLocalChecked());

  AsyncQueueWorker(new TspWorker(costMatrixArr, roundtrip, N, T, lambda,
                                 reheatInterval, nullptr, callback));
}
