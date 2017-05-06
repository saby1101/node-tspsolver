#include <iostream>
#include <nan.h>
#include <vector>

#include "tsp.h"
#include "tsp_v8.h"

using v8::Function;
using v8::Local;
using v8::Integer;
using v8::Array;
using v8::Number;
using v8::String;
using v8::Value;
using v8::Object;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

class TspWorker : public AsyncWorker {
  const bool roundtrip;
  const unsigned N;
  const unsigned T;
  const double lambda;
  const int reheatInterval;

  std::vector<std::vector<double>> costMatrix;

  std::vector<unsigned long> path;

public:
  TspWorker(Local<Array> costMatrixArr, bool roundtrip, unsigned N, unsigned T,
            double lambda, unsigned reheatInterval, Callback *onRound,
            Callback *callback)
      : AsyncWorker(callback), roundtrip(roundtrip), N(N), T(T), lambda(lambda),
        reheatInterval(reheatInterval) {

    auto size = costMatrixArr->Length();
    costMatrix =
        std::vector<std::vector<double>>(size, std::vector<double>(size, 0));

    for (unsigned i = 0; i < size; i++) {
      for (unsigned j = 0; j < size; j++) {
        auto row = costMatrixArr->Get(i).As<Array>();
        auto cost = row->Get(j)->NumberValue();
        costMatrix[i][j] = cost;
      }
    }
  }

  void Execute() {
    path = tms::solveTsp(N, T, lambda, reheatInterval, nullptr, costMatrix,
                         roundtrip);
  }

  void HandleOKCallback() {
    HandleScope scope;
    auto pathArr = New<Array>();
    for (unsigned i = 0; i < path.size(); i++) {
      pathArr->Set(i, New<Integer>((int)path[i]));
    }
    Local<Value> argv[] = {Null(), pathArr};

    callback->Call(2, argv);
  }
};

NAN_METHOD(SolveTsp) {
  Local<Array> costMatrixArr = info[0].As<Array>();
  bool roundtrip = info[1]->BooleanValue();
  Local<Object> opts = info[2].As<Object>();

  unsigned N = opts->Get(New("N").ToLocalChecked())->Uint32Value();
  unsigned T = opts->Get(New("T").ToLocalChecked())->Uint32Value();
  double lambda = opts->Get(New("lambda").ToLocalChecked())->NumberValue();
  unsigned reheatInterval =
      opts->Get(New("reheatInterval").ToLocalChecked())->Uint32Value();

  Callback *callback = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new TspWorker(costMatrixArr, roundtrip, N, T, lambda,
                                 reheatInterval, nullptr, callback));
}
