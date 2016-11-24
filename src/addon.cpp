#include <nan.h>

#include "tsp_v8.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll) {
  Set(target, New<String>("solveTsp").ToLocalChecked(),
      Nan::GetFunction(New<FunctionTemplate>(SolveTsp)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
