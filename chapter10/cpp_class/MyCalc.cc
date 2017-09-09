#include "MyCalc.h"

Nan::Persistent<v8::Function> MyCalc::constructor;

MyCalc::MyCalc(double value): _value(value) {

}

MyCalc::~MyCalc() {

}

void MyCalc::Init(v8::Handle<v8::Object> module) {
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);//使用ShmdbNan::New<Object>函数作为构造函数
    tpl->SetClassName(Nan::New<v8::String>("MyCalc").ToLocalChecked());//js中的类名为MyCalc
    tpl->InstanceTemplate()->SetInternalFieldCount(1);//指定js类的成员字段个数

    Nan::SetPrototypeMethod(tpl,"addOne",PlusOne);//js类的成员函数名为addOne,我们将其映射为 C++中的PlusOne函数
    Nan::SetPrototypeMethod(tpl,"getValue",GetValue);//js类的成员函数名为getValue,我们将其映射为 C++中的GetValue函数
    
    //Persistent<Function> constructor = Persistent<Function>::New/*New等价于js中的new*/(tpl->GetFunction());//new一个js实例
    constructor.Reset(tpl->GetFunction());
    module->Set(Nan::New<v8::String>("exports").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(MyCalc::New) {
    if (info.IsConstructCall()) {
        // 通过 `new MyCalc(...)` 方式调用
        double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
        MyCalc* obj = new MyCalc(value);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        // 通过 `MyCalc(...)` 方式调用, 转成使用构造函数方式调用
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { info[0] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

NAN_METHOD(MyCalc::GetValue) {
    MyCalc* obj = ObjectWrap::Unwrap<MyCalc>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->_value));
}

NAN_METHOD(MyCalc::PlusOne) {
    MyCalc* obj = ObjectWrap::Unwrap<MyCalc>(info.Holder());
    double wannaAddValue = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();

    obj->_value += wannaAddValue;
    info.GetReturnValue().Set(Nan::New(obj->_value));
}