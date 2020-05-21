#include <napi.h>
#include "napi-thread-safe-callback.hpp"
#include "timeswipe.hpp"

class TimeSwipeNAPI: public TimeSwipe, public Napi::ObjectWrap<TimeSwipeNAPI>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    TimeSwipeNAPI(const Napi::CallbackInfo& info);
private:
    static Napi::FunctionReference constructor;

    static Napi::Value EventButtonGetter(const Napi::CallbackInfo& info);
    static void EventButtonSetter(const Napi::CallbackInfo& info, const Napi::Value&);

    static Napi::Value ModePrimaryGetter(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);
        return Napi::String::New(env, "PRIMARY");
    }
    static Napi::Value ModeNormGetter(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);
        return Napi::String::New(env, "NORM");
    }
    static Napi::Value ModeDigitalGetter(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);
        return Napi::String::New(env, "DIGITAL");
    }
    static void EmptySetter(const Napi::CallbackInfo& info, const Napi::Value&) {}

    Napi::Value SetModeWrap(const Napi::CallbackInfo& info);
    Napi::Value GetModeWrap(const Napi::CallbackInfo& info);
    void SetSensorOffsetsWrap(const Napi::CallbackInfo& info);
    void SetSensorGainsWrap(const Napi::CallbackInfo& info);
    void SetSensorTransmissionsWrap(const Napi::CallbackInfo& info);
    void SetBurstSizeWrap(const Napi::CallbackInfo& info);
    Napi::Value SetSampleRateWrap(const Napi::CallbackInfo& info);
    Napi::Value StartWrap(const Napi::CallbackInfo& info);
    Napi::Value SetSettingsWrap(const Napi::CallbackInfo& info);
    Napi::Value GetSettingsWrap(const Napi::CallbackInfo& info);
    Napi::Value onErrorWrap(const Napi::CallbackInfo& info);
    Napi::Value onEventWrap(const Napi::CallbackInfo& info);
    Napi::Value StopWrap(const Napi::CallbackInfo& info);
    Napi::Value StartPWMWrap(const Napi::CallbackInfo& info);
    Napi::Value StopPWMWrap(const Napi::CallbackInfo& info);
    Napi::Value GetPWMWrap(const Napi::CallbackInfo& info);
    void TraceSPIWrap(const Napi::CallbackInfo& info);
};

Napi::FunctionReference TimeSwipeNAPI::constructor;


Napi::Object TimeSwipeNAPI::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "TimeSwipeNAPI",
                  {
                  StaticAccessor("modePrimary", &TimeSwipeNAPI::ModePrimaryGetter, &TimeSwipeNAPI::EmptySetter),
                  StaticAccessor("modeNorm", &TimeSwipeNAPI::ModeNormGetter, &TimeSwipeNAPI::EmptySetter),
                  StaticAccessor("modeDigital", &TimeSwipeNAPI::ModeDigitalGetter, &TimeSwipeNAPI::EmptySetter),

                  InstanceMethod("SetMode", &TimeSwipeNAPI::SetModeWrap),
                  InstanceMethod("GetMode", &TimeSwipeNAPI::GetModeWrap),
                  InstanceMethod("SetSensorOffsets", &TimeSwipeNAPI::SetSensorOffsetsWrap),
                  InstanceMethod("SetSensorGains", &TimeSwipeNAPI::SetSensorGainsWrap),
                  InstanceMethod("SetSensorTransmissions", &TimeSwipeNAPI::SetSensorTransmissionsWrap),
                  InstanceMethod("SetBurstSize", &TimeSwipeNAPI::SetBurstSizeWrap),
                  InstanceMethod("SetSampleRate", &TimeSwipeNAPI::SetSampleRateWrap),
                  InstanceMethod("Start", &TimeSwipeNAPI::StartWrap),
                  InstanceMethod("SetSettings", &TimeSwipeNAPI::SetSettingsWrap),
                  InstanceMethod("GetSettings", &TimeSwipeNAPI::GetSettingsWrap),
                  InstanceMethod("onError", &TimeSwipeNAPI::onErrorWrap),
                  InstanceMethod("onEvent", &TimeSwipeNAPI::onEventWrap),
                  InstanceMethod("Stop", &TimeSwipeNAPI::StopWrap),
                  InstanceMethod("StartPWM", &TimeSwipeNAPI::StartPWMWrap),
                  InstanceMethod("StopPWM", &TimeSwipeNAPI::StopPWMWrap),
                  InstanceMethod("GetPWM", &TimeSwipeNAPI::GetPWMWrap),
                  InstanceMethod("TraceSPI", &TimeSwipeNAPI::TraceSPIWrap),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("TimeSwipeNAPI", func);
  return exports;
}

Napi::Value TimeSwipeNAPI::EventButtonGetter(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    return Napi::String::New(env, "eventButton");
}

void TimeSwipeNAPI::EventButtonSetter(const Napi::CallbackInfo& info, const Napi::Value&) {
}

TimeSwipeNAPI::TimeSwipeNAPI(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<TimeSwipeNAPI>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
}

Napi::Value TimeSwipeNAPI::SetModeWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsString()) {
     Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
  }

  auto value = info[0].As<Napi::String>().Utf8Value();
  TimeSwipe::Mode m;
  if (value == "PRIMARY") {
      m = TimeSwipe::Mode::Primary;
  } else if (value == "NORM") {
      m = TimeSwipe::Mode::Norm;
  } else if (value == "DIGITAL") {
      m = TimeSwipe::Mode::Digital;
  } else {
      return Napi::Boolean::New(env, false);
  }

  this->SetMode(m);
  return Napi::Boolean::New(env, true);
}

Napi::Value TimeSwipeNAPI::GetModeWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  auto m = this->GetMode();

  if (m == TimeSwipe::Mode::Primary) {
      return Napi::String::New(env, "PRIMARY");
  } else if (m == TimeSwipe::Mode::Norm) {
      return Napi::String::New(env, "NORM");
  } else if (m == TimeSwipe::Mode::Digital) {
      return Napi::String::New(env, "DIGITAL");
  }
  return Napi::String::New(env, "");
}

void TimeSwipeNAPI::SetSensorOffsetsWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
    Napi::TypeError::New(env, "4 Numbers expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  Napi::Number value1 = info[1].As<Napi::Number>();
  Napi::Number value2 = info[2].As<Napi::Number>();
  Napi::Number value3 = info[3].As<Napi::Number>();
  this->SetSensorOffsets(value0, value1, value2, value3);
}

void TimeSwipeNAPI::SetSensorGainsWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
    Napi::TypeError::New(env, "4 Numbers expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  Napi::Number value1 = info[1].As<Napi::Number>();
  Napi::Number value2 = info[2].As<Napi::Number>();
  Napi::Number value3 = info[3].As<Napi::Number>();
  this->SetSensorGains(value0, value1, value2, value3);
}

void TimeSwipeNAPI::SetSensorTransmissionsWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 3 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
    Napi::TypeError::New(env, "4 Numbers expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  Napi::Number value1 = info[1].As<Napi::Number>();
  Napi::Number value2 = info[2].As<Napi::Number>();
  Napi::Number value3 = info[3].As<Napi::Number>();
  this->SetSensorTransmissions(value0.DoubleValue(), value1.DoubleValue(), value2.DoubleValue(), value3.DoubleValue());
}

void TimeSwipeNAPI::SetBurstSizeWrap(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value = info[0].As<Napi::Number>();
  this->SetBurstSize(value.Uint32Value());
}

Napi::Value TimeSwipeNAPI::SetSampleRateWrap(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value = info[0].As<Napi::Number>();
  auto res = this->SetSampleRate(value);
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::StartWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  bool fail = false;
  if (length <= 0 || !info[0].IsFunction()) {
      //Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
      fail = true;
  }
  auto callback = std::make_shared<ThreadSafeCallback>(info[0].As<Napi::Function>());

  auto res = this->Start([this, callback, fail](SensorsData&& records, uint64_t errors) {
    try {
      if (fail) throw std::runtime_error("Function expected");
      callback->call([records, errors] (Napi::Env env, std::vector<napi_value>& args) mutable { //TODO: fix const modifiers in timeswipe.hpp and disable mutable
          auto arr = Napi::Array::New(env, records.SensorsSize());
          for (size_t i = 0; i < records.SensorsSize(); i++) {
              auto rec = Napi::Array::New(env, records.DataSize());
              //auto rec = Napi::Float32Array::New(env, records.DataSize());
              for (unsigned j = 0; j < records.DataSize(); j++) {
                rec[j] = Napi::Number::New(env, records[i][j]);
              }
              arr[i] = rec;
          }
          args = { arr, Napi::Number::New(env, errors) };
      });
    }
    catch (std::exception& e) {
        callback->callError(e.what());
    }
  });
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::SetSettingsWrap(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length <= 0 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    auto value = info[0].As<Napi::String>();
    std::string err;
    auto resp = this->SetSettings(value, err);
    auto arr = Napi::Array::New(env, 2);
    arr[0u] = resp;
    arr[1u] = err;
    return arr;
}

Napi::Value TimeSwipeNAPI::GetSettingsWrap(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length <= 0 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    auto value = info[0].As<Napi::String>();
    std::string err;
    auto resp = this->GetSettings(value, err);
    auto arr = Napi::Array::New(env, 2);
    arr[0u] = resp;
    arr[1u] = err;
    return arr;
}

Napi::Value TimeSwipeNAPI::onEventWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  bool fail = false;
  if (length <= 0 || !info[0].IsFunction()) {
    Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
    fail = true;
  }

  auto callback = std::make_shared<ThreadSafeCallback>(info[0].As<Napi::Function>());

  auto res = this->onEvent([this, &info, callback, fail](TimeSwipeEvent&& ev) {
    try {
      if (fail) throw std::runtime_error("Function expected");

      callback->call([ev=std::move(ev)] (Napi::Env env, std::vector<napi_value>& args) mutable {
          auto arr = Napi::Array::New(env);
          if (ev.is<TimeSwipeEvent::Button>()) {
            const auto& button = ev.get<TimeSwipeEvent::Button>();
            arr["type"] = "Button";
            arr["pressed"] = button.pressed();
            arr["count"] = button.count();
         } else if (ev.is<TimeSwipeEvent::Gain>()) {
            arr["type"] = "Gain";
            arr["value"] = ev.get<TimeSwipeEvent::Gain>().value();
         } else if (ev.is<TimeSwipeEvent::SetSecondary>()) {
            arr["type"] = "SetSecondary";
            arr["value"] = ev.get<TimeSwipeEvent::SetSecondary>().value();
         } else if (ev.is<TimeSwipeEvent::Bridge>()) {
            arr["type"] = "Bridge";
            arr["value"] = ev.get<TimeSwipeEvent::Bridge>().value();
         } else if (ev.is<TimeSwipeEvent::Record>()) {
            arr["type"] = "Record";
            arr["value"] = ev.get<TimeSwipeEvent::Record>().value();
         } else if (ev.is<TimeSwipeEvent::Offset>()) {
            arr["type"] = "Offset";
            arr["value"] = ev.get<TimeSwipeEvent::Offset>().value();
         } else if (ev.is<TimeSwipeEvent::Mode>()) {
            arr["type"] = "Mode";
            arr["value"] = ev.get<TimeSwipeEvent::Mode>().value();
         } else {
             arr["type"] = "UNKNOWN";
         }
         args = { arr };
      });
    }
    catch (std::exception& e) {
        callback->callError(e.what());
    }

  });
  return Napi::Boolean::New(env, res);
}


Napi::Value TimeSwipeNAPI::onErrorWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  bool fail = false;
  if (length <= 0 || !info[0].IsFunction()) {
    Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
    fail = true;
  }

  auto callback = std::make_shared<ThreadSafeCallback>(info[0].As<Napi::Function>());

  auto res = this->onError([this, callback, fail](uint64_t errors) {
    try {
      if (fail) throw std::runtime_error("Function expected");
      callback->call([errors] (Napi::Env env, std::vector<napi_value>& args) {
          args = { Napi::Number::New(env, errors) };
      });
    }
    catch (std::exception& e) {
        callback->callError(e.what());
    }

  });
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::StopWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  auto res = this->Stop();
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::StartPWMWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 5 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber() || !info[4].IsNumber() || !info[5].IsNumber()) {
    Napi::TypeError::New(env, "6 Numbers expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  Napi::Number value1 = info[1].As<Napi::Number>();
  Napi::Number value2 = info[2].As<Napi::Number>();
  Napi::Number value3 = info[3].As<Napi::Number>();
  Napi::Number value4 = info[4].As<Napi::Number>();
  Napi::Number value5 = info[5].As<Napi::Number>();
  auto res = this->StartPWM(value0.Uint32Value(), value1.Uint32Value(), value2.Uint32Value(), value3.Uint32Value(), value4.Uint32Value(), value5.FloatValue());
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::StopPWMWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "1 Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  auto res = this->StopPWM(value0.Uint32Value());
  return Napi::Boolean::New(env, res);
}

Napi::Value TimeSwipeNAPI::GetPWMWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "1 Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value0 = info[0].As<Napi::Number>();
  bool active;
  uint32_t frequency;
  uint32_t high;
  uint32_t low;
  uint32_t repeats;
  float duty_cycle;
  auto res = this->GetPWM(value0.Uint32Value(), active, frequency, high, low, repeats, duty_cycle);
  if (!res) return Napi::Boolean::New(env, res);

  auto arr = Napi::Array::New(env, 6);
  arr[0u] = active;
  arr[1u] = frequency;
  arr[2u] = high;
  arr[3u] = low;
  arr[4u] = repeats;
  arr[5u] = duty_cycle;
  return arr;
}

void TimeSwipeNAPI::TraceSPIWrap(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsBoolean()) {
    Napi::TypeError::New(env, "Boolean expected").ThrowAsJavaScriptException();
  }

  Napi::Boolean value = info[0].As<Napi::Boolean>();
  auto res = this->StopPWM(value.Value());
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return TimeSwipeNAPI::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)

