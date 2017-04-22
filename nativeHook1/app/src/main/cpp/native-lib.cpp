//#include <jni.h>
//#include <string>
//
//#include <jni.h>
//#include "substrate.h"
//#include <android/log.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <sys/types.h>
//#include <string.h>
//#include <sys/stat.h>
//
//#define TAG "HOOKTEST"
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
//
//MSConfig(MSFilterLibrary, "libdvm.so"); // 要hook的so库，后缀名即可。
//
//bool (*_dvmLoadNativeCode)(char* pathName, void* classLoader, char** detail);
//
//bool fake_dvmLoadNativeCode(char* soPath, void* classLoader, char** detail)
//{
//    LOGD("fake_dvmLoadNativeCode soPath:%s", soPath);
//    return _dvmLoadNativeCode(soPath,classLoader,detail);
//}
//
////Substrate entry point
//MSInitialize{
//    LOGD("Substrate initialized.");
//    MSImageRef image;
//    image = MSGetImageByName("/system/lib/libdvm.so"); // 此处要绝对路径
//    if (image != NULL)
//    {
//        LOGD("dvm image: 0x%08X", (void*)image);
//        void * dvmload = MSFindSymbol(image, "_Z17dvmLoadNativeCodePKcP6ObjectPPc");
//        if(dvmload == NULL)
//        {
//            LOGD("error find dvmLoadNativeCode.");
//        }
//        else
//        {
//            MSHookFunction(dvmload,(void*)&fake_dvmLoadNativeCode,(void **)&_dvmLoadNativeCode);
//            LOGD("hook dvmLoadNativeCode sucess.");
//        }
//    }
//    else{
//        LOGD("can not find libdvm.");
//    }
//}

//-----------------------------------//-----------------------------------
//#include <android/log.h>
//#include <substrate.h>
//
//#define LOG_TAG "SUBhook"
//
//#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//
//void cigi_hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
//{
//    MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
//}
//MSConfig(MSFilterExecutable, "/system/bin/app_process")
//
//int (*original_arc4random)(void);
//int replaced_arc4random(void)
//{
//    return 5678;
//}
//MSInitialize {
//    cigi_hook((void *)arc4random,(void*)&replaced_arc4random,(void**)&original_arc4random);
//}
//-----------------------------------//-----------------------------------
//

//===========ok==========
//
#include <android/log.h>
#include <substrate.h>
#include <stdio.h>

#define LOG_TAG "haha"

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void cigi_hook(void *orig_fcn, void *new_fcn, void **orig_fcn_ptr) {
    MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
}

MSConfig(MSFilterExecutable, "/system/bin/app_process")

int (*original_getAge)(void);

int replaced_getAge(void) {
    LOGI("(replace _getAge) [====================");
    return 99;
}

int (*original_arc4random)(void);

int replaced_arc4random(void) {
    return 222;
}

void *lookup_symbol(char *libraryname, char *symbolname) {
    void *imagehandle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
    if (imagehandle != NULL) {
        void *sym = dlsym(imagehandle, symbolname);
        if (sym != NULL) {
            LOGI("look success!");
            return sym;
        } else {
            LOGI("(lookup_symbol) dlsym didn't work");
            return NULL;
        }
    } else {
        LOGI("(lookup_symbol) dlerror: %s", dlerror());
        return NULL;
    }
}

MSInitialize {
    LOGI("@@@@@@@@@@@@@@@@");
    cigi_hook((void *) arc4random, (void *) &replaced_arc4random, (void **) &original_arc4random);
    void *getAgeSym = lookup_symbol("/data/data/com.example.charles.demo/lib/libnative-lib.so", "_Z6getAgev");
    cigi_hook(getAgeSym, (void *) &replaced_getAge, (void **) &original_getAge);
}

//===========ok==========


//#include <dlfcn.h>
//#include "substrate.h"
//#include <stdio.h>
//#define TAG "HOOKTEST"
//#include <android/log.h>
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
//
//void * get_base_of_lib_from_maps(char *soname)
//{
//    void *imagehandle = dlopen(soname, RTLD_LOCAL | RTLD_LAZY);
//    if (soname == NULL)
//        return NULL;
//    if (imagehandle == NULL){
//        return NULL;
//    }
//    uintptr_t * irc = NULL;
//    FILE *f = NULL;
//    char line[200] = {0};
//    char *state = NULL;
//    char *tok = NULL;
//    char * baseAddr = NULL;
//    if ((f = fopen("/proc/self/maps", "r")) == NULL)
//        return NULL;
//    while (fgets(line, 199, f) != NULL)
//    {
//        tok = strtok_r(line, "-", &state);
//        baseAddr = tok;
//        tok = strtok_r(NULL, "\t ", &state);
//        tok = strtok_r(NULL, "\t ", &state); // "r-xp" field
//        tok = strtok_r(NULL, "\t ", &state); // "0000000" field
//        tok = strtok_r(NULL, "\t ", &state); // "01:02" field
//        tok = strtok_r(NULL, "\t ", &state); // "133224" field
//        tok = strtok_r(NULL, "\t ", &state); // path field
//
//        if (tok != NULL) {
//            int i;
//            for (i = (int)strlen(tok)-1; i >= 0; --i) {
//                if (!(tok[i] == ' ' || tok[i] == '\r' || tok[i] == '\n' || tok[i] == '\t'))
//                    break;
//                tok[i] = 0;
//            }
//            {
//                size_t toklen = strlen(tok);
//                size_t solen = strlen(soname);
//                if (toklen > 0) {
//                    if (toklen >= solen && strcmp(tok + (toklen - solen), soname) == 0) {
//                        fclose(f);
//                        return (uintptr_t*)strtoll(baseAddr,NULL,16);
//                    }
//                }
//            }
//        }
//    }
//    fclose(f);
//    return NULL;
//}
//
//MSInitialize {
//    LOGI("@@@@@@@@@@@@@@@@");
//    void* lib_base = get_base_of_lib_from_maps("/data/data/com.example.charles.demo/lib/libnative-lib.so");
//    LOGI("lib base is %p",lib_base);
//    if (lib_base!=NULL) {
//        void *getAgeSym = (int*)lib_base + 0x00000590;
//        LOGI("getAge() should be at %p. Let's hook it", getAgeSym);
//    }
//        cigi_hook(getAgeSym,(void*)&replaced_getAge,(void**)&original_getAge);
//    cigi_hook((void *) arc4random, (void *) &replaced_arc4random, (void **) &original_arc4random);
//    void *getAgeSym = lookup_symbol("/data/data/com.example.charles.demo/lib/libnative-lib.so", "_Z6getAgev");
//    cigi_hook(getAgeSym, (void *) &replaced_getAge, (void **) &original_getAge);
//}


//OR
//    void* lib_base = get_base_of_lib_from_soinfo("/data/app-lib/io.koz.targetApp2-1/libtargetLib.so");
