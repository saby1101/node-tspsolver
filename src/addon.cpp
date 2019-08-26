#include "nan.h"

#include "tsp_v8.h"

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New<v8::String>("solveTsp").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SolveTsp))
               .ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
