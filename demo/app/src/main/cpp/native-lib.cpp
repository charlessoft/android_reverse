#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"hooktargetapp",__VA_ARGS__)

static int getAge(void) {
    LOGI("[i] Verbose - getAge located at %p\n", &getAge);
    return 21;
}




extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_charles_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    int r = 2;//arc4random() % 1000;
    int r = arc4random() % 10000;
    LOGI("[+] John Smith is %d years old.\n", getAge());
    LOGI("[+] The totally reliable random seed is: %d\n\n", r);
    return 0;
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
